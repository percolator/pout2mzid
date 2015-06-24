
POUT2MZID
=========

This software adds the calculated values from percolator, that are stored
in the percolator output file, and adds this information to a mzIdentML
file. The software either outputs the new mzIdentMl file to standard output
or to a file of your choosing.
Decoy status flag filters the data.
This version also acts as a stream parser, meaning the XML files will not
be stored in memory, but rather stream from/to XML files.

Building (from source bundle)
=============================

**READ THIS CAREFULLY**
you MUST install the required libraries!
The following instructions are for Ubuntu (versions 10.04+).

Dependencies
------------

:C++ compiler: Works well with the GNU compiler
:CMake: version 2.8.7
:CodeSynthesis XSD: version 3.3.0+
  (Supports percolator XSD version 1.5)
:Xerces C++: version 2.8.0+
:Boost: Version 1.36+

Building procedure
------------------

In order to build just run::

  cd [binary dir]
  cmake [binary dir]/CMakeLists.txt
  make

Files
=====

XMLIO.h file contains constants for all XML path, values and attributes
that are used within the software.
For future upgrade or XML format change, these constants can be changed, which
will change all values within the software.

Options
=======

:-h [ --help ]: Displays available commands
:-p [ --percolatorfile ] [Value]: Percolator Out XML result file
:-m [ --mzidfile ] [Value]: MzIdentML input file
:-i [ --inputdir ] [Value]: Sets the mzIdentML input directory. All 
  mzIdentML inputfiles must be in that 
  directory
:-c [ --changeoutput ] [Value]: Change the outputfile to original 
  filename+[Value]+.mzid.
  DEFAULT: output to stdout
:-o [ --outputdir ] [Value]: Sets the output directory if none exist, it 
  will be created.
:-f [ --filesmzid ] [Value]: File containing a list of mzIdentML filenames
:-d [ --decoy ]: Only adds results to entries with decoy set 
  to true. DEFAULT: false
:-v [ --validate ]: Sets that validation of XML schema should not
  be performed. Faster parsing.
:-w [ --warning ]: Sets that upon warning the software should 
  terminate.

Options are also avialable with the command *pout2mzid -h*

Format
======

pout2mzid recognizes the following formats...

Percolator OUT files
--------------------

In percolator out files *pout2mzid* searches for entries like::

  <psms>
    <psm p:psm_id="MZIDFILENAME_SII_XXXX_Y_SOMETHING">
  </psms>
  ...
  <peptide>
    <psm_id>MZIDFILENAME_SII_XXXX_Y_SOMETHING</psm_id>
  </peptide>  

In short, the software searches for occurrence of *_SII* and stores
as PSMID up until 3 underlines or up until the end of the line.
In the example it finds SII_XXXX_Y as the PSMID.

pout2mzid is also capable to take one percolator out file and add this information to
multiple MZID files. The name of the MZID files should be entered prior of the psm id (see MZIDFILENAME above)
but without the filename extension.

The information that is retreived from this file and the entered into the MZID file is...

======= ==========
Tag     Value type
======= ==========
psm     p_value
psm     q_value
psm     pep
peptide q_value
peptide pep
======= ==========

MZID files
----------

In MZID the format looks like::

  <DataCollection xmlns="http://psidev.info/psi/pi/mzIdentML/1.1">
      <AnalysisData>
          <SpectrumIdentificationList id="...">
              <SpectrumIdentificationResult spectraData_ref="..." spectrumID="..." id="...">
                  <SpectrumIdentificationItem passThreshold="true" rank="1" peptide_ref="..." id="SII_XXXX_Y">
              </SpectrumIdentificationResult>
          </SpectrumIdentificationList>
      </AnalysisData>
   </DataCollection>

pout2mzid will go through this file and add retrieved information from the percolator out file to
the entries with the psm ID SII_XXXX_Y

