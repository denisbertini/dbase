#include "CbmStsDbQaSensor.h"

#include "FairRunIdGenerator.h"
#include "FairDbLogFormat.h"
#include "FairDbLogService.h"
#include "FairDbOutTableBuffer.h"         // for FairDbOutRowStream
#include "FairDbStatement.h"              // for FairDbStatement
#include "FairDbStreamer.h"               // for FairDbStreamer
#include "FairParamList.h"                // for FairParamList
#include "FairDbParFactory.h"            //

#include <iosfwd>                       // for ostream
#include "TString.h"                       // for TString

#include <stdlib.h>                        // for exit
#include <memory>                          // for auto_ptr, etc
#include <vector>                          // for vector, vector<>::iterator
#include <boost/lexical_cast.hpp>


using namespace std;
using boost::lexical_cast;
using boost::bad_lexical_cast;



ClassImp(CbmStsDbQaSensor);


static FairDbParRegistry<CbmStsDbQaSensor> qa_sensor("StsQaSensorPar");


#include "FairDbReader.tpl"
template class  FairDbReader<CbmStsDbQaSensor>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<CbmStsDbQaSensor>;



CbmStsDbQaSensor::CbmStsDbQaSensor(const char* name, const char* title, const char* context, Bool_t own)
  : FairDbParSet(name,title,context, own),
    fCompId(-1),
    fUID(0),
    fSensor_type(""),
    fBatch_id(""),
    fWafer_id(0),
    fReticle_name(""),
    fVendor_name(""),
    fProcessing(""),
    fHeight(0.),
    fWidth(0.),
    fStripsPerSide(0),
    fYear(0),
    fCurrent_owner(""),
    fCurrent_location(""),
    fV_fd(0.),
    fI_150V_20C(0.),
    fI_250V_20C(0.),
    fSdefect_pside(0),
    fSdefect_nside(0),
    fQuality_grade(0),
    fProblem(0),
    fPassed(0),
    fOptPassed(0),
    fComments(""),
    fParam_Writer(NULL), //  Writer Meta-Class for SQL IO
    fParam_Reader(NULL), //  Reader Meta-Class for SQL IO
    fMultConn(FairDbTableInterfaceStore::Instance().fConnectionPool)
{
  // Set the default Db Entry to the first slot
   SetDbEntry(0);
   // Set aggregation
   SetCompId(fCompId);
   // Set Version 0
   SetVersion(0);
}


CbmStsDbQaSensor::~CbmStsDbQaSensor()
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


void CbmStsDbQaSensor::putParams(FairParamList* list)
{
  std::cout<<"-I- CbmStsDbQaSensor::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("comp_id",  fCompId);
  list->add("s_uid",    fUID);
  list->add("sensor_type",  (Text_t*) fSensor_type.c_str());
  list->add("batch_id",     (Text_t*) fBatch_id.c_str());
  list->add("wafer_id",  fWafer_id);
  list->add("reticle_name",  (Text_t*) fReticle_name.c_str());
  list->add("vendor_name",  (Text_t*) fVendor_name.c_str());
  list->add("processing",  (Text_t*) fProcessing.c_str());
  list->add("height",  fHeight);
  list->add("width",  fWidth);
  list->add("strips_per_side",  fStripsPerSide);
  list->add("year",  fYear);
  list->add("owner",  (Text_t*) fCurrent_owner.c_str());
  list->add("location",  (Text_t*) fCurrent_location.c_str());
  list->add("vfd",  fV_fd);
  list->add("i_150V_20C",  fI_150V_20C);
  list->add("i_250V_20C",  fI_250V_20C);
  list->add("sdef_pside",  fSdefect_pside);
  list->add("sdef_nside",  fSdefect_nside);
  list->add("quality",  fQuality_grade);
  list->add("problem",  fProblem);
  list->add("passed",   fPassed);
  list->add("opt_passed",   fOptPassed);
  list->add("comments",  (Text_t*) fComments.c_str());
}

