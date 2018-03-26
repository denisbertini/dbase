#include "CbmStsDbQaIvNewPar.h"

#include "FairDbLogFormat.h"
#include "FairDbLogService.h"
#include "FairDbOutTableBuffer.h"         // for FairDbOutRowStream
#include "FairDbStatement.h"              // for FairDbStatement
#include "FairDbStreamer.h"               // for FairDbStreamer
#include "FairParamList.h"              // for FairParamList
#include "FairDbParFactory.h"


#include <iosfwd>                    // for ostream
#include "TString.h"                    // for TString

#include <stdlib.h>                     // for exit
#include <memory>                       // for auto_ptr, etc
#include <vector>                       // for vector, vector<>::iterator


using namespace std;

ClassImp(CbmStsDbQaIvNewPar);


static FairDbParRegistry<CbmStsDbQaIvNewPar> qa_iv("StsQaIvPar");

#include "FairDbReader.tpl"
template class  FairDbReader<CbmStsDbQaIvNewPar>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<CbmStsDbQaIvNewPar>;



CbmStsDbQaIvNewPar::CbmStsDbQaIvNewPar(const char* name, const char* title, const char* context, Bool_t own)
  : FairDbParSet(name,title,context, own),
    fName(""), 
    fCompId(-1),
    fUID(0),
    fN(0),
    fI(NULL),
	  fV(NULL),
    fParam_Writer(NULL), //  Writer Meta-Class for SQL IO
    fParam_Reader(NULL), //  Reader Meta-Class for SQL IO
    fMultConn(FairDbTableInterfaceStore::Instance().fConnectionPool)
{
     // Set the default Db Entry to the first slot
     SetDbEntry(0);  
     // No  Aggregation
     SetCompId(fCompId);
     // Init Version 
     SetVersion(0);
}


CbmStsDbQaIvNewPar::~CbmStsDbQaIvNewPar()
{

  if (fI) delete[] fI;
  if (fV) delete[] fV;
  
  if (fParam_Writer) {
    delete fParam_Writer;
    fParam_Writer=NULL;
  }

  if (fParam_Reader) {
    delete fParam_Reader;
    fParam_Reader=NULL;
  }

}


void CbmStsDbQaIvNewPar::putParams(FairParamList* list)
{
  std::cout<<"-I- CbmStsDbQaIvNewPar::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("file_name",  (Text_t*) fName.c_str());
  list->add("comp_id",  fCompId);
  list->add("s_uid",    fUID);
  list->add("n_meas",   fN);
  list->add("i_arr",  fI, fN);
  list->add("v_arr",  fV, fN);
}

Bool_t CbmStsDbQaIvNewPar::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  Text_t aName[80];
  if (!list->fill("file_name",    aName,80 )) { return kFALSE;}
  fName = aName;
  if (!list->fill("comp_id",     &fCompId)) { return kFALSE; }
  if (!list->fill("s_uid",       &fUID))    { return kFALSE; }
  if (!list->fill("i_arr",       fI , fN))  { return kFALSE;}
  if (!list->fill("v_arr",       fV , fN))  { return kFALSE;}
  return kTRUE;
}

void CbmStsDbQaIvNewPar::clear()
{
  fName="";

  fCompId=fUID=fN=0;
  
  for(Int_t i=0; i<fN;i++){
      fI[i]=0.;fV[i]=0.;
  }

  if (fParam_Writer) { fParam_Writer->Reset(); }
  if (fParam_Reader) { fParam_Reader->Reset(); }
}


string CbmStsDbQaIvNewPar::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += "STSQAIVPAR"; }
  sql += "( SEQNO          INT NOT NULL,";
  sql += "  ROW_ID         INT NOT NULL,";
  sql += "  FNAME          TEXT,";
  sql += "  COMP_ID        INT,";
  sql += "  UID            INT,";
  sql += "  N              INT,";
  sql += "  I              TEXT,";
  sql += "  V              TEXT,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}


void CbmStsDbQaIvNewPar::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  // Allocate Buffers
   if (!fI) fI = new Double_t[MAX_M];
   if (!fV) fV = new Double_t[MAX_M];
  
   FairDbStreamer Iarr(fI,MAX_M);
   FairDbStreamer Varr(fV,MAX_M);
  
   res_in >> fName >> fCompId >> fUID >> fN >> Iarr >> Varr;
  
  // Update arrays
   Iarr.Fill(fI);
   Varr.Fill(fV); 
}

void CbmStsDbQaIvNewPar::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  // Check first if arrays are defined
  if (!fI || !fV ) {
  	cout << "-E- CbmStsDbQaIvNewPar::Store() SQL I/O  : Array Structure not defined " << endl;
    exit(0); 
  }

  FairDbStreamer Iarr(fI,fN);
  FairDbStreamer Varr(fV,fN);
 
  res_out << fName << fCompId << fUID << fN << Iarr << Varr;
}


