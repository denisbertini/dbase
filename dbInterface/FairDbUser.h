/**
* @file FairDbUser.h
* @brief  FairDb class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 1.12.2017
**/

#ifndef FAIRDBUSER_H
#define FAIRDBUSER_H

#include "FairDbRelationalParSet.h"

#include <jsoncons/json.hpp>

namespace FairDbUserRole
{
  enum UserRole_t {
    kUnknown  = 0,
    kGuest    = 10,
    kOperator = 100,
    kAdmin    = 1000,
  };
}

namespace FairDbUserStatus
{
  enum UserStatus_t {
    kUnknown   = 0,
    kDisabled  = 1,
    kActive    = 2,
  };
}

class FairDbUser : public FairDbRelationalParSet<FairDbUser>
{
  using TObject::Compare;

  public :
    FairDbUser(FairDbDetector::Detector_t detid = FairDbDetector::kSts,
          DataType::DataType_t dataid = DataType::kData,
          const char* name = "FairDbUser",
          const char* title = "",
          const char* context = "DefaultContext",
          Bool_t ownership=kTRUE);

    FairDbUser(const FairDbUser& from);
    FairDbUser& operator=(const FairDbUser& from);
    virtual ~FairDbUser(void);

    // Dump Object
    void   Print();

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    virtual bool Validate(const FairDbValRecord* valrec) const;

    // Atomic IO (intrinsic)
    virtual void Fill(FairDbResultPool& res_in,
                      const FairDbValRecord* valrec);
    virtual void Store(FairDbOutTableBuffer& res_out,
                       const FairDbValRecord* valrec) const;

    UInt_t GetIndex(UInt_t /*def*/) const { return fId; }

    Int_t GetId() const { return fId; }
    string GetFullName() const { return fFullName; }
    string GetEmail() const { return fEmail; }
    string GetAddress() const { return fAddress; }
    string GetPasswordHash() const { return fPasswordHash; }
    string GetPasswordSalt() const { return fPasswordSalt; }
    Int_t GetStatus() const { return fStatus; }
    Int_t GetRole() const { return fRole; }
    string GetToken() const { return fToken; }

    void SetId(Int_t value) { fId = value; SetCompId(fId); }
    void SetFullName(string value) { fFullName = value; }
    void SetEmail(string value) { fEmail = value; }
    void SetAddress(string value) { fAddress = value; }
    void SetPasswordHash(string value) { fPasswordHash = value; }
    void SetPasswordSalt(string value) { fPasswordSalt = value; }
    void SetStatus(Int_t value) { fStatus = value; }
    void SetRole(Int_t value) { fRole = value; }
    void SetToken(string value) { fToken = value; }

    static std::vector<FairDbUser> GetByFullName(string FullName, UInt_t rid=ValTimeStamp());
    static std::vector<FairDbUser> GetByEmail(string Email, UInt_t rid=ValTimeStamp());
    static std::vector<FairDbUser> GetByAddress(string Address, UInt_t rid=ValTimeStamp());
    static std::vector<FairDbUser> GetByStatus(Int_t Status, UInt_t rid=ValTimeStamp());
    static std::vector<FairDbUser> GetByRole(Int_t Role, UInt_t rid=ValTimeStamp());

    virtual void FillFromJson(jsoncons::json json);
    virtual void StoreToJson(jsoncons::json& json);

  private:

    string fFullName;
    string fEmail;
    string fAddress;
    string fPasswordHash;
    string fPasswordSalt;
    Int_t fStatus;
    Int_t fRole;
    string fToken; //! transient

    ClassDef(FairDbUser, 1); // ROOT class definition
};

#endif /* !FAIRDBUSER_H */
