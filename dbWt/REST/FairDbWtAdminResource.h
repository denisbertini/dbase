/**
* @file FairDbWtAdminResource.h
* @brief FairDb web resource class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#ifndef FAIRDBWTADMINRESOURCE_H
#define FAIRDBWTADMINRESOURCE_H

#include <WServer>
#include <WResource>
#include "FairDbWtBaseResource.h"
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include "FairDbUser.h"

#include <jsoncons/json.hpp>

// void EndpointCallback(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json &responseData);

class FairDbWtAdminResource: public FairDbWtBaseResource
{
  public:
    FairDbWtAdminResource(Wt::WServer& server, Wt::WObject *parent = 0);

    void GetUser(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json &responseData);
    void GetAllUsers(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json &responseData);
    void ChangeUserPassword(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json &responseData);
    void ChangeUser(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json &responseData);
    void RegisterUser(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json &responseData);
};

#endif /* !FAIRDBWTADMINRESOURCE_H */

