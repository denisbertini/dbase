#include "CbmStsDbQa.h"


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

#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/progress.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>


using namespace std;
using namespace boost;
using namespace boost::filesystem;
using boost::lexical_cast;
using boost::bad_lexical_cast;


ClassImp(CbmStsDbQa);


static FairDbParRegistry<CbmStsDbQa> qa_par("StsDbQa"); 

CbmStsDbQa::CbmStsDbQa(const char* name, const char* title, const char* context, Bool_t own)
  : FairDbParSet(name,title,context, own),
    fSUID(new TObjArray()),
    fSensors(new TObjArray()),
    fIvMeas(new TObjArray()),
    fChannelMap(new TObjArray()),
    fGeometry(new TObjArray()),
    fOwnership(new TObjArray()),
    fResults(new TObjArray()),
    fReader_Sensor(NULL),
    fReader_Iv(NULL),
    fReader_ChannelMap(NULL),
    fReader_Geometry(NULL),
    fReader_Ownership(NULL),
    fReader_Results(NULL),
    fLastIndexSUID(-1)
{
  // Set the default Db Entry to the first slot
  SetDbEntry(0);  
  // Aggregat
  SetComboNo(0); 
  // Set Version 0 
  SetVersion(0);
}


CbmStsDbQa::~CbmStsDbQa()
{
  if (fSUID) delete fSUID;
  if (fSensors) delete fSensors;
  if (fIvMeas)  delete fIvMeas;
  if (fChannelMap) delete fChannelMap;
  if (fGeometry) delete fGeometry;
  if (fOwnership) delete fOwnership;
  if (fResults) delete fResults;


  // Delete the reader templates
  if (fReader_Sensor) delete fReader_Sensor;
  if (fReader_Iv) delete fReader_Iv;
  if (fReader_ChannelMap) delete fReader_ChannelMap;
  if (fReader_Geometry) delete fReader_Geometry;
  if (fReader_Ownership) delete fReader_Ownership;
  if (fReader_Results)  delete fReader_Results;


}


void CbmStsDbQa::putParams(FairParamList* list)
{
  std::cout<<"-I- CbmStsDbQa::putParams() called"<<std::endl;
  if(!list) { return; }
  list->addObject("suid"      ,    fSUID);
  list->addObject("sensors"   ,    fSensors);
  list->addObject("iv_meas "  ,    fIvMeas);
  // list->addObject("channel_map", fChannelMap);
  // list->addObject("geometry",    fGeometry);
  //list->addObject("ownership",   fOwnership);
  //list->addObject("results",     fResults);
}

Bool_t CbmStsDbQa::getParams(FairParamList* list)
{
  if(!list) { return kFALSE;}
  if (!list->fillObject("suid"   , fSUID   )) {return kFALSE;}
  if (!list->fillObject("sensors", fSensors)) {return kFALSE;}
  if (!list->fillObject("iv_meas", fIvMeas ))  {return kFALSE;}
  //if (!list->fillObject("channel_map", fChannelMap))  {return kFALSE;}
  // if (!list->fillObject("geometry", fGeometry))  {return kFALSE;}
  //if (!list->fillObject("ownership", fOwnership))  {return kFALSE;}
  //if (!list->fillObject("results", fResults))  {return kFALSE;}
  return kTRUE;
}

void CbmStsDbQa::clear()
{
}

