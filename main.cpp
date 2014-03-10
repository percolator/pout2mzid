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

#include "version_config.h"
#include "xmlio.h"

//------------------------------------------------------------------------------
PercolatorOutI percolator;
MzIDIO mzid;
boost::unordered_map<PercolatorOutFeatures, string, PercolatorOutFeatures> pout_values;
//------------------------------------------------------------------------------
void CleanUp(bool exitvalue) {
  pout_values.clear();
  xercesc::XMLPlatformUtils::Terminate();
  exit(exitvalue);
  }
//------------------------------------------------------------------------------
int main(int argc, char **argv) {
  int opt;

  try {
    while ((opt=getopt(argc,argv,"o::m:p:f:vdhw")) != EOF)
      switch(opt) {
        case 'd':
          percolator.setDecoy();
          break;
        case 'v':
          percolator.unsetValidation();
          mzid.unsetValidation();
          break;
        case 'o':
          mzid.setOutputFileEnding(optarg?optarg:MZID_PARAM::FILE_END_DEFAULT);
          break;
        case 'p':
          if (!percolator.setFilename(optarg))
            THROW_ERROR_VALUE(PRINT_TEXT::NO_PERCOLATOR_FILE,optarg);
          break;
        case 'm':
          mzid.setFilename(optarg);
          break;
        case 'f':
          if (!mzid.addFilenames(optarg))
            THROW_ERROR_VALUE(PRINT_TEXT::NO_MZID_FILE,optarg);
          break;
        case 'w':
          mzid.unsetWarningFlag();
          break;
        case 'h':
        case '?':
          printVersion();
          cout << PRINT_TEXT::HELP << endl;
          CleanUp(EXIT_SUCCESS);
          break;
        default:
          CleanUp(EXIT_SUCCESS);
        }
    if (!mzid.checkFilenames())
      CleanUp(EXIT_FAILURE);
    if (percolator.noFilename())
      THROW_ERROR(PRINT_TEXT::PERCOLATOR_FILE_NOT_ENTERED);
    xercesc::XMLPlatformUtils::Initialize();
    percolator.setUniqueMzIDFilename(mzid.getUniqueFilename());
    if (!percolator.getPoutValues(pout_values,mzid.FileOutput()))
      THROW_ERROR(PRINT_TEXT::CANNOT_LOAD_PERCOLATOR_FILE);
    if (!mzid.insertMZIDValues(pout_values))
      THROW_ERROR(PRINT_TEXT::CANNOT_INSERT);
    CleanUp(EXIT_SUCCESS);
    }
  catch(exception &e) {
    cout << e.what() << endl;
    CleanUp(EXIT_FAILURE);
    }
  }
//------------------------------------------------------------------------------

