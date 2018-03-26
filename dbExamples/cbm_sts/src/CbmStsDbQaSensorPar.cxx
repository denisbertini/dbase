#include "CbmStsDbQaSensorPar.h"

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



ClassImp(CbmStsDbQaSensorPar);


static FairDbParRegistry<CbmStsDbQaSensorPar> qa_sensor("CbmStsDbQaSensorPar"); 


#include "FairDbReader.tpl"
template class  FairDbReader<CbmStsDbQaSensorPar>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<CbmStsDbQaSensorPar>;



CbmStsDbQaSensorPar::CbmStsDbQaSensorPar(const char* name, const char* title, const char* context, Bool_t own)
  : FairDbParSet(name,title,context, own),
    fCompId(-1),
    fUID(0), 
    fSensor_type(""),
    fBatch_id(""),
    fWafer_id(0),
    fSensor_name(""),
    fManufacturer(""),    
    fProduction_date(""),         
    fCurrent_owner(""),
    fCurrent_location(""),
	  fV_fd(0.),
    fI_150V_20C(0.),
    fI_250V_20C(0.),
    fSdefect_pside(0),
    fSdefect_nside(0),
    fQuality_grade(0),
    fProblem(""),
    fPassed(""),
    fComments(""), 
    fParam_Writer(NULL), //  Writer Meta-Class for SQL IO
    fParam_Reader(NULL), //  Reader Meta-Class for SQL IO
    fMultConn(FairDbTableInterfaceStore::Instance().fConnectionPool)
{
  // Set the default Db Entry to the first slot
  SetDbEntry(0);  
  // Set no aggregation
  SetCompId(fCompId);
  // Set Version 0 
  SetVersion(0);
}


CbmStsDbQaSensorPar::~CbmStsDbQaSensorPar()
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


void CbmStsDbQaSensorPar::putParams(FairParamList* list)
{
  std::cout<<"-I- CbmStsDbQaSensorPar::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("comp_id",  fCompId);
  list->add("s_uid",    fUID);
  list->add("sensor_type",  (Text_t*) fSensor_type.c_str());
  list->add("batch_id",     (Text_t*) fBatch_id.c_str());
  list->add("wafer_id",  fWafer_id);
  list->add("sensor_name",  (Text_t*) fSensor_name.c_str());
  list->add("manufacturer",  (Text_t*) fManufacturer.c_str());
  list->add("prod_date",  (Text_t*) fProduction_date.c_str());
  list->add("owner",  (Text_t*) fCurrent_owner.c_str());
  list->add("location",  (Text_t*) fCurrent_location.c_str());
  list->add("vfd",  fV_fd);
  list->add("i_150V_20C",  fI_150V_20C);
  list->add("i_250V_20C",  fI_250V_20C);
  list->add("sdef_pside",  fSdefect_pside);
  list->add("sdef_nside",  fSdefect_nside);
  list->add("quality",  fQuality_grade);
  list->add("problem",  (Text_t*) fProblem.c_str());
  list->add("passed",  (Text_t*) fPassed.c_str());
  list->add("comments",  (Text_t*) fComments.c_str());
}

Bool_t CbmStsDbQaSensorPar::getParams(FairParamList* list)
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

  Text_t sName[80];
  if (!list->fill("sensor_name",    sName,80 )) { return kFALSE;}
  fSensor_name = sName;

  Text_t manuf[80];
  if (!list->fill("manufacturer",    manuf,80 )) { return kFALSE;}
  fManufacturer = manuf;

  Text_t prod[80];
  if (!list->fill("prod_date",    prod,80 )) { return kFALSE;}
  fProduction_date = prod;

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
  
  Text_t prob[80];
  if (!list->fill("problem",    prob,80 )) { return kFALSE;}
  fProblem = prob;

  Text_t pass[80];
  if (!list->fill("passed",    pass,80 )) { return kFALSE;}
  fPassed = pass;

  Text_t com[80];
  if (!list->fill("comments",    com,80 )) { return kFALSE;}
  fComments = com;

  return kTRUE;
}

void CbmStsDbQaSensorPar::clear()
{
  fSensor_name = fSensor_type = fBatch_id = "";
  fManufacturer = fProduction_date = fCurrent_location = fCurrent_owner = "";
  fProblem = fPassed = fComments = "";
  fUID = 0;
  fCompId = fWafer_id = fSdefect_pside = fSdefect_nside = fQuality_grade = 0;
  fV_fd = fI_150V_20C = fI_150V_20C = 0.;
     
  if (fParam_Writer) { fParam_Writer->Reset(); }
  if (fParam_Reader) { fParam_Reader->Reset(); }
}


