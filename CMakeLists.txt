project(pout2mzid)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

###############################################################################
# Set version
###############################################################################

set(POUT2MZIML_VERSION_MAJOR "0")
set(POUT2MZIML_VERSION_MINOR "2")
set(POUT2MZIML_VERSION_PATCH "7")

configure_file( version_config.h.in ${CMAKE_BINARY_DIR}/version_config.h )

###############################################################################
# Check for Dependencies
###############################################################################

# INCLUDE HEADER FILES IN SRC DIRECTORY
include_directories(${CMAKE_CURRENT_BINARY_DIR})

MESSAGE( STATUS "Checking packages:" )

find_package(XercesC REQUIRED) # XSD DEPENDS ON ${XERCESC_FOUND}
if(XERCESC_FOUND)
  message(STATUS "Package Xerces-C++ found: ${XERCESC_INCLUDE_DIR}")
else(XERCESC_FOUND)
  message(FATAL_ERROR "The package Apache Xerces-C++ has not been found")
endif(XERCESC_FOUND)
include_directories(${XERCESC_INCLUDE_DIR})

find_package(Xsd REQUIRED) # CODESYNTHESIS
if(XSD_FOUND)
  message(STATUS "Package CodeSynthesis XSD found: ${XSD_INCLUDE_DIR}")
else(XSD_FOUND)
  message(FATAL_ERROR "The package CodeSynthesis XSD has not been found")
endif(XSD_FOUND)
include_directories(${XSD_INCLUDE_DIR})

SET (Boost_ADDITIONAL_VERSIONS "1.36" "1.36.0" "1.41" "1.41.0" "1.39" "1.39.0" "1.42.0" "1.42" "1.43.0"
             "1.43." "1.44.0" "1.44.0" "1.45.0" "1.45" "1.46.0" "1.46" "1.47.0" "1.47")
find_package(Boost COMPONENTS filesystem system program_options REQUIRED)
if(Boost_FOUND)
  message(STATUS "Package Boost-${Boost_VERSION} found : ${Boost_INCLUDE_DIR}")
else(Boost_FOUND)
  message(FATAL_ERROR "The package Boost has not been found")
endif(Boost_FOUND)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_INCLUDE_DIRS})

###############################################################################
# Check for codesynthesis files
###############################################################################

MESSAGE( STATUS "Creating XSD files:" )

# RUN CODESYNTHESIS TO GENERATE xsdfiles for pout (percolator output)
set(percolator_out ${CMAKE_CURRENT_BINARY_DIR}/percolator_out.cxx ${CMAKE_CURRENT_BINARY_DIR}/percolator_out.hxx )
set(percolator_out_xsd ${CMAKE_CURRENT_SOURCE_DIR}/xml/percolator_out.xsd)
add_custom_command(
  OUTPUT ${percolator_out}
  COMMAND xsdcxx
  ARGS cxx-tree --generate-polymorphic --generate-serialization --root-element-all --generate-ostream --namespace-map http://per-colator.com/percolator_out/15=poutXML ${percolator_out_xsd}
  DEPENDS ${percolator_out_xsd}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# RUN CODESYNTHESIS TO GENERATE xsdfiles for mzIdentML (mzIdentML)
set(mzidentml_out ${CMAKE_CURRENT_BINARY_DIR}/mzIdentML1.1.0.cxx ${CMAKE_CURRENT_BINARY_DIR}/mzIdentML1.1.0.hxx )
set(mzidentml_xsd ${CMAKE_CURRENT_SOURCE_DIR}/xml/mzIdentML1.1.0.xsd)
add_custom_command(
  OUTPUT ${mzidentml_out}
  COMMAND xsdcxx
  ARGS cxx-tree --generate-polymorphic --generate-serialization --root-element-all --generate-ostream --namespace-map http://psidev.info/psi/pi/mzIdentML/1.1=mzidXML ${mzidentml_xsd}
  DEPENDS ${mzidentml_xsd}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

###############################################################################
# Set libraries
###############################################################################

add_library(xsdpout2mzidmllibrary ${percolator_out} ${mzidentml_out})
target_link_libraries(${PROJECT_NAME} xsdpout2mzidmllibrary ${Boost_LIBRARIES} ${XERCESC_LIBRARIES})