void CbmStsDbQaIvNewPar::fill(UInt_t rid)
{
  // Get Reader Meta Class
  fParam_Reader=GetParamReader();

  // Define a Context
  ValTimeStamp ts(rid);
  ValCondition context(FairDbDetector::kSts,DataType::kData,ts);

  // Activate reading for this Context
  fParam_Reader->Activate(context, GetVersion());

  // By default use the latest row entry
  // (Other rows would correspond to outdated versions)
  Int_t numRows = fParam_Reader->GetNumRows();
  if ( numRows > 1 ) { numRows = 1; }

  for (int i = 0; i < numRows; ++i) {
    CbmStsDbQaIvNewPar* cgd = (CbmStsDbQaIvNewPar*) fParam_Reader->GetRow(i);
    if (!cgd) { continue; }
    fName = cgd->GetFileName(); 
    fCompId = cgd->GetCompId(); 
    fUID    = cgd->GetUID();
    fN      = cgd->GetN();         
    fI      = cgd->GetI();
    fV      = cgd->GetV();
  }

}


void CbmStsDbQaIvNewPar::store(UInt_t rid)
{

  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    cout << "-E-  CbmStsDbQaIvNewPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSQAIVPAR") ) {
    sql_cmds.push_back(FairDb::GetValDefinition("STSQAIVPAR").Data());
    sql_cmds.push_back(CbmStsDbQaIvNewPar::GetTableDefinition());
  }

  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      cout << "-E- CbmStsDbQaIvNewPar::Store() ******* Error Executing SQL commands ***********  " << endl;
    }

  }

  // Refresh list of tables in connected database
  // for the choosen DB entry
  fMultConn->GetConnection(GetDbEntry())->SetTableExists();

  // Writer Meta-Class Instance
  fParam_Writer = GetParamWriter();


  // Activate Writer Meta-Class with the proper
  // Validity Time Interval (run_id encapsulated)

  // Writer Activate() arguments list
  //                      <Arguments>                   <Type>                  <Comments>
  //
  //                      Validity Interval,            ValInterval
  //                      Composition      ,            Int_t                   set via cont. factory
  //                      Version          ,            Int_t                   set via cont. factory
  //                      DbEntry          ,            Int_t                   set via cont. factory
  //                      LogTitle         ,            std::string             set via cont. factory

  fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS QA IV parameter");

  // Object to Table mapping
  *fParam_Writer<< (*this);

  // Check for eventual IO problems
  if ( !fParam_Writer->Close() ) {
    fail = true;
    cout << "-E- CbmStsDbQaIvNewPar::Store() ******** Cannot do IO on class: " << GetName() <<  endl;
  }

}

void CbmStsDbQaIvNewPar::Print()
{
  std::cout<<"   STS QA IV Paramters <UID> "<< fUID <<  " <comp_Id> " << fCompId << std::endl;
  std::cout<<"   File Name        = " << fName  << std::endl;
  std::cout<<"   Nmeasurements    = " << fN  << std::endl;
  for(Int_t i=0; i<fN; i++) {
	  std::cout<<"   I:   = " << fI[i]  << std::endl;
    std::cout<<"   V:   = " << fV[i]  << std::endl;
  }
}

void CbmStsDbQaIvNewPar::SetN(Int_t nele){
  fN=nele;
  if(!fI) fI = new Double_t[fN];
  if(!fV) fV = new Double_t[fN];
}  

Bool_t CbmStsDbQaIvNewPar::Compare(const CbmStsDbQaIvNewPar& that ) const {
  
  Bool_t test_h =  
	      (fUID      == that.fUID)
	  &&  (fCompId   == that.fCompId)
    &&  (fName.compare(that.fName)==0)
    &&  (fN  == that.fN);
  
  
  Bool_t test_d=kTRUE;
  for(Int_t i=0; i<fN;i++){
	if ( 
	  	(fI[i]!=that.fI[i]) 
  		|| 
  		(fV[i]!=that.fV[i]) 
		 ) test_d = kFALSE;
  }

  return (test_h && test_d); 
}

FairDbWriter<CbmStsDbQaIvNewPar>* CbmStsDbQaIvNewPar::ActivateWriter(Int_t rid)
{
   // delete if already existing
   if (fParam_Writer) { delete fParam_Writer; fParam_Writer=NULL; }
 
   else {
         // Create according to IoV
         Bool_t fail= kFALSE;
        
         // Create a unique statement on choosen DB entry
         auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
         if ( ! stmtDbn.get() ) {
           cout << "-E-  CbmStsDbQaIvNewPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
                    << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
           exit(1);
         }
        
         // Check if for this DB entry the table already exists.
         // If not call the corresponding Table Definition Function
         std::vector<std::string> sql_cmds;
         TString atr(GetName());
         atr.ToUpper();
        
         if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSQAIVPAR") ) {
           sql_cmds.push_back(FairDb::GetValDefinition("STSQAIVPAR").Data());
           sql_cmds.push_back(CbmStsDbQaIvNewPar::GetTableDefinition());
         }
        
         // Packed SQL commands executed internally via SQL processor
         std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
         while( itr != itrEnd ) {
           std::string& sql_cmd(*itr++);
           stmtDbn->Commit(sql_cmd.c_str());
           if ( stmtDbn->PrintExceptions() ) {
                 fail = true;
                 cout << "-E- CbmStsDbQaSensorPar::ActivateWriter() ******* Error Executing SQL commands ***********  " << endl;
           }
          
         }
        
         // Refresh list of tables in connected database
         // for the choosen DB entry
         fMultConn->GetConnection(GetDbEntry())->SetTableExists();
        
         // Writer Meta-Class Instance
         fParam_Writer = GetParamWriter();
                
         fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS QA Sensor Parameter"); 
         return fParam_Writer;
        
   }
 
   return NULL;
}



