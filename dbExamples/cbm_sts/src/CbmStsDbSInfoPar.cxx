#include "CbmStsDbSInfoPar.h"

#include "FairDbLogFormat.h"
#include "FairDbLogService.h"
#include "FairDbOutTableBuffer.h"         // for FairDbOutRowStream
#include "FairDbStatement.h"              // for FairDbStatement
#include "FairParamList.h"                // for FairParamList
#include "FairDbParFactory.h"

#include <iosfwd>                     // for ostream
#include "TString.h"                     // for TString

#include <stdlib.h>                      // for exit
#include <memory>                        // for auto_ptr, etc
#include <vector>                        // for vector, vector<>::iterator

ClassImp(CbmStsDbSInfoPar);

static FairDbParRegistry<CbmStsDbSInfoPar> qa_par("StsDbSInfoPar");

CbmStsDbSInfoPar::CbmStsDbSInfoPar(const char* name, const char* title, const char* context, Bool_t own)
  : FairDbParSet(name,title,context, own)
	, fSNoisyCh(new TObjArray())
{
}


CbmStsDbSInfoPar::~CbmStsDbSInfoPar()
{
  // Delete all noisy Channel Objects
  if (fSNoisyCh)
  {delete fSNoisyCh; fSNoisyCh=nullptr;}
  // Delete the reader templates
  if (fReader_SNoisyCh)
  {delete fReader_SNoisyCh; fReader_SNoisyCh=nullptr;}
}


void CbmStsDbSInfoPar::putParams(FairParamList* list)
{
  std::cout<<"-I- CbmStsDbSInfoParPar::putParams() called"<<std::endl;
  if(!list) { return; }
  list->addObject("Sts SInfo", fSNoisyCh);
}

Bool_t CbmStsDbSInfoPar::getParams(FairParamList* list)
{
  if(!list) { return kFALSE;}
  if (!list->fillObject("Sts SInfo", fSNoisyCh)) {return kFALSE;}
  return kTRUE;
}

void CbmStsDbSInfoPar::clear()
{
}

void CbmStsDbSInfoPar::fill(UInt_t rid)
{
  // Fill the lists with correspondin TimeStamps (runID)
  cout << "-I- CbmStsDbSInfoPar::fill() called with RID# " << rid << endl;

  // Sensors List
  CbmStsDbSNoisyChPar mpar;
  fReader_SNoisyCh = mpar.GetParamReader();

  // Define a Global Context
  ValTimeStamp ts(rid);
  ValCondition context(FairDbDetector::kSts,DataType::kData,ts);

  // Activate reading for this Context
  fReader_SNoisyCh->Activate(context, GetVersion());

  Int_t numSensors = fReader_SNoisyCh->GetNumRows();
  for (int i = 0; i <=numSensors; ++i) {
    CbmStsDbSNoisyChPar* noisych_par = (CbmStsDbSNoisyChPar*) fReader_SNoisyCh->GetRow(i);
    if (!noisych_par) { continue; }

    fSNoisyCh->AddAtAndExpand(noisych_par, noisych_par->GetDetId());
    cout << " fill NoisyChInfo at -----> " << noisych_par->GetDetId() << endl;
  }

}


void CbmStsDbSInfoPar::store(UInt_t rid)
{
  //<DB> store the lists
  cout << "-I- CbmStsDbSInfoPar::store() called with RID# " << rid << endl;

  // CbmStsDbQaSensorPar stored row-wized
  CbmStsDbSNoisyChPar sPar;
  FairDbWriter<CbmStsDbSNoisyChPar>* sW = sPar.ActivateWriter(rid);

  if (sW)
  {
    // List of Qa Objects
    for(Int_t i=0;i<fSNoisyCh->GetSize();i++)
    {
      CbmStsDbSNoisyChPar* s_par = (CbmStsDbSNoisyChPar*) fSNoisyCh->At(i);
      if (s_par) *sW << *s_par;
    }

    // Reset the Writer
    if(!sW->Close())
    {
      cout << " -W- CbmStsDbSInfoPar:: IO Error writing  sensor parameters ******** "<< endl;
    }

  }//! (aWriter)
  else {
    cout << " -W- CbmStsDbSInfoPar:: Writer for CbmStsDbQaSensorPar not created "<< endl;
  }
}

void CbmStsDbSInfoPar::Print()
{
  std::cout<<" -----------  STS DB Sensor Noisy Channels -------------  "<<std::endl;
  for(Int_t i=0;i<fSNoisyCh->GetSize(); i++)
  {
    CbmStsDbSNoisyChPar* s_par = (CbmStsDbSNoisyChPar*) fSNoisyCh->At(i);
    cout << "----------------------------------------------------------------------" << endl;
    if (s_par) s_par->Print();
  }

}
