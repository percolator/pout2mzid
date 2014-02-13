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
XMLIO percolator,mzid;
boost::unordered_map<pair<string, int>, double> pout_values;
//------------------------------------------------------------------------------
void CleanUp(bool exitvalue) {
  pout_values.clear();
  exit(exitvalue);
  }
//------------------------------------------------------------------------------
int main(int argc, char **argv) {
  int i1;

  if (argc<global::N_MAIN_ARGUMENT) {
    printVersion ();
    ErrorReporter::throwError(ErrorReporter::TEXT::HELP);
    CleanUp(EXIT_SUCCESS);
    }
  for (i1=0; i1<argc; i1++) {
    if (strcmp(argv[i1],global::DECOY_COMMAND)==0)
      percolator.setDecoy();
    if (strcmp(argv[i1],global::OUTPUTFILE_COMMAND)==0 && (i1+1)<argc)
      mzid.setOutputFilename(argv[i1+1]);
    }
  if (!percolator.setFilename(argv[1])) {
    ErrorReporter::throwError(ErrorReporter::TEXT::NO_PERCOLATOR_FILE,argv[1]);
    CleanUp(EXIT_FAILURE);
    }
  if (!mzid.setFilename(argv[2])) {
    ErrorReporter::throwError(ErrorReporter::TEXT::NO_MZID_FILE,argv[2]);
    CleanUp(EXIT_FAILURE);
    }
  if (!percolator.getPoutValues(pout_values,mzid.FileOutput())) {
    ErrorReporter::throwError(ErrorReporter::TEXT::CANNOT_LOAD_PERCOLATOR_FILE);
    CleanUp(EXIT_FAILURE);
    }
  if (!mzid.insertMZIDValues(pout_values)) {
    ErrorReporter::throwError(ErrorReporter::TEXT::CANNOT_INSERT);
    CleanUp(EXIT_FAILURE);
    }
  CleanUp(EXIT_SUCCESS);
  }
//------------------------------------------------------------------------------