Bool_t CbmStsDbQaSensor::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }

  if (!list->fill("comp_id",     &fCompId)) { return kFALSE; }
  if (!list->fill("s_uid",       &fUID))    { return kFALSE; }

  Text_t tName[80];
  if (!list->fill("sensor_type",    tName,80 )) { return kFALSE;}
  fSensor_type = tName;

  Text_t batch[80];
  if (!list->fill("batch_id",    batch,80 )) { return kFALSE;}
  fBatch_id = batch;

  if (!list->fill("wafer_id",       &fWafer_id))  { return kFALSE; }

  Text_t rName[80];
  if (!list->fill("reticle_name",    rName,80 )) { return kFALSE;}
  fReticle_name = rName;

  Text_t sName[80];
  if (!list->fill("vendor_name",    sName,80 )) { return kFALSE;}
  fVendor_name = sName;

  Text_t manuf[80];
  if (!list->fill("processing",    manuf,80 )) { return kFALSE;}
  fProcessing = manuf;

  if (!list->fill("height",                &fHeight))  { return kFALSE; }
  if (!list->fill("width",                 &fWidth))  { return kFALSE; }
  if (!list->fill("strips_per_side",       &fStripsPerSide))  { return kFALSE; }

  Text_t owner[80];
  if (!list->fill("owner",    owner,80 )) { return kFALSE;}
  fCurrent_owner = owner;

  Text_t loca[80];
  if (!list->fill("location",    loca,80 )) { return kFALSE;}
  fCurrent_location = loca;

  if (!list->fill("vfd",            &fV_fd)) { return kFALSE;}
  if (!list->fill("i_150V_20C",     &fI_150V_20C )) { return kFALSE;}
  if (!list->fill("i_250V_20C",     &fI_250V_20C )) { return kFALSE;}
  if (!list->fill("sdef_pside",     &fSdefect_pside )) { return kFALSE;}
  if (!list->fill("sdef_nside",     &fSdefect_nside )) { return kFALSE;}
  if (!list->fill("quality",        &fQuality_grade )) { return kFALSE;}
  if (!list->fill("problem",        &fProblem       )) { return kFALSE;}
  if (!list->fill("passed",         &fPassed        )) { return kFALSE;}
  if (!list->fill("opt_passed",     &fOptPassed     )) { return kFALSE;}

  Text_t com[80];
  if (!list->fill("comments",    com,80 )) { return kFALSE;}
  fComments = com;

  return kTRUE;
}

void CbmStsDbQaSensor::clear()
{
  fVendor_name = fSensor_type = fBatch_id = fReticle_name = fProcessing = fComments = "";
  fCurrent_location = fCurrent_owner = "";
  fProblem = fPassed = fOptPassed = fStripsPerSide =  0;
  fUID = 0;
  fCompId = fWafer_id = fSdefect_pside = fSdefect_nside = fQuality_grade = 0;
  fV_fd = fI_150V_20C = fI_150V_20C  = fHeight = fWidth = 0.;

  if (fParam_Writer) { fParam_Writer->Reset(); }
  if (fParam_Reader) { fParam_Reader->Reset(); }
}


string CbmStsDbQaSensor::GetTableDefinition(const char* Name)
{

  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += "STSQASENSORPAR"; }
  sql += "( SEQNO           INT NOT NULL,";
  sql += "  ROW_ID          INT NOT NULL,";
  sql += "  COMP_ID         INT,";
  sql += "  UID             INT,";
  sql += "  SENSOR_TYPE     TEXT,";
  sql += "  BATCH_ID        TEXT,";
  sql += "  WAFER_ID        INT,";
  sql += "  RETICLE_NAME    TEXT,";
  sql += "  VENDOR_NAME     TEXT,";
  sql += "  PROCESSING      TEXT,";
  sql += "  HEIGHT          DOUBLE,";
  sql += "  WIDTH           DOUBLE,";
  sql += "  STRIPS_SIDE     INT,";
  sql += "  YEAR            INT,";
  sql += "  OWNER           TEXT,";
  sql += "  LOCATION        TEXT,";
  sql += "  V_FD            DOUBLE,";
  sql += "  I_150V_20C      DOUBLE,";
  sql += "  I_250V_20C      DOUBLE,";
  sql += "  DEF_PSIDE       INT,";
  sql += "  DEF_NSIDE       INT,";
  sql += "  QUALITY         INT,";
  sql += "  PROBLEM         INT,";
  sql += "  PASSED          INT,";
  sql += "  OPT_PASSED      INT,";
  sql += "  COMMENTS        TEXT,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}



