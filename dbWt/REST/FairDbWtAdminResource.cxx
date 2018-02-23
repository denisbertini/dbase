/**
* @file FairDbWtAdminResource.cxx
* @brief FairDb web resource class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#include "FairDbWtAdminResource.h"
#include "FairDbWtUserSessionStore.h"
#include <Wt/Auth/PasswordVerifier>
#include <Wt/Auth/HashFunction>
#include <Wt/Auth/PasswordHash>

FairDbWtAdminResource::FairDbWtAdminResource(Wt::WServer& server, Wt::WObject *parent)
  : FairDbWtBaseResource(server, parent)
{
  AddEndpoint("/Admin/RegisterUser", FairDbUserRole::kAdmin, boost::bind(&FairDbWtAdminResource::RegisterUser, this, _1, _2, _3, _4));
  AddEndpoint("/Admin/ChangeUserPassword", FairDbUserRole::kAdmin, boost::bind(&FairDbWtAdminResource::ChangeUserPassword, this, _1, _2, _3, _4));
  AddEndpoint("/Admin/ChangeUser", FairDbUserRole::kAdmin, boost::bind(&FairDbWtAdminResource::ChangeUser, this, _1, _2, _3, _4));
  AddEndpoint("/Admin/GetAllUsers", FairDbUserRole::kAdmin, boost::bind(&FairDbWtAdminResource::GetAllUsers, this, _1, _2, _3, _4));
  AddEndpoint("/Admin/GetUser", FairDbUserRole::kAdmin, boost::bind(&FairDbWtAdminResource::GetUser, this, _1, _2, _3, _4));
}

void FairDbWtAdminResource::RegisterUser(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
{
  Json::Value userJson = requestData.get("user", Json::nullValue);
  if (userJson == Json::nullValue)
  {
    responseData["error"] = "User not specified";
    response.setStatus(400);
    return;
  }

  std::string password = requestData.get("password", std::string()).asString();
  if (password.empty())
  {
    responseData["error"] = "Password is empty or not specified";
    response.setStatus(400);
    return;
  }

  std::unique_ptr<FairDbUser> user = FairDbUser::FromJson(userJson);
  user->SetId(-1);

  Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
  Wt::Auth::BCryptHashFunction *bcrypt = new Wt::Auth::BCryptHashFunction(7);
  verifier->addHashFunction(bcrypt);

  Wt::Auth::PasswordHash hash = verifier->hashPassword(password);
  user->SetPasswordHash(hash.value());
  user->SetPasswordSalt(hash.salt());
  user->store(ValTimeStamp());
  FairDbUser::PurgeCache();

  responseData["data"]["Id"] = user->GetId();
}

void FairDbWtAdminResource::ChangeUserPassword(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
{
  Int_t userId = requestData.get("Id", -1).asInt();
  if (userId == -1)
  {
    responseData["error"] = "User not specified";
    response.setStatus(400);
    return;
  }

  std::string password = requestData.get("password", std::string()).asString();
  if (password.empty())
  {
    responseData["error"] = "Password is empty or not specified";
    response.setStatus(400);
    return;
  }

  std::unique_ptr<FairDbUser> user = FairDbUser::GetById(userId, ValTimeStamp());

  Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
  Wt::Auth::BCryptHashFunction *bcrypt = new Wt::Auth::BCryptHashFunction(7);
  verifier->addHashFunction(bcrypt);

  Wt::Auth::PasswordHash hash = verifier->hashPassword(password);
  user->SetPasswordHash(hash.value());
  user->SetPasswordSalt(hash.salt());
  user->store(ValTimeStamp());
  FairDbUser::PurgeCache();
}

void FairDbWtAdminResource::ChangeUser(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
{
  Json::Value userJson = requestData.get("user", Json::nullValue);
  if (userJson == Json::nullValue)
  {
    responseData["error"] = "User not specified";
    response.setStatus(400);
    return;
  }

  std::unique_ptr<FairDbUser> modifiedUser = FairDbUser::FromJson(userJson);
  std::unique_ptr<FairDbUser> user = FairDbUser::GetById(modifiedUser->GetId(), ValTimeStamp());
  if (!user)
  {
    responseData["error"] = "User not found";
    response.setStatus(400);
    return;
  }

  // carefully copy over only some selected properties
  user->SetFullName(modifiedUser->GetFullName());
  user->SetAddress(modifiedUser->GetAddress());
  user->SetEmail(modifiedUser->GetEmail());
  user->SetStatus(modifiedUser->GetStatus());
  user->SetRole(modifiedUser->GetRole());

  user->store(ValTimeStamp());
  FairDbUser::PurgeCache();
  // remove all user sessions maybe?
}

void FairDbWtAdminResource::GetAllUsers(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
{
  UInt_t rid = requestData.get("Rid", (UInt_t)ValTimeStamp()).asUInt();
  std::vector<FairDbUser> array = FairDbUser::GetAll(rid);
  Json::Value jsonArray = FairDbUser::ToJsonArray(array);
  responseData["data"] = jsonArray;
}

void FairDbWtAdminResource::GetUser(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
{
  Int_t id = requestData.get("Id", -1).asInt();
  UInt_t rid = requestData.get("Rid", (UInt_t)ValTimeStamp()).asUInt();
  std::unique_ptr<FairDbUser> user = FairDbUser::GetById(id, rid);

  if (user)
  {
    responseData["data"] = user->ToJson();
  } else {
    responseData["error"] = "Not found";
  }
}