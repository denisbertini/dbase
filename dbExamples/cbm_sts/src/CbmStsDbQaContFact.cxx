#include "CbmStsDbQaContFact.h"

#include "CbmStsDbQaSensorPar.h"      
#include "CbmStsDbQaSensorNewPar.h"   
#include "CbmStsDbQaIvPar.h"          
#include "CbmStsDbQaPar.h"            
#include "CbmStsDbQaNewPar.h"         
#include "CbmStsDbSNoisyChPar.h"      
#include "CbmStsDbSInfoPar.h"         

#include "CbmStsDbQa.h"            

#include "FairDbParSet.h"                // for FairParSet
#include "FairRuntimeDb.h"              // for FairRuntimeDb

#include <iosfwd>                    // for ostream
#include "TList.h"                      // for TList
#include "TString.h"                    // for TString

#include <string.h>                     // for strcmp, NULL
#include <iostream>                     // for operator<<, basic_ostream, etc

using namespace std;

ClassImp(CbmStsDbQaContFact);

static CbmStsDbQaContFact gCbmStsDbQaContFact;

CbmStsDbQaContFact::CbmStsDbQaContFact()
{
  // Constructor (called when the library is loaded)
  fName="CbmStsDbQaContFact";
  fTitle="Cbm Sts QA factory for parameter containers";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmStsDbQaContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers.*/

  FairContainer* p1 = new FairContainer("CbmStsDbQaSensorPar", "Cbm Qa Global Parameters",
                                        "TestDefaultContext");
  p1->addContext("TestNonDefaultContext");
  containers->Add(p1);

  FairContainer* p2 = new FairContainer("CbmStsDbQaIvPar", "Cbm Qa IV Parameters",
                                        "TestDefaultContext");
  p2->addContext("TestDefaultContext");
  containers->Add(p2);

  FairContainer* p3 = new FairContainer("CbmStsDbQaPar", "Cbm Qa Parameters",
                                        "TestDefaultContext");
  p3->addContext("TestDefaultContext");
  containers->Add(p3);

  // <DB>   Add now new parameter version
  //        With new naming convention

  FairContainer* p11 = new FairContainer("StsQaSensorPar", "Cbm Qa Global New Parameters",
                                        "TestDefaultContext");
  p11->addContext("TestNonDefaultContext");
  containers->Add(p11);

  FairContainer* p22 = new FairContainer("StsQaPar", "Cbm Qa New Parameters",
                                         "TestDefaultContext");
  p22->addContext("TestDefaultContext");
  containers->Add(p22);

  FairContainer* p33 = new FairContainer("StsQaIvPar", "Cbm Qa IV New Parameters",
                                        "TestDefaultContext");
  p33->addContext("TestDefaultContext");
  containers->Add(p33);

  FairContainer* p44 = new FairContainer("StsQaGeomPar", "Cbm Qa Geometry New Parameters",
                                        "TestDefaultContext");
  p44->addContext("TestDefaultContext");
  containers->Add(p44);

  FairContainer* p55 = new FairContainer("StsQaResultsPar", "Cbm Qa Results New Parameters",
                                        "TestDefaultContext");
  p55->addContext("TestDefaultContext");
  containers->Add(p55);

  FairContainer* p66 = new FairContainer("StsQaOwnerPar", "Cbm Qa Results New Parameters",
                                        "TestDefaultContext");
  p66->addContext("TestDefaultContext");
  containers->Add(p66);

  FairContainer* p77 = new FairContainer("StsQaChMapPar", "Cbm Qa Channel Map Parameters",
                                        "TestDefaultContext");
  p77->addContext("TestDefaultContext");
  containers->Add(p77);


  FairContainer* p88 = new FairContainer("StsDbSInfoPar", "Cbm Sts Info Parameters",
                                        "TestDefaultContext");
  p88->addContext("TestDefaultContext");
  containers->Add(p88);

  FairContainer* p99 = new FairContainer("StsDbSNoisyChPar", "Cbm Sts Info Parameters",
                                        "TestDefaultContext");
  p99->addContext("TestDefaultContext");
  containers->Add(p99);


  FairContainer* p100 = new FairContainer("StsDbQa", "Cbm Qa Parameters",
                                         "TestDefaultContext");
  p100->addContext("TestDefaultContext");
  containers->Add(p100);

  
}

FairParSet* CbmStsDbQaContFact::createContainer(FairContainer* c)
{
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */

  const char* name=c->GetName();
  cout << "-I-CbmStsDbQaContFact::createContainer " << name << endl;
  FairDbParSet* p=NULL;

  if (strcmp(name,"CbmStsDbQaSensorPar")==0) {
    p=new CbmStsDbQaSensorPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  } else if (strcmp(name,"CbmStsDbQaIvPar")==0) {
    p=new CbmStsDbQaSensorPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  } else if (strcmp(name,"CbmStsDbQaPar")==0) {
    p=new CbmStsDbQaPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  } else if (strcmp(name,"StsQaSensorPar")==0) {
    p=new CbmStsDbQaSensorNewPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  } else if (strcmp(name,"StsQaPar")==0) {
    p=new CbmStsDbQaNewPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  } else if (strcmp(name,"StsDbQa")==0) {
    p=new CbmStsDbQa(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  } else if (strcmp(name,"StsQaIvPar")==0) {
  	p=new CbmStsDbQaIvNewPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  	// Set Arguments needed for SQL versioning managment
  	p->SetVersion(0);
  	p->SetDbEntry(0);
  	p->SetLogTitle(name);
  } else if (strcmp(name,"StsQaChMapPar")==0) {
	p=new CbmStsDbQaChannelMapPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  } else if (strcmp(name,"StsQaGeomPar")==0) {
	p=new CbmStsDbQaGeometryPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  } else if (strcmp(name,"StsQaResultsPar")==0) {
	p=new CbmStsDbQaResultsPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  } else if (strcmp(name,"StsQaOwnerPar")==0) {
    p=new CbmStsDbQaOwnershipPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);

  } else if (strcmp(name,"StsDbSNoisyChPar")==0) {
    p=new CbmStsDbSNoisyChPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  } else if (strcmp(name,"StsDbSInfoPar")==0) {
    p=new CbmStsDbSInfoPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name); 
  } else if (strcmp(name,"StsDbQa")==0) {
    p=new CbmStsDbQa(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  }

  return p;
}
