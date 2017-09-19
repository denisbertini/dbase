#ifndef STSSENSORBATCH_H
#define STSSENSORBATCH_H

#include "DataType.h"
#include "FairDbRelationalParSet.h"
#include "TString.h"
#include "TObjArray.h"
#include "Rtypes.h"
#include "json/json.h"

#include <iostream>
#include <string>

using namespace std;
class StsSensor;

class StsSensorBatch : public FairDbRelationalParSet<StsSensorBatch>
{
  using TObject::Compare;

  public :
    StsSensorBatch(FairDbDetector::Detector_t detid = FairDbDetector::kSts,
          DataType::DataType_t dataid = DataType::kData,
          const char* name = "StsSensorBatch",
          const char* title = "Sts Sensor Batch Entity",
          const char* context = "StsSensorDefaultContext",
          Bool_t ownership=kTRUE);

    virtual ~StsSensorBatch(void);
    StsSensorBatch(const StsSensorBatch& from);
    StsSensorBatch& operator=(const StsSensorBatch& from);

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
    const TObjArray* GetSensors();

    static TObjArray* GetByNumber(string Number, UInt_t rid=0);
#endif

    string GetNumber() const { return fNumber; }
    ValTimeStamp GetDate() const { return fDate; }
    string GetComment() const { return fComment; }

    void SetNumber(string value) { fNumber = value; }
    void SetDate(ValTimeStamp value) { fDate = value; }
    void SetComment(string value) { fComment = value; }

    virtual void FillFromJson(Json::Value& json);
    virtual void StoreToJson(Json::Value& json);

  private:
    TObjArray* fSensors; //! transient relation to StsSensor, Has Many

    string fNumber;
    ValTimeStamp fDate;
    string fComment;

    ClassDef(StsSensorBatch, 1); // ROOT class definition
};

#endif /* !STSSENSORBATCH_H */
