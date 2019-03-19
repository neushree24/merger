#ifndef PSPMT__DATA_HPP
#define PSPMT__DATA_HPP

#include "ProcessorRootStruc.hpp"

struct pspmt_struc{
   Double_t pos_x_ = -999;
   Double_t pos_y_ = -999;
   Int_t valid_ = 0;
   Double_t trace_energy_ = 0;
   Double_t energy_ = 0;
   Double_t time_ = 0;
};
static pspmt_struc DEFAULT_PSPMT_STRUC;

class PspmtData : public TObject {
public:

   PspmtData(){}
   virtual ~PspmtData(){}

   PspmtData(const PspmtData &obj):TObject(obj)
   {
      external_ts_high_ = obj.external_ts_high_;
      external_ts_low_ = obj.external_ts_low_;
      desi_top_energy_ = obj.desi_top_energy_;
      desi_bottom_energy_ = obj.desi_bottom_energy_;
      veto_first_energy_ = obj.veto_first_energy_;
      veto_second_energy_ = obj.veto_second_energy_;
      desi_top_time_ = obj.desi_top_time_;
      desi_bottom_time_ = obj.desi_bottom_time_;
      veto_first_time_ = obj.veto_first_time_;
      veto_second_time_ = obj.veto_second_time_;
      ion_white_energy_ = obj.ion_white_energy_;
      ion_green_energy_ = obj.ion_green_energy_;
      ion_blue_energy_ = obj.ion_blue_energy_;
      ion_black_energy_ = obj.ion_black_energy_;
      ion_white_time_ = obj.ion_white_time_;
      ion_green_time_ = obj.ion_green_time_;
      ion_blue_time_ = obj.ion_blue_time_;
      ion_black_time_ = obj.ion_black_time_;
      f11_left_energy_ = obj.f11_left_energy_;
      f11_right_energy_ = obj.f11_right_energy_;
      f11_right_time_ = obj.f11_right_time_;
      high_gain_ = obj.high_gain_;
      low_gain_ = obj.low_gain_;
   }

   void Clear(){
      external_ts_high_ = 0;
      external_ts_low_ = 0;
      desi_top_energy_ = 0;
      desi_bottom_energy_ = 0;
      veto_first_energy_ = 0;
      veto_second_energy_ = 0;
      desi_top_time_ = 0;
      desi_bottom_time_ = 0;
      veto_first_time_ = 0;
      veto_second_time_ = 0;
      ion_white_energy_ = 0;
      ion_green_energy_ = 0;
      ion_blue_energy_ = 0;
      ion_black_energy_ = 0;
      ion_white_time_ = 0;
      ion_green_time_ = 0;
      ion_blue_time_ = 0;
      ion_black_time_ = 0;
      f11_left_energy_ = 0;
      f11_right_energy_ = 0;
      f11_left_time_ = 0;
      f11_right_time_ = 0;
      high_gain_ = DEFAULT_PSPMT_STRUC;
      low_gain_ = DEFAULT_PSPMT_STRUC;
   }

   ULong64_t external_ts_high_;
   ULong64_t external_ts_low_;
   Double_t desi_top_energy_;
   Double_t desi_bottom_energy_;
   Double_t veto_first_energy_;
   Double_t veto_second_energy_;
   Double_t desi_top_time_;
   Double_t desi_bottom_time_;
   Double_t veto_first_time_;
   Double_t veto_second_time_;
   Double_t ion_white_energy_;
   Double_t ion_green_energy_;
   Double_t ion_blue_energy_;
   Double_t ion_black_energy_;
   Double_t ion_white_time_;
   Double_t ion_green_time_;
   Double_t ion_blue_time_;
   Double_t ion_black_time_;
   Double_t f11_left_energy_;
   Double_t f11_right_energy_;
   Double_t f11_left_time_;
   Double_t f11_right_time_;
   pspmt_struc high_gain_;
   pspmt_struc low_gain_;
  
   ClassDef(PspmtData,1)
};


#endif
