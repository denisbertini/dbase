#include "FairDbExContFact.h"

#include "FairDbParSet.h"
#include "FairRuntimeDb.h"
#include "FairDbExPar1.h"              
#include "FairDbExPar2.h"              
#include "FairDbExPar3.h"            

#include <iosfwd>                   
#include "TList.h"                     
#include "TString.h"                   

#include <string.h>                    
#include <iostream>                    

using namespace std;

ClassImp(FairDbExContFact);

static FairDbExContFact gFairDbExContFact;

FairDbExContFact::FairDbExContFact()
{
  // Constructor (called when the library is loaded)
  fName="FairDbExContFact";
  fTitle="Tutorial factory for parameter containers";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void FairDbExContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers.*/

  FairContainer* p1 = new FairContainer("ExPar1", "Par1", "TestDefaultContext");
  p1->addContext("TestNonDefaultContext");
  containers->Add(p1);
  FairContainer* p2 = new FairContainer("ExPar2", "Par2", "TestDefaultContext");
  p2->addContext("TestNonDefaultContext");
  containers->Add(p2);

}

FairParSet* FairDbExContFact::createContainer(FairContainer* c)
{
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */

  const char* name=c->GetName();
  cout << "-I- FairDbExContFact::createContainer " << name << endl;
  FairDbParSet* p=NULL;

  if (strcmp(name,"ExPar1")==0) {
    p=new FairDbExPar1(FairDbDetector::kSts, DataType::kData,
                       c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetComboNo(-1);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  }
  if (strcmp(name,"ExPar2")==0) {
    p=new FairDbExPar2(FairDbDetector::kSts, DataType::kData,
                       c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetComboNo(-1);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  }
  
  return (FairParSet*) p;
}
