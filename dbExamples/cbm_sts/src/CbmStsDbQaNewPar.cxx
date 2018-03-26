#include "CbmStsDbQaNewPar.h"

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


ClassImp(CbmStsDbQaNewPar);


static FairDbParRegistry<CbmStsDbQaNewPar> qa_par("StsQaPar"); 

CbmStsDbQaNewPar::CbmStsDbQaNewPar(const char* name, const char* title, const char* context, Bool_t own)
  : FairDbParSet(name,title,context, own),
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
    fReader_Results(NULL)
{
}


CbmStsDbQaNewPar::~CbmStsDbQaNewPar()
{
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


void CbmStsDbQaNewPar::putParams(FairParamList* list)
{
  std::cout<<"-I- CbmStsDbQaNewPar::putParams() called"<<std::endl;
  if(!list) { return; }
  list->addObject("sensors",     fSensors);
  list->addObject("iv_meas ",    fIvMeas);
  // list->addObject("channel_map", fChannelMap);
  // list->addObject("geometry",    fGeometry);
  //list->addObject("ownership",   fOwnership);
  //list->addObject("results",     fResults);
}

Bool_t CbmStsDbQaNewPar::getParams(FairParamList* list)
{
  if(!list) { return kFALSE;}
  if (!list->fillObject("sensors", fSensors)) {return kFALSE;}
  if (!list->fillObject("iv_meas", fIvMeas))  {return kFALSE;}
  //if (!list->fillObject("channel_map", fChannelMap))  {return kFALSE;}
  // if (!list->fillObject("geometry", fGeometry))  {return kFALSE;}
  //if (!list->fillObject("ownership", fOwnership))  {return kFALSE;}
  //if (!list->fillObject("results", fResults))  {return kFALSE;}
  return kTRUE;
}

void CbmStsDbQaNewPar::clear()
{
}

void CbmStsDbQaNewPar::fill(UInt_t rid)
{
  // Fill the lists with correspondin TimeStamps (runID)   
  cout << "-I- CbmStsDbQaNewPar::fill() called with RID# " << rid << endl; 

  // Sensors List
  CbmStsDbQaSensorNewPar mpar;
  fReader_Sensor = mpar.GetParamReader();  

  // Define a Global Context
  ValTimeStamp ts(rid);
  ValCondition context(FairDbDetector::kSts,DataType::kData,ts);

  // Activate reading for this Context
  fReader_Sensor->Activate(context, GetVersion());

  Int_t numSensors = fReader_Sensor->GetNumRows(); 
  for (int i = 0; i <=numSensors; ++i) {
    CbmStsDbQaSensorNewPar* sensor_par = (CbmStsDbQaSensorNewPar*) fReader_Sensor->GetRow(i);
    if (!sensor_par) { continue; }
    // !!!Change to addat !!!
    fSensors->AddAtAndExpand(sensor_par, sensor_par->GetUID());
    cout << " fill fSensor at -----> " << sensor_par->GetUID() << endl;
  }
 
  // IvMeas List
  CbmStsDbQaIvNewPar ivpar;
  fReader_Iv = ivpar.GetParamReader();  
  // Activate reading from previous Context
  fReader_Iv->Activate(context, GetVersion());

  Int_t numIvMeas = fReader_Iv->GetNumRows(); 
  for (int i = 0; i <= numIvMeas; ++i) {
    CbmStsDbQaIvNewPar* iv_par = (CbmStsDbQaIvNewPar*) fReader_Iv->GetRow(i);
    if (!iv_par) { continue; }
    // !!!Change to addat !!!
    fIvMeas->AddAtAndExpand(iv_par, iv_par->GetUID());
    cout << " fill  fIvMeas at -----> " << iv_par->GetUID() << endl;
  }

 // Channel Map List
  CbmStsDbQaChannelMapPar chpar;
  fReader_ChannelMap = chpar.GetParamReader();  
  // Activate reading from previous Context
  fReader_ChannelMap->Activate(context, GetVersion());

  Int_t numChannelMap = fReader_ChannelMap->GetNumRows(); 
  for (int i = 0; i <= numChannelMap; ++i) {
    CbmStsDbQaChannelMapPar* ch_par = (CbmStsDbQaChannelMapPar*) fReader_ChannelMap->GetRow(i);
    if (!ch_par) { continue; }
    // !!!Change to addat !!!
    fChannelMap->AddAtAndExpand(ch_par, ch_par->GetUID());
    cout << " fill  fChannelMap at -----> " << ch_par->GetUID() << endl;
  }

 // Geometry List
  CbmStsDbQaGeometryPar geopar;
  fReader_Geometry = geopar.GetParamReader();  
  // Activate reading from previous Context
  fReader_Geometry->Activate(context, GetVersion());

  Int_t numGeo = fReader_Geometry->GetNumRows(); 
  for (int i = 0; i <= numGeo; ++i) {
    CbmStsDbQaGeometryPar* g_par = (CbmStsDbQaGeometryPar*) fReader_Geometry->GetRow(i);
    if (!g_par) { continue; }
    // !!!Change to addat !!!
    fGeometry->AddAtAndExpand(g_par, g_par->GetUID());
    cout << " fill  fGeometry at -----> " << g_par->GetUID() << endl;
  }


 // OwnerShip List
  CbmStsDbQaOwnershipPar ownpar;
  fReader_Ownership = ownpar.GetParamReader();  
  // Activate reading from previous Context
  fReader_Ownership->Activate(context, GetVersion());

  Int_t numOwner = fReader_Ownership->GetNumRows(); 
  for (int i = 0; i <= numOwner; ++i) {
    CbmStsDbQaOwnershipPar* o_par = (CbmStsDbQaOwnershipPar*) fReader_Ownership->GetRow(i);
    if (!o_par) { continue; }
    // !!!Change to addat !!!
    fOwnership->AddAtAndExpand(o_par, o_par->GetUID());
    cout << " fill  fOwnership at -----> " << o_par->GetUID() << endl;
  }

  // Results List
  CbmStsDbQaResultsPar respar;
  fReader_Results = respar.GetParamReader();  
  // Activate reading from previous Context
  fReader_Results->Activate(context, GetVersion());

  Int_t numRes = fReader_Results->GetNumRows(); 
  for (int i = 0; i <= numRes; ++i) {
    CbmStsDbQaResultsPar* res_par = (CbmStsDbQaResultsPar*) fReader_Results->GetRow(i);
    if (!res_par) { continue; }
    // !!!Change to addat !!!
    fResults->AddAtAndExpand(res_par, res_par->GetUID());
    cout << " fill  fResults at -----> " << res_par->GetUID() << endl;
  }


}


void CbmStsDbQaNewPar::store(UInt_t rid)
{
  //<DB> store the lists
  cout << "-I- CbmStsDbQaNewPar::store() called with RID# " << rid << endl; 
  
  // CbmStsDbQaSensorNewPar stored row-wized
  CbmStsDbQaSensorNewPar sPar;
  FairDbWriter<CbmStsDbQaSensorNewPar>* sW = sPar.ActivateWriter(rid); 
  
  if (sW) 
	{
      // List of Qa Objects
      for(Int_t i=0;i<fSensors->GetSize();i++)
		{
		  CbmStsDbQaSensorNewPar* s_par = (CbmStsDbQaSensorNewPar*) fSensors->At(i);
		  if (s_par) *sW << *s_par; 
		}
	  
	  // Reset the Writer
	  if(!sW->Close())
		{
		  cout << " -W- CbmStsDbQaNewPar:: IO Error writing  sensor parameters ******** "<< endl;  
		}
	  
	}//! (aWriter)
  else {
	cout << " -W- CbmStsDbQaNewPar:: Writer for CbmStsDbQaSensorNewPar not created "<< endl;  
  }
  
  
  // CbmStsDbQaIvNewPar stored row-wized
  CbmStsDbQaIvNewPar ivPar;
  FairDbWriter<CbmStsDbQaIvNewPar>* iW = ivPar.ActivateWriter(rid); 
  
  if (iW) 
	{
      // List of Qa Objects
      for(Int_t i=0;i<fIvMeas->GetSize();i++)
		{
		  CbmStsDbQaIvNewPar* iv_par = (CbmStsDbQaIvNewPar*) fIvMeas->At(i);
		  if (iv_par) *iW << *iv_par; 
		}
	  
	  // Reset the Writer
	  if(!iW->Close())
		{
		  cout << " -W- CbmStsDbQaNewPar:: IO Error writing  IV meas. parameters ******** "<< endl;  
		}
	  
	}//! (aWriter)
  else {
	cout << " -W- CbmStsDbQaNewPar:: Writer for CbmStsDbQaIvNewPar not created "<< endl;  
  }
  
  // CbmStsDbQaChannelMapPar stored row-wized
  CbmStsDbQaChannelMapPar chPar;
  FairDbWriter<CbmStsDbQaChannelMapPar>* chW = chPar.ActivateWriter(rid); 
  
  if (chW) 
	{
      // List of Qa Objects
      for(Int_t i=0;i<fChannelMap->GetSize();i++)
		{
		  CbmStsDbQaChannelMapPar* ch_par = (CbmStsDbQaChannelMapPar*) fChannelMap->At(i);
		  if (ch_par) *chW << *ch_par; 
		}
	  
	  // Reset the Writer
	  if(!chW->Close())
		{
		  cout << " -W- CbmStsDbQaNewPar:: IO Error writing  ChannelMap. parameters ******** "<< endl;  
		}
	  
	}//! (aWriter)
  else {
	cout << " -W- CbmStsDbQaNewPar:: Writer for CbmStsDbQaChannelMap not created "<< endl;  
  }
  
  
  // CbmStsDbQaGeometryPar stored row-wized
  CbmStsDbQaGeometryPar geoPar;
  FairDbWriter<CbmStsDbQaGeometryPar>* gW = geoPar.ActivateWriter(rid); 
  
  if (gW) 
	{
      // List of Qa Objects
      for(Int_t i=0;i<fGeometry->GetSize();i++)
		{
		  CbmStsDbQaGeometryPar* geo_par = (CbmStsDbQaGeometryPar*) fGeometry->At(i);
		  if (geo_par) *gW << *geo_par; 
		}
	  
	  // Reset the Writer
	  if(!gW->Close())
		{
		  cout << " -W- CbmStsDbQaNewPar:: IO Error writing  Geometry parameters ******** "<< endl;  
		}
	  
	}//! (aWriter)
  else {
	cout << " -W- CbmStsDbQaNewPar:: Writer for CbmStsDbQaGeometry not created "<< endl;  
  }
  
  
  // CbmStsDbQaOwnershipPar stored row-wized
  CbmStsDbQaOwnershipPar oPar;
  FairDbWriter<CbmStsDbQaOwnershipPar>* oW = oPar.ActivateWriter(rid); 
  
  if (oW) 
	{
      // List of Qa Objects
      for(Int_t i=0;i<fOwnership->GetSize();i++)
		{
		  CbmStsDbQaOwnershipPar* o_par = (CbmStsDbQaOwnershipPar*) fOwnership->At(i);
		  if (o_par) *oW << *o_par; 
		}
	  
	  // Reset the Writer
	  if(!oW->Close())
		{
		  cout << " -W- CbmStsDbQaNewPar:: IO Error writing  Ownership parameters ******** "<< endl;  
		}
	  
	}//! (aWriter)
  else {
	cout << " -W- CbmStsDbQaNewPar:: Writer for CbmStsDbQaOwnership not created "<< endl;  
  }
  
  
  
 // CbmStsDbQaResultsPar stored row-wized
  CbmStsDbQaResultsPar resPar;
  FairDbWriter<CbmStsDbQaResultsPar>* resW = resPar.ActivateWriter(rid); 
  
  if (resW) 
	{
      // List of Qa Objects
      for(Int_t i=0;i<fResults->GetSize();i++)
		{
		  CbmStsDbQaResultsPar* res_par = (CbmStsDbQaResultsPar*) fResults->At(i);
		  if (res_par) *resW << *res_par; 
		}
	  
	  // Reset the Writer
	  if(!resW->Close())
		{
		  cout << " -W- CbmStsDbQaNewPar:: IO Error writing  Results parameters ******** "<< endl;  
		}
	  
	}//! (aWriter)
  else {
	cout << " -W- CbmStsDbQaNewPar:: Writer for CbmStsDbQaResults not created "<< endl;  
  }

}

void CbmStsDbQaNewPar::Print()
{

  std::cout<<" -----------  STS Qa Parameters -------------  "<<std::endl;

  
  cout << "-----------------------    Sensors Summary -------------------------------" << endl;
  for(Int_t i=0;i<fSensors->GetSize(); i++)
  {
    CbmStsDbQaSensorNewPar* s_par = (CbmStsDbQaSensorNewPar*) fSensors->At(i);  
  
    if (s_par) s_par->Print();
  }

  cout << "-------------------------  Iv Curves Measurements ------------------------------" << endl;
  for(Int_t i=0;i<fIvMeas->GetSize(); i++)
  {
    CbmStsDbQaIvNewPar* iv_par = (CbmStsDbQaIvNewPar*) fIvMeas->At(i);  

    if (iv_par) iv_par->Print();
  }

  cout << "------------------------   Geometry                ------------------------" << endl;
  for(Int_t i=0;i<fGeometry->GetSize(); i++)
  {
    CbmStsDbQaGeometryPar* geo_par = (CbmStsDbQaGeometryPar*) fGeometry->At(i);  

    if (geo_par) geo_par->Print();
  }

  cout << "------------------------   Ownership                ------------------------" << endl;
  for(Int_t i=0;i<fOwnership->GetSize(); i++)
  {
    CbmStsDbQaOwnershipPar* own_par = (CbmStsDbQaOwnershipPar*) fOwnership->At(i);  

    if (own_par) own_par->Print();
  }

  cout << "------------------------   Results                ------------------------" << endl;
  for(Int_t i=0;i<fResults->GetSize(); i++)
  {
    CbmStsDbQaResultsPar* res_par = (CbmStsDbQaResultsPar*) fResults->At(i);  

    if (res_par) res_par->Print();
  }

  cout << "------------------------   Channel Map                ------------------------" << endl;
  for(Int_t i=0;i<fChannelMap->GetSize(); i++)
  {
    CbmStsDbQaChannelMapPar* ch_par = (CbmStsDbQaChannelMapPar*) fChannelMap->At(i);  

    if (ch_par) ch_par->Print();
  }

}

void CbmStsDbQaNewPar::ImportFromCsvFile(const string& fname)
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
  
  int suid = -1;
  Bool_t same_uid=kFALSE;
  vector<CbmStsDbQaChannelMapPar*> schannelmap;
	vector<CbmStsDbQaChannelMapPar*>::iterator ch_itr = schannelmap.begin();     
        
        for (vector< vector<string> >::const_iterator it = csv_values.begin()+1; it != csv_values.end(); ++it)
          {
            const vector<string>& values = *it;
            
            // Get the Sensor UID    
            int current_uid = lexical_cast<int>(values[0]); 
            
            // Check if UID change
            if (suid != current_uid ) same_uid=kFALSE;
            else same_uid = kTRUE;
            suid=current_uid;
            
            // Set the index
            int index = suid-1;
            
            if (fname.find("summary") != std::string::npos){ 
              CbmStsDbQaSensorNewPar *par = new CbmStsDbQaSensorNewPar();
              Bool_t adding = par->Import(values); 
              Bool_t e_end = std::next(it) == csv_values.end();
              if(adding || e_end ) {
                std::cout << "-I- CbmStsDbQaNewPar:: adding new object QaSensorNewPar UID# " << par->GetUID() << endl;  
                fSensors->AddAtAndExpand(par,par->GetUID());
              }  
            }else if (fname.find("geometry") != std::string::npos){
              // for geometry
              std::cout << "-I- CbmStsDbQaNewPar:: reading geometry ... " << endl;
              CbmStsDbQaGeometryPar *par = new CbmStsDbQaGeometryPar();
              Bool_t adding = par->Import(values); 
              Bool_t e_end = std::next(it) == csv_values.end();
              if(adding || e_end ) {
                std::cout << "-I- CbmStsDbQaNewPar:: adding new object Qa Geometry UID# " << par->GetUID() << endl;  
                fGeometry->AddAtAndExpand(par,par->GetUID());
              }  
              
            }else if (fname.find("ownership") != std::string::npos){
              // for ownership
              std::cout << "-I- CbmStsDbQaNewPar:: reading ownership ... " << endl;
              CbmStsDbQaOwnershipPar *par = new CbmStsDbQaOwnershipPar();
              Bool_t adding = par->Import(values); 
              Bool_t e_end = std::next(it) == csv_values.end();
              if(adding || e_end ) {
                std::cout << "-I- CbmStsDbQaNewPar:: adding new object Qa Ownership UID# " << par->GetUID() << endl;  
                fOwnership->AddAtAndExpand(par,par->GetUID());
              }  		
              
            }else if (fname.find("results") != std::string::npos){
              // for results
              std::cout << "-I- CbmStsDbQaNewPar:: reading results ... " << endl;
              CbmStsDbQaResultsPar *par = new CbmStsDbQaResultsPar();
              Bool_t adding = par->Import(values); 
              Bool_t e_end = std::next(it) == csv_values.end();
              if(adding || e_end ) {
                std::cout << "-I- CbmStsDbQaNewPar:: adding new object Qa Results UID# " << par->GetUID() << endl;  
                fResults->AddAtAndExpand(par,par->GetUID());
              }  		
              
            }else if (fname.find("channel") != std::string::npos){
              // for channel 
              //std::cout << "-I- CbmStsDbQaNewPar:: getting ptr suid " << suid << " index " << index << " same: " << same_uid << endl;
              
              CbmStsDbQaChannelMapPar *cpar=nullptr;
              CbmStsDbQaChannelMapPar *pre_par=nullptr;
              if (index<schannelmap.size()) cpar = schannelmap[index];
              //std::cout << "-I- CbmStsDbQaNewPar:: ptr---> " << cpar <<  endl;
              
              if (!cpar) { 
                // if (previous)  save previous object 
                if (index>0){ 
                  pre_par=schannelmap[index-1];  
                  if (pre_par) fChannelMap->AddAtAndExpand(pre_par,pre_par->GetUID());
                }
                // Or  add new object 
                cpar = new CbmStsDbQaChannelMapPar();  
                //std::cout << "-I- CbmStsDbQaNewPar:: insert ptr---> 1 " << index <<  endl;
                schannelmap.insert(schannelmap.begin()+index, cpar );
                //std::cout << "-I- CbmStsDbQaNewPar:: insert ptr---> 2" << index <<  endl;
              }
              
              Bool_t adding = cpar->Import(values); 
              Bool_t e_end = std::next(it) == csv_values.end();
              if( adding || e_end ) {
                std::cout << "-I- CbmStsDbQaNewPar:: adding new object QaChannelMapPar UID# " << cpar->GetUID() << endl;  
                fChannelMap->AddAtAndExpand(cpar,cpar->GetUID());
              }        
              
            }
          }//!for
	
	cout << " FSensors array  entries ----- >" << fSensors->GetEntries() << endl;	
	return;
}


