#ifndef STSSENSOR_H
#define STSSENSOR_H

#include "DataType.h"
#include "FairDbRelationalParSet.h"
#include "TString.h"
#include "TObjArray.h"
#include "Rtypes.h"
#include "json/json.h"

#include <iostream>
#include <string>

using namespace std;
class StsSensorBatch;

class StsSensor : public FairDbRelationalParSet<StsSensor>
{
  using TObject::Compare;

  public :
    StsSensor(FairDbDetector::Detector_t detid = FairDbDetector::kSts,
          DataType::DataType_t dataid = DataType::kData,
          const char* name = "StsSensor",
          const char* title = "Sts Sensor Entity",
          const char* context = "StsSensorDefaultContext",
          Bool_t ownership=kTRUE);

    virtual ~StsSensor(void);
    StsSensor(const StsSensor& from);
    StsSensor& operator=(const StsSensor& from);

    // Dump Object
    void   Print();

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    // Atomic IO (intrinsic)
    virtual void Fill(FairDbResultPool& res_in,
                      const FairDbValRecord* valrec);
    virtual void Store(FairDbOutTableBuffer& res_out,
                       const FairDbValRecord* valrec) const;

#ifndef __CINT__
    const StsSensorBatch* GetBatch();

    static TObjArray* GetByBatchId(Int_t BatchId, UInt_t rid=0);
    static TObjArray* GetBySUID(string SUID, UInt_t rid=0);
#endif

    Int_t GetBatchId() const { return fBatchId; }
    string GetSUID() const { return fSUID; }

    void SetBatchId(Int_t value) { fBatchId = value; }
    void SetSUID(string value) { fSUID = value; }

    virtual void FillFromJson(Json::Value& json);
    virtual void StoreToJson(Json::Value& json);

  private:
    StsSensorBatch* fBatch; //! transient relation to StsSensorBatch, Has One

    Int_t fBatchId;
    string fSUID;

    ClassDef(StsSensor, 1); // ROOT class definition
};

#endif /* !STSSENSOR_H */
