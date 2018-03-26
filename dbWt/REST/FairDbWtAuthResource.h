/**
* @file FairDbWtAuthResource.h
* @brief FairDb web resource class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#ifndef FAIRDBWTAUTHRESOURCE_H
#define FAIRDBWTAUTHRESOURCE_H

#include <WServer>
#include <WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include "FairDbWtBaseResource.h"

#include <jsoncons/json.hpp>

class FairDbWtAuthResource: public FairDbWtBaseResource
{
  public:
    FairDbWtAuthResource(Wt::WServer& server, Wt::WObject *parent = 0);

    void Login(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json &responseData);
};

#endif /* !FAIRDBWTAUTHRESOURCE_H */

