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
  }
//------------------------------------------------------------------------------
// strings used for reporting error messages and status
//------------------------------------------------------------------------------
namespace PRINT_TEXT {
  const char HELP[]="pout2mzid [OPTIONS]\n"
                    "\nOPTIONS\n\n"
                    "-h\t\t\tDisplays this help text.\n"
                    "-p [PercolatorOut file]\tPercolator Out XML result file\n"
                    "-m [mzIdentML file]\tMzIdentML input file\n"
                    "-o [mzIdentML file end]\tOutputs the results to original filenam+[mzIdentML file end].\n"
                    "\t\t\tIf [mzIdentML file end] is omitted _output will be added to filename. DEFAULT: output to stdout\n"
                    "-f [file]\t\tFile containing a list of mzIdentML filenames\n"
                    "-d\t\t\tOnly adds results to entries with decoy set to true. DEFAULT: false\n"
                    "-v\t\t\tSets that validation of XML schema should not be performed. Faster parsing.\n"
                    "-w\t\t\tSets that upon warning the software should terminate.\n";
  const char PERCOLATOR_FILE_NOT_ENTERED[]="Percolator out file has not been defined\n";
  const char MZID_FILE_NOT_ENTERED[]="mzIdentML file has not been defined\n";
  const char NO_PERCOLATOR_FILE[]="Percolator XML file %s does not exist\n";
  const char NO_MZID_FILE[]="mzIdentML file %s does not exist\n";
  const char CANNOT_LOAD[]="Cannot load file %s\n";
  const char CANNOT_LOAD_PERCOLATOR_FILE[]="Cannot read percolator out file XML Path\n";
  const char BAD_PATH[]="Bad path in file %s\n";
  const char NO_ELEMENT_PERCOLATOR[]="Element %s in percolator_out file not found\n";
  const char WRONG_FORMAT_PSM[]="Psm ID %s in percolator out file has the wrong format\n";
  const char NO_UNIQUE_MZID_FILE[]="Percolator out file does not contain MzID file ID, on mutiple MzID files\n";
  const char CANNOT_INSERT[]="Cannot insert new percolator out values into mzIdentML file\n";
  const char CANNOT_SAVE[]="Cannot save mzIdentML file\n";
  const char CANNOT_READ_MZID[]="Cannot read mzIdentML file XML Path\n";
  const char ELEMENT_NOT_FOUND_MZID[]="Element %s in mzIdentML file not found\n";
  const char PSM_NOT_ENTERED[]="Warning: %s psm_id: %s not entered.\n";
  const char READ_PSM[]="Read %d psm results\n";
  const char READ_PEPTIDE[]="Read %d peptide results\n";
  const char TOTAL_READ[]="Total of %d results\n";
  const char INSERTED[]="Inserted %d new values in the mzIdentML file\n";
  }
//------------------------------------------------------------------------------
#endif // GLOBAL_H
