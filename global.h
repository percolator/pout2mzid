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
  const char* const HELP_OPTION[]={"help,h","help","Displays available commands\n"};
  const char* const PERCOLATORFILE_OPTION[]={"percolatorfile,p","percolatorfile","Percolator Out XML result file\n"};
  const char* const MZIDFILE_OPTION[]={"mzidfile,m","mzidfile","MzIdentML input file\n"};
  const char* const MZIDOUTPUT_OPTION[]={"changeoutput,c","changeoutput","Change the outputfile to original filename+[Value]+.mzid.\n"
                                          "DEFAULT: output to stdout\n"};
  const char* const MZIDFILES_OPTION[]={"filesmzid,f","filesmzid","File containing a list of mzIdentML filenames\n"};
  const char* const DECOY_OPTION[]={"decoy,d","decoy","Only adds results to entries with decoy set to true. DEFAULT: false\n"};
  const char* const VALIDATION_OPTION[]={"validate,v","validate","Sets that validation of XML schema should not be performed. Faster parsing.\n"};
  const char* const WARNING_OPTION[]={"warning,w","warning","Sets that upon warning the software should terminate.\n"};
  const char* const OUTPUTDIR_OPTION[]={"outputdir,o","outputdir","Sets the output directory if none exist, it will be created.\n"};
  const char* const INPUTDIR_OPTION[]={"inputdir,i","inputdir","Sets the mzIdentML input directory. All mzIdentML inputfiles must be in that directory\n"};
  }
//------------------------------------------------------------------------------
// strings used for reporting error messages, status
//------------------------------------------------------------------------------
namespace PRINT_TEXT {
  const char PERCOLATOR_FILE_NOT_ENTERED[]="Percolator out file has not been defined";
  const char MZID_FILE_NOT_ENTERED[]="mzIdentML file has not been defined";
  const char NO_PERCOLATOR_FILE[]="Percolator XML file %s does not exist";
  const char NO_MZID_FILE[]="mzIdentML file %s does not exist";
  const char MZIDOUTPUTDIR_NOT_CREATED[]="Outputdirectory did not exist, and could not be created";
  const char MZIDINPUTDIR_NOT_FOUND[]="Input direcotory for mzIdentML file(s) does not exist";
  const char CANNOT_LOAD[]="Cannot load file %s";
  const char CANNOT_LOAD_PERCOLATOR_FILE[]="Cannot read percolator out file XML Path";
  const char BAD_PATH[]="Bad path in file %s";
  const char NO_ELEMENT_PERCOLATOR[]="Element %s in percolator_out file not found";
  const char WRONG_FORMAT_PSM[]="Psm ID %s in percolator out file has the wrong format";
  const char NO_UNIQUE_MZID_FILE[]="Percolator out file does not contain MzID file ID, on mutiple MzID files";
  const char CANNOT_INSERT[]="Cannot insert new percolator out values into mzIdentML file";
  const char BAD_XML[]="Badly formed XML structure in %s mzIdentML file";
  const char CANNOT_SAVE[]="Cannot save mzIdentML file";
  const char CANNOT_READ_MZID[]="Cannot read mzIdentML file XML Path";
  const char ELEMENT_NOT_FOUND_MZID[]="Element %s in mzIdentML file not found.";
  const char PSM_NOT_ENTERED[]="Warning: %s psm_id: %s not entered.";
  const char TOTAL_READ[]="Total of %d results.";
  const char INSERTED[]="Inserted %d new values in the mzIdentML file.";
  }
//------------------------------------------------------------------------------
#endif // GLOBAL_H
