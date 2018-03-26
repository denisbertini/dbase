#include "CbmStsDbQaGeometryPar.h"

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


ClassImp(CbmStsDbQaGeometryPar);


static FairDbParRegistry<CbmStsDbQaGeometryPar> qa_iv("StsQaGeomPar");

#include "FairDbReader.tpl"
template class  FairDbReader<CbmStsDbQaGeometryPar>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<CbmStsDbQaGeometryPar>;



CbmStsDbQaGeometryPar::CbmStsDbQaGeometryPar(const char* name, const char* title, const char* context, Bool_t own)
  : FairDbParSet(name,title,context, own), 
    fCompId(-1),
    fUID(0),
    fVendor(""),
    fType(""),
    fWaferId(0),
    fReticleName(""),
	fProcessing(""),
    fHeight(0.),
    fWidth(0.),
    fPitch(0.),
    fStereoAngle_p(0.),
    fStereoAngle_n(0.),
    fStripsPerSide(0),
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


CbmStsDbQaGeometryPar::~CbmStsDbQaGeometryPar()
{

  fVendor = fType = fReticleName = fProcessing = "";
  fHeight = fWidth = fPitch = fStereoAngle_p = fStereoAngle_n = 0.;
  fStripsPerSide = fWaferId = 0 ;

  if (fParam_Writer) {
    delete fParam_Writer;
    fParam_Writer=NULL;
  }

  if (fParam_Reader) {
    delete fParam_Reader;
    fParam_Reader=NULL;
  }

}


void CbmStsDbQaGeometryPar::putParams(FairParamList* list)
{
  std::cout<< "-I- CbmStsDbQaGeometryPar::putParams() called " <<std::endl;
  if(!list) { return; }
  list->add("comp_id",  fCompId);
  list->add("s_uid",    fUID);
  list->add("vendor",   (Text_t*) fVendor.c_str());
  list->add("type",    (Text_t*) fType.c_str());
  list->add("wafer_id", fWaferId);
  list->add("reticle_name", (Text_t*) fReticleName.c_str());
  list->add("processing",   (Text_t*) fProcessing.c_str());
  list->add("height", fHeight);
  list->add("width", fWidth);
  list->add("pitch", fPitch);
  list->add("stereo_p", fStereoAngle_p);
  list->add("stereo_n", fStereoAngle_n);
  list->add("strips_per_side", fStripsPerSide);
}

Bool_t CbmStsDbQaGeometryPar::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  if (!list->fill("comp_id",     &fCompId)) { return kFALSE; }
  if (!list->fill("s_uid",       &fUID))    { return kFALSE; }

  Text_t aName[155];

  if (!list->fill("vendor",    aName,155 )) { return kFALSE;}
  fVendor = aName;

  if (!list->fill("type",    aName,155 )) { return kFALSE;}
  fType = aName;
   
  if (!list->fill("wafer_id",      &fWaferId))    { return kFALSE; }

  if (!list->fill("reticle_name",    aName,155 )) { return kFALSE;}
  fReticleName = aName;
  if (!list->fill("processing",    aName,155 ))   { return kFALSE;}
  fProcessing = aName;
  
  if (!list->fill("height",      &fHeight))                  { return kFALSE; }
  if (!list->fill("width",       &fWidth))                   { return kFALSE; }
  if (!list->fill("pitch",       &fPitch))                   { return kFALSE; }
  if (!list->fill("stereo_p",       &fStereoAngle_p))         { return kFALSE; }
  if (!list->fill("stereo_n",       &fStereoAngle_n))         { return kFALSE; }
  
  if (!list->fill("strips_per_side",   &fStripsPerSide) )    { return kFALSE; }
 
  return kTRUE;
}

