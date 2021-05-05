#include "AnamergerSelector.h"
#include <fstream>
#include <iostream>
#include <string>

ClassImp(AnamergerSelector);

AnamergerSelector::AnamergerSelector(TTree* mergedData) : tree_reader_(mergedData),
                                                          beta_(tree_reader_, "mergedBeta"),
                                                          clover_vec_(tree_reader_, "clover_vec_"),
                                                          vandle_vec_(tree_reader_, "corrected_vandle_vec"),
                                                          gamma_scint_vec_(tree_reader_, "gamma_scint_vec_"),
                                                          output_file_name_("anamerger_output.root") {
}

AnamergerSelector::~AnamergerSelector()
{
	if (fHistArray) {
		delete fHistArray;
		fHistArray = nullptr;
	}
	if (fOutputFile) {
		delete fOutputFile;
		fOutputFile = nullptr;
	}
}

void AnamergerSelector::Begin(TTree* mergedData) {
    GetOutputList()->Clear();
    if (fInput) {
        TNamed* named = (TNamed*)fInput->FindObject("output_file_name");
        if (named)
            output_file_name_ = named->GetTitle();
    }
}

void AnamergerSelector::SlaveBegin(TTree* mergedData)
{
	if (fHistArray) {
		delete fHistArray;
		fHistArray = nullptr;
	}
	fHistArray = new TObjArray();

	fHistArray->Add(new TH2F("Tib_ClvE", "ClvE_Tib", 4000, 0, 4000, 1000, -3, 3));
	fHistArray->Add(new TH2F("Tib_HagDynE", "HagDynE_Tib", 4000, 0, 4000, 1000, -3, 3));
	fHistArray->Add(new TH2F("Tib_HagAnE", "HagAnE_Tib", 4000, 0, 4000, 1000, -3, 3));
	fHistArray->Add(new TH2F("Tib_HighE", "Tib_HighE", 4000, -1000, 18000, 1000, -3, 3));
	fHistArray->Add(new TH2F("Tib_LowE", "Tib_LowE", 4000, -1000, 18000, 1000, -3, 3));
	fHistArray->Add(new TH2F("Tib_DynE", "Tib_DynE", 4000, -1000, 18000, 1000, -3, 3));
	fHistArray->Add(new TH2F("Tib_Esum", "Tib_DynE", 4000, -1000, 18000, 1000, -3, 3));
	fHistArray->Add(new TH2F("nQDC_nToF", "nQDC_nToF", 3200, -100, 1500, 8000, 0, 32000));
	fHistArray->Add(new TH2F("nQDC_nEnergy", "nQDC_nEnergy", 4000, 0, 10, 8000, 0, 32000));
	fHistArray->Add(new TH2F("nQDC_nToF_g", "nQDC_nToF_g", 3200, -100, 1500, 8000, 0, 32000));
	fHistArray->Add(new TH2F("nToF_banana", "nToF_banana", 3200, -100, 1500, 8000, 0, 32000));
	fHistArray->Add(new TH1F("nEnergy_banana", "nEnergy_banana", 4000,0,10));
	fHistArray->Add(new TH2F("nToF_g_banana", "nToF_banana", 3200, -100, 1500, 8000, 0, 32000));
	fHistArray->Add(new TH2F("nQDC_nToF_Tib", "nQDC_nToF_Tib", 500, -100, 400, 100,-0.5,0.5));
	//fHistArray->Add(new TH2F("nQDC_nToF_banana_C", "nQDC_nToF_banana_C", 3200, -100, 1500, 1000, 0, 32000));
	fHistArray->Add(new TH2F("ClvE_nToF", "ClvE_nToF", 3200, -100, 1500, 2000, 0, 4000));
	fHistArray->Add(new TH2F("ClvE_nToF_BG", "ClvE_nToF_BG", 3200, -100, 1500, 2000, 0, 4000));
	fHistArray->Add(new TH2F("HagDynE_nToF", "HagDynE_nToF", 3200, -100, 1500, 2000, 0, 4000));
	fHistArray->Add(new TH2F("HagDynE_nToF_BG", "HagDynE_nToF_BG", 3200, -100, 1500, 2000, 0, 4000));
	fHistArray->Add(new TH2F("HagAnE_nToF", "HagAnE_nToF", 3200, -100, 1500, 2000, 0, 4000));
	fHistArray->Add(new TH2F("HagAnE_nToF_BG", "HagAnE_nToF_BG", 3200, -100, 1500, 2000, 0, 4000));
	//fHistArray->Add(new TH2F("nQDC_nToF_ht", "nQDC_nToF_ht", 3200, -100, 1500, 1000, 0, 32000));
	//fHistArray->Add(new TH2F("nQDC_nToF_orig", "nQDC_nToF_orig", 3200, -100, 1500, 1000, 0, 32000));
	fHistArray->Add(new TH2F("nQDC_nToF_BG", "nQDC_nToF_BG", 3200, -100, 1500, 8000, 0, 32000));
	fHistArray->Add(new TH2F("nQDC_nEnergy_BG", "nQDC_nEnergy_BG", 4000, 0, 10, 8000, 0, 32000));
	fHistArray->Add(new TH2F("nQDC_nToF_g_BG", "nQDC_nToF_g_BG", 3200, -100, 1500, 8000, 0, 32000));
	fHistArray->Add(new TH2F("nToF_BG_banana", "nToF_BG_banana", 3200, -100, 1500, 8000, 0, 32000));
	fHistArray->Add(new TH1F("nEnergy_BG_banana", "nEnergy_BG_banana", 4000,0,10));
	fHistArray->Add(new TH2F("nToF_g_BG_banana", "nToF_BG_banana", 3200, -100, 1500, 8000, 0, 32000));
	//fHistArray->Add(new TH2F("nQDC_nToF_BG_banana_C", "nQDC_nToF_BG_banana_C", 3200, -100, 1500, 1000, 0, 32000));
	//fHistArray->Add(new TH2F("nQDC_nToF_BG_ht", "nQDC_nToF_BG_ht", 3200, -100, 1500, 1000, 0, 32000));
	//fHistArray->Add(new TH2F("nQDC_nToF_BG_orig", "nQDC_nToF_BG_orig", 3200, -100, 1500, 1000, 0, 32000));
	fHistArray->Add(new TH2F("nToF_nQDC", "nTof_nQDC", 1000, 0, 32000, 1600, -100, 1000));
	fHistArray->Add(new TH2F("BarN_nToF", "BarN_nToF", 1600, -100, 1500, 50, -0.5, 49.5));
	fHistArray->Add(new TH2F("BarN_ntdiff_top", "BarN_ntdiff_top", 1600, -20, 20, 50, -0.5, 49.5));
	fHistArray->Add(new TH2F("BarN_ntdiff_bottom", "BarN_ntdiff_bottom", 1600, -20, 20, 50, -0.5, 49.5));
	fHistArray->Add(new TH1F("Tib", "Tib", 1000, -3, 3));
	fHistArray->Add(new TH2F("Tib_nToF", "Tib_nToF", 1600, -100, 1500, 1000, -3, 3));
	fHistArray->Add(new TH2F("Tib_nToF_banana", "Tib_nToF_banana", 1200, -100, 500, 1000, -3, 3));
	fHistArray->Add(new TH2F("Tib_nEnergy_banana", "Tib_nEnergy_banana", 4000, 0, 10, 1000, -3, 3));
	fHistArray->Add(new TH1F("nMult", "nMult", 50, -0.5, 49.5));
	fHistArray->Add(new TH1F("nToF", "nToF", 2500, -500, 2000));

	//adding histograms to output list
	TIter next(fHistArray);
	while (auto hist = next()) {
		GetOutputList()->Add(hist);
	}

	n_correction = new TF1("n_correction", "[0]+[1]*pow(x,2)", 0, 65536);
	n_correction->SetParameters(0.0, 6.5E-9);

	n_banana_up = new TF1("banana_up", "[0]/(x-[1])^[2]+[3]", 35, 1000);
	n_banana_up->SetParameters(170000, 35, 1.2, 400);
	n_banana_low = new TF1("banana_low", "[0]/(x-[1])^[2]+[3]", 25, 1000);
	n_banana_low->SetParameters(170000, 25, 1.3, -500);

	t_entry_ = 0;

	if (fInput) {
		TParameter<Double_t>* time_window = (TParameter<Double_t>*)fInput->FindObject("TimeWindow");
		if (time_window)
			time_window_ = time_window->GetVal();
		else
			time_window_ = 1.0;
	}

	if (gProofServ) {
		const TString msg = TString::Format("SlaveBegin() of Ord = %s called. %d histograms are initialized.",
			gProofServ->GetOrdinal(), GetOutputList()->GetEntries());
		gProofServ->SendAsynMessage(msg);
	}
	else {
		std::cout << "SalveBegin() called. " << GetOutputList()->GetEntries() << " histograms are initialized." << std::endl;
	}

	return;
}

