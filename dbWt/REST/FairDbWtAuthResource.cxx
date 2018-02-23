/**
* @file FairDbWtAuthResource.cxx
* @brief FairDb web resource class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#include "FairDbWtAuthResource.h"
#include "FairDbWtUserSessionStore.h"
#include "FairDbUser.h"


constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1)*33) ^ str[h];
}

FairDbWtAuthResource::FairDbWtAuthResource(Wt::WServer& server, Wt::WObject *parent)
  : Wt::WResource(parent)
{
  server.addResource(this, "/Auth/Login");
}

FairDbWtAuthResource::~FairDbWtAuthResource()
{
  beingDeleted();
}

void FairDbWtAuthResource::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
  response.addHeader("Access-Control-Allow-Origin", "*");
  response.addHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, auth-token");
  response.setMimeType("application/json");
  if (request.method() == "OPTIONS")
  {
    return;
  }

  Json::Value responseData;
  responseData["data"] = Json::Value(Json::nullValue);
  responseData["error"] = Json::Value(Json::nullValue);

  try
  {
    std::string url(request.path());
    Json::Value requestData;
    request.in() >> requestData;
#ifdef REST_SERVER_VERBOSE
    std::cout << "Request Data:\n" << requestData << std::endl;
#endif

    switch (str2int(url.c_str()))
    {
      case str2int("/Auth/Login"):
      {
        Login(request, requestData, response, responseData);
        break;
      }
    }

    response.out() << responseData;
#ifdef REST_SERVER_VERBOSE
    std::cout << "Response Data:\n" << responseData << std::endl;
#endif
  } catch (Json::Exception &e) {
    responseData["error"] = e.what();
    response.out() << responseData;
    response.setStatus(400);
#ifdef REST_SERVER_VERBOSE
    std::cout << "Error processing request:\n" << responseData << std::endl;
#endif
  }
}

// void FairDbWtAuthResource::Login(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
// {
//   std::string email = requestData.get("email", "").asString();
//   std::string password = requestData.get("password", "").asString();

//   std::string token = FairDbWtUserSessionStore::Instance()->GetToken(email, password);

//   if (!token.empty()) {
//     responseData["data"]["auth-token"] = token;
//   } else {
//     responseData["error"] = "Authorization error";
//     response.setStatus(401);
//   }
// }

void FairDbWtAuthResource::Login(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
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