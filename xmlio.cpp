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
#include "pout_parser.h"

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
bool PercolatorOutFeatures::operator==(PercolatorOutFeatures const &p) const {
  return this->filename.compare(p.filename)==0 &&
         this->psmid.compare(p.psmid)==0 &&
         this->parameter==p.parameter;
  }
//------------------------------------------------------------------------------
size_t PercolatorOutFeatures::operator()(PercolatorOutFeatures const &p) const {
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
  outputdir="";
  }
//------------------------------------------------------------------------------
void MzIDIO::setOutputFileEnding(string fileending) {
  this->outputfileending=fileending;
  }
//------------------------------------------------------------------------------
bool MzIDIO::setOutputDirectory(string outputdir) {
  if (outputfileending.length()==0) {
    cerr << boost::format(PRINT_TEXT::NO_FILE_ENDING) << endl;
    return false;
    }
  this->outputdir=outputdir;
  if (!boost::filesystem::is_directory(outputdir.c_str()))
    return boost::filesystem::create_directory(outputdir.c_str());
  return true;
  }
//------------------------------------------------------------------------------
string MzIDIO::setOutputFileName(string filename) {
  boost::filesystem::path filepath(filename);
  if (outputdir.length()>0)
    filepath=boost::lexical_cast<boost::filesystem::path>(outputdir)/filepath.filename();
  return filepath.replace_extension("").string()+outputfileending+filepath.extension().string();
  }
//------------------------------------------------------------------------------
bool MzIDIO::insertMZIDValues(boost::unordered_map<PercolatorOutFeatures, string, PercolatorOutFeatures> &pout_values,
                              vector <string> &filenames, bool multiplemzidfiles) {
  ifstream fpr;
  ofstream fpw;
  ostream *wout;
  string mzidname,s1,psmid;
  int i1,vi1,n,xmlindent;

  n=0;
  psmid="";
  try {
    for (vi1=0; vi1<filenames.size(); vi1++) {
      mzidname=boost::lexical_cast<boost::filesystem::path>(filenames[vi1]).stem().string();
      if (!multiplemzidfiles)
        mzidname="";
      fpr.open(filenames[vi1].c_str());
      if (outputfileending.length()>0) {
        fpw.open(setOutputFileName(filenames[vi1]).c_str());
        wout=&fpw;
        }
      else
        wout=&cout;
      while (getline(fpr,s1)) {
        if (s1.find(MZID_PARAM::END_INSERT_TAG)!=string::npos) {
          if (psmid.length()==0)
            THROW_ERROR_VALUE(PRINT_TEXT::BAD_XML,filenames[vi1]);
          for (i1=0; i1<ARRAYSIZE(MZID_PARAM::ELEMENT_DATA::ELEMENTS); i1++) {
            if (pout_values.find(PercolatorOutFeatures(mzidname,psmid,i1))==pout_values.end())
              continue;
            n++;
            switch (MZID_PARAM::ELEMENT_DATA::ELEMENTS[i1]) {
              case MZID_PARAM::CVPARAM: {
                *wout << boost::format(MZID_PARAM::CVPARAM_TAG) % string(xmlindent,' ')
                        % MZID_PARAM::ELEMENT_DATA::ACCESSIONS[i1]
                        % MZID_PARAM::ELEMENT_DATA::CVREFS[i1]
                        % MZID_PARAM::ELEMENT_DATA::NAMES[i1]
                        % pout_values[PercolatorOutFeatures(mzidname,psmid,i1)];
                break;
                }
              case MZID_PARAM::USERPARAM: {
                *wout << boost::format(MZID_PARAM::USERPARAM_TAG) % string(xmlindent,' ')
                        % MZID_PARAM::ELEMENT_DATA::NAMES[i1]
                        % pout_values[PercolatorOutFeatures(mzidname,psmid,i1)];
                break;
                }
              }
            pout_values.erase(PercolatorOutFeatures(mzidname,psmid,i1));
            }
          psmid="";
          }
        xmlindent=s1.find_first_not_of(" ");
        *wout << s1 << endl;
        if (s1.find(MZID_PARAM::PSMID_TAG)!=string::npos && s1.find(MZID_PARAM::START_INSERT_TAG)!=string::npos) {
          i1=s1.find(MZID_PARAM::PSMID_TAG)+strlen(MZID_PARAM::PSMID_TAG);
          psmid=s1.substr(i1,s1.find("\"",i1)-i1);
          }
        }
      fpr.close();
      fpw.close();
      }
    clog << boost::format(PRINT_TEXT::INSERTED) % n << endl;
    for (boost::unordered_map<PercolatorOutFeatures, string, PercolatorOutFeatures>::iterator
        it=pout_values.begin(); it!=pout_values.end(); it++)
      cerr << boost::format(PRINT_TEXT::PSM_NOT_ENTERED) % it->first.filename % it->first.psmid << endl;
    return true;
    }
  catch (exception &e) {
    cerr << e.what() << endl;
    fpr.close();
    fpw.close();
    return false;
    }
  }
