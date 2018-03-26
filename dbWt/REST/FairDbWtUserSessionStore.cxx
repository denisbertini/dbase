/**
* @file FairDbWtUserSessionStore.cxx
* @author Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#include "FairDbWtUserSessionStore.h"
#include "FairDbWtPasswordService.h"

FairDbWtUserSessionStore* FairDbWtUserSessionStore::fInstance  = 0;

FairDbWtUserSessionStore* FairDbWtUserSessionStore::Instance()
{
  if (!fInstance) {
    fInstance = new FairDbWtUserSessionStore();
  }
  return fInstance;
}

FairDbWtUserSessionStore::FairDbWtUserSessionStore()
  : fSessionAuth()
{
}

FairDbWtUserSessionStore::~FairDbWtUserSessionStore()
{
}

std::string FairDbWtUserSessionStore::GetToken(std::string email, std::string password)
{
  std::vector<FairDbUser> users = FairDbUser::GetByEmail(email, ValTimeStamp());
  if (!users.size()) {
    return std::string();
  }

  FairDbUser user = users.front();
  if (user.GetStatus() != FairDbUserStatus::kActive) {
    return std::string();
  }

  bool result = FairDbWtPasswordService::VerifyPassword(password, { user.GetPasswordHash(), user.GetPasswordSalt() } );

  if (!result) {
    return std::string();
  }

  std::string token(FairDbWtPasswordService::HashPassword(std::string()).Hash);
  fSessionAuth[token] = user;

  return token;
}

std::unique_ptr<FairDbUser> FairDbWtUserSessionStore::Login(std::string email, std::string password)
{
  std::vector<FairDbUser> users = FairDbUser::GetByEmail(email, ValTimeStamp());
  if (!users.size()) {
    return nullptr;
  }

  FairDbUser user = users.front();
  if (user.GetStatus() != FairDbUserStatus::kActive) {
    return nullptr;
  }

  bool result = FairDbWtPasswordService::VerifyPassword(password, { user.GetPasswordHash(), user.GetPasswordSalt() } );

  if (!result) {
    return nullptr;
  }

  std::string token(FairDbWtPasswordService::HashPassword(std::string()).Hash);
  user.SetToken(token);
  fSessionAuth[token] = user;

  return std::unique_ptr<FairDbUser>(new FairDbUser(user));
}

std::unique_ptr<FairDbUser> FairDbWtUserSessionStore::GetUser(std::string token)
{
  if (fSessionAuth.find(token) == fSessionAuth.end()) {
    return nullptr;
  }

  FairDbUser user = fSessionAuth[token];
  return std::unique_ptr<FairDbUser>(new FairDbUser(user));
}

bool FairDbWtUserSessionStore::VerifyToken(std::string token)
{
  return fSessionAuth.find(token) != fSessionAuth.end() && fSessionAuth[token].GetToken() == token;
}

bool FairDbWtUserSessionStore::CheckUserRole(std::string token, FairDbUserRole::UserRole_t role)
{
  return VerifyToken(token) && fSessionAuth[token].GetRole() >= role;
}
