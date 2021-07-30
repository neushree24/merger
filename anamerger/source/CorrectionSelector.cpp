#include "CorrectionSelector.h"
#include <fstream>
#include <iostream>
#include <string>

ClassImp(CorrectionSelector);

CorrectionSelector::CorrectionSelector(TTree *mergedData) : tree_reader_(mergedData),
                                                            next_(tree_reader_, "mergedNEXTBeta"),
                                                            clover_vec_(tree_reader_, "clover_vec_"),
                                                            vandle_vec_(tree_reader_, "vandle_vec_"){
}

CorrectionSelector::~CorrectionSelector()
{
}

double l_tof[10] = {47.1, 47.5, 47.5, 47.5, 47.8, 48.4, 49.1, 49.2, 49.8, 50.};

void CorrectionSelector::Begin(TTree *mergedData)
{
    std::cout << "[CorrectionSelector]: Begin() called." << std::endl;
    GetOutputList()->Clear();
    if (fInput)
    {
        auto param = (TParameter<Bool_t> *)fInput->FindObject("use_proof");
        if (param)
            use_proof_ = true;
        else
            use_proof_ = false;
    }
    else
    {
        use_proof_ = false;
    }

    if (use_proof_)
    {
        // If this is a proof process, do nothing in Begin()
        std::cout << "[CorrectionSelector]: a proofserv found." << std::endl;
        return;
    }
    else
    {
        // If this is not a proof process, open output file and initialize tree
        std::cout << "[CorrectionSelector]: no proofserv found." << std::endl;
        std::cout << "[CorrectionSelector]: running with a single thread." << std::endl;
        file_name_ = file_name_ + ".root";
        if (fOutputFile)
        {
            delete fOutputFile;
            fOutputFile = nullptr;
        }
        if (fOutputTree)
        {
            delete fOutputTree;
            fOutputTree = nullptr;
        }
        fOutputFile = new TFile(file_name_.c_str(), "RECREATE", "", ROOT::CompressionSettings(ROOT::kLZMA, 8));
        fOutputTree = new TTree("mergedCorrectedBeta", "mergedCorrectedBeta");
        fOutputTree->Branch("corrected_vandle_vec", "std::vector<CorrectedVANDLEData>", &corrected_vandle_vec_);
        fOutputTree->SetDirectory(fOutputFile);
    }
}

