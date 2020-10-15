/** E19044BetaTSScanor.hpp generated by R. Yokoyama 10/06/2020 **/

#ifndef E19044_BETATSSCANOR_HPP_
#define E19044_BETATSSCANOR_HPP_

#include "TSScannorBase.hpp"
#include "PaassRootStruct.hpp"
#include "PspmtData.hpp"

/** timestamp scannor class for beta events **/
class E19044BetaTSScanor : public TSScannorBase<PspmtData>
{
public:
    const static std::string kMsgPrefix;
    E19044BetaTSScanor(){}
    ~E19044BetaTSScanor(){}
    void SetReader();

protected:
    /** energy gates on PSPMT dynode**/
    Double_t high_gain_min_;
    Double_t high_gain_max_;
    Double_t low_gain_min_;
    Double_t low_gain_max_;
    ULong64_t prev_implant_;

    ULong64_t GetTS() const
    {
        if(!tree_data_) throw kMsgPrefix + "In GetTS(), tree_data_ is null";
        if(!tree_data_->Get()) throw kMsgPrefix + "In GetTS(), tree_data_->Get() returned null";
        return tree_data_->Get()->external_ts_high_;
        //return tree_data_->Get()->dyn_single_.time_;
    }

    Bool_t IsInGate(); // gate conditions
};

#endif /* E19044_BETATSSCANOR_HPP_ */