void AnamergerSelector::Init(TTree* mergedData) {
    tree_reader_.SetTree(mergedData);
    return;
}

Bool_t AnamergerSelector::Process(Long64_t entry) {
	t_entry_++;
	//if (!(t_entry_ == 1333 || t_entry_ == 461365 || t_entry_ == 773098 || t_entry_ == 458544 || t_entry_ == 1400992))
	//	return kTRUE;
	//if (t_entry_ == 458544)
	//	return kTRUE;
	tree_reader_.SetLocalEntry(entry);
	{
		auto beta = beta_.Get();
		if (!beta)
			return kTRUE;

		//bool if_continue = false;
		//if (beta->event_number_ == 3969128 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_71-6")
		//	if_continue = true;
		//if (beta->event_number_ == 204940 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_97-6")
		//	if_continue = true;
		//if (beta->event_number_ == 2352919 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_23-0")
		//	if_continue = true;
		//if (beta->event_number_ == 2054082 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_96-5")
		//	if_continue = true;
		//if (beta->event_number_ == 1776659 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_83-0")
		//	if_continue = true;
		//if (beta->event_number_ == 3961499 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_69-0")
		//	if_continue = true;
		//if (beta->event_number_ == 3639162 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_47-2")
		//	if_continue = true;
		//if (beta->event_number_ == 2085814 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_106-2")
		//	if_continue = true;
		//if (beta->event_number_ == 3218590 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_90-6")
		//	if_continue = true;
		//if (beta->event_number_ == 2300296 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_109-4")
		//	if_continue = true;
		//if (beta->event_number_ == 21927 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_53-0")
		//	if_continue = true;
		//if (beta->event_number_ == 332588 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_70-4")
		//	if_continue = true;
		//if (beta->event_number_ == 4729968 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_33-1")
		//	if_continue = true;
		//if (beta->event_number_ == 4119072 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_72-2")
		//	if_continue = true;
		//if (beta->event_number_ == 3091616 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_64-6")
		//	if_continue = true;
		//if (beta->event_number_ == 2259751 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_85-8")
		//	if_continue = true;
		//if (beta->event_number_ == 1199119 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_32-1")
		//	if_continue = true;
		//if (beta->event_number_ == 3998164 && beta->file_name_ == "/SCRATCH/DRunScratch2/ry/production_109-4")
		//	if_continue = true;
		//if (!if_continue)
		//	return kTRUE;

		auto clover_vec = clover_vec_.Get();
		if (!clover_vec)
			return kTRUE;
		auto vandle_vec = vandle_vec_.Get();
		if (!vandle_vec)
			return kTRUE;
		auto gamma_scint_vec = gamma_scint_vec_.Get();
		if (!gamma_scint_vec)
			return kTRUE;
		//if (beta->high_gain_.energy_sum_ < 200)
		//	return kTRUE;
		if (beta->dyn_single_.energy_ > 20000)
			return kTRUE;
		if (beta->f11_left_.energy_ > 0 && beta->f11_right_.energy_ > 0)
			return kTRUE;
		//if (beta->low_gain_.energy_ > 2000)
		//	return kTRUE;
		for (const auto& imp : beta->output_vec_) {
			if (imp.output_vec_.empty())
				continue;
			if( imp.output_vec_.at(0).sts != 6 )
				continue;
			const Double_t tib = (((double)beta->dyn_single_.time_ - (double)imp.dyn_single_.time_)) / 1.E+9;
			((TH1F*)fHistArray->FindObject("Tib"))->Fill(tib);
			((TH2F*)fHistArray->FindObject("Tib_HighE"))->Fill(beta->high_gain_.energy_sum_, tib);
			((TH2F*)fHistArray->FindObject("Tib_LowE"))->Fill(beta->low_gain_.energy_, tib);
			((TH2F*)fHistArray->FindObject("Tib_DynE"))->Fill(beta->dyn_single_.energy_, tib);
			((TH2F*)fHistArray->FindObject("Tib_Esum"))->Fill(beta->high_gain_.energy_sum_, tib);

			Double_t clover1 = 0;
			Double_t clover2 = 0;
			Bool_t g_gate = false;
			for (const auto& clv : *clover_vec) {
				const Double_t bg_tdiff = (beta->dyn_single_.time_ - clv.time);
				if (bg_tdiff < -1000 || bg_tdiff > 1000)
					continue;
				if (clv.detNum < 4)
					clover1 += clv.energy;
				else
					clover2 += clv.energy;
			}
			std::vector<double> clv_hits;
			if (clover1 > 0)
				clv_hits.push_back(clover1);
			if (clover2 > 0)
				clv_hits.push_back(clover2);
			for(const auto &hit : clv_hits)
			{
				auto hist = (TH2F*)fHistArray->FindObject("Tib_ClvE");
				hist->Fill(hit, tib);
				if (hit > 728 && hit < 732)
					g_gate = true;
			}


			for (const auto& hag : *gamma_scint_vec) {
				const Double_t bg_tdiff = (beta->dyn_single_.time_ - hag.time);
				if (bg_tdiff < -1000 || bg_tdiff > 1000)
					continue;
				if (hag.detNum % 2) {
					auto hist = (TH2F*)fHistArray->FindObject("Tib_HagAnE");
					hist->Fill(hag.energy, tib);
				}
				else {
					auto hist = (TH2F*)fHistArray->FindObject("Tib_HagDynE");
					hist->Fill(hag.energy, tib);

				}
			}
			{
				auto hist = (TH1F*)fHistArray->FindObject("nMult");
				hist->Fill(vandle_vec->size());
			}
			for (auto& vandle : *vandle_vec) {
				const double tdiff_vb = (double)vandle.GetVandleData()->sTime - (double)beta->dyn_single_.time_;
				if (tdiff_vb < 200 || tdiff_vb > 250)
					continue;
				Bool_t banana = false;
				
				if ( vandle.GetVandleData()->qdc < 15000. && n_banana_up->Eval(vandle.GetCorrectedToF()) > vandle.GetVandleData()->qdc) {
					banana = true;
				}
				if ( (*vandle_vec).size()<3 ){
					banana = false;
				}	
				/*	
				if (vandle.GetCorrectedToF() > 25 && vandle.GetCorrectedToF() < 800 && vandle.GetVandleData()->qdc < 300) {
					banana = true;
				}
				*/
				{
					auto hist = (TH2F*)fHistArray->FindObject("Tib_nToF");
					hist->Fill(vandle.GetCorrectedToF(),tib);
				}
				if (banana ) {
					auto hist = (TH2F*)fHistArray->FindObject("nQDC_nToF_Tib");
					hist->Fill(vandle.GetCorrectedToF(), tib );
				}
				if (banana) {
					{
						auto hist = (TH2F*)fHistArray->FindObject("Tib_nToF_banana");
						hist->Fill(vandle.GetCorrectedToF(), tib);
					}
					{
						auto hist = (TH2F*)fHistArray->FindObject("Tib_nEnergy_banana");
						hist->Fill(vandle.GetNeutronEnergy(), tib);
					}
					//if(vandle.GetNeutronEnergy()>0.04&&vandle.GetNeutronEnergy()<0.0425)
				}
				if (tib > 0.01 && tib < time_window_) {
					{
						auto hist = (TH2F*)fHistArray->FindObject("nQDC_nToF");
						hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->qdc);
					}
					{
						auto hist = (TH2F*)fHistArray->FindObject("nQDC_nEnergy");
						hist->Fill(vandle.GetNeutronEnergy(), vandle.GetVandleData()->qdc);
					}
					if (g_gate) {
						auto hist = (TH2F*)fHistArray->FindObject("nQDC_nToF_g");
						hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->qdc);
					}
					/*
					if (beta->high_gain_.energy_ > 377) {
						auto hist = (TH2F*)fHistArray->FindObject("nQDC_nToF_ht");
						hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->qdc);
					}
					{
						auto hist = (TH2F*)fHistArray->FindObject("nQDC_nToF_orig");
						hist->Fill(vandle.GetVandleData()->corTof-n_correction->Eval(vandle.GetVandleData()->qdc), vandle.GetVandleData()->qdc);
					}
					*/
					{
						auto hist = (TH2F*)fHistArray->FindObject("nToF_nQDC");
						hist->Fill(vandle.GetVandleData()->qdc, vandle.GetCorrectedToF());
					}
					{
						auto hist = (TH2F*)fHistArray->FindObject("BarN_nToF");
						hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->barNum);
					}
					{
						if (vandle.GetTranformedY() < -2) {
							auto hist = (TH2F*)fHistArray->FindObject("BarN_ntdiff_bottom");
							hist->Fill(vandle.GetVandleData()->tDiff, vandle.GetVandleData()->barNum);
						}
					}
					{
						if (vandle.GetTranformedY() > 2) {
							auto hist = (TH2F*)fHistArray->FindObject("BarN_ntdiff_top");
							hist->Fill(vandle.GetVandleData()->tDiff, vandle.GetVandleData()->barNum);
						}
					}
					{
						auto hist = (TH1F*)fHistArray->FindObject("nToF");
						hist->Fill(vandle.GetCorrectedToF());
					}
					if (banana) {
						{
							auto hist = (TH2F*)fHistArray->FindObject("nToF_banana");
							hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->qdc);
						}
						{
							auto hist = (TH1F*)fHistArray->FindObject("nEnergy_banana");
							hist->Fill(vandle.GetNeutronEnergy());
						}
						if (g_gate) {
							auto hist = (TH2F*)fHistArray->FindObject("nToF_g_banana");
							hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->qdc);
						}
						for (const auto& hit : clv_hits) {
							auto hist = (TH2F*)fHistArray->FindObject("ClvE_nToF");
							hist->Fill(vandle.GetCorrectedToF(), hit);
						}

						for (const auto& hag : *gamma_scint_vec) {
							if (hag.detNum % 2) {
								auto hist = (TH2F*)fHistArray->FindObject("HagAnE_nToF");
								hist->Fill(vandle.GetCorrectedToF(), hag.energy);
							}
							else {
								auto hist = (TH2F*)fHistArray->FindObject("HagDynE_nToF");
								hist->Fill(vandle.GetCorrectedToF(), hag.energy);
							}
						}
					}

					/*
					if (vandle.GetCorrectedToF()>25 && n_banana_low->Eval(vandle.GetCorrectedToF())<vandle.GetVandleData()->qdc) {
						if (vandle.GetCorrectedToF() < 35 || n_banana_up->Eval(vandle.GetCorrectedToF()) > vandle.GetVandleData()->qdc) {
							{
								auto hist = (TH2F*)fHistArray->FindObject("nQDC_nToF_banana");
								hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->qdc);
							}
							for (const auto& hit : clv_hits) {
								auto hist = (TH2F*)fHistArray->FindObject("ClvE_nToF");
								hist->Fill(vandle.GetCorrectedToF(), hit);
							}
							for (const auto& hag : *gamma_scint_vec) {
								//const Double_t bg_tdiff = (beta->dyn_single_.time_ - hag.time);
								//if (bg_tdiff < -1000 || bg_tdiff > 1000)
								//	continue;
								if (hag.detNum % 2) {
									auto hist = (TH2F*)fHistArray->FindObject("HagAnE_nToF");
									hist->Fill(vandle.GetCorrectedToF(), hag.energy);
								}
								else {
									auto hist = (TH2F*)fHistArray->FindObject("HagDynE_nToF");
									hist->Fill(vandle.GetCorrectedToF(), hag.energy);
								}
							}
						}
					}
					else if ((vandle.GetCorrectedToF() > 25 && n_banana_low->Eval(vandle.GetCorrectedToF()) > vandle.GetVandleData()->qdc && vandle.GetVandleData()->qdc < 1000)
						|| vandle.GetCorrectedToF() < 25 && vandle.GetVandleData()->qdc < 1000) {
							{
								auto hist = (TH2F*)fHistArray->FindObject("nQDC_nToF_banana_C");
								hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->qdc);
							}
					}
					*/
				}
				if (tib > (0.0 - time_window_) && tib < -0.01) {
					{
						auto hist = (TH2F*)fHistArray->FindObject("nQDC_nToF_BG");
						hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->qdc);
					}
					{
						auto hist = (TH2F*)fHistArray->FindObject("nQDC_nEnergy_BG");
						hist->Fill(vandle.GetNeutronEnergy(), vandle.GetVandleData()->qdc);
					}
					if (g_gate) {
						auto hist = (TH2F*)fHistArray->FindObject("nQDC_nToF_g_BG");
						hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->qdc);
					}
					/*
					if (beta->high_gain_.energy_ > 377) {
						auto hist = (TH2F*)fHistArray->FindObject("nQDC_nToF_BG_ht");
						hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->qdc);
					}
					{
						auto hist = (TH2F*)fHistArray->FindObject("nQDC_nToF_BG_orig");
						hist->Fill(vandle.GetVandleData()->corTof-n_correction->Eval(vandle.GetVandleData()->qdc), vandle.GetVandleData()->qdc);
					}*/
					if (banana) {
						{
							auto hist = (TH2F*)fHistArray->FindObject("nToF_BG_banana");
							hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->qdc);
						}
						{
							auto hist = (TH1F*)fHistArray->FindObject("nEnergy_BG_banana");
							hist->Fill(vandle.GetNeutronEnergy());
						}
						if (g_gate) {
							auto hist = (TH2F*)fHistArray->FindObject("nToF_g_BG_banana");
							hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->qdc);
						}
						for (const auto& hit : clv_hits) {
							auto hist = (TH2F*)fHistArray->FindObject("ClvE_nToF_BG");
							hist->Fill(vandle.GetCorrectedToF(), hit);
						}

						for (const auto& hag : *gamma_scint_vec) {
							if (hag.detNum % 2) {
								auto hist = (TH2F*)fHistArray->FindObject("HagAnE_nToF_BG");
								hist->Fill(vandle.GetCorrectedToF(), hag.energy);
							}
							else {
								auto hist = (TH2F*)fHistArray->FindObject("HagDynE_nToF_BG");
								hist->Fill(vandle.GetCorrectedToF(), hag.energy);
							}
						}
					}

					/*
					if (vandle.GetCorrectedToF() > 25 && n_banana_low->Eval(vandle.GetCorrectedToF()) < vandle.GetVandleData()->qdc) {
						if (vandle.GetCorrectedToF() < 35 || n_banana_up->Eval(vandle.GetCorrectedToF()) > vandle.GetVandleData()->qdc) {
							{
								auto hist = (TH2F*)fHistArray->FindObject("nQDC_nToF_BG_banana");
								hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->qdc);
							}
							for (const auto& hit : clv_hits) {
								auto hist = (TH2F*)fHistArray->FindObject("ClvE_nToF_BG");
								hist->Fill(vandle.GetCorrectedToF(), hit);
							}
							for (const auto& hag : *gamma_scint_vec) {
								const Double_t bg_tdiff = (beta->dyn_single_.time_ - hag.time);
								if (bg_tdiff < -1000 || bg_tdiff > 1000)
									continue;
								if (hag.detNum % 2) {
									auto hist = (TH2F*)fHistArray->FindObject("HagAnE_nToF_BG");
									hist->Fill(vandle.GetCorrectedToF(), hag.energy);
								}
								//else {
								//	auto hist = (TH2F*)fHistArray->FindObject("HagDynE_nToF");
								//	hist->Fill(vandle.GetCorrectedToF(), hag.energy);
								//}
							}
						}
					}
					else if ((vandle.GetCorrectedToF() > 25 && n_banana_low->Eval(vandle.GetCorrectedToF()) > vandle.GetVandleData()->qdc && vandle.GetVandleData()->qdc < 1000)
						|| vandle.GetCorrectedToF() < 25 && vandle.GetVandleData()->qdc < 1000) {
							{
								auto hist = (TH2F*)fHistArray->FindObject("nQDC_nToF_BG_banana_C");
								hist->Fill(vandle.GetCorrectedToF(), vandle.GetVandleData()->qdc);
							}
					}*/
				}
			}
		}

	}//end loop through the mergedData TTree

	return kTRUE;
}

void AnamergerSelector::Terminate() {
    fOutputFile = new TFile(output_file_name_.c_str(), "recreate");
    std::cout << "[AnamergerSelector::Terminate()]: output file: " << output_file_name_ << std::endl;
    // write the histograms
    TIter next(GetOutputList());
    while (TObject* obj = next()) {
        std::cout << "[AnamergerSelector::Terminate]: writing " << obj->GetName() << " to file." << std::endl;
        obj->Write();
    }

	fOutputFile->Close();

	if (fOutputFile) {
		delete fOutputFile;
		fOutputFile = nullptr;
	}

    return;
}

double AnamergerSelector::CalculateNEnergy(const double& tof) const {
	const double v = 1.05 / (tof * 1.e-9);
	const double c = 299792458;
	const double beta = v * v / (c * c);
	const double gamma = 1. / sqrt(1. - beta);
	const double mass = 939.5654133;
	return mass * (gamma - 1.);
}