void CbmStsDbQaGeometryPar::clear()
{
  fCompId = fUID = fStripsPerSide = fWaferId = 0;
  fVendor = fType = fReticleName = fProcessing = "";
  fStereoAngle_n = fStereoAngle_n = 0.; 
  fHeight = fWidth = fPitch = 0.;

  if (fParam_Writer) { fParam_Writer->Reset(); }
  if (fParam_Reader) { fParam_Reader->Reset(); }
}


string CbmStsDbQaGeometryPar::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += "STSQAGEOMPAR"; }
  sql += "( SEQNO            INT NOT NULL,";
  sql += "  ROW_ID           INT NOT NULL,";
  sql += "  COMP_ID          INT,";
  sql += "  UID              INT,";
  sql += "  VENDOR           TEXT,";
  sql += "  TYPE             TEXT,";
  sql += "  WAFER_ID          INT,";
  sql += "  RETICLE          TEXT,";
  sql += "  PROCESS          TEXT,";
  sql += "  HEIGHT           DOUBLE,";
  sql += "  WIDTH            DOUBLE,";
  sql += "  PITCH            DOUBLE,";
  sql += "  STEREO_P         DOUBLE,";
  sql += "  STEREO_N         DOUBLE,";
  sql += "  STRIPSPERSIDE    INT,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}


void CbmStsDbQaGeometryPar::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  
   res_in >>  fCompId >> fUID 
          >>  fVendor
          >>  fType
          >>  fWaferId
          >>  fReticleName
          >>  fProcessing
          >>  fHeight
          >>  fWidth
          >>  fPitch
          >>  fStereoAngle_p
          >>  fStereoAngle_n
          >>  fStripsPerSide;

}

void CbmStsDbQaGeometryPar::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  res_out << fCompId << fUID 
          <<  fVendor
          <<  fType
          <<  fWaferId
          <<  fReticleName
          <<  fProcessing
          <<  fHeight
          <<  fWidth
          <<  fPitch
          <<  fStereoAngle_p
          <<  fStereoAngle_n
          <<  fStripsPerSide;
}


void CbmStsDbQaGeometryPar::fill(UInt_t rid)
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
    CbmStsDbQaGeometryPar* cgd = (CbmStsDbQaGeometryPar*) fParam_Reader->GetRow(i);
    if (!cgd) { continue; }
    fCompId        =  cgd->GetCompId(); 
    fUID           =  cgd->GetUID();
    fVendor        =  cgd->GetVendor();
    fType          =  cgd->GetType();
    fWaferId       =  cgd->GetWaferId();
    fReticleName   =  cgd->GetReticleName(); 
    fProcessing    =  cgd->GetProcessing();
    fHeight        =  cgd->GetHeight();
    fWidth         =  cgd->GetWidth();
    fPitch         =  cgd->GetPitch();
    fStereoAngle_p   =  cgd->GetStereoAngleP();
    fStereoAngle_n   =  cgd->GetStereoAngleN();
    fStripsPerSide   =  cgd->GetStripsPerSide();    
  }

}


void CbmStsDbQaGeometryPar::store(UInt_t rid)
{

  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    cout << "-E-  CbmStsDbQaGeometryPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSQAGEOMPAR") ) {
    sql_cmds.push_back(FairDb::GetValDefinition("STSQAGEOMPAR").Data());
    sql_cmds.push_back(CbmStsDbQaGeometryPar::GetTableDefinition());
  }

  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      cout << "-E- CbmStsDbQaGeometryPar::Store() ******* Error Executing SQL commands ***********  " << endl;
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

  fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS QA Sensor Geometry Parameter");

  // Object to Table mapping
  *fParam_Writer<< (*this);

  // Check for eventual IO problems
  if ( !fParam_Writer->Close() ) {
    fail = true;
    cout << "-E- CbmStsDbQaGeometryPar::Store() ******** Cannot do IO on class: " << GetName() <<  endl;
  }

}

