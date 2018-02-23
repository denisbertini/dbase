/**
* @file FairDbWtUserResource.cxx
* @brief FairDb web resource class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#include "FairDbWtUserResource.h"
#include "FairDbWtUserSessionStore.h"
#include "FairDbUser.h"
#include <Wt/Auth/PasswordVerifier>
#include <Wt/Auth/HashFunction>
#include <Wt/Auth/PasswordHash>

FairDbWtUserResource::FairDbWtUserResource(Wt::WServer& server, Wt::WObject *parent)
  : FairDbWtBaseResource(server, parent)
{
  AddEndpoint("/User/Login", FairDbUserRole::kUnknown, boost::bind(&FairDbWtUserResource::Login, this, _1, _2, _3, _4));
  // AddEndpoint("/User/Register", FairDbUserRole::kGuest, boost::bind(&FairDbWtUserResource::Register, this, _1, _2, _3, _4));
  AddEndpoint("/User/GetById", FairDbUserRole::kGuest, boost::bind(&FairDbWtUserResource::GetById, this, _1, _2, _3, _4));
  AddEndpoint("/User/Get", FairDbUserRole::kGuest, boost::bind(&FairDbWtUserResource::Get, this, _1, _2, _3, _4));
  AddEndpoint("/User/GetAll", FairDbUserRole::kAdmin, boost::bind(&FairDbWtUserResource::GetAll, this, _1, _2, _3, _4));
}

void FairDbWtUserResource::Login(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
{
  std::string email = requestData.get("email", "").asString();
  std::string password = requestData.get("password", "").asString();

  std::unique_ptr<FairDbUser> user = FairDbWtUserSessionStore::Instance()->Login(email, password);

  if (user) {
    responseData["data"]["auth-token"] = user->GetToken();
    responseData["data"]["auth-user"] = user->ToJson();
  } else {
    responseData["error"] = "Authorization error";
    response.setStatus(401);
  }
}

void FairDbWtUserResource::Register(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
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

  Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
  Wt::Auth::BCryptHashFunction *bcrypt = new Wt::Auth::BCryptHashFunction(7);
  verifier->addHashFunction(bcrypt);

  Wt::Auth::PasswordHash hash = verifier->hashPassword(password);
  user->SetPasswordHash(hash.value());
  user->SetPasswordSalt(hash.salt());
  user->store(ValTimeStamp());
  FairDbUser::PurgeCache();

  requestData["email"] = user->GetEmail();
  Login(request, requestData, response, responseData);
}

void FairDbWtUserResource::GetById(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
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

void FairDbWtUserResource::Get(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
{
  std::string authToken = request.headerValue("auth-token");
  std::unique_ptr<FairDbUser> user = FairDbWtUserSessionStore::Instance()->GetUser(authToken);

  if (user)
  {
    responseData["data"] = user->ToJson();
  } else {
    responseData["error"] = "Not found";
  }
}

void FairDbWtUserResource::GetAll(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
{
  UInt_t rid = requestData.get("Rid", (UInt_t)ValTimeStamp()).asUInt();
  std::vector<FairDbUser> array = FairDbUser::GetAll(rid);
  Json::Value jsonArray = FairDbUser::ToJsonArray(array);
  responseData["data"] = jsonArray;
}