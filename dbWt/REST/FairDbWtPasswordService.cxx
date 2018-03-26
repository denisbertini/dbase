#include "FairDbWtPasswordService.h"

#include <Wt/Auth/PasswordVerifier>
#include <Wt/Auth/HashFunction>
#include <Wt/Auth/PasswordHash>
#include <Wt/WString>

FairDbWtPasswordData FairDbWtPasswordService::HashPassword(std::string password)
{
  Wt::Auth::PasswordVerifier verifier = Wt::Auth::PasswordVerifier();
  Wt::Auth::BCryptHashFunction *bcrypt = new Wt::Auth::BCryptHashFunction(7);
  verifier.addHashFunction(bcrypt);

  Wt::Auth::PasswordHash hash = verifier.hashPassword(password);
  return { hash.value(), hash.salt() };
}

bool FairDbWtPasswordService::VerifyPassword(std::string password, FairDbWtPasswordData passwordData)
{
  Wt::Auth::PasswordVerifier verifier = Wt::Auth::PasswordVerifier();
  Wt::Auth::BCryptHashFunction *bcrypt = new Wt::Auth::BCryptHashFunction(7);
  verifier.addHashFunction(bcrypt);

  Wt::Auth::PasswordHash hash(bcrypt->name(), passwordData.Salt, passwordData.Hash);
  return verifier.verify(password, hash);
}
