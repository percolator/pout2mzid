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

#ifndef XMLIO_H
#define XMLIO_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/unordered_map.hpp>
#include <boost/format.hpp>

#include "global.h"
#include "percolator_out.hxx"
#include "mzIdentML1.1.0.hxx"

//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
// Namespaces containing parameters for mzID and percolator out files
// in case future version of MZID, and percolator out parameters change
//------------------------------------------------------------------------------
namespace MZID_PARAM {
  const char SCHEMA_NAME[]="http://psidev.info/psi/pi/mzIdentML/1.1";
  const char SCHEMA[]="mzIdentML1.1.0.xsd";
  const char FILE_END_DEFAULT[]="_output";
  const char FILE_EXTENSION[]=".mzid";
  enum ELEMENT_TYPE { USERPARAM,CVPARAM };

  namespace ELEMENT_DATA {
    const int ELEMENTS[]={
      USERPARAM,CVPARAM,CVPARAM,CVPARAM,USERPARAM
      };
    const string ACCESSIONS[]={
      "","MS:1002054","MS:1002056","MS:1002055",""
      };
    const string CVREFS[]={
      "","PSI-PI","PSI-PI","PSI-PI",""
      };
    const string NAMES[]={
      "svm-score","MS-GF:QValue","MS-GF:PEP","MS-GF:PepQValue","peptide-level-PEP"
      };
    }
  }
//------------------------------------------------------------------------------
namespace PERCOLATOR_PARAM {
  const int N_DELIMINATOR_PSM_ID=3;
  const char PSMID_START[]="_SII";
  enum VALUES { SVM_SCORE,Q_VALUE,PEP,PEPTIDE_Q_VALUE,PEPTIDE_PEP };
  }
//------------------------------------------------------------------------------
class PercolatorOutFeatures {
  public:
  string filename;
  string psmid;
  int parameter;

  PercolatorOutFeatures();
  PercolatorOutFeatures(string filename, string psmid, int parameter);
  bool operator==(PercolatorOutFeatures const& p) const;
  size_t operator()(PercolatorOutFeatures const& p) const;
  };
//------------------------------------------------------------------------------
class XMLIO {
  protected:
  long validatexml;

  public:
  XMLIO();
  void unsetValidation();
  };
//------------------------------------------------------------------------------
class MzIDIO : public XMLIO {
  private:
  vector<string> filename;
  string outputfileending;
  bool warning;

  public:
  MzIDIO();
  void setOutputFileEnding(string fileending);
  void unsetWarningFlag();
  void setFilename(string filename);
  bool addFilenames(string filenamefile);
  bool checkFilenames();
  string getUniqueFilename();
  bool FileOutput();
  string setOutputFileName(int mzidfilenameid);
  bool insertMZIDValues(boost::unordered_map<PercolatorOutFeatures, string, PercolatorOutFeatures> pout_values);
  bool saveMZIDFile(auto_ptr<mzidXML::MzIdentMLType> &pmzid, int mzidfilenameid);
  ~MzIDIO();
  };
//------------------------------------------------------------------------------
class PercolatorOutI : public XMLIO {
  private:
  string filename,uniquemzidfilename;
  bool decoy;

  public:
  PercolatorOutI();
  bool setFilename(string filename);
  void setUniqueMzIDFilename(string filename);
  bool noFilename();
  void setDecoy();
  bool getPoutValues(boost::unordered_map<PercolatorOutFeatures, string, PercolatorOutFeatures> &pout_values, bool writemsg);
  string convertPSMIDFileName(string percolatorid);
  static string convertPSMID(string percolatorid);
  };
//------------------------------------------------------------------------------
#endif // XMLIO_H
