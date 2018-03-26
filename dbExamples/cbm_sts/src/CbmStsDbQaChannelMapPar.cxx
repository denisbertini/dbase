#include "CbmStsDbQaChannelMapPar.h"

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


ClassImp(CbmStsDbQaChannelMapPar);


static FairDbParRegistry<CbmStsDbQaChannelMapPar> qa_iv("StsQaChMapPar");

#include "FairDbReader.tpl"
template class  FairDbReader<CbmStsDbQaChannelMapPar>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<CbmStsDbQaChannelMapPar>;



CbmStsDbQaChannelMapPar::CbmStsDbQaChannelMapPar(const char* name, const char* title, const char* context, Bool_t own)
  : FairDbParSet(name,title,context, own),
    fCompId(-1),
    fUID(0),
    fType(""),
    fWaferId(0),
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
     // reset clear
     clear();
}


CbmStsDbQaChannelMapPar::~CbmStsDbQaChannelMapPar()
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


void CbmStsDbQaChannelMapPar::putParams(FairParamList* list)
{
  std::cout<<"-I- CbmStsDbQaChannelMapPar::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("comp_id",  fCompId);
  list->add("s_uid",    fUID);
  list->add("type",     (Text_t*) fType.c_str());
  list->add("wafer_id",    fWaferId);

  list->add("ac_cap_ok_pside", fAC_Cap_Ok_pside, NCHMAX);
  list->add("ac_cap_ok_nside", fAC_Cap_Ok_nside, NCHMAX);
  list->add("ac_cap_value_pside", fAC_Cap_Value_pside, NCHMAX);
  list->add("ac_cap_value_nside", fAC_Cap_Value_nside, NCHMAX);
  list->add("i_strip_pside", fI_Strip_pside, NCHMAX);
  list->add("i_strip_nside", fI_Strip_nside, NCHMAX);

}

Bool_t CbmStsDbQaChannelMapPar::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  if (!list->fill("comp_id",     &fCompId)) { return kFALSE; }
  if (!list->fill("s_uid",       &fUID))    { return kFALSE; }
  Text_t tName[80];
  if (!list->fill("type",       tName,80 )) { return kFALSE;}
  fType = tName;
  if (!list->fill("wafer_id",          &fWaferId))              { return kFALSE; } 

  if(!list->fill("ac_cap_ok_pside",    fAC_Cap_Ok_pside, NCHMAX))       { return kFALSE; }
  if(!list->fill("ac_cap_ok_nside",    fAC_Cap_Ok_nside, NCHMAX))       { return kFALSE; } 
  if(!list->fill("ac_cap_value_pside", fAC_Cap_Value_pside, NCHMAX))    { return kFALSE; }
  if(!list->fill("ac_cap_value_nside", fAC_Cap_Value_nside, NCHMAX))    { return kFALSE; }
  if(!list->fill("i_strip_pside",      fI_Strip_pside, NCHMAX))         { return kFALSE; }
  if(!list->fill("i_strip_nside",      fI_Strip_nside, NCHMAX))         { return kFALSE; }

  return kTRUE;
}

void CbmStsDbQaChannelMapPar::clear()
{
  fCompId = fUID = fWaferId = 0;
  fType = "";
  // <DB> Not so much overhead here.
  for(Int_t i=0; i<NCHMAX; i++) { 
	fAC_Cap_Ok_pside[i] = fAC_Cap_Ok_nside[i] = 0;
	fAC_Cap_Value_pside[i] = fAC_Cap_Value_pside[i] = 0.;
	fI_Strip_pside[i] = fI_Strip_nside[i] = 0.;     
  }
 
  if (fParam_Writer) { fParam_Writer->Reset(); }
  if (fParam_Reader) { fParam_Reader->Reset(); }

}


string CbmStsDbQaChannelMapPar::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += "STSQACHMAPPAR"; }
  sql += "( SEQNO          INT NOT NULL,";
  sql += "  ROW_ID         INT NOT NULL,";
  sql += "  COMP_ID        INT,";
  sql += "  UID            INT,";
  sql += "  TYPE           TEXT,";
  sql += "  WAFER_ID       INT,";
  sql += "  AC_CAP_OK_PSIDE             TEXT,";
  sql += "  AC_CAP_OK_NSIDE             TEXT,";
  sql += "  AC_CAP_VAL_PSIDE            TEXT,";
  sql += "  AC_CAP_VAL_NSIDE            TEXT,";
  sql += "  I_STRIP_PSIDE               TEXT,";
  sql += "  I_STRIP_NSIDE               TEXT,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}


