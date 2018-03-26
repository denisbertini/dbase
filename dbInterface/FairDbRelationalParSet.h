#ifndef FAIRDBRELATIONALPARSET_H
#define FAIRDBRELATIONALPARSET_H

#include "FairDbGenericParSet.h"

#include <jsoncons/json.hpp>

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

    static std::unique_ptr<T> GetById(Int_t id, UInt_t rid=ValTimeStamp());
    static std::vector<T> GetByIds(std::vector<Int_t> ids, UInt_t rid=ValTimeStamp());

    static std::unique_ptr<T> FromJsonString(std::string jsonString);
    static std::unique_ptr<T> FromJson(jsoncons::json json);
    static std::vector<T> FromJsonArray(jsoncons::json jsonArray);
    std::string ToJsonString();
    jsoncons::json ToJson();
    static jsoncons::json ToJsonArray(std::vector<T> array);

    virtual void FillFromJson(jsoncons::json json) { ; }
    virtual void StoreToJson(jsoncons::json& json) { ; }

  protected:
    Int_t fId;

  private:
    Int_t AllocateNextId();

    ClassDefT(FairDbRelationalParSet<T>,0); // ROOT template class definition
};
ClassDefT2(FairDbRelationalParSet,T)

#endif /* !FAIRDBRELATIONALPARSET_H */