string CbmStsDbQaSensorPar::GetTableDefinition(const char* Name)
{

  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += "CBMSTSDBQASENSORPAR"; }
  sql += "( SEQNO           INT NOT NULL,";
  sql += "  ROW_ID          INT NOT NULL,";
  sql += "  COMP_ID         INT,";
  sql += "  UID             INT,";
  sql += "  SENSOR_TYPE     TEXT,";
  sql += "  BATCH_ID        TEXT,";
  sql += "  WAFER_ID        INT,";
  sql += "  SENSOR_NAME     TEXT,";
  sql += "  MANUFACT        TEXT,";
  sql += "  PROD_DATE       TEXT,";
  sql += "  OWNER           TEXT,";
  sql += "  LOCATION        TEXT,";
  sql += "  V_FD            DOUBLE,";
  sql += "  I_150V_20C      DOUBLE,";
  sql += "  I_250V_20C      DOUBLE,";
  sql += "  DEF_PSIDE       INT,";
  sql += "  DEF_NSIDE       INT,";
  sql += "  QUALITY         INT,";
  sql += "  PROBLEM         TEXT,";
  sql += "  PASSED          TEXT,";
  sql += "  COMMENTS        TEXT,"; 
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}



void CbmStsDbQaSensorPar::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{

  res_in >> fCompId  >> fUID >> fSensor_type >> fBatch_id
         >> fWafer_id >> fSensor_name >> fManufacturer
         >> fProduction_date >> fCurrent_owner >> fCurrent_location
         >> fV_fd >> fI_150V_20C >> fI_250V_20C >> fSdefect_pside >> fSdefect_nside
         >> fQuality_grade >> fProblem >> fPassed >> fComments;
}

void CbmStsDbQaSensorPar::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{

  res_out << fCompId   << fUID << fSensor_type << fBatch_id
          << fWafer_id << fSensor_name << fManufacturer
          << fProduction_date << fCurrent_owner << fCurrent_location
          << fV_fd << fI_150V_20C << fI_250V_20C << fSdefect_pside << fSdefect_nside
          << fQuality_grade << fProblem << fPassed << fComments;

}


void CbmStsDbQaSensorPar::fill(UInt_t rid)
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
    CbmStsDbQaSensorPar* cgd = (CbmStsDbQaSensorPar*) fParam_Reader->GetRow(i);
 
    if (!cgd) { continue; }
      
      fCompId = cgd->GetCompId(); 
      fUID = cgd->GetUID(); 
      fSensor_type = cgd->GetSensorType(); 
      fBatch_id = cgd->GetBatchId(); 
      fWafer_id = cgd->GetWaferId(); 
      fSensor_name = cgd->GetSensorName(); 
      fManufacturer = cgd->GetManufacturer(); 
      fProduction_date = cgd->GetProductionDate(); 
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
      fComments =  cgd->GetComment(); 
   }
}


void CbmStsDbQaSensorPar::store(UInt_t rid)
{

  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    cout << "-E-  CbmStsDbQaSensorPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists("CBMSTSDBQASENSORPAR") ) {
    sql_cmds.push_back(FairDb::GetValDefinition("CBMSTSDBQASENSORPAR").Data());
    sql_cmds.push_back(CbmStsDbQaSensorPar::GetTableDefinition());
  }

  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      cout << "-E- CbmStsDbQaSensorPar::Store() ******* Error Executing SQL commands ***********  " << endl;
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
    cout << "-E- CbmStsDbQaSensorPar::Store() ******** Cannot do IO on class: " << GetName() <<  endl;
  }

}

void CbmStsDbQaSensorPar::Print()
{
  std::cout<<"   STS QA SENSOR  <UID> " << fUID <<  " <comp_Id> " << fCompId << std::endl;
  std::cout<<"   Name               = " << fSensor_name  << std::endl;
  std::cout<<"   Type               = " << fSensor_type  << std::endl;
  std::cout<<"   Batch_id           = " << fBatch_id  << std::endl;
  std::cout<<"   Wafer_id           = " << fWafer_id  << std::endl;
  std::cout<<"   Manufacturer       = "  << fManufacturer  << std::endl;
  std::cout<<"   Production date    = " << fProduction_date << std::endl;
  std::cout<<"   Current_owner      = "  << fCurrent_owner  << std::endl;
  std::cout<<"   Current_location   = "  << fCurrent_location  << std::endl;
  std::cout<<"   Current_location   = "  << fCurrent_location  << std::endl;
  std::cout<<"   V_fd  (Volts)      = "  << fV_fd  << std::endl;
  std::cout<<"   I_150V_20C  (muA)  = "  << fI_150V_20C  << std::endl;
  std::cout<<"   I_250V_20C  (muA)  = "  << fI_250V_20C  << std::endl;
  std::cout<<"   Defect   (pside)   = "  << fSdefect_pside  << std::endl;
  std::cout<<"   Defect   (nside)   = "  << fSdefect_nside  << std::endl;
  std::cout<<"   Quality grade      = "  << fQuality_grade  << std::endl;
  std::cout<<"   Problem            = "  << fProblem  << std::endl;
  std::cout<<"   Passed             = "  << fPassed  << std::endl;
  std::cout<<"   Comments           = "  << fComments << std::endl;

}