void CbmStsDbQaChannelMapPar::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  
  clear();
 
  FairDbStreamer b_1(fAC_Cap_Ok_pside , NCHMAX);
  FairDbStreamer b_2(fAC_Cap_Ok_nside , NCHMAX);
  FairDbStreamer b_3(fAC_Cap_Value_pside, NCHMAX);
  FairDbStreamer b_4(fAC_Cap_Value_nside, NCHMAX);
  FairDbStreamer b_5(fI_Strip_pside, NCHMAX);
  FairDbStreamer b_6(fI_Strip_nside, NCHMAX);

  res_in >> fCompId >> fUID >> fType >> fWaferId 
		 >> b_1 >> b_2 >> b_3 >> b_4 >> b_5 >> b_6;

  b_1.Fill(fAC_Cap_Ok_pside);
  b_2.Fill(fAC_Cap_Ok_nside);
  b_3.Fill(fAC_Cap_Value_pside);
  b_4.Fill(fAC_Cap_Value_nside);
  b_5.Fill(fI_Strip_pside);
  b_6.Fill(fI_Strip_nside);
  
}

void CbmStsDbQaChannelMapPar::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{ 

  FairDbStreamer b_1(fAC_Cap_Ok_pside , NCHMAX);
  FairDbStreamer b_2(fAC_Cap_Ok_nside , NCHMAX);
  FairDbStreamer b_3(fAC_Cap_Value_pside, NCHMAX);
  FairDbStreamer b_4(fAC_Cap_Value_nside, NCHMAX);
  FairDbStreamer b_5(fI_Strip_pside, NCHMAX);
  FairDbStreamer b_6(fI_Strip_nside, NCHMAX);

  res_out << fCompId << fUID << fType << fWaferId 
 		 << b_1 << b_2 << b_3 << b_4 << b_5 << b_6;
}


void CbmStsDbQaChannelMapPar::fill(UInt_t rid)
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
    CbmStsDbQaChannelMapPar* cgd = (CbmStsDbQaChannelMapPar*) fParam_Reader->GetRow(i);
    if (!cgd) { continue; }
    fCompId = cgd->GetCompId(); 
    fUID    = cgd->GetUID();
    fType   = cgd->GetType();
    fWaferId = cgd->GetWaferId();

    for(Int_t j=0;j<NCHMAX;j++){
	   SetAC_Cap_Ok_psideAt(j, GetAC_Cap_Ok_psideAt(j));   
       SetAC_Cap_Ok_nsideAt(j, GetAC_Cap_Ok_nsideAt(j));    
       SetAC_Cap_Value_psideAt(j, GetAC_Cap_Value_psideAt(j));
       SetAC_Cap_Value_nsideAt(j, GetAC_Cap_Value_nsideAt(j));
       SetI_Strip_psideAt(j, GetI_Strip_psideAt(j));
       SetI_Strip_nsideAt(j, GetI_Strip_nsideAt(j)); 
   }
  }

}


void CbmStsDbQaChannelMapPar::store(UInt_t rid)
{

  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    cout << "-E-  CbmStsDbQaChannelMapPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSQACHMAPPAR") ) {
    sql_cmds.push_back(FairDb::GetValDefinition("STSQACHMAPPAR").Data());
    sql_cmds.push_back(CbmStsDbQaChannelMapPar::GetTableDefinition());
  }

  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      cout << "-E- CbmStsDbQaChannelMapPar::Store() ******* Error Executing SQL commands ***********  " << endl;
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

  fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS QA Channel Map parameter");

  // Object to Table mapping
  *fParam_Writer<< (*this);

  // Check for eventual IO problems
  if ( !fParam_Writer->Close() ) {
    fail = true;
    cout << "-E- CbmStsDbQaChannelMapPar::Store() ******** Cannot do IO on class: " << GetName() <<  endl;
  }

}

void CbmStsDbQaChannelMapPar::Print()
{

  std::cout<<"   STS QA Channel Map Paramters <UID> "<< fUID <<  " <comp_Id> " << fCompId << std::endl;
  std::cout<<"   Type "<<  fType <<  std::endl;
  std::cout<<"   WaferId "<<  fWaferId <<  std::endl;

  for (Int_t i=0;i<NCHMAX;i++) {
	if (fAC_Cap_Value_pside[i]>0.){
      int ch=i+1;
	  std::cout<< "ch# "  << ch << "   AC_CAP_OK_PSIDE    = " << fAC_Cap_Ok_pside[i] << std::endl;
	  std::cout<<  "ch# " << ch << "   AC_CAP_OK_NSIDE    = " << fAC_Cap_Ok_nside[i] << std::endl;
	  std::cout<<  "ch# " << ch << "   AC_CAP_VAL_PSIDE   = " << fAC_Cap_Value_pside[i] << std::endl;
	  std::cout<<  "ch# " << ch << "   AC_CAP_VAL_NSIDE   = " << fAC_Cap_Value_nside[i] << std::endl;
	  std::cout<<  "ch# " << ch << "   I_STRIP_PSIDE      = " << fI_Strip_pside[i] << std::endl;
	  std::cout<<  "ch# " << ch << "   I_STRIP_NSIDE      = " << fI_Strip_nside[i] << std::endl;
	  std::cout << " " << std::endl; 
	}
  }
  
}