void CbmStsDbQaSensor::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{

  res_in >> fCompId  >> fUID >> fSensor_type >> fBatch_id
         >> fWafer_id >> fReticle_name >> fVendor_name
         >> fProcessing >> fHeight >> fWidth >> fStripsPerSide
         >> fYear >> fCurrent_owner >> fCurrent_location
         >> fV_fd >> fI_150V_20C >> fI_250V_20C >> fSdefect_pside >> fSdefect_nside
         >> fQuality_grade >> fProblem >> fPassed >> fOptPassed >> fComments;
}

void CbmStsDbQaSensor::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{

  res_out << fCompId   << fUID << fSensor_type << fBatch_id
          << fWafer_id << fReticle_name << fVendor_name
          << fProcessing << fHeight << fWidth <<  fStripsPerSide
          << fYear << fCurrent_owner << fCurrent_location
          << fV_fd << fI_150V_20C << fI_250V_20C << fSdefect_pside << fSdefect_nside
          << fQuality_grade << fProblem << fPassed << fOptPassed << fComments;

}


void CbmStsDbQaSensor::fill(UInt_t rid)
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

  for (int i = 0; i < numRows; ++i)
  {
    CbmStsDbQaSensor* cgd = (CbmStsDbQaSensor*) fParam_Reader->GetRow(i);

    if (!cgd) { continue; }

      fCompId = cgd->GetCompId();
      fUID = cgd->GetUID();
      fSensor_type = cgd->GetSensorType();
      fBatch_id = cgd->GetBatchId();
      fWafer_id = cgd->GetWaferId();
      fReticle_name = cgd->GetReticleName();
      fVendor_name = cgd->GetVendorName();
      fProcessing = cgd->GetProcessing();
      fHeight     = cgd->GetHeight();
      fWidth     = cgd->GetWidth();
      fStripsPerSide = cgd->GetStripsPerSide();
      fYear          = cgd->GetYear();
      fCurrent_owner = cgd->GetOwner();
      fCurrent_location = cgd->GetLocation();
      fV_fd =  cgd->GetV_fd();
      fI_150V_20C = cgd->GetI_150V_20C();
      fI_250V_20C = cgd->GetI_250V_20C();
      fSdefect_pside = cgd->GetSdefect_pside();
      fSdefect_nside = cgd->GetSdefect_nside();
      fQuality_grade = cgd->GetQuality_grade();
      fProblem = cgd->GetProblem();
      fPassed = cgd->GetPassed();
      fOptPassed = cgd->GetOpticalCheck();
      fComments =  cgd->GetComment();
   }
}


void CbmStsDbQaSensor::store(UInt_t rid)
{

  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    cout << "-E-  CbmStsDbQaSensor::Store()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSQASENSORPAR") ) {
    sql_cmds.push_back(FairDb::GetValDefinition("STSQASENSORPAR").Data());
    sql_cmds.push_back(CbmStsDbQaSensor::GetTableDefinition());
  }

  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      cout << "-E- CbmStsDbQaSensor::Store() ******* Error Executing SQL commands ***********  " << endl;
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

  fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"Cbm Sensor parameter");

  // Object to Table mapping
  *fParam_Writer<< (*this);

  // Check for eventual IO problems
  if ( !fParam_Writer->Close() ) {
    fail = true;
    cout << "-E- CbmStsDbQaSensor::Store() ******** Cannot do IO on class: " << GetName() <<  endl;
  }

}

