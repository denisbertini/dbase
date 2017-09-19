#ifndef FAIRDBRELATIONALPARSET_H
#define FAIRDBRELATIONALPARSET_H


#include "DataType.h"
#include "FairDbGenericParSet.h"
#include "TString.h"
#include "TObjArray.h"
#include "Rtypes.h"

#ifndef __CINT__
#include "json/json.h"
#endif

#include <iostream>
#include <string>

template <typename T>
class FairDbRelationalParSet : public FairDbGenericParSet<T>
{
  public :
    FairDbRelationalParSet();
    FairDbRelationalParSet(FairDbDetector::Detector_t detid, DataType::DataType_t dataid, 
                        const char* name, const char* title, const char* context, Bool_t ownership=kFALSE);

    virtual ~FairDbRelationalParSet(void);

    UInt_t GetIndex(UInt_t /*def*/) const { return fId; }
    
    Int_t GetId() const { return fId; }
    void SetId(Int_t value) { fId = value; this->SetCompId(fId); }

#ifndef __CINT__
    static T* GetById(Int_t id, UInt_t rid=0);
    static TObjArray* GetByIds(Int_t* ids, UInt_t count, UInt_t rid=0);
#endif

    static T* FromJsonString(string& jsonString);
    static T* FromJson(Json::Value& json);
    static TObjArray* FromJsonArray(Json::Value& jsonArray);
    string ToJsonString();
    Json::Value ToJson();
    static Json::Value ToJsonArray(TObjArray* array);

    virtual void FillFromJson(Json::Value& json) { ; }
    virtual void StoreToJson(Json::Value& json) { ; }

  protected:
    Int_t fId;

  private:
    ClassDefT(FairDbRelationalParSet<T>,0); // ROOT template class definition
};
ClassDefT2(FairDbRelationalParSet,T)

#endif /* !FAIRDBRELATIONALPARSET_H */

