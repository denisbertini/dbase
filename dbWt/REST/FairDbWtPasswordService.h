#ifndef FAIRDBWTPASSWORDSERVICE_H
#define FAIRDBWTPASSWORDSERVICE_H

#include <string>

struct FairDbWtPasswordData
{
  std::string Hash;
  std::string Salt;
};

class FairDbWtPasswordService
{
  public:
    static FairDbWtPasswordData HashPassword(std::string password);
    static bool VerifyPassword(std::string password, FairDbWtPasswordData passwordData);
};

#endif /* !FAIRDBWTPASSWORDSERVICE_H */

