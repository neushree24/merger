#include "VANDLEToFCorrector.h"

void VANDLEToFCorrector::Configure(const std::string& config_file) {
    std::cout << "[VANDLEToFCorrector]: Configure() called. config_file = " << config_file << std::endl;
    YamlParameter::Create(config_file);
    {
        // definition of pspmt position transformation
        if (pspmt_x_correction_)
            delete pspmt_x_correction_;
        if (pspmt_y_correction_)
            delete pspmt_y_correction_;
        YamlReader yaml_reader("PspmtCorrection");
        {
            const std::string func = yaml_reader.GetString("XFunction");
            pspmt_x_correction_ = new TF1("XCorrection", func.c_str(), -20, 20);
            auto param_list = yaml_reader.GetNode("XParameters");
            int i = 0;
            for (const auto& param : param_list) {
                pspmt_x_correction_->SetParameter(i, param.as<double>());
                ++i;
            }
        }
        {
            const std::string func = yaml_reader.GetString("YFunction");
            pspmt_y_correction_ = new TF1("YCorrection", func.c_str(), -20, 20);
            auto param_list = yaml_reader.GetNode("YParameters");
            int i = 0;
            for (const auto& param : param_list) {
                pspmt_y_correction_->SetParameter(i, param.as<double>());
                ++i;
            }
        }
        pspmt_rotation_angle_ = TMath::Pi() * yaml_reader.GetDouble("RotationAngle") / 180.;
    }
    {
        // generates a vandle bar list from file
        vandle_bar_map_.clear();
        YamlReader yaml_reader("VANDLEBars");
        auto bar_list = yaml_reader.GetNode("BarList");
        for (const auto& bar : bar_list) {
            const int bar_num = bar["BarNum"].as<int>();
            const double z_off = bar["ZOffset"].as<double>();
            const double lr_off = bar["LROffset"].as<double>();
            const double angle = TMath::Pi() * bar["Angle"].as<double>() / 180.0;
            vandle_bar_map_.emplace(bar_num, VANDLEBar(bar_num, z_off, angle, lr_off));
        }
		  speed_of_light_medium_ = yaml_reader.GetDouble("SpeedOfLightMedium",false,13.4414);
		  ideal_flight_path_ = yaml_reader.GetDouble("IdealFlightPath",false,105.);
		  tof_offset_ = yaml_reader.GetDouble("ToFOffset",false,1.48);

    }
    YamlParameter::Destroy();

    return;
}

double VANDLEToFCorrector::CorrectToF(const PspmtData& pspmt_data, const processor_struct::VANDLES& vandle, CorrectedVANDLEData& correctedData) const {
    const auto beta_pos = GetBetaPosition(pspmt_data);
    const auto vandle_pos = GetVandlePosition(vandle);
    const double flight_length = ((*beta_pos) - (*vandle_pos)).Mag();
    correctedData.SetTranformedX((*beta_pos).X());
    correctedData.SetTranformedY((*beta_pos).Y());
	 //std::cout << "beta(" << (*beta_pos).X() << ", " << (*beta_pos).Y() << "), vandle(" << (*vandle_pos).X() << ", " << (*vandle_pos).Y() << ", " << (*vandle_pos).Z() << "), length: " << flight_length << std::endl;
    delete beta_pos;
    delete vandle_pos;

	 const double z0 = vandle_bar_map_.at(vandle.barNum).GetZZero();
	 const double cor_tof = (ideal_flight_path_ / flight_length) * (vandle.tof+tof_offset_); // vandle tof correction by flight length
	 //const double walk_cor_tof = cor_tof - vandle_walk_correction_->Eval(vandle.qdc); // vandle tof walk correction
	 //std::cout << "tof (corrected/raw/FL/IFL/Offset) = (" << cor_tof << " / " << vandle.tof << " / " << flight_length << "/" << ideal_flight_path_ << "/" << tof_offset_ << ")" << std::endl;
	 return cor_tof;
	 
}

const TVector3* VANDLEToFCorrector::GetBetaPosition(const PspmtData& pspmt_data) const {
    const double x = pspmt_x_correction_->Eval(pspmt_data.high_gain_.pos_x_);
    const double y = pspmt_y_correction_->Eval(pspmt_data.high_gain_.pos_y_);
    TVector3* vector3 = new TVector3(x, y, 0);
    vector3->RotateZ(pspmt_rotation_angle_);
    return vector3;
}

const TVector3* VANDLEToFCorrector::GetVandlePosition(const processor_struct::VANDLES& vandle) const {
    const double angle = vandle_bar_map_.at(vandle.barNum).GetAngle();
    const double z0 = vandle_bar_map_.at(vandle.barNum).GetZZero();
    const double lrffset = vandle_bar_map_.at(vandle.barNum).GetLROffset();

    //const double z = speed_of_light_medium_ * 0.5 * vandle.tDiff;  //
    const double z = (-5.94268e+06 * pow(vandle.qdc + 454.261, -2.27498) + 13.4352) * 0.5 * vandle.wcTdiff;  //
    const double x = z0 * TMath::Cos(angle);
    const double y = z0 * TMath::Sin(angle);
    return new TVector3(x, y, z);
}