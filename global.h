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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

#define ARRAYSIZE(array) (sizeof(array)/sizeof(*(array)))
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
// global constants and functions used
//------------------------------------------------------------------------------
namespace global {
  const int N_MAIN_ARGUMENT=3;
  const char OUTPUTFILE_COMMAND[]="-o";
  const char DECOY_COMMAND[]="-d";

  string to_string(double value);
  }
//------------------------------------------------------------------------------
// functions and strings used for reporting error messages
//------------------------------------------------------------------------------
namespace ErrorReporter {
  void throwError(const char *error);
  void throwError(const char *error, exception e);
  void throwError(const char *error, string parameter);
  void throwError(const char *error, exception e, string parameter);
//------------------------------------------------------------------------------
  namespace TEXT {
    const char HELP[]="pout2mzid [Percolator XML file] [mzIdentML file] [OPTIONS]\n"
                      "\nOPTIONS\n\n"
                      "-o [mzIdentML file]\tOutputs the results to this file\n"
                      "-d\t\t\tOnly adds results to entries with decoy set to true. DEFAULT: false\n";
    const char NO_PERCOLATOR_FILE[]="Percolator XML file %s does not exist\n";
    const char NO_MZID_FILE[]="mzIdentML file %s does not exist\n";
    const char CANNOT_LOAD[]="Cannot load file %s\n";
    const char CANNOT_LOAD_PERCOLATOR_FILE[]="Cannot read percolator out file XML Path\n";
    const char BAD_PATH[]="Bad path in file %s\n";
    const char NO_ELEMENT_PERCOLATOR[]="Element %s in percolator_out file not found\n";
    const char WRONG_FORMAT_PSM[]="Psm ID %s in percolator out file has the wrong format\n";
    const char CANNOT_INSERT[]="Cannot insert new percolator out values into mzIdentML file\n";
    const char CANNOT_SAVE[]="Cannot save mzIdentML file\n";
    const char CANNOT_READ_MZID[]="Cannot read mzIdentML file XML Path\n";
    const char ELEMENT_NOT_FOUND_MZID[]="Element %s in mzIdentML file not found\n";
    }
  }
//------------------------------------------------------------------------------
// strings used for status messages
//------------------------------------------------------------------------------
namespace STATUS {
  namespace TEXT {
    const char READ_PSM[]="Read %d psm results\n";
    const char READ_PEPTIDE[]="Read %d peptide results\n";
    const char TOTAL_READ[]="Total of %d results\n";
    const char INSERTED[]="Inserted %d new values in the mzIdentML file\n";
    }
  }
//------------------------------------------------------------------------------
#endif // GLOBAL_H
