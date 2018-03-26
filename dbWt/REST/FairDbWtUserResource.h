/**
* @file FairDbWtUserResource.h
* @brief FairDb web resource class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#ifndef FAIRDBWTUSERRESOURCE_H
#define FAIRDBWTUSERRESOURCE_H

#include <WServer>
#include <WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include "FairDbWtBaseResource.h"

#include <jsoncons/json.hpp>

class FairDbWtUserResource: public FairDbWtBaseResource
{
  public:
    FairDbWtUserResource(Wt::WServer& server, Wt::WObject *parent = 0);

    void Login(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json &responseData);
    void Register(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json &responseData);
    void GetById(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json &responseData);
    void Get(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json &responseData);
    void GetAll(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json &responseData);
};

#endif /* !FAIRDBWTUSERRESOURCE_H */

