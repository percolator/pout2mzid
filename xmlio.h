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
#include <stdexcept>

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/unordered_map.hpp>
#include <boost/format.hpp>

#include "global.h"
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
// Namespaces containing parameters for mzID and percolator out files
// in case future version of MZID, and percolator out parameters change
// Accession and cvRef are kept for future migration of obtained values
// into mzIdentML Controlled Vocabulary
//------------------------------------------------------------------------------
namespace MZID_PARAM {
  const char SCHEMA_NAME[]="http://psidev.info/psi/pi/mzIdentML/1.1";
  const char SCHEMA[]="mzIdentML1.1.0.xsd";
  const char START_INSERT_TAG[]="<SpectrumIdentificationItem ";
  const char END_INSERT_TAG[]="</SpectrumIdentificationItem>";
  const char PSMID_TAG[]=" id=\"";
  const char CVPARAM_TAG[]="%s<cvparam accession=\"%s\" cvref==\"%s\" name=\"%s\" value=\"%s\"/>\n";
  const char USERPARAM_TAG[]="%s<userparam name=\"%s\" value=\"%s\"/>\n";
  enum ELEMENT_TYPE { USERPARAM,CVPARAM };

  namespace ELEMENT_DATA {
    const int ELEMENTS[]={
      USERPARAM,USERPARAM,USERPARAM,USERPARAM,USERPARAM,USERPARAM
      };
    const string ACCESSIONS[]={
      "","MS:1002054","MS:1002056","MS:1002055","",""
      };
    const string CVREFS[]={
      "","PSI-PI","PSI-PI","PSI-PI","",""
      };
    const string NAMES[]={
      "percolator:score","percolator:psm_p_value","percolator:psm_q_value","percolator:psm_pep","percolator:peptide_q_value","percolator:peptide_pep"
      };
    }
  }
//------------------------------------------------------------------------------
namespace PERCOLATOR_PARAM {
  const char SCHEMA_NAME[]="http://per-colator.com/percolator_out/15";
  const char HEAD_TAG[]="percolator_output";
  const int N_DELIMINATOR_PSM_ID=3;
  const char PSMID_START[]="_SII";
  enum VALUES { SVM_SCORE,P_VALUE,Q_VALUE,PEP,PEPTIDE_Q_VALUE,PEPTIDE_PEP };
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
  string getFirstFilename();
  string setOutputFileName(int mzidfilenameid);
  bool insertMZIDValues(boost::unordered_map<PercolatorOutFeatures, string, PercolatorOutFeatures>& pout_values);
  ~MzIDIO();
  };
//------------------------------------------------------------------------------
class PercolatorOutI : public XMLIO {
  private:
  string filename,firstmzidfilename;
  bool decoy;

  public:
  PercolatorOutI();
  bool setFilename(string filename);
  void setFirstMzIDFilename(string filename);
  bool noFilename();
  void setDecoy();
  bool checkDecoy(bool decoy);
  bool getPoutValues(boost::unordered_map<PercolatorOutFeatures, string, PercolatorOutFeatures>& pout_values);
  string convertPSMIDFileName(string percolatorid);
  static string convertPSMID(string percolatorid);
  };
//------------------------------------------------------------------------------
#endif // XMLIO_H
