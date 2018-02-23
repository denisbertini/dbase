/**
* @file FairDbUserAuthToken.cxx
* @brief  FairDb class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#include "FairDbUserAuthToken.h"
#include "FairDbUser.h"

ClassImp(FairDbUserAuthToken);

static FairDbGenericParRegistry<FairDbUserAuthToken> ClassRegistry("FairDbUserAuthToken", FairDbDetector::kUnknown, DataType::kUnknown);

#include "FairDbRelationalParSet.tpl"
template class FairDbRelationalParSet<FairDbUserAuthToken>;

#include "FairDbGenericParSet.tpl"
template class FairDbGenericParSet<FairDbUserAuthToken>;

#include "FairDbReader.tpl"
template class FairDbReader<FairDbUserAuthToken>;

#include "FairDbWriter.tpl"
template class FairDbWriter<FairDbUserAuthToken>;

FairDbUserAuthToken::FairDbUserAuthToken(FairDbDetector::Detector_t detid,
              DataType::DataType_t dataid,
              const char* name,
              const char* title,
              const char* context,
              Bool_t ownership):
  FairDbRelationalParSet<FairDbUserAuthToken>(detid, dataid, name, title, context, ownership),
  fUserId(0),
  fUser(nullptr),
  fToken(""),
  fExpirationTime(ValTimeStamp::GetBOT())
{
}

FairDbUserAuthToken::FairDbUserAuthToken(const FairDbUserAuthToken& from)
  :FairDbRelationalParSet<FairDbUserAuthToken>(from)
{
  fUserId = from.fUserId;
  fUser = copy_unique(from.fUser);
  fToken = from.fToken;
  fExpirationTime = from.fExpirationTime;
}

FairDbUserAuthToken& FairDbUserAuthToken::operator=(const FairDbUserAuthToken& from)
{
  if (this == &from) { return *this; }

  FairDbRelationalParSet<FairDbUserAuthToken>::operator=(from);

  SetUserId(from.GetUserId());
  SetToken(from.GetToken());
  SetExpirationTime(from.GetExpirationTime());

  return *this;
}

FairDbUserAuthToken::~FairDbUserAuthToken()
{
}

void FairDbUserAuthToken::Print()
{
  std::cout << " Instance:" << std::endl;

  std::cout << "Id = " << fId << std::endl;
  std::cout << "UserId = " << fUserId << std::endl;
  std::cout << "Token = " << fToken << std::endl;
  std::cout << "ExpirationTime = " << fExpirationTime << std::endl;

  std::cout << std::endl;
}

std::unique_ptr<FairDbUser> FairDbUserAuthToken::GetUser() {
  return copy_unique(fUser);
}

void FairDbUserAuthToken::SetUser(FairDbUser& value) {
  fUser = std::unique_ptr<FairDbUser>(new FairDbUser(value));
  fUserId = value.GetId();
}

string FairDbUserAuthToken::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO INT NOT NULL,";
  sql += "  ROW_ID INT NOT NULL,";

  sql += "  ID INT NOT NULL,";
  sql += "  USER_ID INT,";
  sql += "  TOKEN TEXT,";
  sql += "  EXPIRATION_TIME DATETIME,";

  sql += "  primary key(SEQNO,ROW_ID,ID) )";
  return sql;
}

void FairDbUserAuthToken::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{

  res_in >> fId;
  res_in >> fUserId;
  res_in >> fToken;
  res_in >> fExpirationTime;
}

void FairDbUserAuthToken::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{

  res_out << fId;
  res_out << fUserId;
  res_out << fToken;
  res_out << fExpirationTime;
}


std::vector<FairDbUserAuthToken> FairDbUserAuthToken::GetByUserId(Int_t UserId, UInt_t rid)
{
  return FairDbUserAuthToken::GetBy(
    [&UserId](const FairDbUserAuthToken inst) -> bool
      {
        return UserId == inst.GetUserId();
      }, rid);
}

std::vector<FairDbUserAuthToken> FairDbUserAuthToken::GetByToken(string Token, UInt_t rid)
{
  return FairDbUserAuthToken::GetBy(
    [&Token](const FairDbUserAuthToken inst) -> bool
      {
        return Token == inst.GetToken();
      }, rid);
}

std::vector<FairDbUserAuthToken> FairDbUserAuthToken::GetByExpirationTime(ValTimeStamp ExpirationTime, UInt_t rid)
{
  return FairDbUserAuthToken::GetBy(
    [&ExpirationTime](const FairDbUserAuthToken inst) -> bool
      {
        return ExpirationTime == inst.GetExpirationTime();
      }, rid);
}

void FairDbUserAuthToken::FillFromJson(Json::Value json)
{
  SetId(json["Id"].asInt());
  SetUserId(json["UserId"].asInt());
  SetToken(json["Token"].asString());
  SetExpirationTime(json["ExpirationTime"].asInt());


}

void FairDbUserAuthToken::StoreToJson(Json::Value& json)
{
  json["Id"] = fId;
  json["UserId"] = fUserId;
  json["Token"] = fToken;
  json["ExpirationTime"] = (Int_t) fExpirationTime;


  json["User"] = Json::Value(Json::nullValue);

}