void CbmStsDbQaGeometryPar::Print()
{
  std::cout<<"   STS QA Sensor Geometry Paramters <UID> "<< fUID <<  " <comp_Id> " << fCompId << std::endl;
  std::cout<<"   Vendor        = " << fVendor  << std::endl;
  std::cout<<"   Type       = " << fType  << std::endl;
  std::cout<<"   WaferId    = " << fWaferId  << std::endl;
  std::cout<<"   ReticleName    = " << fReticleName  << std::endl;
  std::cout<<"   Processing    = " << fProcessing  << std::endl;
  std::cout<<"   height    = " << fHeight  << std::endl;
  std::cout<<"   width    = " << fWidth  << std::endl;
  std::cout<<"   pitch    = " << fPitch  << std::endl;
  std::cout<<"   stereo_p    = " << fStereoAngle_p  << std::endl;
  std::cout<<"   stereo_n    = " << fStereoAngle_n  << std::endl;
  std::cout<<"   strips_per_side    = " << fStripsPerSide  << std::endl;
}


Bool_t CbmStsDbQaGeometryPar::Compare(const CbmStsDbQaGeometryPar& that ) const {
  
  Bool_t test_h =  
	      (fUID      == that.fUID)
	  &&  (fCompId   == that.fCompId)
      &&  (fVendor.compare(that.fVendor)==0)
      &&  (fType.compare(that.fType)==0)
      &&  (fWaferId ==  that.fWaferId)
      &&  (fReticleName.compare(that.fReticleName)==0)
      &&  (fProcessing.compare(that.fProcessing)==0)
      &&  (fHeight   == that.fHeight)
      &&  (fWidth    == that.fWidth)
      &&  (fPitch    == that.fPitch)
      &&  (fStereoAngle_p == that.fStereoAngle_p )
      &&  (fStereoAngle_n == that.fStereoAngle_n )
      &&  (fStripsPerSide    == that.fStripsPerSide);

  return (test_h); 
}

FairDbWriter<CbmStsDbQaGeometryPar>* CbmStsDbQaGeometryPar::ActivateWriter(Int_t rid)
{
   // delete if already existing
   if (fParam_Writer) { delete fParam_Writer; fParam_Writer=NULL; }
 
   else {
         // Create according to IoV
         Bool_t fail= kFALSE;
        
         // Create a unique statement on choosen DB entry
         auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
         if ( ! stmtDbn.get() ) {
           cout << "-E-  CbmStsDbQaGeometryPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
                    << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
           exit(1);
         }
        
         // Check if for this DB entry the table already exists.
         // If not call the corresponding Table Definition Function
         std::vector<std::string> sql_cmds;
         TString atr(GetName());
         atr.ToUpper();
        
         if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSQAGEOMPAR") ) {
           sql_cmds.push_back(FairDb::GetValDefinition("STSQAGEOMPAR").Data());
           sql_cmds.push_back(CbmStsDbQaGeometryPar::GetTableDefinition());
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
                
         fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS QA Sensor Geometry Parameter"); 
         return fParam_Writer;
        
   }
 
   return NULL;
}

Bool_t CbmStsDbQaGeometryPar::Import(const vector<string>& value)
{
     if (value.size()>0)
      { 
       SetCompId(fCompId); // No composition
       cout << "0" << value[0] << endl; 
       SetUID(lexical_cast<int>(value[0]));
       SetVendor(value[1]);
       SetType(value[2]);
       cout << "3" << value[3] << endl; 
       SetWaferId(lexical_cast<int>(value[3]));
       SetReticleName(value[4]);
       SetProcessing(value[5]);
       SetHeight(lexical_cast<double>(value[6]));
       SetWidth(lexical_cast<double>(value[7]));
       SetPitch(lexical_cast<double>(value[8]));
       SetStereoAngleP(lexical_cast<double>(value[9]));
       SetStereoAngleN(lexical_cast<double>(value[10]));
       SetStripsPerSide(lexical_cast<int>(value[11]));

       return kTRUE; 
      }   
     return kFALSE;    
     //Print();
}