void CbmStsDbQaNewPar::ImportIvFilesFromDir()
{
  // list all files in data directory.
    TString vmc(getenv("VMCWORKDIR"));
    TString data_path = vmc + "/../dbase/dbExamples/cbm_sts/data/sensor-iv";
   
    path p (data_path.Data());
 
    directory_iterator end_itr;
 
    for (directory_iterator itr(p); itr != end_itr; ++itr)
    {
        if (is_regular_file(itr->path())) 
        {
            string current_file = itr->path().string();
            cout << " Analysing file ----> " << current_file << endl;

             LoadDataFromIvFile(current_file);
        }
    }

   cout << " fIvMeas entries:" << fIvMeas->GetSize() << endl; 
   for(Int_t i=0; i<fIvMeas->GetSize();++i)
   {
     CbmStsDbQaIvNewPar* par = (CbmStsDbQaIvNewPar*) fIvMeas->At(i);
     if (par) {par->Print();} 
   }
 
  return;  
}   

void CbmStsDbQaNewPar::LoadDataFromIvFile(const string& fname)
{

   boost::regex re("([0-9]+)");
   boost::sregex_iterator m_itr(fname.begin(), fname.end(), re);
   boost::sregex_iterator m_end;

   // Get the UID, Wafer_id form file name 
   int s_id[2]={-1,-1};
   int ii=0; 
   for( ; m_itr != m_end; ++m_itr)
     {
        cout<< m_itr->str() << endl;
	cout << "-I- lexical_cast call for int conversion: " << m_itr->str() << endl; 
        s_id[ii++] = lexical_cast<int>(m_itr->str());  
     }
      
   //for(int i=0;i<2;i++) cout << "i = " << i << " : " << s_id[i] << endl;      


  // Load Measurement
   std::ifstream infile(fname.c_str());   
  if(infile.fail()) // checks to see if file opended 
    { 
       cout << "-E- CbmStsDbQaNewPar::LoadDataFromIvFile error opening iv file:  " <<  fname << endl; 
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
        //cout << " a = " << a[n_meas] << setprecision(5) << " b= " << b[n_meas] <<  " meas# " << n_meas << endl;
        ++n_meas;
      }
      else break;
   }

  // Create Container and add it to the list
  CbmStsDbQaIvNewPar * par = new CbmStsDbQaIvNewPar();
  par->SetUID(s_id[0]);
  par->SetFileName(fname);
  par->SetN(n_meas);
  par->SetI(a);
  par->SetV(b);
  
  fIvMeas->AddAtAndExpand(par,par->GetUID());
  
  
 return;
} 

std::istream& CbmStsDbQaNewPar::safeGetline(std::istream& is, std::string& t)
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


