#include "FairDbExTask.h"

#include "FairDbExPar4.h"               
#include "FairRunAna.h"                 // for FairRunAna
#include "FairRuntimeDb.h"              // for FairRuntimeDb

#include <iosfwd>                    // for ostream

#include <iostream>                     // for operator<<, basic_ostream, etc

using std::cout;
using std::endl;

ClassImp(FairDbExTask);

void FairDbExTask::SetParContainers()
{
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();
  fTutParDef = (FairDbExPar4*)(rtdb->getContainer("ExPar4"));
  fTutParAlt = (FairDbExPar4*)(rtdb->getContainer("ExPar4New"));
}

InitStatus FairDbExTask::Init()
{
  if (!fTutParDef) {
    std::cout<<"-E- FairDbExTask: fTutParDef Container does not exist!"<<std::endl;
    return kERROR;
  }
  if (!fTutParAlt) {
    std::cout<<"-E- FairDbExTask: fTutParAlt Container does not exist!"<<std::endl;
    return kERROR;
  }
  return kSUCCESS;

}

InitStatus FairDbExTask::ReInit()
{
  SetParContainers();
  return kSUCCESS;
}

void
FairDbExTask::Exec(Option_t* opt)
{
  return;
}

void
FairDbExTask::Finish()
{
  cout << " -I- FairDbExTask::Finish() ***** " << endl;

  fTutParDef->Print();
  fTutParAlt->Print();

  return;
}
