/*******************************************************************************
 Copyright 2014 Henric Zazzi <hzazzi@kth.se>

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 *******************************************************************************/

#include "xmlio.h"

//------------------------------------------------------------------------------
PercolatorOutFeatures::PercolatorOutFeatures() {
  filename="";
  psmid="";
  parameter=-1;
  }
//------------------------------------------------------------------------------
PercolatorOutFeatures::PercolatorOutFeatures(string filename, string psmid, int parameter) {
  this->filename=filename;
  this->psmid=psmid;
  this->parameter=parameter;
  }
//------------------------------------------------------------------------------
bool PercolatorOutFeatures::operator==(PercolatorOutFeatures const& p) const {
  return this->filename.compare(p.filename)==0 &&
         this->psmid.compare(p.psmid)==0 &&
         this->parameter==p.parameter;
  }
//------------------------------------------------------------------------------
size_t PercolatorOutFeatures::operator()(PercolatorOutFeatures const& p) const {
  size_t seed=0;
  boost::hash_combine(seed,p.filename);
  boost::hash_combine(seed,p.psmid);
  boost::hash_combine(seed,p.parameter);
  return seed;
  }
//------------------------------------------------------------------------------
XMLIO::XMLIO() {
  validatexml=xml_schema::flags::dont_initialize;
  }
//------------------------------------------------------------------------------
void XMLIO::unsetValidation() {
  validatexml|=xml_schema::flags::dont_validate;
  }
//------------------------------------------------------------------------------
MzIDIO::MzIDIO() {
  outputfileending="";
  warning=true;
  }
//------------------------------------------------------------------------------
void MzIDIO::setOutputFileEnding(string fileending) {
  outputfileending=fileending;
  }
//------------------------------------------------------------------------------
void MzIDIO::unsetWarningFlag() {
  warning=false;
  }
//------------------------------------------------------------------------------
void MzIDIO::setFilename(string filename) {
  this->filename.push_back(filename);
  }
//------------------------------------------------------------------------------
bool MzIDIO::addFilenames(string filenamefile) {
  ifstream fp1;
  string s1;

  if (!boost::filesystem::exists(filenamefile.c_str()))
    return false;
  fp1.open(filenamefile.c_str());
  while (!fp1.eof()) {
    getline(fp1,s1);
    if (s1.length()==0)
      continue;
    setFilename(s1);
    }
  fp1.close();
  return true;
  }
//------------------------------------------------------------------------------
bool MzIDIO::checkFilenames() {
  int i1;
  bool b1;

  b1=true;
  for (i1=0; i1<filename.size(); i1++)
    if (!boost::filesystem::exists(filename[i1].c_str())) {
      ErrorReporter::throwError(ErrorReporter::TEXT::NO_MZID_FILE,filename[i1]);
      b1=false;
      }
  return b1 || warning;
  }
//------------------------------------------------------------------------------
string MzIDIO::getUniqueFilename() {
  return filename.size()==1?filename[0]:"";
  }
//------------------------------------------------------------------------------
bool MzIDIO::FileOutput() {
  return outputfileending.length()>0;
  }
//------------------------------------------------------------------------------
string MzIDIO::setOutputFileName(int mzidfilenameid) {
  boost::filesystem::path filepath(filename[mzidfilenameid]);
  return filepath.replace_extension("").string()+outputfileending+filepath.extension().string();
  }
