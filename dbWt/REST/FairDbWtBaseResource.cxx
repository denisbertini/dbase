/**
* @file FairDbWtBaseResource.cxx
* @brief Sts Sensor Batch Entity FairDb web resource class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#include "FairDbWtBaseResource.h"
#include "FairDbWtUserSessionStore.h"
#include <Wt/Auth/PasswordVerifier>
#include <Wt/Auth/HashFunction>
#include <Wt/Auth/PasswordHash>

FairDbWtBaseResource::FairDbWtBaseResource(Wt::WServer& server, Wt::WObject *parent)
  : Wt::WResource(parent),
    fEndpoints(),
    fServer(&server)
{
}

FairDbWtBaseResource::~FairDbWtBaseResource()
{
  beingDeleted();
}

void FairDbWtBaseResource::AddEndpoint(std::string endpoint, FairDbUserRole::UserRole_t requiredRole, EndpointCallback function)
{
  fEndpoints[endpoint] = std::make_tuple(requiredRole, function);
  fServer->addResource(this, endpoint);
}

void FairDbWtBaseResource::SetEndpointRequiredRole(std::string endpoint, FairDbUserRole::UserRole_t requiredRole)
{
  if (fEndpoints.find(endpoint) == fEndpoints.end())
  {
    return;
  }

  std::get<0>(fEndpoints[endpoint]) = requiredRole;
}

bool FairDbWtBaseResource::CheckRole(FairDbUserRole::UserRole_t requiredRole, const Wt::Http::Request& request)
{
  std::string authToken = request.headerValue("auth-token");
  bool result = FairDbWtUserSessionStore::Instance()->CheckUserRole(authToken, requiredRole);

  return result;
}

void FairDbWtBaseResource::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
  // enforce https
  if (true && request.urlScheme() != "https")
  {
    response.setStatus(426);
    response.addHeader("Upgrade", "TLS/1.0, HTTP/1.1");
    response.addHeader("Connection", "Upgrade");
    response.setMimeType("text/plain");
    response.out() << "HTTPS Required";

    return;
  }

  response.addHeader("Access-Control-Allow-Origin", "*");
  response.addHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, auth-token");
  response.setMimeType("application/octet");
  if (request.method() != "POST")
  {
    return;
  }

  jsoncons::json requestData;
  jsoncons::json responseData;
  responseData["data"] = jsoncons::json::object();
  responseData["error"] = jsoncons::json::object();

  try
  {
    std::string url(request.path());
    if (fEndpoints.find(url) == fEndpoints.end())
    {
      return;
    }

    request.in() >> requestData;
#ifdef REST_SERVER_VERBOSE
    std::cout << "Request Data:\n" << requestData << std::endl;
#endif

    FairDbUserRole::UserRole_t requiredRole = std::get<0>(fEndpoints[url]);
    EndpointCallback endpointCallback = std::get<1>(fEndpoints[url]);

    if (requiredRole == 0)
    {
      endpointCallback(request, requestData, response, responseData);
    } else if (requiredRole > 0)
    {
      bool roleCheckResult = CheckRole(requiredRole, request);
      if (roleCheckResult)
      {
        endpointCallback(request, requestData, response, responseData);
      } else
      {
        response.setStatus(403);
        responseData["error"] = "Permission denied";
      }
    }

    if (responseData["data"].empty()) {
      responseData["data"] = jsoncons::json::null();
    }
    if (responseData["error"].empty()) {
      responseData["error"] = jsoncons::json::null();
    }

    response.out() << responseData;
#ifdef REST_SERVER_VERBOSE
    std::cout << "Response Data:\n" << responseData << std::endl;
#endif
  } catch (std::exception &e) {
    responseData["data"] = jsoncons::json::null();
    responseData["error"] = e.what();
    response.out() << responseData;
    response.setStatus(400);
#ifdef REST_SERVER_VERBOSE
    std::cout << "Error processing request:\n" << requestData << std::endl;
#endif
  }
}