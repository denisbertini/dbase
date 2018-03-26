/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

/** 
 *  FairDbTutPar.cxx 
 * 
 *  created @ 16-01-2017 
 *  by         D.Bertini  
 */ 

#include "FairDbTutPar.h"
#include "FairDbGenericParSet.h"
#include "FairDbLogFormat.h"
#include "FairDbLogService.h"
#include "FairDbOutTableBuffer.h"         // for FairDbOutRowStream
#include "FairDbStatement.h"            // for FairDbStatement
#include "FairParamList.h"              // for FairParamList
#include <iosfwd>                    // for ostream
#include "TString.h"                    // for TString
#include <stdlib.h>                     // for exit
#include <memory>                       // for auto_ptr, etc
#include <vector>                       // for vector, vector<>::iterator


using namespace std;

ClassImp(FairDbTutPar);


static FairDbGenericParRegistry<FairDbTutPar> qa_iv("FairDbTutPar", FairDbDetector::kSts, DataType::kData);


#include "FairDbGenericParSet.tpl"
template class  FairDbGenericParSet<FairDbTutPar>;

#include "FairDbReader.tpl"
template class  FairDbReader<FairDbTutPar>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<FairDbTutPar>;



FairDbTutPar::FairDbTutPar(const FairDbDetector::Detector_t detid, const DataType::DataType_t dataid,
                           const char* name, const char* title, const char* context, Bool_t own)
  : FairDbGenericParSet<FairDbTutPar>(detid, dataid, name, title, context, own),
    fTopPitch(0.),
    fTopAnchor(0.),
    fTopNrFE(0),
    fFeType("")
{
}

FairDbTutPar::~FairDbTutPar()
{
}

void FairDbTutPar::putParams(FairParamList* list)
{
  std::cout<<"-I- FairDbTutPar::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("top_pitch",  fTopPitch);
  list->add("top_anchor", fTopAnchor);
  list->add("nr_fe_top",  fTopNrFE);
  list->add("fe_Type",    (Text_t*) &fFeType);
}

Bool_t FairDbTutPar::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  if (!list->fill("top_pitch", &fTopPitch)) { return kFALSE; }
  if (!list->fill("top_anchor", &fTopAnchor)) { return kFALSE; }
  if (!list->fill("nr_fe_top", &fTopNrFE)) { return kFALSE; }
  Text_t feName[80];
  if (!list->fill("fe_Type",feName,80)) { return kFALSE; }
  fFeType = feName;
  return kTRUE;
}

void FairDbTutPar::clear()
{

  fTopPitch=fTopAnchor=0.0;
  fTopNrFE=0;
  fFeType="";
  
  FairDbGenericParSet<FairDbTutPar>::clear();
}

string FairDbTutPar::GetTableDefinition(const char* Name)
{  
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO          INT NOT NULL,";
  sql += "  ROW_ID         INT NOT NULL,";
  sql += "  TOPPITCH       DOUBLE,";
  sql += "  TOPANCHOR      DOUBLE,";
  sql += "  TOPNRFE        INT,";
  sql += "  FETYPE         TEXT,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}

void FairDbTutPar::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  res_in >> fTopPitch  >> fTopAnchor   >> fTopNrFE  >> fFeType;
}

void FairDbTutPar::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  res_out << fTopPitch  << fTopAnchor   << fTopNrFE  << fFeType;
}

void FairDbTutPar::Print()
{
  std::cout<<"   Tutorial SQL  Parameters:"<<std::endl;
  std::cout<<"   Top Pitch    = "<<fTopPitch<<std::endl;
  std::cout<<"   Top Anchor   = "<<fTopAnchor<< std::endl;
  std::cout<<"   Nr of Frontends (Top Side)   = "<<fTopNrFE<<std::endl;
  std::cout<<"   Frontend type name is        = "<<fFeType<<std::endl;
}


FairDbTutPar::FairDbTutPar(const FairDbTutPar& from){
  // Copy ctor 
  fTopPitch  = from.fTopPitch;
  fTopAnchor = from.fTopAnchor;
  fTopNrFE = from. fTopNrFE;
  fFeType = from.fFeType;
}

FairDbTutPar& FairDbTutPar::operator=(const FairDbTutPar& from){
  fTopPitch  = from.fTopPitch;
  fTopAnchor = from.fTopAnchor;
  fTopNrFE = from. fTopNrFE;
  fFeType = from.fFeType;
  return *this;
}
