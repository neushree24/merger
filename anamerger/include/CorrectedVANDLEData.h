// Corrected VANDLE data class created by Rin Yokoyama on 8/30/2019

#ifndef CORRECTED_VANDLE_DATA_H
#define CORRECTED_VANDLE_DATA_H

#include <iostream>
#include "PaassRootStruct.hpp"

class CorrectedVANDLEData : public TObject {
   public:
    CorrectedVANDLEData() {}

    virtual ~CorrectedVANDLEData(){};

    double corToF_;
    double rawToF_;
    double tdiff_;
    int modNum_;
    int segID_;
    double qdc_;
    double lqdc_;
    double rqdc_;
    double sqdc_;
    double psd_;
    double lmax_;
    double rmax_;
    double ncToF_;
    double ncToF_50_;
    double corToF_50_;
    double ypos_;
    double xpos_;
    double zpos_;
    bool gBarZ_;
    double tsdiff_;
    double betats_;
    double impts_;
    // double betaposX_;
    // double impposX_;
    // double betaposY_;
    // double impposY_;
    double r_;


 

    ClassDef(CorrectedVANDLEData, 1)
};

#endif
