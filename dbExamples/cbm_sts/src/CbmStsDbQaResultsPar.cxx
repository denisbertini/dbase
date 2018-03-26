#include "CbmStsDbQaResultsPar.h"

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
#include <boost/lexical_cast.hpp>

using namespace std;
using boost::lexical_cast;
using boost::bad_lexical_cast;


ClassImp(CbmStsDbQaResultsPar);


static FairDbParRegistry<CbmStsDbQaResultsPar> qa_iv("StsQaResultsPar");

#include "FairDbReader.tpl"
template class  FairDbReader<CbmStsDbQaResultsPar>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<CbmStsDbQaResultsPar>;



CbmStsDbQaResultsPar::CbmStsDbQaResultsPar(const char* name, const char* title, const char* context, Bool_t own)
  : FairDbParSet(name,title,context, own),
    fCompId(-1),
    fUID(0),
    fType(""),
    fWaferId(0),
    fV_fd(0.),
    fI_150V_20C(0.),
  	fI_250V_20C(0.),
    fSdefect_pside(0),
    fSdefect_nside(0),
    fQuality_grade(0),
    fProblem(0),
    fQaPassed(0),
    fOpticalCheckPassed(0),
    fComments(""),
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


CbmStsDbQaResultsPar::~CbmStsDbQaResultsPar()
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


void CbmStsDbQaResultsPar::putParams(FairParamList* list)
{
  std::cout<<"-I- CbmStsDbQaResultsPar::putParams() called"<<std::endl;
  if(!list) { return; }

  list->add("comp_id",       fCompId);
  list->add("s_uid",         fUID);
  list->add("type",     (Text_t*) fType.c_str());
  list->add("wafer_id",    fWaferId);
  list->add("vfd",           fV_fd);
  list->add("i_150V_20C",    fI_150V_20C);
  list->add("i_250V_20C",    fI_250V_20C);
  list->add("sdef_pside",    fSdefect_pside);
  list->add("sdef_nside",    fSdefect_nside);
  list->add("quality",       fQuality_grade);
  list->add("problem",       fProblem);
  list->add("qa_passed",     fQaPassed);
  list->add("ocheck_passed", fOpticalCheckPassed);
  list->add("comments",  (Text_t*) fComments.c_str());

}

Bool_t CbmStsDbQaResultsPar::getParams(FairParamList* list)
{

  if (!list) { return kFALSE; }

  if (!list->fill("comp_id",     &fCompId)) { return kFALSE; }
  if (!list->fill("s_uid",       &fUID))    { return kFALSE; }
  Text_t aName[155];
  if (!list->fill("type",    aName, 155 )) { return kFALSE;}
  fType = aName;
  if (!list->fill("wafer_id",       &fWaferId))    { return kFALSE; }
  if (!list->fill("vfd",            &fV_fd)) { return kFALSE;}
  if (!list->fill("i_150V_20C",     &fI_150V_20C )) { return kFALSE;}
  if (!list->fill("i_250V_20C",     &fI_250V_20C )) { return kFALSE;}
  if (!list->fill("sdef_pside",     &fSdefect_pside )) { return kFALSE;}
  if (!list->fill("sdef_nside",     &fSdefect_nside )) { return kFALSE;}
  if (!list->fill("quality",        &fQuality_grade )) { return kFALSE;}
  if (!list->fill("problem",        &fProblem )) { return kFALSE;}
  if (!list->fill("qa_passed",      &fQaPassed )) { return kFALSE;}
  if (!list->fill("ocheck_passed",  &fOpticalCheckPassed )) { return kFALSE;}

  //<DB> What size here ??!
  Text_t bName[155];
  if (!list->fill("comments",    bName,155)) { return kFALSE;}
  fComments = bName;

  return kTRUE;
}

void CbmStsDbQaResultsPar::clear()
{

  fCompId=fUID=fWaferId=0;

  fProblem = fQaPassed = fOpticalCheckPassed = 0;
  fSdefect_pside = fSdefect_nside = fQuality_grade = 0;
  fV_fd = fI_150V_20C = fI_150V_20C = 0.;
  fComments= fType="";

  if (fParam_Writer) { fParam_Writer->Reset(); }
  if (fParam_Reader) { fParam_Reader->Reset(); }
}


