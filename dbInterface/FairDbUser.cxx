/**
* @file FairDbUser.cxx
* @brief  FairDb class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 1.12.2017
**/

#include "FairDbUser.h"

ClassImp(FairDbUser);

static FairDbGenericParRegistry<FairDbUser> ClassRegistry("FairDbUser", FairDbDetector::kUnknown, DataType::kUnknown);

#include "FairDbRelationalParSet.tpl"
template class FairDbRelationalParSet<FairDbUser>;

#include "FairDbGenericParSet.tpl"
template class FairDbGenericParSet<FairDbUser>;

#include "FairDbReader.tpl"
template class FairDbReader<FairDbUser>;

#include "FairDbWriter.tpl"
template class FairDbWriter<FairDbUser>;

FairDbUser::FairDbUser(FairDbDetector::Detector_t detid,
              DataType::DataType_t dataid,
              const char* name,
              const char* title,
              const char* context,
              Bool_t ownership):
  FairDbRelationalParSet<FairDbUser>(detid, dataid, name, title, context, ownership),
  fFullName(""),
  fEmail(""),
  fAddress(""),
  fPasswordHash(""),
  fPasswordSalt(""),
  fStatus(0),
  fRole(0),
  fToken("")
{
}

FairDbUser::FairDbUser(const FairDbUser& from)
  :FairDbRelationalParSet<FairDbUser>(from)
{
  fFullName = from.fFullName;
  fEmail = from.fEmail;
  fAddress = from.fAddress;
  fPasswordHash = from.fPasswordHash;
  fPasswordSalt = from.fPasswordSalt;
  fStatus = from.fStatus;
  fRole = from.fRole;
  fToken = from.fToken;
}

FairDbUser& FairDbUser::operator=(const FairDbUser& from)
{
  if (this == &from) { return *this; }

  FairDbRelationalParSet<FairDbUser>::operator=(from);

  SetFullName(from.GetFullName());
  SetEmail(from.GetEmail());
  SetAddress(from.GetAddress());
  SetPasswordHash(from.GetPasswordHash());
  SetPasswordSalt(from.GetPasswordSalt());
  SetStatus(from.GetStatus());
  SetRole(from.GetRole());
  SetToken(from.GetToken());

  return *this;
}

FairDbUser::~FairDbUser()
{
}

void FairDbUser::Print()
{
  std::cout << " Instance:" << std::endl;

  std::cout << "Id = " << fId << std::endl;
  std::cout << "FullName = " << fFullName << std::endl;
  std::cout << "Email = " << fEmail << std::endl;
  std::cout << "Address = " << fAddress << std::endl;
  std::cout << "PasswordHash = " << fPasswordHash << std::endl;
  std::cout << "PasswordSalt = " << fPasswordSalt << std::endl;
  std::cout << "Status = " << fStatus << std::endl;
  std::cout << "Role = " << fRole << std::endl;
  std::cout << "Token = " << fToken << std::endl;

  std::cout << std::endl;
}

string FairDbUser::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO INT NOT NULL,";
  sql += "  ROW_ID INT NOT NULL,";

  sql += "  ID INT NOT NULL,";
  sql += "  FULL_NAME TEXT,";
  sql += "  EMAIL TEXT,";
  sql += "  ADDRESS TEXT,";
  sql += "  PASSWORD_HASH TEXT,";
  sql += "  PASSWORD_SALT TEXT,";
  sql += "  STATUS INT,";
  sql += "  ROLE INT,";

  sql += "  primary key(SEQNO,ROW_ID,ID) )";
  return sql;
}

void FairDbUser::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{

  res_in >> fId;
  res_in >> fFullName;
  res_in >> fEmail;
  res_in >> fAddress;
  res_in >> fPasswordHash;
  res_in >> fPasswordSalt;
  res_in >> fStatus;
  res_in >> fRole;
}

void FairDbUser::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  if (fPasswordHash.empty()) {
    std::cout << "FairDbUser: Will not accept an empty password hash" << std::endl;
    throw std::runtime_error("FairDbUser: Will not accept an empty password hash");
  }

  res_out << fId;
  res_out << fFullName;
  res_out << fEmail;
  res_out << fAddress;
  res_out << fPasswordHash;
  res_out << fPasswordSalt;
  res_out << fStatus;
  res_out << fRole;
}

std::vector<FairDbUser> FairDbUser::GetByFullName(string FullName, UInt_t rid)
{
  return FairDbUser::GetBy(
    [&FullName](const FairDbUser& inst) -> bool
      {
        return FullName == inst.GetFullName();
      }, rid);
}

std::vector<FairDbUser> FairDbUser::GetByEmail(string Email, UInt_t rid)
{
  return FairDbUser::GetBy(
    [&Email](const FairDbUser& inst) -> bool
      {
        return Email == inst.GetEmail();
      }, rid);
}

std::vector<FairDbUser> FairDbUser::GetByAddress(string Address, UInt_t rid)
{
  return FairDbUser::GetBy(
    [&Address](const FairDbUser& inst) -> bool
      {
        return Address == inst.GetAddress();
      }, rid);
}

std::vector<FairDbUser> FairDbUser::GetByStatus(Int_t Status, UInt_t rid)
{
  return FairDbUser::GetBy(
    [&Status](const FairDbUser& inst) -> bool
      {
        return Status == inst.GetStatus();
      }, rid);
}

std::vector<FairDbUser> FairDbUser::GetByRole(Int_t Role, UInt_t rid)
{
  return FairDbUser::GetBy(
    [&Role](const FairDbUser& inst) -> bool
      {
        return Role == inst.GetRole();
      }, rid);
}

void FairDbUser::FillFromJson(Json::Value json)
{
  SetId(json["Id"].asInt());
  SetFullName(json["FullName"].asString());
  SetEmail(json["Email"].asString());
  SetAddress(json["Address"].asString());
  SetStatus(json["Status"].asInt());
  SetRole(json["Role"].asInt());
}

void FairDbUser::StoreToJson(Json::Value& json)
{
  json["Id"] = fId;
  json["FullName"] = fFullName;
  json["Email"] = fEmail;
  json["Address"] = fAddress;
  json["Status"] = fStatus;
  json["Role"] = fRole;
}
