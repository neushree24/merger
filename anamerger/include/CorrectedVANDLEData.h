// Corrected VANDLE data class created by Rin Yokoyama on 8/30/2019

#ifndef CORRECTED_VANDLE_DATA_H
#define CORRECTED_VANDLE_DATA_H

#include <iostream>
#include "PaassRootStruct.hpp"

class CorrectedVANDLEData : public TObject {
   public:
    CorrectedVANDLEData() {}
    CorrectedVANDLEData(const processor_struct::VANDLES& vandle_data) {
        vandle_data_ = vandle_data;
    }
    virtual ~CorrectedVANDLEData(){};
    void SetTestData(const double& test_data) { test_data_ = test_data; }
    void SetCorrectedTof(const double& tof) { corrected_tof_ = tof; }
    void SetNeutronEnergy(const double& energy) { neutron_energy_ = energy; }
	 void SetTranformedX(const double& X) { transformed_X = X; }
	 void SetTranformedY(const double& Y) { transformed_Y = Y; }
	 double GetTranformedX() const { return transformed_X; }
	 double GetTranformedY() const { return transformed_Y; }
	 processor_struct::VANDLES *GetVandleData() { return &vandle_data_; }
	 double GetCorrectedToF() const { return corrected_tof_; }
	 double GetNeutronEnergy() const { return neutron_energy_; }
    double GetTransformedX() const {return transformed_X; }
    double GetTransformedY() const {return transformed_Y; }


   protected:
    processor_struct::VANDLES vandle_data_;
    double test_data_;
    double corrected_tof_;
	 double neutron_energy_;
    double transformed_X;
    double transformed_Y;

    ClassDef(CorrectedVANDLEData, 1)
};

#endif
