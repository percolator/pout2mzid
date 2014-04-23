// Not copyrighted - public domain.
//
// This sample parser implementation was generated by CodeSynthesis XSD,
// an XML Schema to C++ data binding compiler. You may use it in your
// programs without any restrictions.
//
//------------------------------------------------------------------------------
#include <iostream>
#include "pout_parser.h"
//------------------------------------------------------------------------------
namespace poutXML {
  void probability_t_pimpl::_characters(const ::xml_schema::ro_string& s) {
    probability_t=s;
    }
//------------------------------------------------------------------------------
  void peptide_pimpl::pre(probability_t_pimpl* probability_p) {
    this->probability_p=probability_p;
    }
//------------------------------------------------------------------------------
  void peptide_pimpl::q_value() {
    pq_value=probability_p->probability_t;
    }
//------------------------------------------------------------------------------
  void peptide_pimpl::pep() {
    ppep=probability_p->probability_t;
    }
//------------------------------------------------------------------------------
  void peptide_pimpl::decoy(bool decoy) {
    pdecoy=decoy;
    }
//------------------------------------------------------------------------------
  void psm_ids_pimpl::pre(PercolatorOutI* percolator,peptide_pimpl* peptide_p,
                          boost::unordered_map<PercolatorOutFeatures, string, PercolatorOutFeatures>& pout_values) {
    this->percolator=percolator;
    this->peptide_p=peptide_p;
    this->peptide_p->pq_value="";
    this->peptide_p->ppep="";
    this->pout_values=&pout_values;
    }
//------------------------------------------------------------------------------
  void psm_ids_pimpl::post_psm_ids() {
    peptide_p->pq_value="";
    peptide_p->ppep="";
    }
//------------------------------------------------------------------------------
  void psm_ids_pimpl::psm_id(const ::std::string& psm_id) {
    string psmid,psmidfile;

    if (!percolator->checkDecoy(peptide_p->pdecoy))
      return;
    psmid=percolator->convertPSMID(psm_id);
    if (psmid.length()==0)
      THROW_ERROR_VALUE(PRINT_TEXT::WRONG_FORMAT_PSM,psm_id);
    psmidfile=percolator->convertPSMIDFileName(psm_id);
    if (psmidfile.length()==0)
      THROW_ERROR_VALUE(PRINT_TEXT::NO_UNIQUE_MZID_FILE,psm_id);
    if (peptide_p->ppep.length()>0)
      (*pout_values)[PercolatorOutFeatures(psmidfile,psmid,PERCOLATOR_PARAM::PEPTIDE_PEP)]=peptide_p->ppep;
    if (peptide_p->pq_value.length()>0)
      (*pout_values)[PercolatorOutFeatures(psmidfile,psmid,PERCOLATOR_PARAM::PEPTIDE_Q_VALUE)]=peptide_p->pq_value;
    }
//------------------------------------------------------------------------------
  void psm_pimpl::pre(PercolatorOutI* percolator,probability_t_pimpl* probability_p,
                      boost::unordered_map<PercolatorOutFeatures, string, PercolatorOutFeatures>& pout_values) {
    this->percolator=percolator;
    this->probability_p=probability_p;
    this->pout_values=&pout_values;
    }
//------------------------------------------------------------------------------
  void psm_pimpl::svm_score(double svm_score) {
    if (psm_decoy)
      (*pout_values)[PercolatorOutFeatures(psmidfile,psmid,PERCOLATOR_PARAM::SVM_SCORE)]=global::to_string(svm_score);
    }
//------------------------------------------------------------------------------
  void psm_pimpl::q_value() {
    if (psm_decoy)
      (*pout_values)[PercolatorOutFeatures(psmidfile,psmid,PERCOLATOR_PARAM::Q_VALUE)]=probability_p->probability_t;
    }
//------------------------------------------------------------------------------
  void psm_pimpl::pep() {
    if (psm_decoy)
      (*pout_values)[PercolatorOutFeatures(psmidfile,psmid,PERCOLATOR_PARAM::PEP)]=probability_p->probability_t;
    }
//------------------------------------------------------------------------------
  void psm_pimpl::p_value() {
    if (psm_decoy)
      (*pout_values)[PercolatorOutFeatures(psmidfile,psmid,PERCOLATOR_PARAM::P_VALUE)]=probability_p->probability_t;
    }
//------------------------------------------------------------------------------
  void psm_pimpl::psm_id(const ::std::string& psm_id) {
    psmid=percolator->convertPSMID(psm_id);
    if (psmid.length()==0)
      THROW_ERROR_VALUE(PRINT_TEXT::WRONG_FORMAT_PSM,psm_id);
    psmidfile=percolator->convertPSMIDFileName(psm_id);
    if (psmidfile.length()==0)
      THROW_ERROR_VALUE(PRINT_TEXT::NO_UNIQUE_MZID_FILE,psm_id);
    }
//------------------------------------------------------------------------------
  void psm_pimpl::decoy(bool decoy) {
    psm_decoy=percolator->checkDecoy(decoy);
    }
//------------------------------------------------------------------------------
  }