void CbmStsDbQaSensor::Print()
{
  std::cout<<"   STS QA SENSOR  <UID> "<< fUID <<  " <comp_Id> " << fCompId << std::endl;
  std::cout<<"   Type               = " << fSensor_type  << std::endl;
  std::cout<<"   Batch_id           = " << fBatch_id  << std::endl;
  std::cout<<"   Wafer_id           = " << fWafer_id  << std::endl;
  std::cout<<"   Reticle_name       = " << fReticle_name  << std::endl;
  std::cout<<"   Vendor_name        = " << fVendor_name  << std::endl;
  std::cout<<"   Processing         = " << fProcessing  << std::endl;
  std::cout<<"   Height             = " << fHeight  << std::endl;
  std::cout<<"   Width              = " << fWidth  << std::endl;
  std::cout<<"   StripsPerSide      = " << fStripsPerSide  << std::endl;
  std::cout<<"   Year               = " << fYear  << std::endl;
  std::cout<<"   Current_owner      = "  << fCurrent_owner  << std::endl;
  std::cout<<"   Current_location   = "  << fCurrent_location  << std::endl;
  std::cout<<"   V_fd  (Volts)      = "  << setprecision(5) << fV_fd  << std::endl;
  std::cout<<"   I_150V_20C  (muA)  = "  << setprecision(5) << fI_150V_20C  << std::endl;
  std::cout<<"   I_250V_20C  (muA)  = "  << setprecision(5) << fI_250V_20C  << std::endl;
  std::cout<<"   Defect   (pside)   = "  << fSdefect_pside  << std::endl;
  std::cout<<"   Defect   (nside)   = "  << fSdefect_nside  << std::endl;
  std::cout<<"   Quality grade      = "  << fQuality_grade  << std::endl;
  std::cout<<"   Problem            = "  << fProblem  << std::endl;
  std::cout<<"   Passed             = "  << fPassed  << std::endl;
  std::cout<<"   OptPassed          = "  << fOptPassed  << std::endl;
  std::cout<<"   Comments           = "  << fComments << std::endl;

}

Bool_t CbmStsDbQaSensor::Compare(const CbmStsDbQaSensor& that ) const {

  Bool_t test_s =
        (fSensor_type.compare(that.fSensor_type)==0)
    &&  (fBatch_id.compare(that.fBatch_id)==0)
    &&  (fReticle_name.compare(that.fReticle_name)==0)
    &&  (fVendor_name.compare(that.fVendor_name)==0)
    &&  (fProcessing.compare(that.fProcessing)==0)
    &&  (fCurrent_owner.compare(that.fCurrent_owner)==0)
    &&  (fCurrent_location.compare(that.fCurrent_location)==0)
    &&  (fComments.compare(that.fComments)==0);


  Bool_t test_d=
        (fCompId == that.fCompId)
    &&  (fUID == that.fUID)
    &&  (fWafer_id == that.fWafer_id)
    &&  (fHeight == that.fHeight)
    &&  (fWidth == that.fWidth)
    &&  (fStripsPerSide == that.fStripsPerSide)
    &&  (fYear == that.fYear)
    &&  (fV_fd == that.fV_fd)
    &&  (fI_150V_20C == that.fI_150V_20C)
    &&  (fI_250V_20C == that.fI_250V_20C)
    &&  (fSdefect_pside == that.fSdefect_pside)
    &&  (fSdefect_nside == that.fSdefect_nside)
    &&  (fQuality_grade == that.fQuality_grade)
    &&  (fProblem == that.fProblem)
    &&  (fPassed == that.fPassed)
    &&  (fOptPassed == that.fOptPassed);

  return (test_s && test_d);
}

