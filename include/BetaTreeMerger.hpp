/** BetaBetaTreeMerger.hpp generated by R. Yokoyama 09/07/2018 **/

#ifndef VANDLE_MERGER_BETATREEMERGER_HPP_
#define VANDLE_MERGER_BETATREEMERGER_HPP_

#include "TreeMerger.hpp"
#include "BetaTreeMerger.hpp"
#include "YSOMap.h"

/** merger class **/
// input_2 events will be merged to a vector in input_1

template <class TOUT, class TIN1, class TIN2>
class BetaTreeMerger : public TreeMerger<TOUT,TIN1,TIN2>
{
public:
    const static std::string kMsgPrefix;

    BetaTreeMerger();
    BetaTreeMerger(TSScannorBase<TIN1> *input1, TSScannorBase<TIN2> *input2);
    virtual ~BetaTreeMerger();

    //void Configure(const std::string &yaml_node_name); // loads configuration from yaml
    //void Merge(); // merge events from input2 to input1
    //void Write(); // writes tree to the file
    virtual bool IsInGate(const TIN1 &in1, const TIN2 &in2);

protected:
    YSOMap *yso_map_ = nullptr;
    Double_t correlation_radius_;
};

template <class TOUT, class TIN1, class TIN2>
const std::string BetaTreeMerger<TOUT,TIN1,TIN2>::kMsgPrefix("[BetaTreeMerger]:");

template <class TOUT, class TIN1, class TIN2>
BetaTreeMerger<TOUT,TIN1,TIN2>::BetaTreeMerger() : TreeMerger<TOUT,TIN1,TIN2>()
{
}

template <class TOUT, class TIN1, class TIN2>
BetaTreeMerger<TOUT,TIN1,TIN2>::BetaTreeMerger(TSScannorBase<TIN1> *input1, TSScannorBase<TIN2> *input2)
   : TreeMerger<TOUT,TIN1,TIN2>(input1,input2)
{
    YamlReader yaml_reader("BetaTreeMerger");
    yso_map_ = new YSOMap(yaml_reader.GetString("YSOMapFile"));
    yso_map_->GenerateMap(10);
    correlation_radius_ = yaml_reader.GetDouble("CorrelationRadius");
}

template <class TOUT, class TIN1, class TIN2>
BetaTreeMerger<TOUT,TIN1,TIN2>::~BetaTreeMerger()
{
    if(yso_map_)
        delete yso_map_;
}
/*
template <class TOUT, class TIN1, class TIN2>
void BetaTreeMerger<TOUT,TIN1,TIN2>::Configure(const std::string &yaml_node_name)
   : TreeMerger(yaml_node_name)
{
}

template <class TOUT, class TIN1, class TIN2>
void BetaTreeMerger<TOUT,TIN1,TIN2>::Merge()
{
    return;
}

template <class TOUT, class TIN1, class TIN2>
void BetaTreeMerger<TOUT,TIN1,TIN2>::Write()
{
    return;
}
*/
template <class TOUT, class TIN1, class TIN2>
bool BetaTreeMerger<TOUT,TIN1,TIN2>::IsInGate(const TIN1 &in1, const TIN2 &in2)

{  
    const auto pspmt_imp = in2.low_gain_;
    const auto pspmt_beta = in1.high_gain_;
    /* if high gain position is available, use it for correlation */
    if(pspmt_beta.valid_){
        //const double beta_x = 11.9135504*pspmt_beta.pos_x_;
        //const double beta_y = 10.0*pspmt_beta.pos_y_;
        //const double imp_x = 23.664712*pspmt_imp.pos_x_ - 2.862757;
        //const double imp_y = 15.5979*pspmt_imp.pos_y_ - 1.4067;
        const double beta_x = pspmt_beta.pos_x_;
        const double beta_y = pspmt_beta.pos_y_;
        const double imp_x = pspmt_imp.pos_x_;
        const double imp_y = pspmt_imp.pos_y_;

        if(yso_map_->IsInside(beta_x,beta_y,imp_x,imp_y,correlation_radius_))
            return true;
        else
            return false;
    }
    /* otherwise, use low gain position */
    else{
        //const double beta_x = 23.664712*in1.low_gain_.pos_x_ - 2.862757;
        //const double beta_y = 15.5979*in1.low_gain_.pos_y_ - 1.4067;
        //const double imp_x = 23.664712*pspmt_imp.pos_x_ - 2.862757;
        //const double imp_y = 15.5979*pspmt_imp.pos_y_ - 1.4067;
        const double beta_x = in1.low_gain_.pos_x_;
        const double beta_y = in1.low_gain_.pos_y_;
        const double imp_x = pspmt_imp.pos_x_;
        const double imp_y = pspmt_imp.pos_y_;

        if(TMath::Power(beta_x-imp_x,2)+TMath::Power(beta_y-imp_y,2)<correlation_radius_*correlation_radius_)
            return true;
        else
            return false;
    }
}

#endif /* VANDLE_MERGER_TREEMERGER_HPP_ */
