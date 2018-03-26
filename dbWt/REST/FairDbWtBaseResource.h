/**
* @file FairDbWtBaseResource.h
* @brief FairDb web resource class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#ifndef FAIRDBWTBASERESOURCE_H
#define FAIRDBWTBASERESOURCE_H

#include <WServer>
#include <WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include "FairDbUser.h"

#include <jsoncons/json.hpp>

class FairDbWtBaseResource: public Wt::WResource
{
  public:
    typedef boost::function<void (const Wt::Http::Request&, jsoncons::json, Wt::Http::Response&, jsoncons::json&)> EndpointCallback;

    FairDbWtBaseResource(Wt::WServer& server, Wt::WObject *parent = 0);

    virtual ~FairDbWtBaseResource();

    void handleRequest(const Wt::Http::Request& request,
                             Wt::Http::Response& response);

  protected:
    void AddEndpoint(std::string endpoint, FairDbUserRole::UserRole_t requiredRole, EndpointCallback function);
    void SetEndpointRequiredRole(std::string endpoint, FairDbUserRole::UserRole_t requiredRole);
    Wt::WServer* fServer;

  private:
    bool CheckRole(FairDbUserRole::UserRole_t requiredRole, const Wt::Http::Request& request);
    std::map< std::string, std::tuple<FairDbUserRole::UserRole_t, EndpointCallback> > fEndpoints;
};

#endif /* !FAIRDBWTBASERESOURCE_H */