string CbmStsDbQaResultsPar::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += "STSQARESULTSPAR"; }
  sql += "( SEQNO              INT NOT NULL,";
  sql += "  ROW_ID             INT NOT NULL,";
  sql += "  COMP_ID            INT,";
  sql += "  UID                INT,";
  sql += "  TYPE               TEXT,";
  sql += "  WAFER_ID           INT,";
  sql += "  V_FD               DOUBLE,";
  sql += "  I_150V_20C         DOUBLE,";
  sql += "  I_250V_20C         DOUBLE,";
  sql += "  DEF_PSIDE          INT,";
  sql += "  DEF_NSIDE          INT,";
  sql += "  QUALITY            INT,";
  sql += "  PROBLEM            INT,";
  sql += "  QAPASSED           INT,";
  sql += "  OPTPASSED          INT,";
  sql += "  COMMENTS           TEXT,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}


void CbmStsDbQaResultsPar::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{

  res_in >> fCompId  >> fUID
		 >> fType >> fWaferId
         >> fV_fd >> fI_150V_20C >> fI_250V_20C
         >> fSdefect_pside >> fSdefect_nside
         >> fQuality_grade >> fProblem
         >> fQaPassed >> fOpticalCheckPassed >> fComments;


}

void CbmStsDbQaResultsPar::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{

  res_out << fCompId   << fUID
          << fType << fWaferId
          << fV_fd << fI_150V_20C << fI_250V_20C
          << fSdefect_pside << fSdefect_nside
          << fQuality_grade << fProblem << fQaPassed
          << fOpticalCheckPassed << fComments;

}



void CbmStsDbQaResultsPar::fill(UInt_t rid)
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
    CbmStsDbQaResultsPar* cgd = (CbmStsDbQaResultsPar*) fParam_Reader->GetRow(i);
    if (!cgd) { continue; }
    fCompId = cgd->GetCompId();
    fUID    = cgd->GetUID();
    fType         = cgd->GetType();
    fWaferId      = cgd->GetWaferId();
    fV_fd =  cgd->GetV_fd();
    fI_150V_20C = cgd->GetI_150V_20C();
    fI_250V_20C = cgd->GetI_250V_20C();
    fSdefect_pside = cgd->GetSdefect_pside();
    fSdefect_nside = cgd->GetSdefect_nside();
    fQuality_grade = cgd->GetQuality_grade();
    fProblem = cgd->GetProblem();
    fQaPassed = cgd->GetQaPassed();
    fOpticalCheckPassed = cgd->GetOpticalCheckPassed();
    fComments =  cgd->GetComment();
   }


}




void CbmStsDbQaResultsPar::store(UInt_t rid)
{

  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    cout << "-E-  CbmStsDbQaResultsPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSQARESULTSPAR") ) {
    sql_cmds.push_back(FairDb::GetValDefinition("STSQARESULTSPAR").Data());
    sql_cmds.push_back(CbmStsDbQaResultsPar::GetTableDefinition());
  }

  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      cout << "-E- CbmStsDbQaResultsPar::Store() ******* Error Executing SQL commands ***********  " << endl;
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

  fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS QA Results parameter");

  // Object to Table mapping
  *fParam_Writer<< (*this);

  // Check for eventual IO problems
  if ( !fParam_Writer->Close() ) {
    fail = true;
    cout << "-E- CbmStsDbQaResultsPar::Store() ******** Cannot do IO on class: " << GetName() <<  endl;
  }

}

