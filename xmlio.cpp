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
XMLIO::XMLIO() {
  filename="";
  outputfilename="";
  decoy=false;
  }
//------------------------------------------------------------------------------
bool XMLIO::setFilename(string filename) {
  this->filename=filename;
  return boost::filesystem::exists(filename.c_str());
  }
//------------------------------------------------------------------------------
void XMLIO::setOutputFilename(string filename) {
  this->outputfilename=filename;
  }
//------------------------------------------------------------------------------
void XMLIO::setDecoy() {
  decoy=true;
  }
//------------------------------------------------------------------------------
bool XMLIO::getPoutValues(boost::unordered_map<pair<string, int>, double> &pout_values, bool writemsg) {
  string psmid;
  poutXML::psms::psm_const_iterator psmit;
  poutXML::peptides::peptide_const_iterator peptideit;
  poutXML::psm_ids::psm_id_const_iterator psmidsit;

  try {
    auto_ptr<poutXML::percolator_output> ppout (poutXML::percolator_output_(filename, xml_schema::flags::dont_validate));
    for (psmit=ppout->psms().psm().begin(); psmit!=ppout->psms().psm().end(); psmit++) {
      if (decoy!=boost::lexical_cast<bool>(psmit->decoy()))
        continue;
      psmid=convertPSMID(psmit->psm_id());
      if (psmid.length()==0) {
        ErrorReporter::throwError(ErrorReporter::TEXT::WRONG_FORMAT_PSM,psmid);
        return false;
        }
      pout_values[make_pair(psmid,PERCOLATOR_PARAM::VALUES::SVM_SCORE)]=boost::lexical_cast<double>(psmit->svm_score());
      pout_values[make_pair(psmid,PERCOLATOR_PARAM::VALUES::Q_VALUE)]=boost::lexical_cast<double>(psmit->q_value());
      pout_values[make_pair(psmid,PERCOLATOR_PARAM::VALUES::PEP)]=boost::lexical_cast<double>(psmit->pep());
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
          return false;
          }
        pout_values[make_pair(psmid,PERCOLATOR_PARAM::VALUES::PEPTIDE_Q_VALUE)]=boost::lexical_cast<double>(peptideit->q_value());
        pout_values[make_pair(psmid,PERCOLATOR_PARAM::VALUES::PEPTIDE_PEP)]=boost::lexical_cast<double>(peptideit->pep());
        }
      }
    if (writemsg)
      printf(STATUS::TEXT::TOTAL_READ,(int)pout_values.size());
    return true;
    }
  catch (xml_schema::expected_element &e) {
    cerr << e << endl;
    return false;
    }
  catch(xml_schema::exception &e) {
    cerr << e << endl;
    return false;
    }
  catch (exception &e) {
    ErrorReporter::throwError(ErrorReporter::TEXT::CANNOT_LOAD_PERCOLATOR_FILE,e);
    return false;
    }
  }
//------------------------------------------------------------------------------
bool XMLIO::insertMZIDValues(boost::unordered_map<pair<string, int>, double> pout_values) {
  int i1,n;
  mzidXML::AnalysisDataType::SpectrumIdentificationList_iterator listit;
  mzidXML::SpectrumIdentificationListType::SpectrumIdentificationResult_iterator resultit;
  mzidXML::SpectrumIdentificationResultType::SpectrumIdentificationItem_iterator itemit;

  n=0;
  try {
    auto_ptr<mzidXML::MzIdentMLType> pmzid (mzidXML::MzIdentML(filename));
    for (listit=pmzid->DataCollection().AnalysisData().SpectrumIdentificationList().begin();
         listit!=pmzid->DataCollection().AnalysisData().SpectrumIdentificationList().end(); listit++)
      for (resultit=listit->SpectrumIdentificationResult().begin();
           resultit!=listit->SpectrumIdentificationResult().end(); resultit++)
        for (itemit=resultit->SpectrumIdentificationItem().begin();
             itemit!=resultit->SpectrumIdentificationItem().end(); itemit++) {
          for (i1=0; i1<ARRAYSIZE(MZID_PARAM::ELEMENTS); i1++) {
            if (pout_values.find(make_pair(itemit->id(),i1))==pout_values.end())
              continue;
            n++;
            switch (MZID_PARAM::ELEMENTS[i1]) {
              case MZID_PARAM::USERPARAM: {
                auto_ptr<mzidXML::UserParamType> newuserparam (new mzidXML::UserParamType(MZID_PARAM::ATTRIBUTES::NAMES[i1]));
                newuserparam->value(global::to_string(pout_values[make_pair(itemit->id(),i1)]));
                itemit->userParam().push_back(newuserparam);
                break;
                }
              case MZID_PARAM::CVPARAM: {
                auto_ptr<mzidXML::CVParamType> newcvparam (new mzidXML::CVParamType(MZID_PARAM::ATTRIBUTES::NAMES[i1],
                                                                                    MZID_PARAM::ATTRIBUTES::CVREFS[i1],
                                                                                    MZID_PARAM::ATTRIBUTES::ACCESSIONS[i1]));
                newcvparam->value(global::to_string(pout_values[make_pair(itemit->id(),i1)]));
                itemit->cvParam().push_back(newcvparam);
                break;
                }
              }
            }
          }
    if (!saveMZIDFile(pmzid))
      return false;
    if (FileOutput())
      printf(STATUS::TEXT::INSERTED,n);
    return true;
    }
  catch (xml_schema::expected_element &e) {
    cerr << e << endl;
    return false;
    }
  catch(xml_schema::exception &e) {
    cerr << e << endl;
    return false;
    }
  catch (exception &e) {
    ErrorReporter::throwError(ErrorReporter::TEXT::CANNOT_INSERT,e);
    return false;
    }
  }
//------------------------------------------------------------------------------
bool XMLIO::saveMZIDFile(auto_ptr<mzidXML::MzIdentMLType> &pmzid) {
  xml_schema::namespace_infomap map;

  try {
    map[""].name=MZID_PARAM::SCHEMA_NAME;
    map[""].schema=MZID_PARAM::SCHEMA;
    if (outputfilename.length()!=0) {
      ofstream ofs(outputfilename.c_str());
      mzidXML::MzIdentML(ofs, *pmzid, map);
      }
    else
      mzidXML::MzIdentML(cout, *pmzid, map);
    return true;
    }
  catch(exception &e) {
    ErrorReporter::throwError(ErrorReporter::TEXT::CANNOT_SAVE,e);
    return false;
    }
  }
//------------------------------------------------------------------------------
string XMLIO::convertPSMID(string percolatorid) {
  int i1,i2;

  for (i1=0, i2=-1; i1<PERCOLATOR_PARAM::N_DELIMINATOR_PSM_ID; i1++)
    if ((i2=percolatorid.find("_",i2+1))==string::npos) {
      ErrorReporter::throwError(ErrorReporter::TEXT::WRONG_FORMAT_PSM,percolatorid);
      return "";
      }
  return percolatorid.substr(0,i2);
  }
//------------------------------------------------------------------------------
bool XMLIO::checkDecoy(string decoyattribute) {
  return (decoyattribute.compare("true")==0)==decoy;
  }
//------------------------------------------------------------------------------
bool XMLIO::FileOutput() {
  return outputfilename.length()>0;
  }
//------------------------------------------------------------------------------
XMLIO::~XMLIO() {
  }
//------------------------------------------------------------------------------