FairDbWriter<CbmStsDbQaSensor>* CbmStsDbQaSensor::ActivateWriter(Int_t rid)
{

  if (rid == 0 ) {
    FairRunIdGenerator runID;
    rid =  runID.generateId();
  }
  
  // delete if already existing
  if (fParam_Writer) {
    cout << "-E- CbmStsDbQaSensor::ActivateWriter() already in use ---> reset Writer Template." << endl;
    delete fParam_Writer; fParam_Writer=NULL;
  }else {
    // Create according to IoV
    Bool_t fail= kFALSE;
    
    // Create a unique statement on choosen DB entry
    auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
    if ( ! stmtDbn.get() ) {
      cout << "-E-  CbmStsDbQaSensor::Store()  Cannot create statement for Database_id: " << GetDbEntry()
           << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
      exit(1);
    }
    
    // Check if for this DB entry the table already exists.
    // If not call the corresponding Table Definition Function
    std::vector<std::string> sql_cmds;
    TString atr(GetName());
    atr.ToUpper();
    
    if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSQASENSORPAR") ) {
      sql_cmds.push_back(FairDb::GetValDefinition("STSQASENSORPAR").Data());
      sql_cmds.push_back(CbmStsDbQaSensor::GetTableDefinition());
    }
    
    // Packed SQL commands executed internally via SQL processor
    std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
    while( itr != itrEnd ) {
      std::string& sql_cmd(*itr++);
      stmtDbn->Commit(sql_cmd.c_str());
      if ( stmtDbn->PrintExceptions() ) {
        fail = true;
        cout << "-E- CbmStsDbQaSensor::ActivateWriter() ******* Error Executing SQL commands ***********  " << endl;
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

Bool_t CbmStsDbQaSensor::Import(Int_t compid, const vector<string>& value)
{
  if (value.size()>0)
    {
      SetCompId(compid); 
      SetUID(compid);
      
      SetSensorType(value[0]);
      SetBatchId(value[1]);
      
      //cout << "2" << value[2] << endl;
      SetWaferId(lexical_cast<int>(value[2]));
      
      SetReticleName(value[3]);
      SetVendorName(value[4]);
      SetProcessing(value[5]);
      
      SetHeight(lexical_cast<double>(value[6]));
      SetWidth(lexical_cast<double>(value[7]));
      SetStripsPerSide(lexical_cast<int>(value[8]));
      SetYear(lexical_cast<int>(value[9]));
      
      SetCurrentOwner(value[10]);
      SetCurrentLocation(value[11]);
      
      //cout << "12:" << value[12] << endl;
      SetVfd(lexical_cast<double>(value[12]));
      //cout << "13:" << value[13] << endl;
      SetI_150V_20C(lexical_cast<double>(value[13]));
      //cout << "14:" << value[14] << endl;
      SetI_250V_20C(lexical_cast<double>(value[14]));
      //cout << "15:" << value[15] << endl;
      SetSdefect_pside(lexical_cast<int>(value[15]));
      //cout << "16:" << value[16] << endl;
      SetSdefect_nside(lexical_cast<int>(value[16]));
      //cout << "17:" << value[17] << endl;
      SetQuality_grade(lexical_cast<int>(value[17]));
      
      SetProblem(lexical_cast<int>(value[18]));
      SetPassed(lexical_cast<int>(value[19]));
      SetOpticalCheck(lexical_cast<int>(value[20]));
      SetComments(value[21]);
      return kTRUE;
    }
  return kFALSE;
}


void CbmStsDbQaSensor::CreateDbTable(Int_t db_entry)
{
  // Set new DB Entry
  SetDbEntry(db_entry);
  
  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
   if ( ! stmtDbn.get() ) {
    cout << "-E-  FairDbDemoPar1::CreateDbTable()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }


  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSQASENSORPAR") ) {
    sql_cmds.push_back(FairDb::GetValDefinition("STSQASENSORPAR").Data());
    sql_cmds.push_back(CbmStsDbQaSensor::GetTableDefinition());
  }
  

  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      cout << "-E- CbmStsDbQaSUID::CreateDbTable() * Error Executing SQL commands **  " << endl;
    }

  }

  // Refresh list of tables in connected database
  // for the choosen DB entry
  fMultConn->GetConnection(GetDbEntry())->SetTableExists();
  
}