Bool_t CbmStsDbQaChannelMapPar::Compare(const CbmStsDbQaChannelMapPar& that ) const {
  
  Bool_t test_h =  
	      (fUID      == that.fUID)
	  &&  (fCompId   == that.fCompId)
	  &&  (fType.compare(that.fType)==0) 
	  &&  (fWaferId   == that.fWaferId);

  Bool_t test_d=kTRUE;

  for(Int_t i=0; i<NCHMAX;i++){
    Int_t  a =  GetAC_Cap_Ok_psideAt(i); 
    Int_t  b =  that.GetAC_Cap_Ok_psideAt(i); 
    Int_t  c =  GetAC_Cap_Ok_nsideAt(i); 
    Int_t  d =  that.GetAC_Cap_Ok_nsideAt(i);

    Double_t  e =  GetAC_Cap_Value_psideAt(i); 
    Double_t  f =  that.GetAC_Cap_Value_psideAt(i);
    Double_t  g =  GetAC_Cap_Value_nsideAt(i); 
    Double_t  h =  that.GetAC_Cap_Value_nsideAt(i);

    Double_t  ee = GetI_Strip_psideAt(i); 
    Double_t  ff =  that.GetI_Strip_psideAt(i); 
    Double_t  gg =  GetI_Strip_nsideAt(i); 
    Double_t  hh =  that.GetI_Strip_nsideAt(i); 
 
	if (    ( a != b ) ||   ( c != d  )  || (  e != f  )  
         || 
            (g != h )  ||  ( ee != ff )  || ( gg != hh )  
        ){
	  test_d = kFALSE;
      break;
    }      
  }

  return ( test_h && test_d); 
}

FairDbWriter<CbmStsDbQaChannelMapPar>* CbmStsDbQaChannelMapPar::ActivateWriter(Int_t rid)
{
   // delete if already existing
   if (fParam_Writer) { delete fParam_Writer; fParam_Writer=NULL; }
 
   else {
         // Create according to IoV
         Bool_t fail= kFALSE;
        
         // Create a unique statement on choosen DB entry
         auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
         if ( ! stmtDbn.get() ) {
           cout << "-E-  CbmStsDbQaChannelMapPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
                    << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
           exit(1);
         }
        
         // Check if for this DB entry the table already exists.
         // If not call the corresponding Table Definition Function
         std::vector<std::string> sql_cmds;
         TString atr(GetName());
         atr.ToUpper();
        
         if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSQACHMAPPAR") ) {
           sql_cmds.push_back(FairDb::GetValDefinition("STSQACHMAPPAR").Data());
           sql_cmds.push_back(CbmStsDbQaChannelMapPar::GetTableDefinition());
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
                
         fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS QA Channel Map Parameter"); 
         return fParam_Writer;
        
   }
 
   return NULL;
}


Bool_t CbmStsDbQaChannelMapPar::Import(const vector<string>& value)
{
 if (value.size()>0)
      { 
       SetCompId(fCompId); // No composition
       //cout << "0" << value[0] << endl; 
       SetUID(lexical_cast<int>(value[0]));
       SetType(value[1]);
       //cout << "3" << value[2] << endl; 
       SetWaferId(lexical_cast<int>(value[2]));
	   // Get the channel id  
       Int_t ich = lexical_cast<int>(value[3]);
       Int_t index = ich-1; 
       
       SetAC_Cap_Ok_psideAt(index, lexical_cast<int>(value[4]));          
       SetAC_Cap_Ok_nsideAt(index, lexical_cast<int>(value[5]));          
       SetAC_Cap_Value_psideAt(index, lexical_cast<double>(value[6]));          
       SetAC_Cap_Value_nsideAt(index, lexical_cast<double>(value[7]));
       SetI_Strip_psideAt(index, lexical_cast<double>(value[8]));           
       SetI_Strip_nsideAt(index, lexical_cast<double>(value[9]));
       
       // <DB> to simplify i assume ordering of channel#
       // in the input csv file   
       // Import is true ONLY if all channel in the sensor
	   // are completed   
       if (ich == NCHMAX) return kTRUE;
		 else return kFALSE; 
      }   
     return kFALSE;    

}

