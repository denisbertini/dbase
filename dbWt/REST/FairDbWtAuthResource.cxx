/**
* @file FairDbWtAuthResource.cxx
* @brief FairDb web resource class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#include "FairDbWtAuthResource.h"
#include "FairDbWtUserSessionStore.h"
#include "FairDbUser.h"


FairDbWtAuthResource::FairDbWtAuthResource(Wt::WServer& server, Wt::WObject *parent)
  : FairDbWtBaseResource(server, parent)
{
  AddEndpoint("/Auth/Login", FairDbUserRole::kUnknown, boost::bind(&FairDbWtAuthResource::Login, this, _1, _2, _3, _4));
}

void FairDbWtAuthResource::Login(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json &responseData)
{
  std::string email = requestData.get_with_default("email", std::string());
  std::string password = requestData.get_with_default("password", std::string());

  std::unique_ptr<FairDbUser> user = FairDbWtUserSessionStore::Instance()->Login(email, password);

  if (user) {
    responseData["data"]["auth-token"] = user->GetToken();
    responseData["data"]["auth-user"] = user->ToJson();
  } else {
    responseData["error"] = "Authorization error";
    response.setStatus(401);
  }
}