Bool_t CbmStsDbQaSensorPar::Compare(const CbmStsDbQaSensorPar& that ) const {
  
  Bool_t test_s = 
        (fSensor_type.compare(that.fSensor_type)==0) 
    &&  (fSensor_name.compare(that.fSensor_name)==0) 
    &&  (fBatch_id.compare(that.fBatch_id)==0) 
    &&  (fManufacturer.compare(that.fManufacturer)==0) 
    &&  (fProduction_date.compare(that.fProduction_date)==0) 
    &&  (fCurrent_owner.compare(that.fCurrent_owner)==0) 
    &&  (fCurrent_location.compare(that.fCurrent_location)==0) 
    &&  (fProblem.compare(that.fProblem)==0)
    &&  (fPassed.compare(that.fPassed)==0)
    &&  (fComments.compare(that.fComments)==0);

  
  
  Bool_t test_d=
        (fCompId == that.fCompId) 
    &&  (fUID == that.fUID) 
    &&  (fWafer_id == that.fWafer_id) 
    &&  (fV_fd == that.fV_fd) 
    &&  (fI_150V_20C == that.fI_150V_20C) 
    &&  (fI_250V_20C == that.fI_250V_20C) 
    &&  (fSdefect_pside == that.fSdefect_pside) 
    &&  (fSdefect_nside == that.fSdefect_nside) 
    &&  (fQuality_grade == that.fQuality_grade); 

  return (test_s && test_d); 
}

FairDbWriter<CbmStsDbQaSensorPar>* CbmStsDbQaSensorPar::ActivateWriter(Int_t rid)
{
   // delete if already existing
   if (fParam_Writer) { delete fParam_Writer; fParam_Writer=NULL; }
 
   else {
         // Create according to IoV
         Bool_t fail= kFALSE;
        
         // Create a unique statement on choosen DB entry
         auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
         if ( ! stmtDbn.get() ) {
           cout << "-E-  CbmStsDbQaSensorPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
                    << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
           exit(1);
         }
        
         // Check if for this DB entry the table already exists.
         // If not call the corresponding Table Definition Function
         std::vector<std::string> sql_cmds;
         TString atr(GetName());
         atr.ToUpper();
        
         if (! fMultConn->GetConnection(GetDbEntry())->TableExists("CBMSTSDBQASENSORPAR") ) {
           sql_cmds.push_back(FairDb::GetValDefinition("CBMSTSDBQASENSORPAR").Data());
           sql_cmds.push_back(CbmStsDbQaSensorPar::GetTableDefinition());
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


Bool_t CbmStsDbQaSensorPar::Import(const vector<string>& value)
{
     if (value.size()>0)
      { 
       SetCompId(fCompId); // No composition
       cout << "0" << value[0] << endl; 
       SetUID(lexical_cast<int>(value[0]));
       SetSensorType(value[1]);
       SetBatchId(value[2]);
       cout << "3" << value[3] << endl; 
       SetWaferId(lexical_cast<int>(value[3]));
       SetSensorName(value[4]);
       SetManufacturer(value[5]);
       SetProductionDate(value[6]);
       SetCurrentOwner(value[7]);
       SetCurrentLocation(value[8]);
       cout << "9:" << value[9] << endl; 
       SetVfd(lexical_cast<double>(value[9]));
       cout << "10:" << value[10] << endl; 
       SetI_150V_20C(lexical_cast<double>(value[10]));
       cout << "11:" << value[11] << endl; 
       SetI_250V_20C(lexical_cast<double>(value[11]));
       cout << "12:" << value[12] << endl; 
       SetSdefect_pside(lexical_cast<int>(value[12]));
       cout << "13:" << value[13] << endl; 
       SetSdefect_nside(lexical_cast<int>(value[13]));
       cout << "14:" << value[14] << endl; 
       SetQuality_grade(lexical_cast<int>(value[14]));
       SetProblem(value[15]);
       SetPassed(value[16]);
       SetComments(value[17]);
       return kTRUE; 
      }   
     return kFALSE;    
     //Print();
}


