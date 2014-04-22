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
#include <stdexcept>

#define ARRAYSIZE(array) (sizeof(array)/sizeof(*(array)))
#define THROW_ERROR(text) throw runtime_error(global::to_string(text))
#define THROW_ERROR_VALUE(text,value) throw runtime_error(global::to_string(boost::format(text) % value))
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
// global constants and functions used
//------------------------------------------------------------------------------
namespace global {
  template<typename T> string to_string(T value) {  //lexical_cast does funny things with double
    stringstream s1;

    s1 << value;
    return s1.str();
    }

  const int MAX_CHAR_SIZE=1000;
  }
//------------------------------------------------------------------------------
// strings used for options
//------------------------------------------------------------------------------
namespace CMDOPTIONS {
  const char HELP_OPTION[]="help,h";
  const char HELP_TEST[]="help";
  const char HELP_TEXT[]="Displays available commands\n";
  const char PERCOLATORFILE_OPTION[]="percolatorfile,p";
  const char PERCOLATORFILE_TEST[]="percolatorfile";
  const char PERCOLATORFILE_TEXT[]="Percolator Out XML result file\n";
  const char MZIDFILE_OPTION[]="mzidfile,m";
  const char MZIDFILE_TEST[]="mzidfile";
  const char MZIDFILE_TEXT[]="MzIdentML input file\n";
  const char MZIDOUTPUT_OPTION[]="output,o";
  const char MZIDOUTPUT_TEST[]="output";
  const char MZIDOUTPUT_TEXT[]="Outputs the results to original filename+[File]+.mzid.\n"
                               "DEFAULT: output to stdout\n";
  const char MZIDFILES_OPTION[]="filesmzid,f";
  const char MZIDFILES_TEST[]="filesmzid";
  const char MZIDFILES_TEXT[]="File containing a list of mzIdentML filenames\n";
  const char DECOY_OPTION[]="decoy,d";
  const char DECOY_TEST[]="decoy";
  const char DECOY_TEXT[]="Only adds results to entries with decoy set to true. DEFAULT: false\n";
  const char VALIDATION_OPTION[]="validate,v";
  const char VALIDATION_TEST[]="validate";
  const char VALIDATION_TEXT[]="Sets that validation of XML schema should not be performed. Faster parsing.\n";
  const char WARNING_OPTION[]="warning,w";
  const char WARNING_TEST[]="warning";
  const char WARNING_TEXT[]="Sets that upon warning the software should terminate.\n";
  }
//------------------------------------------------------------------------------
// strings used for reporting error messages, status
//------------------------------------------------------------------------------
namespace PRINT_TEXT {
  const char PERCOLATOR_FILE_NOT_ENTERED[]="Percolator out file has not been defined";
  const char MZID_FILE_NOT_ENTERED[]="mzIdentML file has not been defined";
  const char NO_PERCOLATOR_FILE[]="Percolator XML file %s does not exist";
  const char NO_MZID_FILE[]="mzIdentML file %s does not exist";
  const char CANNOT_LOAD[]="Cannot load file %s";
  const char CANNOT_LOAD_PERCOLATOR_FILE[]="Cannot read percolator out file XML Path";
  const char BAD_PATH[]="Bad path in file %s";
  const char NO_ELEMENT_PERCOLATOR[]="Element %s in percolator_out file not found";
  const char WRONG_FORMAT_PSM[]="Psm ID %s in percolator out file has the wrong format";
  const char NO_UNIQUE_MZID_FILE[]="Percolator out file does not contain MzID file ID, on mutiple MzID files";
  const char CANNOT_INSERT[]="Cannot insert new percolator out values into mzIdentML file";
  const char CANNOT_SAVE[]="Cannot save mzIdentML file";
  const char CANNOT_READ_MZID[]="Cannot read mzIdentML file XML Path";
  const char ELEMENT_NOT_FOUND_MZID[]="Element %s in mzIdentML file not found.";
  const char PSM_NOT_ENTERED[]="Warning: %s psm_id: %s not entered.";
  const char TOTAL_READ[]="Total of %d results.";
  const char INSERTED[]="Inserted %d new values in the mzIdentML file.";
  }
//------------------------------------------------------------------------------
#endif // GLOBAL_H
