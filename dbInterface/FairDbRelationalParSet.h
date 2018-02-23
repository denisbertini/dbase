#ifndef FAIRDBRELATIONALPARSET_H
#define FAIRDBRELATIONALPARSET_H

#include "FairDbGenericParSet.h"

#ifndef __CINT__
#include "json/json.h"
#endif

template <typename T>
class FairDbRelationalParSet : public FairDbGenericParSet<T>
{
  public :
    FairDbRelationalParSet();
    FairDbRelationalParSet(FairDbDetector::Detector_t detid, DataType::DataType_t dataid,
                        const char* name, const char* title, const char* context, Bool_t ownership=kFALSE);
    FairDbRelationalParSet(const FairDbRelationalParSet& from);
    FairDbRelationalParSet& operator=(const FairDbRelationalParSet&);

    virtual ~FairDbRelationalParSet(void);

    static void PurgeCache();

    virtual void store(UInt_t rid=0);

    Int_t GetId() const { return fId; }
    void SetId(Int_t value) { fId = value; this->SetCompId(fId); }

    static std::unique_ptr<T> GetById(Int_t id, UInt_t rid=0);
    static std::vector<T> GetByIds(std::vector<Int_t> ids, UInt_t rid=0);

#ifndef __CINT__
    static std::unique_ptr<T> FromJsonString(std::string jsonString);
    static std::unique_ptr<T> FromJson(Json::Value json);
    static std::vector<T> FromJsonArray(Json::Value jsonArray);
    std::string ToJsonString();
    Json::Value ToJson();
    static Json::Value ToJsonArray(std::vector<T> array);

    virtual void FillFromJson(Json::Value json) { ; }
    virtual void StoreToJson(Json::Value& json) { ; }
#endif

  protected:
    Int_t fId;

  private:
    Int_t AllocateNextId();

    ClassDefT(FairDbRelationalParSet<T>,0); // ROOT template class definition
};
ClassDefT2(FairDbRelationalParSet,T)

#endif /* !FAIRDBRELATIONALPARSET_H */

