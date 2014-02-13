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

#include "global.h"
#include "percolator_out.hxx"
#include "mzIdentML1.1.0.hxx"

//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
class XMLIO {
  public:
  XMLIO();
  bool setFilename(string filename);
  void setOutputFilename(string filename);
  void setDecoy();
  bool getPoutValues(boost::unordered_map<pair<string, int>, double> &pout_values, bool writemsg);
  bool insertMZIDValues(boost::unordered_map<pair<string, int>, double> pout_values);
  bool saveMZIDFile(auto_ptr<mzidXML::MzIdentMLType> &pmzid);
  static string convertPSMID(string percolatorid);
  bool checkDecoy(string decoyattribute);
  bool FileOutput();
  ~XMLIO();

  private:
  string filename;
  string outputfilename;
  bool decoy;
  };
//------------------------------------------------------------------------------
// Namespaces containing parameters for mzID and percolator out files
// in case future version of MZID, and percolator out parameters change
//------------------------------------------------------------------------------
namespace MZID_PARAM {
  const int USERPARAM=0;
  const int CVPARAM=1;
  const char SCHEMA_NAME[]="http://psidev.info/psi/pi/mzIdentML/1.1";
  const char SCHEMA[]="mzIdentML1.1.0.xsd";

  const int ELEMENTS[]={
    USERPARAM,CVPARAM,CVPARAM,CVPARAM,USERPARAM
    };

  namespace ATTRIBUTES {
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
  namespace VALUES {
    const int SVM_SCORE=0;
    const int Q_VALUE=1;
    const int PEP=2;
    const int PEPTIDE_Q_VALUE=3;
    const int PEPTIDE_PEP=4;
    }
  }
//------------------------------------------------------------------------------
#endif // XMLIO_H
