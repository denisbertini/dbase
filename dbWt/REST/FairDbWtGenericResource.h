/**
* @file FairDbWtGenericResource.h
* @brief FairDb web resource class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#ifndef FAIRDBWTGENERICRESOURCE_H
#define FAIRDBWTGENERICRESOURCE_H

#include <WServer>
#include <WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include "FairDbWtBaseResource.h"

#include <jsoncons/json.hpp>

template <typename T>
class FairDbWtGenericResource: public FairDbWtBaseResource
{
  public:
    FairDbWtGenericResource(Wt::WServer& server, Wt::WObject *parent = 0);

    void GetById(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json& responseData);
    void GetAllVersions(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json& responseData);
    void GetArray(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json& responseData);
    void GetAll(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json& responseData);
    void Store(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json& responseData);
    void StoreArray(const Wt::Http::Request& request, jsoncons::json requestData, Wt::Http::Response& response, jsoncons::json& responseData);
};

#endif /* !FAIRDBWTGENERICRESOURCE_H */