void CorrectionSelector::SlaveBegin(TTree *mergedData)
{

    std::ifstream in;
    {
        in.open("/home/sneupan/E19044_Analysis/pos_all.txt");

        // double POS[10][9];
        for (int row = 0; row < 10; row++)
        {
            for (int col = 0; col < 9; col++)
            {
                in >> XPOS[row][col];
                // std::cout << XPOS[row][col] << std::endl;
            }
        }

        in.close();
    }

    {
        in.open("/home/sneupan/E19044_Analysis/toff_13_18.txt");

        // double TOFF[10][8];
        for (int row = 0; row < 10; row++)
        {
            for (int col = 0; col < 8; col++)
            {
                in >> TOFF[row][col];
                // std::cout << TOFF[row][col] << std::endl;
            }
        }

        in.close();
    }

    {
        in.open("/home/sneupan/E19044_Analysis/tdiff_13_18.txt");
        // double TDOFF[10][8];
        for (int row = 0; row < 10; row++)
        {
            for (int col = 0; col < 8; col++)
            {
                in >> TDOFF[row][col];
                // std::cout << TDOFF[row][col] << std::endl;
            }
        }

        in.close();
    }

    {
        for (int m = 0; m < 10; m++)
        {
            for (int n = 0; n < 8; n++)
            {
                l[m][n] = l_tof[m] + 0.3 + 0.6 * n;
                //  cout << l[m][n] << endl;
            }
        }
    }

    std::string filename;
    if (fInput)
    {
        {
            // read output file name.
            TNamed *named = (TNamed *)fInput->FindObject("output_file_prefix");
            if (named)
                filename = named->GetTitle();
            else
                filename = "corrected_tree";
        }
        {
            // read output file path
            TNamed *named = (TNamed *)fInput->FindObject("proof_output_location");
            if (named)
                proof_output_location_ = named->GetTitle();
            else
                proof_output_location_ = "./";
        }
        {
            // read corrector config file name
            TNamed *named = (TNamed *)fInput->FindObject("vandle_corrector_config");
            if (named)
                vandle_corrector_config_ = named->GetTitle();
            else
                vandle_corrector_config_ = "config_vandle_corrector.yaml";
        }
    }

    if (gProofServ)
    {
        filename = filename + ".root";
        // send message to the client process
        const TString msg = TString::Format("SlaveBegin() of Ord = %s called.",
                                            gProofServ->GetOrdinal());
        gProofServ->SendAsynMessage(msg);
    }
    else
    {
        std::cout << "SalveBegin() called. (PROOF OFF) " << std::endl;
        return;
    }

    if (fProofFile)
    {
        delete fProofFile;
        fProofFile = nullptr;
    }
    // open a TProofOutputFile
    std::string fullname = proof_output_location_ + filename;
    fProofFile = new TProofOutputFile(fullname.c_str(), "");
    fProofFile->SetOutputFileName(fullname.c_str());

    if (fOutputFile)
    {
        delete fOutputFile;
        fOutputFile = nullptr;
    }
    TDirectory *savedir = gDirectory;
    fOutputFile = fProofFile->OpenFile("RECREATE");
    if (!fOutputFile)
        std::cout << "Failed to open output file. " << std::endl;
    std::cout << "File opened at " << fProofFile->GetDir() << fProofFile->GetFileName() << std::endl;
    fOutputFile->SetCompressionSettings(ROOT::CompressionSettings(ROOT::kLZMA, 8));

    if (fOutputTree)
    {
        delete fOutputTree;
        fOutputTree = nullptr;
    }
    // initialize tree
    fOutputTree = new TTree("mergedCorrectedBeta", "mergedCorrectedBeta");
    fOutputTree->Branch("corrected_vandle_vec", "std::vector<CorrectedVANDLEData>", &corrected_vandle_vec_);
    fOutputTree->SetDirectory(fOutputFile);
    fOutputTree->AutoSave();
    gDirectory = savedir;

    return;
}

void CorrectionSelector::Init(TTree *mergedData)
{
    tree_reader_.SetTree(mergedData);
    SetBranch();
    return;
}

void CorrectionSelector::SetBranch()
{
    auto list = tree_reader_.GetTree()->GetListOfBranches();
    TIter next(list);
    /* loops over the branches in the input file */
    int count = -1;
    while (TBranch *br = (TBranch *)next())
    {
        count++;
        if (!count)
            continue;
        if (count == 3)
            continue;
        TClass *tclass = (TClass *)gROOT->GetListOfClasses()->FindObject(br->GetClassName());
        auto addr = tclass->New();                                    //new instance of the class object filled in the branch
        br->SetAddress(addr);                                         // SetBranchAddress to the input tree
        fOutputTree->Branch(br->GetName(), br->GetClassName(), addr); // Branch to the output tree
        std::cout << "SetBranchAddress(" << br->GetName() << "," << br->GetClassName() << "," << addr << ")" << std::endl;
    }
    // Somehow, tclass->New() doesn't work for OutputTreeData
    // temporaly set branch to the output tree for the mergedBeta branch.
    //fBetaBranch = fOutputTree->Branch("mergedBeta", "OutputTreeData<PspmtData,PutputTreeData<PspmtData,TreeData>>", &beta_data_);
    return;
}

