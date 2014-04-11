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
#include "boost/program_options.hpp"

//------------------------------------------------------------------------------
namespace prgm_opt=boost::program_options;
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
  prgm_opt::variables_map option_map;
  prgm_opt::options_description options("Options");

  try {
    prgm_opt::arg="[File]";
    options.add_options()
      (CMDOPTIONS::HELP_OPTION,CMDOPTIONS::HELP_TEXT)
      (CMDOPTIONS::PERCOLATORFILE_OPTION,prgm_opt::value<string>()->required(),CMDOPTIONS::PERCOLATORFILE_TEXT)
      (CMDOPTIONS::MZIDFILE_OPTION,prgm_opt::value<string>()->required(),CMDOPTIONS::MZIDFILE_TEXT)
      (CMDOPTIONS::MZIDOUTPUT_OPTION,prgm_opt::value<string>()->required(),CMDOPTIONS::MZIDOUTPUT_TEXT)
      (CMDOPTIONS::MZIDFILES_OPTION,prgm_opt::value<string>()->required(),CMDOPTIONS::MZIDFILES_TEXT)
      (CMDOPTIONS::DECOY_OPTION,CMDOPTIONS::DECOY_TEXT)
      (CMDOPTIONS::VALIDATION_OPTION,CMDOPTIONS::VALIDATION_TEXT)
      (CMDOPTIONS::WARNING_OPTION,CMDOPTIONS::WARNING_TEXT);
    prgm_opt::store(prgm_opt::parse_command_line(argc,argv,options),option_map);
    if (option_map.count(CMDOPTIONS::HELP_TEST)) {
      printVersion();
      cout << options;
      CleanUp(EXIT_SUCCESS);
      }
    if (option_map.count(CMDOPTIONS::DECOY_TEST))
      percolator.setDecoy();
    if (option_map.count(CMDOPTIONS::VALIDATION_TEST)) {
      percolator.unsetValidation();
      mzid.unsetValidation();
      }
    if (option_map.count(CMDOPTIONS::MZIDOUTPUT_TEST))
      mzid.setOutputFileEnding(option_map[CMDOPTIONS::MZIDOUTPUT_TEST].as<string>());
    if (option_map.count(CMDOPTIONS::PERCOLATORFILE_TEST))
      if (!percolator.setFilename(option_map[CMDOPTIONS::PERCOLATORFILE_TEST].as<string>()))
        THROW_ERROR_VALUE(PRINT_TEXT::NO_PERCOLATOR_FILE,option_map[CMDOPTIONS::PERCOLATORFILE_TEST].as<string>());
    if (option_map.count(CMDOPTIONS::MZIDFILE_TEST))
      mzid.setFilename(option_map[CMDOPTIONS::MZIDFILE_TEST].as<string>());
    if (option_map.count(CMDOPTIONS::MZIDFILES_TEST))
      if (!mzid.addFilenames(option_map[CMDOPTIONS::MZIDFILES_TEST].as<string>()))
        THROW_ERROR_VALUE(PRINT_TEXT::NO_MZID_FILE,option_map[CMDOPTIONS::MZIDFILES_TEST].as<string>());
    if (option_map.count(CMDOPTIONS::WARNING_TEST))
       mzid.unsetWarningFlag();
    if (!mzid.checkFilenames())
      CleanUp(EXIT_FAILURE);
    if (percolator.noFilename())
      THROW_ERROR(PRINT_TEXT::PERCOLATOR_FILE_NOT_ENTERED);
    xercesc::XMLPlatformUtils::Initialize();
    percolator.setFirstMzIDFilename(mzid.getFirstFilename());
    if (!percolator.getPoutValues(pout_values))
      THROW_ERROR(PRINT_TEXT::CANNOT_LOAD_PERCOLATOR_FILE);
    if (!mzid.insertMZIDValues(pout_values))
      THROW_ERROR(PRINT_TEXT::CANNOT_INSERT);
    CleanUp(EXIT_SUCCESS);
    }
  catch(exception &e) {
    cerr << e.what() << endl;
    CleanUp(EXIT_FAILURE);
    }
  }
//------------------------------------------------------------------------------