//------------------------------------------------------------------------------
PercolatorOutI::PercolatorOutI() {
  filename="";
  decoy=false;
  multiplemzidfiles=true;
  inputdir="";
  warning=true;
  }
//------------------------------------------------------------------------------
bool PercolatorOutI::setFilename(string filename) {
  this->filename=filename;
  return boost::filesystem::exists(filename.c_str());
  }
//------------------------------------------------------------------------------
bool PercolatorOutI::noFilename() {
  return filename.length()==0;
  }
//------------------------------------------------------------------------------
bool PercolatorOutI::setInputDirectory(string inputdir) {
  this->inputdir=inputdir;
  return boost::filesystem::is_directory(inputdir.c_str());
  }
//------------------------------------------------------------------------------
bool PercolatorOutI::addFilenames(string filename, bool addextension) {
  string filename2;

  filename2=filename;
  if (addextension)
    filename2+=MZID_PARAM::FILE_EXTENSION;
  if (inputdir.length()>0)
    filename2=(boost::lexical_cast<boost::filesystem::path>(inputdir)/
      (boost::lexical_cast<boost::filesystem::path>(filename)).filename()).string();
  if (!boost::filesystem::exists(filename2.c_str())) {
    cerr << boost::format(PRINT_TEXT::NO_MZID_FILE) % filename2 << endl;
    return warning;
    }
  if(find(this->mzidfilenames.begin(),this->mzidfilenames.end(),filename2)==this->mzidfilenames.end())
    this->mzidfilenames.push_back(filename2);
  return true;
  }
//------------------------------------------------------------------------------
void PercolatorOutI::unsetWarningFlag() {
  warning=false;
  }
//------------------------------------------------------------------------------
void PercolatorOutI::setDecoy() {
  decoy=true;
  }
//------------------------------------------------------------------------------
bool PercolatorOutI::checkDecoy(bool decoy) {
  return this->decoy==decoy;
  }
//------------------------------------------------------------------------------
bool PercolatorOutI::getPoutValues() {
  poutXML::psms_pskel psms_p;
  poutXML::peptides_pskel peptides_p;
  poutXML::proteins_pskel proteins_p;
  xml_schema::unsigned_short_pimpl u_short_p;
  poutXML::percolator_output_pskel pout_p;
  poutXML::process_info_pskel process_info_p;
  xml_schema::double_pimpl double_p;
  poutXML::retentionTime_pskel retentionTime_p;
  poutXML::peptide_seq_pskel peptide_seq_p;
  poutXML::aa_seq_t_pimpl aa_seq_t_p;
  xml_schema::string_pimpl string_p;
  xml_schema::boolean_pimpl boolean_p;
  poutXML::psm_ids_pimpl psm_ids_p;
  poutXML::peptide_pimpl peptide_p;
  poutXML::psm_pimpl psm_p;
  poutXML::probability_t_pimpl probability_t_p;

  try {
    psm_p.pre(this,&probability_t_p);
    psm_ids_p.pre(this,&peptide_p);
    peptide_p.pre(&probability_t_p);
    pout_p.parsers (process_info_p,psms_p,peptides_p,proteins_p,
                    string_p,u_short_p,u_short_p);
    psms_p.parsers (psm_p);
    psm_p.parsers (double_p,probability_t_p,probability_t_p,double_p,double_p,retentionTime_p,
                   peptide_seq_p,string_p,probability_t_p,string_p,boolean_p);
    peptides_p.parsers(peptide_p);
    peptide_p.parsers(double_p,probability_t_p,probability_t_p,double_p,double_p,retentionTime_p,
                      string_p,probability_t_p,psm_ids_p,aa_seq_t_p,boolean_p);
    psm_ids_p.parsers(string_p);
    xml_schema::document doc_p(pout_p,PERCOLATOR_PARAM::SCHEMA_NAME,PERCOLATOR_PARAM::HEAD_TAG);
    doc_p.parse(filename.c_str(),validatexml);
    clog << boost::format(PRINT_TEXT::TOTAL_READ) % pout_values.size() << endl;
    return true;
    }
  catch (exception &e) {
    cerr << e.what() << endl;
    return false;
    }
  }
//------------------------------------------------------------------------------
string PercolatorOutI::convertPSMIDFileName(string percolatorid) {
  int i1;

  i1=percolatorid.find(PERCOLATOR_PARAM::PSMID_START);
  if (i1!=string::npos)
    return percolatorid.substr(0,i1);
  else
    return "";
  }
//------------------------------------------------------------------------------
string PercolatorOutI::convertPSMID(string percolatorid) {
  int i1,i2,i3;

  i2=i3=percolatorid.find(PERCOLATOR_PARAM::PSMID_START);
  for (i1=0; i1<PERCOLATOR_PARAM::N_DELIMINATOR_PSM_ID; i1++)
    if ((i2=percolatorid.find('_',i2+1))==string::npos)
      return "";
  return percolatorid.substr(i3+1,i2-i3-1);
  }
//------------------------------------------------------------------------------
PercolatorOutI::~PercolatorOutI() {
  mzidfilenames.clear();
  pout_values.clear();
  }
//------------------------------------------------------------------------------