//------------------------------------------------------------------------------
bool MzIDIO::insertMZIDValues(boost::unordered_map<PercolatorOutFeatures, string, PercolatorOutFeatures> pout_values) {
  int i1,vi1,n;
  mzidXML::AnalysisDataType::SpectrumIdentificationList_iterator listit;
  mzidXML::SpectrumIdentificationListType::SpectrumIdentificationResult_iterator resultit;
  mzidXML::SpectrumIdentificationResultType::SpectrumIdentificationItem_iterator itemit;
  ifstream fpi;

  n=0;
  try {
    for (vi1=0; vi1<filename.size(); vi1++) {
      fpi.open(filename[vi1].c_str(),ifstream::in);
      auto_ptr<mzidXML::MzIdentMLType> pmzid (mzidXML::MzIdentML(fpi, validatexml));
      for (listit=pmzid->DataCollection().AnalysisData().SpectrumIdentificationList().begin();
           listit!=pmzid->DataCollection().AnalysisData().SpectrumIdentificationList().end(); listit++)
        for (resultit=listit->SpectrumIdentificationResult().begin();
             resultit!=listit->SpectrumIdentificationResult().end(); resultit++)
          for (itemit=resultit->SpectrumIdentificationItem().begin();
               itemit!=resultit->SpectrumIdentificationItem().end(); itemit++) {
            for (i1=0; i1<ARRAYSIZE(MZID_PARAM::ELEMENT_DATA::ELEMENTS); i1++) {
              if (pout_values.find(PercolatorOutFeatures(filename[vi1],itemit->id(),i1))==pout_values.end())
                continue;
              n++;
              switch (MZID_PARAM::ELEMENT_DATA::ELEMENTS[i1]) {
                case MZID_PARAM::USERPARAM: {
                  auto_ptr<mzidXML::UserParamType> newuserparam (new mzidXML::UserParamType(MZID_PARAM::ELEMENT_DATA::NAMES[i1]));
                  newuserparam->value(pout_values[PercolatorOutFeatures(filename[vi1],itemit->id(),i1)]);
                  itemit->userParam().push_back(newuserparam);
                  break;
                  }
                case MZID_PARAM::CVPARAM: {
                  auto_ptr<mzidXML::CVParamType> newcvparam (new mzidXML::CVParamType(MZID_PARAM::ELEMENT_DATA::NAMES[i1],
                                                                                      MZID_PARAM::ELEMENT_DATA::CVREFS[i1],
                                                                                      MZID_PARAM::ELEMENT_DATA::ACCESSIONS[i1]));
                  newcvparam->value(pout_values[PercolatorOutFeatures(filename[vi1],itemit->id(),i1)]);
                  itemit->cvParam().push_back(newcvparam);
                  break;
                  }
                }
              pout_values.erase(PercolatorOutFeatures(filename[vi1],itemit->id(),i1));
              }
            }
      fpi.close();
      if (!saveMZIDFile(pmzid,vi1)) {
        return false;
        }
      }
    if (FileOutput()) {
      printf(STATUS::TEXT::INSERTED,n);
      for (boost::unordered_map<PercolatorOutFeatures, string, PercolatorOutFeatures>::iterator
          it=pout_values.begin(); it!=pout_values.end(); it++)
        cout << "Warning: " << it->first.filename << " psm_id: " << it->first.psmid << " not entered." << endl;
      }
    return true;
    }
  catch (xml_schema::expected_element &e) {
    cerr << e << endl;
    fpi.close();
    return false;
    }
  catch(xml_schema::exception &e) {
    cerr << e << endl;
    fpi.close();
    return false;
    }
  catch (exception &e) {
    ErrorReporter::throwError(ErrorReporter::TEXT::CANNOT_INSERT,e);
    fpi.close();
    return false;
    }
  }
//------------------------------------------------------------------------------
bool MzIDIO::saveMZIDFile(auto_ptr<mzidXML::MzIdentMLType> &pmzid, int mzidfilenameid) {
  xml_schema::namespace_infomap map;
  ofstream ofs;

  try {
    map[""].name=MZID_PARAM::SCHEMA_NAME;
    map[""].schema=MZID_PARAM::SCHEMA;
    if (FileOutput())
      ofs.open(setOutputFileName(mzidfilenameid).c_str());
    mzidXML::MzIdentML(ofs.is_open()?ofs:cout, *pmzid, map,"UTF-8",validatexml);
    ofs.close();
    return true;
    }
  catch(exception &e) {
    ofs.close();
    ErrorReporter::throwError(ErrorReporter::TEXT::CANNOT_SAVE,e);
    return false;
    }
  }
//------------------------------------------------------------------------------
MzIDIO::~MzIDIO() {
  filename.clear();
  }
//------------------------------------------------------------------------------
PercolatorOutI::PercolatorOutI() {
  filename="";
  uniquemzidfilename="";
  decoy=false;
  }
//------------------------------------------------------------------------------
bool PercolatorOutI::setFilename(string filename) {
  this->filename=filename;
  return boost::filesystem::exists(filename.c_str());
  }
//------------------------------------------------------------------------------
void PercolatorOutI::setUniqueMzIDFilename(string filename) {
  uniquemzidfilename=filename;
  }
//------------------------------------------------------------------------------
bool PercolatorOutI::noFilename() {
  return filename.length()==0;
  }
//------------------------------------------------------------------------------
void PercolatorOutI::setDecoy() {
  decoy=true;
  }