void CbmStsDbQa::fill(UInt_t rid)
{
  // Fill the lists with correspondin TimeStamps (runID)   
  cout << "-I- CbmStsDbQa::fill() called with RID# " << rid << endl; 

  // Define a Global Context and activate reader
  ValTimeStamp ts(rid);
  ValCondition context(FairDbDetector::kSts,DataType::kData,ts);


  // First fill the UID array
  CbmStsDbQaSUID  suid;
  fReader_SUID = suid.GetParamReader();  
  fReader_SUID->Activate(context);

  // Get the rows 
  Int_t numRes = fReader_SUID->GetNumRows();
  //cout << "-I- CbmStsDbQa::Fill() -  found nRows: " << numRes << endl;

  // Clear the map indexes
  fMapIndex.clear();

  if(numRes>0){  
    for (int i = 0; i <= numRes; ++i) {
      CbmStsDbQaSUID* suid_par = (CbmStsDbQaSUID*) fReader_SUID->GetRowByIndex(i);
      if (!suid_par) { continue; }
      fSUID->AddAtAndExpand(suid_par, suid_par->GetCompId());
      //cout << "-I- CbmStsDbQa::Fill() -  add SUID object  at -----> " << suid_par->GetCompId() << endl;
      // Add Index in map 
      fMapIndex[suid_par->CreateStringUID()] = suid_par->GetCompId();
    }

    //cout << "-I- CbmStsDbQa::Fill() read n_suid# " << fSUID->GetEntriesFast()
    //     <<  " SUID objects from DB, last_index# " << fSUID->GetLast() << endl; 
    fLastIndexSUID = fSUID->GetLast();
  } else {
    cout << "-E- CbmStsQa::Fill() no SUID object found aborting !... " << endl;
    exit(0);
  }  
  
  // Fill the Sensors array 
  CbmStsDbQaSensor mpar;
  fReader_Sensor = mpar.GetParamReader();  
  fReader_Sensor->Activate(context);

  Int_t numSensors = fReader_Sensor->GetNumRows(); 
  for (int i = 0; i <=numSensors; ++i) {
    CbmStsDbQaSensor* sensor_par = (CbmStsDbQaSensor*) fReader_Sensor->GetRowByIndex(i);
    if (!sensor_par) { continue; }
    // !!!Change to addat !!!
    fSensors->AddAtAndExpand(sensor_par, sensor_par->GetCompId());
    //cout << "-I- CbmStsQa::fill() Sensor at -----> " << sensor_par->GetCompId() << endl;
  }

  // IvMeas List
  CbmStsDbQaIv ivpar;
  fReader_Iv = ivpar.GetParamReader();  
  // Activate reading from previous Context
  fReader_Iv->Activate(context, GetVersion());

  Int_t numIvMeas = fReader_Iv->GetNumRows(); 
  for (int i = 0; i <= numIvMeas; ++i) {
    CbmStsDbQaIv* iv_par = (CbmStsDbQaIv*) fReader_Iv->GetRowByIndex(i);
    if (!iv_par) { continue; }
    // !!!Change to addat !!!
    fIvMeas->AddAtAndExpand(iv_par, iv_par->GetCompId());
    //cout << "-I-CbmStsQa::fill()  Iv-Meas at -----> " << iv_par->GetCompId() << endl;
  }


}


void CbmStsDbQa::store(UInt_t rid)
{
  //<DB> store the lists
  cout << "-I- CbmStsDbQa::store() called with RID# " << rid << endl; 

  FairDbWriter<CbmStsDbQaSensor> *sW=nullptr;
  
  // List of Sensor Qa Objects
  for(Int_t i=0;i<fSensors->GetEntriesFast();i++){
    CbmStsDbQaSensor* s_par = (CbmStsDbQaSensor*) fSensors->At(i);
    if (s_par){
      sW = s_par->ActivateWriter();
      *sW << *s_par;
      if(!sW->Close()){
          cout << " -W- CbmStsDbQa:: IO Error writing  CbmStsDbQaSensor parameters ******** "<< endl;  
        }
    }
  } //! for()

  FairDbWriter<CbmStsDbQaIv> *iW=nullptr;
  
  // List of Sensor Qa Objects
  for(Int_t i=0;i<fIvMeas->GetEntriesFast();i++){
    CbmStsDbQaIv* iv_par = (CbmStsDbQaIv*) fIvMeas->At(i);
    if (iv_par){
      iW = iv_par->ActivateWriter();
      *iW << *iv_par;
      if(!iW->Close()){
        cout << " -W- CbmStsDbQa:: IO Error writing  CbmStsDbQaIv parameters ******** "<< endl;  
      }
    }
  } //! for()

  
}

