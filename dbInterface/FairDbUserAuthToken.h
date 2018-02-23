/**
* @file FairDbUserAuthToken.h
* @brief  FairDb class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#ifndef FAIRDBUSERAUTHTOKEN_H
#define FAIRDBUSERAUTHTOKEN_H

#include "FairDbRelationalParSet.h"

#ifndef __CINT__
#include "json/json.h"
#endif


class FairDbUser;

class FairDbUserAuthToken : public FairDbRelationalParSet<FairDbUserAuthToken>
{
  using TObject::Compare;

  public :
    FairDbUserAuthToken(FairDbDetector::Detector_t detid = FairDbDetector::kUnknown,
          DataType::DataType_t dataid = DataType::kUnknown,
          const char* name = "FairDbUserAuthToken",
          const char* title = "",
          const char* context = "DefaultContext",
          Bool_t ownership=kTRUE);

    FairDbUserAuthToken(const FairDbUserAuthToken& from);
    FairDbUserAuthToken& operator=(const FairDbUserAuthToken& from);
    virtual ~FairDbUserAuthToken(void);

    // Dump Object
    void   Print();

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    // Atomic IO (intrinsic)
    virtual void Fill(FairDbResultPool& res_in,
                      const FairDbValRecord* valrec);
    virtual void Store(FairDbOutTableBuffer& res_out,
                       const FairDbValRecord* valrec) const;

    UInt_t GetIndex(UInt_t /*def*/) const { return fId; }

    std::unique_ptr<FairDbUser> GetUser();

    void SetUser(FairDbUser& value);

    Int_t GetId() const { return fId; }
    Int_t GetUserId() const { return fUserId; }
    string GetToken() const { return fToken; }
    ValTimeStamp GetExpirationTime() const { return fExpirationTime; }

    void SetId(Int_t value) { fId = value; SetCompId(fId); }
    void SetUserId(Int_t value) { fUserId = value; }
    void SetToken(string value) { fToken = value; }
    void SetExpirationTime(ValTimeStamp value) { fExpirationTime = value; }

    static std::vector<FairDbUserAuthToken> GetByUserId(Int_t UserId, UInt_t rid=0);
    static std::vector<FairDbUserAuthToken> GetByToken(string Token, UInt_t rid=0);
    static std::vector<FairDbUserAuthToken> GetByExpirationTime(ValTimeStamp ExpirationTime, UInt_t rid=0);

#ifndef __CINT__
    virtual void FillFromJson(Json::Value json);
    virtual void StoreToJson(Json::Value& json);
#endif

  private:
    std::unique_ptr<FairDbUser> fUser; //! transient relation to FairDbUser, Has One

    Int_t fUserId;
    string fToken;
    ValTimeStamp fExpirationTime;

    ClassDef(FairDbUserAuthToken, 1); // ROOT class definition
};

#endif /* !FAIRDBUSERAUTHTOKEN_H */