void CbmStsDbQaResultsPar::Print()
{
  std::cout<<"   STS QA Results Paramters <UID> "<< fUID <<  " <comp_Id> " << fCompId << std::endl;
  std::cout<<"   Type          = " << fType       << std::endl;
  std::cout<<"   WaferId       = " << fWaferId    << std::endl;
  std::cout<<"   V_fd  (Volts)       = "  << fV_fd  << std::endl;
  std::cout<<"   I_150V_20C  (muA)   = "  << fI_150V_20C  << std::endl;
  std::cout<<"   I_250V_20C  (muA)   = "  << fI_250V_20C  << std::endl;
  std::cout<<"   Defect   (pside)    = "  << fSdefect_pside  << std::endl;
  std::cout<<"   Defect   (nside)    = "  << fSdefect_nside  << std::endl;
  std::cout<<"   Quality grade       = "  << fQuality_grade  << std::endl;
  std::cout<<"   Problem             = "  << fProblem  << std::endl;
  std::cout<<"   QaPassed            = "  << fQaPassed  << std::endl;
  std::cout<<"   OpticalCheckPassed  = "  << fOpticalCheckPassed  << std::endl;
  std::cout<<"   Comments            = "  << fComments << std::endl;

}


Bool_t CbmStsDbQaResultsPar::Compare(const CbmStsDbQaResultsPar& that ) const {

  Bool_t test_h =
	      (fUID      == that.fUID)
	  &&  (fCompId   == that.fCompId)
      &&  (fType.compare(that.fType)==0)
      &&  (fWaferId   == that.fWaferId)
      &&  (fV_fd == that.fV_fd)
      &&  (fI_150V_20C == that.fI_150V_20C)
      &&  (fI_250V_20C == that.fI_250V_20C)
      &&  (fSdefect_pside == that.fSdefect_pside)
      &&  (fSdefect_nside == that.fSdefect_nside)
      &&  (fQuality_grade == that.fQuality_grade)
      &&  (fProblem == that.fProblem)
      &&  (fQaPassed == that.fQaPassed)
      &&  (fOpticalCheckPassed == that.fOpticalCheckPassed)
      &&  (fComments.compare(that.fComments)==0);


  return (test_h);
}

FairDbWriter<CbmStsDbQaResultsPar>* CbmStsDbQaResultsPar::ActivateWriter(Int_t rid)
{
   // delete if already existing
   if (fParam_Writer) { delete fParam_Writer; fParam_Writer=NULL; }

   else {
         // Create according to IoV
         Bool_t fail= kFALSE;

         // Create a unique statement on choosen DB entry
         auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
         if ( ! stmtDbn.get() ) {
           cout << "-E-  CbmStsDbQaResultsPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
                    << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
           exit(1);
         }

         // Check if for this DB entry the table already exists.
         // If not call the corresponding Table Definition Function
         std::vector<std::string> sql_cmds;
         TString atr(GetName());
         atr.ToUpper();

         if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSQARESULTSPAR") ) {
           sql_cmds.push_back(FairDb::GetValDefinition("STSQARESULTSPAR").Data());
           sql_cmds.push_back(CbmStsDbQaResultsPar::GetTableDefinition());
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

         fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS QA Results Parameter");
         return fParam_Writer;

   }

   return NULL;
}


Bool_t CbmStsDbQaResultsPar::Import(const vector<string>& value)
{
     if (value.size()>0)
      {
       SetCompId(fCompId); // No composition
       cout << "0" << value[0] << endl;
       SetUID(lexical_cast<int>(value[0]));
       SetType(value[1]);
       cout << "3" << value[2] << endl;
       SetWaferId(lexical_cast<int>(value[2]));
       SetVfd(lexical_cast<double>(value[3]));
       SetI_150V_20C(lexical_cast<double>(value[4]));
       SetI_250V_20C(lexical_cast<double>(value[5]));
       SetSdefect_pside(lexical_cast<int>(value[6]));
       SetSdefect_nside(lexical_cast<int>(value[7]));
	   SetQuality_grade(lexical_cast<int>(value[8]));
       SetProblem(lexical_cast<int>(value[9]));
       SetQaPassed(lexical_cast<int>(value[10]));
       SetOpticalCheckPassed(lexical_cast<int>(value[11]));
       SetComment(value[12]);

      return kTRUE;
      }
     return kFALSE;

}