Bool_t CorrectionSelector::Process(Long64_t entry)
{

    tree_reader_.SetLocalEntry(entry);
    {
     
        auto next_output = next_.Get();
        // auto output_vec = next_.Get()->output_vec_;

        auto beta_output = next_.Get()->input_.input_;
        auto implant_output = next_.Get()->input_.output_vec_;

        // auto clover_vec = clover_vec_.Get();
        // if (!clover_vec)
        //     return kTRUE;

        // auto vandle_vec = vandle_vec_.Get();
        // if (!vandle_vec)
        //     return kTRUE;

        for (auto const &imp : implant_output)
        { 
            CorrectedVANDLEData output;
            corrected_vandle_vec_.clear();
            output.tsdiff_ = beta_output.high_gain_.time_ - imp.low_gain_.time_;
            output.betats_ = beta_output.high_gain_.time_;
            output.impts_ = imp.low_gain_.time_;
            x = beta_output.high_gain_.pos_x_ - imp.low_gain_.pos_x_;
            y = beta_output.high_gain_.pos_y_ - imp.low_gain_.pos_y_;

            r = sqrt(x * x + y * y);
            output.r_ = r;

            if (next_output->output_vec_.empty())
                return kTRUE;

            for (auto const &next : next_output->output_vec_.at(0).next_vec_)
            {

                // output.tsdiff_ = beta_output.high_gain_.time_ - implant_output.low_gain_.time_ ;
                RawToF = next.tof;
                mod = next.modNum;
                xpos = next.QZpos;
                Tdiff = next.tdiff;

                for (int b = 0; b < 10; b++)
                {
                    if (mod == b)
                    {
                        for (int j = 1; j < 9; j++)
                        {
                            if (xpos > XPOS[b][j - 1] && xpos < XPOS[b][j])

                            {
                                ncToF = RawToF - TOFF[b][j - 1] + l[b][j - 1] / clight;
                                ncToF_50 = ncToF * 50. / l[b][j - 1];
                                tdiff = Tdiff - TDOFF[b][j - 1];
                                // ToF = ncToF;

                                zpos = tdiff * c_bar / 2.;
                                if (abs(zpos) < 12.7)
                                    gBarZ = true;
                                else
                                    gBarZ = false;
                                l_h = sqrt(zpos * zpos + l[b][j - 1] * l[b][j - 1]);
                                ToF = ncToF * l[b][j - 1] / l_h;
                                corToF = ToF * (l[b][0] / l[b][j - 1]);
                                corToF_50 = corToF * 50. / l[b][0];
                                segID = j - 1;
                                output.rawToF_ = RawToF;
                                output.corToF_ = corToF;
                                output.tdiff_ = tdiff;
                                output.modNum_ = mod;
                                output.corToF_50_ = corToF_50;
                                output.qdc_ = next.qdc;
                                output.lqdc_ = next.lqdc;
                                output.rqdc_ = next.rqdc;
                                output.psd_ = next.psd;
                                output.sqdc_ = next.sQdc;
                                output.lmax_ = next.lMax;
                                output.rmax_ = next.rMax;
                                output.xpos_ = xpos;
                                output.ypos_ = next.QYpos;
                                output.zpos_ = zpos;
                                output.gBarZ_ = gBarZ;
                                output.segID_ = segID;
                                output.ncToF_ = ncToF;
                                output.ncToF_50_ = ncToF_50;
                            }
                        }
                    }
                }
            }
            corrected_vandle_vec_.emplace_back(output);
            fOutputTree->Fill();
        }
        
        // }

    } //end loop through the mergedData TTree

    return kTRUE;
}

void CorrectionSelector::SlaveTerminate()
{
    // wirte the output file then add it to fOutput for merging
    std::cout << "[CorrectionSelector::SlaveTerminate()]: called. " << std::endl;
    if (gProofServ)
    {
        if (fOutputTree)
        {
            std::cout << "fOutputTree::GetEntries() " << fOutputTree->GetEntries() << std::endl;
            if (fOutputFile)
            {
                TDirectory *savedir = gDirectory;
                fOutputFile->cd();
                fOutputTree->Write();
                fProofFile->Print();
                //fOutput->Add(fProofFile);
                fOutputTree->SetDirectory(0);
                gDirectory = savedir;
                std::cout << "File written " << fProofFile->GetDir() << fProofFile->GetFileName() << std::endl;
                fOutputFile->Close();
            }
        }
    }
    tree_reader_.SetTree((TTree *)nullptr);
    return;
}

void CorrectionSelector::Terminate()
{
    std::cout << "[CorrectionSelector::Terminate()]: called. " << std::endl;
    if (!use_proof_)
    {
        // If it was not a proof session, write output file here.
        if (fOutputTree)
        {
            std::cout << "fOutputTree::GetEntries() " << fOutputTree->GetEntries() << std::endl;
            fOutputTree->Write();
        }
        if (fOutputFile)
        {
            fOutputFile->Close();
        }
    }
    else
    {
    }

    return;
}