void CbmStsDbQa::Print()
{
  std::cout<<" -----------  STS Qa Parameters -------------  "<<std::endl;

  cout << "-----------------------    SUID Summary -------------------------------" << endl;
  for(Int_t i=0;i<fSUID->GetEntriesFast(); i++)
    {
      CbmStsDbQaSUID* suid_par = (CbmStsDbQaSUID*) fSUID->At(i);  
      if (suid_par) suid_par->Print();
    }
  
  cout << "-----------------------    Sensors Summary -------------------------------" << endl;
  for(Int_t i=0;i<fSensors->GetEntriesFast(); i++)
    {
      CbmStsDbQaSensor* s_par = (CbmStsDbQaSensor*) fSensors->At(i);  
      if (s_par) s_par->Print();
    }

  cout << "-----------------------    Sensors Summary -------------------------------" << endl;
  for(Int_t i=0;i<fIvMeas->GetEntriesFast(); i++)
    {
      CbmStsDbQaIv* iv_par = (CbmStsDbQaIv*) fIvMeas->At(i);  
      if (iv_par) iv_par->Print();
    }
  
}

void CbmStsDbQa::LoadSUIDFromDB(){
  
  // Create a unique statement on choosen DB entry
  FairDbConnectionPool* cPool = FairDbTableInterfaceStore::Instance().fConnectionPool;
  auto_ptr<FairDbStatement> stmtDbn(cPool->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    cout << "-E-  CbmStsDbQa::LoadSUIDFromDB()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  // If table does not exist set compId index to -1 
  if (! cPool->GetConnection(GetDbEntry())->TableExists("STSQASUID") ) {
    fLastIndexSUID=-1;
    return;
  } 

  // Load the SUID
  CbmStsDbQaSUID  suid;
  fReader_SUID = suid.GetParamReader();  

  // Define a Global Context and activate reader
  ValTimeStamp ts;
  ValCondition context(FairDbDetector::kSts,DataType::kData,ts);
  fReader_SUID->Activate(context, 0);

  // Get the rows 
  Int_t numRes = fReader_SUID->GetNumRows();
  //cout << "-I- CbmStsDbQa: found nRows: " << numRes << endl;

  if(numRes>0){  
    for (int i = 0; i <= numRes; ++i) {
      CbmStsDbQaSUID* suid_par = (CbmStsDbQaSUID*) fReader_SUID->GetRowByIndex(i);
      if (!suid_par) { continue; }
      fSUID->AddAtAndExpand(suid_par, suid_par->GetCompId());
      //cout << "-I- CbmStsDbQa: add SUID object  at -----> " << suid_par->GetCompId() << endl;
      // Add Index in map 
      fMapIndex[suid_par->CreateStringUID()] = suid_par->GetCompId();
    }
    
    cout << "-I- CbmStsDbQa: read n_suid# " << fSUID->GetEntriesFast()
         <<  " SUID objects from DB, last_index# " << fSUID->GetLast() << endl; 
    fLastIndexSUID = fSUID->GetLast();
  }else{
    fLastIndexSUID = -1;  
  }
    
}



void CbmStsDbQa::GenerateSUIDFromCsvFile(const string& fname){
  //
  // Function to generate UID from Qa Overview table
  //
  
  // First Read from Db to get all already assigned SUID.
  LoadSUIDFromDB(); 

  std::ifstream file(fname.c_str());
  vector< vector<string> > csv_values;
  
  typedef boost::tokenizer< boost::char_separator<char> > Tokenizer;
  boost::char_separator<char> sep(";"); 
  
  while(file)
    {
      string line;
      
      if (!(safeGetline(file,line).eof()) && !(line.empty())) 
        if (file)
          {
            //cout << " -----> Get a  Line---->  " << line << endl;
            Tokenizer info(line, sep);   
            vector<string> values;
            for (Tokenizer::iterator it = info.begin(); it != info.end(); ++it)
              {
                //cout <<" tokens <  " << *it <<  " > " <<  endl;
                // convert data into string value, and store
                string it_str ( *it );
                values.push_back(it_str);
              }

            // store array of values
            //cout << " pushing in csv value vector---> " << csv_values.size() << endl;    
            csv_values.push_back(values);
          }
    }
  
  // display results
  cout.precision(1);
  cout.setf(ios::fixed,ios::floatfield);

  // Initiate the sensor UID to the last index in array
  Int_t sensor_uid=fLastIndexSUID;
  
  for (vector< vector<string> >::const_iterator it = csv_values.begin()+1; it != csv_values.end(); ++it)
    {
      const vector<string>& values = *it;
      
      // Create the Sensor UID    
      CbmStsDbQaSUID *suid = new CbmStsDbQaSUID();
      suid->SetVendor(values[4]);
      suid->SetBatchId(values[1]);
      suid->SetBatchTime(values[9]);      
      suid->SetWaferId(lexical_cast<int>(values[2]));
      suid->SetSensorId(lexical_cast<int>(values[3]));
      string str_uid = suid->CreateStringUID();
      suid->SetCompId(++sensor_uid);

      // Test if SUID already exist in memory   
      if ( TObject* f_obj= fSUID->FindObject (suid) ) {
        CbmStsDbQaSUID *f_suid = (CbmStsDbQaSUID*) f_obj; 
        cout << "-W- CbmStsDbQa: Already defined SUID object  ["<< str_uid<<"], corresponding compId#  "
             << f_suid->GetCompId() << " skipping ..." << endl;
        continue; 
      }

      // Add new object locally in memory 
      AddSUIDAt(suid, suid->GetCompId());
      
      // map new object corresponding index
      fMapIndex[suid->CreateStringUID()] = suid->GetCompId();
      
      // Create Writer templater for this suid object
      FairDbWriter<CbmStsDbQaSUID>* resW = suid->ActivateWriter();
      
      // Store in DB backend     
      if (resW) 
	{
          if (suid) *resW << *suid; 
	}//! (resW)
      else {
	cout << " -W- CbmStsDbQa:: Writer for CbmStsDbQaSUID not created "<< endl;  
        break;
      }
      
      // Now Commit in DB
      if(!resW->Close())
        {
          cout << " -W- CbmStsDbQa:: IO Error writing  SUID objects in DB Store ******** "<< endl;  
        }
   
    }//! (for)
  
  cout << "-I- CbmStsDbQa::  n_suid# " << GetNumSUID()  << " imported SUID objects " << endl;
}


void CbmStsDbQa::ImportFromCsvFile(const string& fname)
{
  std::ifstream file(fname.c_str());
  vector< vector<string> > csv_values;
  
  typedef boost::tokenizer< boost::char_separator<char> > Tokenizer;
  boost::char_separator<char> sep(";"); 
  
  while(file)
    {
      string line;
      
      if (!(safeGetline(file,line).eof()) && !(line.empty())) 
        if (file)
          {
	  //cout << " -----> Get a  Line---->  " << line << endl;
            Tokenizer info(line, sep);   
            vector<string> values;
            for (Tokenizer::iterator it = info.begin(); it != info.end(); ++it)
              {
                cout <<" tokens <  " << *it <<  " > " <<  endl;
                // convert data into string value, and store
                string it_str ( *it );
                values.push_back(it_str);
              }

            // store array of values
            cout << " pushing in csv value vector---> " << csv_values.size() << endl;    
            csv_values.push_back(values);
          }
    }
  
  // display results
  cout.precision(1);
  cout.setf(ios::fixed,ios::floatfield);
  
  for (vector< vector<string> >::const_iterator it = csv_values.begin()+1; it != csv_values.end(); ++it)
    {
      const vector<string>& values = *it;
      
      // Get the Sensor UID
      string v_id = lexical_cast<string>(values[4]); 
      string b_id = lexical_cast<string>(values[1]); 
      int w_id    = lexical_cast<int>(values[2]);
      int s_id    = lexical_cast<int>(values[3]);

      std::ostringstream str_id;
      str_id << v_id << "_" << b_id << "_" << w_id << "_" << s_id;   
      string res_id = str_id.str();
      int index = fMapIndex[res_id.c_str()];
      //cout << " ----> UID ---> " << res_id <<  " index " << index << endl;
                
      
      if (fname.find("overview") != std::string::npos){ 
        CbmStsDbQaSensor *par = new CbmStsDbQaSensor();
        Bool_t adding = par->Import(index, values); 
        Bool_t e_end = std::next(it) == csv_values.end();
        if(adding || e_end ) {
          std::cout << "-I- CbmStsDbQa:: adding new object QaSensor with comp_UID# " << par->GetCompId() << endl;  
          fSensors->AddAtAndExpand(par,par->GetCompId());
        }  
       }
    }//!for
  
  cout << " FSensors array  entries ----- >" << fSensors->GetEntries() << endl;	
  return;
}


void CbmStsDbQa::ImportIvFilesFromDir(const string& dirname)
{
   
  path p (dirname.data());
 
  directory_iterator end_itr;
 
  for (directory_iterator itr(p); itr != end_itr; ++itr)
    {
      // Look only for directory
      if (is_regular_file(itr->path())) continue;
        
        // if found directory 
        if(is_directory(itr->status())){
          cout <<"-I- CbmStsQa::ImportIvFilesFromDir(): found iv-data directory:\n";
          typedef vector<path> vec;            
          vec v;
          copy(directory_iterator(p), directory_iterator(), back_inserter(v));
          
          for (vec::const_iterator it (v.begin()); it != v.end(); ++it)
            {
              string dname = (*it).string();
              if (dname.find("iv") != std::string::npos){ 
                cout << "   " << (*it).string() << '\n';
                LoadDataFromIvDir( dname );
              }
            }        
        }
      }
    
    /*
   cout << " fIvMeas entries:" << fIvMeas->GetSize() << endl; 
   for(Int_t i=0; i<fIvMeas->GetSize();++i)
   {
     CbmStsDbQaIvNewPar* par = (CbmStsDbQaIvNewPar*) fIvMeas->At(i);
     if (par) {par->Print();} 
   }
   */ 

  return;  
}   

void CbmStsDbQa::LoadDataFromIvDir(const string& dirname)
{
  // <CHECK ME ! > this part is directory layout dependent
  // find vendor from directory name
  std::string vendor;
  int batch_id=-1;
  
  if (dirname.find("cis") != std::string::npos) vendor = "CiS";
    else vendor = "hpc";
  
  boost::regex re("([0-9]{6,8})");
  boost::sregex_iterator m_itr(dirname.begin(), dirname.end(), re);
  boost::sregex_iterator m_end;

  // <CHECK ME> Get the batch_id
  // (should be only one because of specific regexp)
  int s_id[1]={-1};
  int ii=0; 
  for( ; m_itr != m_end; ++m_itr)
    {
      cout << "-I- CbmStsQa::LoadDataFromIvDir(): vendor: " <<  vendor << " batch: " << m_itr->str() << endl; 
      batch_id = lexical_cast<int>(m_itr->str()); 
      s_id[ii++] = batch_id;  
    }
  
  // Now Lookup directory
  path p (dirname.data());
  directory_iterator end_itr;
  for (directory_iterator itr(p); itr != end_itr; ++itr)
    {
      if (is_regular_file(itr->path())) {
        string current_file = itr->path().string();

        LoadDataFromIvFile(vendor, batch_id,  current_file);                         
      }
    }
}


void CbmStsDbQa::LoadDataFromIvFile(const string& vendor, int& b_id, const string& fname)
{

  string bname = remove_ext(basename(fname));
  cout << "-I- CbmStsQa::LoadDataFromIvDir: Analysing file ----> " << bname << endl;

  boost::regex re("([0-9]+)");
  boost::sregex_iterator m_itr(bname.begin(), bname.end(), re);
  boost::sregex_iterator m_end;

  int wafer_id=-1;
  int sensor_id=-1;
  // Get the UID, Wafer_id form file name 
  int s_id[2]={0,0};
  int ii=0; 
  for( ; m_itr != m_end; ++m_itr)
    {
      //cout << "-I- lexical_cast call for int conversion: " << m_itr->str() << endl; 
      s_id[ii++] = lexical_cast<int>(m_itr->str());  
    }
  wafer_id = s_id[0];
  sensor_id = s_id[1];
  
  // Now found the corresponding ID
  if (fMapIndex.empty() ) LoadSUIDFromDB();
  std::ostringstream str_id;
  str_id << vendor << "_" << b_id << "_" << wafer_id << "_" << sensor_id; 

  int compid = fMapIndex[str_id.str()];
  cout << " SUID--->  key: " << str_id.str() << " uid: " << compid << endl;
  
  // Load Measurement
   std::ifstream infile(fname.c_str());   
  if(infile.fail()) // checks to see if file opended 
    { 
       cout << "-E- CbmStsDbQa::LoadDataFromIvFile error opening iv file:  " <<  fname << endl; 
       return;
    } 

  double a[MAX_MEAS+1];
  double b[MAX_MEAS+1]; 
  int n_meas = 0;
  while (!infile.eof())
   {  
    
    if (infile && n_meas <= MAX_MEAS)
      {   
        a[n_meas]= -1;
        b[n_meas]= -1.;            
        infile >> a[n_meas] >> b[n_meas];
        cout << " a = " << a[n_meas] << setprecision(5) << " b= " << b[n_meas] <<  " meas# " << n_meas << endl;
        ++n_meas;
      }
      else break;
   }

  // Create Container and add it to the list
  CbmStsDbQaIv * par = new CbmStsDbQaIv();
  par->SetCompId(compid);
  par->SetUID(compid);
  par->SetFileName(fname);
  par->SetN(n_meas);
  par->SetI(a);
  par->SetV(b);
  
  fIvMeas->AddAtAndExpand(par,par->GetCompId());
 
 return;
} 

std::istream& CbmStsDbQa::safeGetline(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case EOF:
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}


std::string CbmStsDbQa::basename( std::string const& pathname )
{
  return std::string( 
                     std::find_if( pathname.rbegin(), pathname.rend(), MatchPathSeparator() ).base(),
                     pathname.end() );
}

std::string CbmStsDbQa::remove_ext( std::string const& filename )
{
  std::string::const_reverse_iterator
    pivot
    = std::find( filename.rbegin(), filename.rend(), '.' );
  return pivot == filename.rend()
    ? filename
    : std::string( filename.begin(), pivot.base() - 1 );
}


Int_t CbmStsDbQa::GetSUID(string vendor, string batch, Int_t wafer, Int_t sensor)
{
  Int_t not_found = 1;
  
  if (fSUID->IsEmpty()) LoadSUIDFromDB();
  for (Int_t i=0; i<fSUID->GetEntriesFast(); i++)
    {
      CbmStsDbQaSUID* suid = (CbmStsDbQaSUID*) (fSUID->At(i));
      if ( suid )
        {
          if
            ( 
             (strcmp(suid->GetVendor().c_str(), vendor.c_str()) == 0)
             &&
             (strcmp(suid->GetBatchId().c_str(), batch.c_str()) == 0)
             &&
             (suid->GetWaferId() == wafer)
             &&
             (suid->GetSensorId() ==sensor)
              ) return suid->GetCompId();
          else continue;   
        }  
    }
  cout << "-I- CbmStsDbQa::GetSUID() :  cannot found the SUID in the database list"<< endl; 
  return not_found;
}