//------------------------------------------------------------------------------
bool PercolatorOutI::getPoutValues(boost::unordered_map<PercolatorOutFeatures, string, PercolatorOutFeatures> &pout_values, bool writemsg) {
  string psmid,psmidfile;
  poutXML::psms::psm_const_iterator psmit;
  poutXML::peptides::peptide_const_iterator peptideit;
  poutXML::psm_ids::psm_id_const_iterator psmidsit;
  ifstream fpi;

  try {
    fpi.open(filename.c_str(),ifstream::in);
    auto_ptr<poutXML::percolator_output> ppout (poutXML::percolator_output_(fpi,validatexml));
    for (psmit=ppout->psms().psm().begin(); psmit!=ppout->psms().psm().end(); psmit++) {
      if (decoy!=boost::lexical_cast<bool>(psmit->decoy()))
        continue;
      psmid=convertPSMID(psmit->psm_id());
      if (psmid.length()==0) {
        ErrorReporter::throwError(ErrorReporter::TEXT::WRONG_FORMAT_PSM,psmid);
        fpi.close();
        return false;
        }
      psmidfile=convertPSMIDFileName(psmit->psm_id());
      if (psmidfile.length()==0) {
        ErrorReporter::throwError(ErrorReporter::TEXT::NO_UNIQUE_MZID_FILE,psmid);
        fpi.close();
        return false;
        }
      pout_values[PercolatorOutFeatures(psmidfile,psmid,PERCOLATOR_PARAM::SVM_SCORE)]=global::to_string(psmit->svm_score());
      pout_values[PercolatorOutFeatures(psmidfile,psmid,PERCOLATOR_PARAM::Q_VALUE)]=global::to_string(psmit->q_value());
      pout_values[PercolatorOutFeatures(psmidfile,psmid,PERCOLATOR_PARAM::PEP)]=global::to_string(psmit->pep());
      }
    if (writemsg)
      printf(STATUS::TEXT::READ_PSM,(int)pout_values.size());
    for (peptideit=ppout->peptides()->peptide().begin(); peptideit!=ppout->peptides()->peptide().end(); peptideit++) {
      if (decoy!=boost::lexical_cast<bool>(peptideit->decoy()))
        continue;
      for (psmidsit=peptideit->psm_ids().psm_id().begin(); psmidsit!=peptideit->psm_ids().psm_id().end(); psmidsit++) {
        psmid=convertPSMID(*psmidsit);
        if (psmid.length()==0) {
          ErrorReporter::throwError(ErrorReporter::TEXT::WRONG_FORMAT_PSM,psmid);
          fpi.close();
          return false;
          }
        psmidfile=convertPSMIDFileName(*psmidsit);
        if (psmidfile.length()==0) {
          ErrorReporter::throwError(ErrorReporter::TEXT::NO_UNIQUE_MZID_FILE,psmid);
          fpi.close();
          return false;
          }
        pout_values[PercolatorOutFeatures(psmidfile,psmid,PERCOLATOR_PARAM::PEPTIDE_Q_VALUE)]=global::to_string(peptideit->q_value());
        pout_values[PercolatorOutFeatures(psmidfile,psmid,PERCOLATOR_PARAM::PEPTIDE_PEP)]=global::to_string(peptideit->pep());
        }
      }
    if (writemsg)
      printf(STATUS::TEXT::TOTAL_READ,(int)pout_values.size());
    fpi.close();
    return true;
    }
  catch (xml_schema::expected_element &e) {
    cerr << e << endl;
    fpi.close();
    return false;
    }
  catch(xml_schema::exception &e) {
    cerr << e << endl;
    fpi.close();
    return false;
    }
  catch (exception &e) {
    ErrorReporter::throwError(ErrorReporter::TEXT::CANNOT_LOAD_PERCOLATOR_FILE,e);
    fpi.close();
    return false;
    }
  }
//------------------------------------------------------------------------------
string PercolatorOutI::convertPSMIDFileName(string percolatorid) {
  string psmidfile;
  int i1;

  psmidfile=uniquemzidfilename;
  i1=percolatorid.find(PERCOLATOR_PARAM::PSMID_START);
  if (i1!=string::npos)
    psmidfile=percolatorid.substr(0,i1)+MZID_PARAM::FILE_EXTENSION;
  return psmidfile;
  }
//------------------------------------------------------------------------------
string PercolatorOutI::convertPSMID(string percolatorid) {
  int i1,i2;

  i2=percolatorid.find(PERCOLATOR_PARAM::PSMID_START);
  for (i1=0; i1<PERCOLATOR_PARAM::N_DELIMINATOR_PSM_ID; i1++)
    if ((i2=percolatorid.find("_",i2+1))==string::npos) {
      ErrorReporter::throwError(ErrorReporter::TEXT::WRONG_FORMAT_PSM,percolatorid);
      return "";
      }
  return percolatorid.substr(0,i2);
  }
//------------------------------------------------------------------------------

