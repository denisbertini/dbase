#include "FairDbExPar3.h"

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

ClassImp(FairDbExPar3);

#include "FairDbReader.tpl"
template class  FairDbReader<FairDbExPar3>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<FairDbExPar3>;



FairDbExPar3::FairDbExPar3(Int_t base, const char* name, const char* title, const char* context, Bool_t own)
  : FairDbParSet(name,title,context, own),
    fParam_Writer(NULL), //  Writer Meta-Class for SQL IO
    fParam_Reader(NULL), //  Reader Meta-Class for SQL IO
    fMultConn(FairDbTableInterfaceStore::Instance().fConnectionPool)
{
  // Set the default Db Entry to the first slot
  SetDbEntry(0);
  
  // Init Version
  SetVersion(0);

  fData[0]= base;
  for ( int elem =1; elem < datasize; ++elem )
       fData[elem] = 1 + fData[elem-1];
  
}


FairDbExPar3::~FairDbExPar3()
{
  if (fParam_Writer) {
    delete fParam_Writer;
    fParam_Writer=NULL;
  }

  if (fParam_Reader) {
    delete fParam_Reader;
    fParam_Reader=NULL;
  }

}

Bool_t FairDbExPar3::IsValid() const {

  Bool_t ok = kTRUE;
  for ( int elem =1; elem < datasize; ++elem ) 
    if ( fData[elem] != 1 + fData[elem-1] ) ok = kFALSE;
  return ok;

}


void FairDbExPar3::putParams(FairParamList* list)
{
  std::cout<<"-I- FairDbExPar3::putParams() called"<<std::endl;
  if(!list) { return; }

}

Bool_t FairDbExPar3::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  return kTRUE;
}

void FairDbExPar3::clear()
{

  if (fParam_Writer) { fParam_Writer->Reset(); }
  if (fParam_Reader) { fParam_Reader->Reset(); }
}


string FairDbExPar3::GetTableDefinition(const char* Name)
{

  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += "EXPAR3"; }
  sql += "( SEQNO          INT NOT NULL,";
  sql += "  ROW_ID         INT NOT NULL,";
  sql += "  DATA           DOUBLE,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}



void FairDbExPar3::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{

 for (int elem =0; elem < datasize; ++elem ) res_in >> fData[elem];

}

void FairDbExPar3::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
 for (int elem =0; elem < datasize; ++elem ) res_out <<  fData[elem]; 
}


void FairDbExPar3::fill(UInt_t rid)
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
    FairDbExPar3* cgd = (FairDbExPar3*) fParam_Reader->GetRow(i);
    if (!cgd) { continue; }
    // Not implemented
  }

}


void FairDbExPar3::store(UInt_t rid)
{
  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    cout << "-E-  FairDbExPar3::Store()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists("EXPAR3") ) {
    sql_cmds.push_back(FairDb::GetValDefinition("EXPAR3").Data());
    sql_cmds.push_back(FairDbExPar3::GetTableDefinition());
  }

  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      cout << "-E- FairDbExPar3::Store() ******* Error Executing SQL commands ***********  " << endl;
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

  fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),GetLogTitle());

  // Object to Table mapping
  *fParam_Writer<< (*this);

  // Check for eventual IO problems
  if ( !fParam_Writer->Close() ) {
    fail = true;
    cout << "-E- FairDbExPar3::Store() ******** Cannot do IO on class: " << GetName() <<  endl;
  }

}

void FairDbExPar3::Print()
{
  std::cout<<"   ExPar3   Parameters: " <<std::endl;

}



