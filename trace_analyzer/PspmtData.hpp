#ifndef PSPMT__DATA_HPP
#define PSPMT__DATA_HPP

#include "PaassRootStruct.hpp"

struct pspmt_struc{
   Double_t pos_x_ = -999;
   Double_t pos_y_ = -999;
   Int_t id_x_ = -999;
   Int_t id_y_ = -999;
   Int_t valid_ = 0;
   Double_t trace_energy_ = 0;
   Double_t energy_ = 0;
   Double_t time_ = 0;
   Double_t energy_sum_ = 0;

	Double_t xa_energy_ = -999;
	Double_t xb_energy_ = -999;
	Double_t ya_energy_ = -999;
	Double_t yb_energy_ = -999;
	Double_t xa_trace_energy_ = -999;
	Double_t xb_trace_energy_ = -999;
	Double_t ya_trace_energy_ = -999;
	Double_t yb_trace_energy_ = -999;
};
static pspmt_struc DEFAULT_PSPMT_STRUC;

struct beamline_detector_struc{
   Double_t energy_ = 0;
   Double_t time_ = -9999;
};
static beamline_detector_struc DEFAULT_BEAMLINE_DETECTOR_STRUC;

struct event_info_struc {
   std::string file_name_;
   ULong64_t pixie_event_num_ = 0;
};

class PspmtData : public TObject {
public:

	PspmtData() :TObject() { Clear(); }
   virtual ~PspmtData(){}

   PspmtData(const PspmtData &obj):TObject(obj)
   {
		event_number_ = obj.event_number_;
		file_name_ = obj.file_name_;
      external_ts_high_ = obj.external_ts_high_;
      external_ts_low_ = obj.external_ts_low_;
      desi_top_ = obj.desi_top_;
      desi_bottom_ = obj.desi_bottom_;
      veto_first_ = obj.veto_first_;
      veto_second_ = obj.veto_second_;
      ion_white_ = obj.ion_white_;
      ion_green_ = obj.ion_green_;
      ion_blue_ = obj.ion_blue_;
      ion_black_ = obj.ion_black_;
      f11_left_ = obj.f11_left_;
      f11_right_ = obj.f11_right_;
      high_gain_ = obj.high_gain_;
      low_gain_ = obj.low_gain_;
      dyn_single_ = obj.dyn_single_;
   }

   void Clear(){
		event_number_ = 0;
		file_name_ = "";
      external_ts_high_ = 0;
      external_ts_low_ = 0;
      desi_top_ = DEFAULT_BEAMLINE_DETECTOR_STRUC;
      desi_bottom_ = DEFAULT_BEAMLINE_DETECTOR_STRUC;
      veto_first_ = DEFAULT_BEAMLINE_DETECTOR_STRUC;
      veto_second_ = DEFAULT_BEAMLINE_DETECTOR_STRUC;
      desi_top_ = DEFAULT_BEAMLINE_DETECTOR_STRUC;
      desi_bottom_ = DEFAULT_BEAMLINE_DETECTOR_STRUC;
      ion_white_ = DEFAULT_BEAMLINE_DETECTOR_STRUC;
      ion_green_ = DEFAULT_BEAMLINE_DETECTOR_STRUC;
      ion_blue_ = DEFAULT_BEAMLINE_DETECTOR_STRUC;
      ion_black_ = DEFAULT_BEAMLINE_DETECTOR_STRUC;
      f11_left_ = DEFAULT_BEAMLINE_DETECTOR_STRUC;
      f11_right_ = DEFAULT_BEAMLINE_DETECTOR_STRUC;
      dyn_single_ = DEFAULT_BEAMLINE_DETECTOR_STRUC;

      high_gain_ = DEFAULT_PSPMT_STRUC;
      low_gain_ = DEFAULT_PSPMT_STRUC;
   }

	ULong64_t event_number_;
	std::string file_name_;
   ULong64_t external_ts_high_;
   ULong64_t external_ts_low_;
   beamline_detector_struc desi_top_;
   beamline_detector_struc desi_bottom_;
   beamline_detector_struc veto_first_;
   beamline_detector_struc veto_second_;
   beamline_detector_struc ion_white_;
   beamline_detector_struc ion_green_;
   beamline_detector_struc ion_blue_;
   beamline_detector_struc ion_black_;
   beamline_detector_struc f11_left_;
   beamline_detector_struc f11_right_;
   beamline_detector_struc dyn_single_;
   pspmt_struc high_gain_;
   pspmt_struc low_gain_;
  
   ClassDef(PspmtData,4)
};


#endif
