/**
* @file FairDbWtUserSessionStore.h
* @author Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#ifndef FAIRDBWTUSERSESSIONSTORE_H
#define FAIRDBWTUSERSESSIONSTORE_H

#include "FairDbUser.h"

class FairDbWtUserSessionStore
{
  public:
    static FairDbWtUserSessionStore* Instance();

    std::string GetToken(std::string email, std::string password);
    std::unique_ptr<FairDbUser> Login(std::string email, std::string password);
    std::unique_ptr<FairDbUser> GetUser(std::string token);
    bool VerifyToken(std::string token);
    bool CheckUserRole(std::string token, FairDbUserRole::UserRole_t role);

  private:
    FairDbWtUserSessionStore();
    virtual ~FairDbWtUserSessionStore();

    std::map<std::string, FairDbUser> fSessionAuth;
    static FairDbWtUserSessionStore* fInstance;
};

#endif /* !FAIRDBWTUSERSESSIONSTORE_H */

