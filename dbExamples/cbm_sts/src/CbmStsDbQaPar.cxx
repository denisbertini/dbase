#include "CbmStsDbQaPar.h"

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


ClassImp(CbmStsDbQaPar);


static FairDbParRegistry<CbmStsDbQaPar> qa_par("CbmStsDbQaPar"); 


CbmStsDbQaPar::CbmStsDbQaPar(const char* name, const char* title, const char* context, Bool_t own)
  : FairDbParSet(name,title,context, own),
	fSensors(new TObjArray()),
  fIvMeas(new TObjArray()),
  fReader_Sensor(NULL),
  fReader_Iv(NULL)
{
}


CbmStsDbQaPar::~CbmStsDbQaPar()
{
  if (fSensors) delete fSensors;
  if (fIvMeas)  delete fIvMeas;

  // Delete the reader templates
  if (fReader_Sensor) delete fReader_Sensor;
  if (fReader_Iv)  delete fReader_Iv;

}


void CbmStsDbQaPar::putParams(FairParamList* list)
{
  std::cout<<"-I- CbmStsDbQaPar::putParams() called"<<std::endl;
  if(!list) { return; }
  list->addObject("Sts Sensors", fSensors);
  list->addObject("Sts IvMeas ", fIvMeas);
}

Bool_t CbmStsDbQaPar::getParams(FairParamList* list)
{
  if(!list) { return kFALSE;}
  if (!list->fillObject("Sts Sensors", fSensors)) {return kFALSE;}
  if (!list->fillObject("Sts IvMeas ", fIvMeas))  {return kFALSE;}
  return kTRUE;
}

void CbmStsDbQaPar::clear()
{
}

void CbmStsDbQaPar::fill(UInt_t rid)
{
  // Fill the lists with correspondin TimeStamps (runID)   
  cout << "-I- CbmStsDbQaPar::fill() called with RID# " << rid << endl; 

  // Sensors List
  CbmStsDbQaSensorPar mpar;
  //FairDbReader<CbmStsDbQaSensorPar>* r_mpar = mpar.GetParamReader();  
  fReader_Sensor = mpar.GetParamReader();  

  // Define a Global Context
  ValTimeStamp ts(rid);
  ValCondition context(FairDbDetector::kSts,DataType::kData,ts);

  // Activate reading for this Context
  fReader_Sensor->Activate(context, GetVersion());

  Int_t numSensors = fReader_Sensor->GetNumRows(); 
  for (int i = 0; i <=numSensors; ++i) {
    CbmStsDbQaSensorPar* sensor_par = (CbmStsDbQaSensorPar*) fReader_Sensor->GetRow(i);
    if (!sensor_par) { continue; }
    // !!!Change to addat !!!
    fSensors->AddAtAndExpand(sensor_par, sensor_par->GetUID());
    cout << " fill fSensor at -----> " << sensor_par->GetUID() << endl;
  }
 
  // IvMeas List
  CbmStsDbQaIvPar ivpar;
  //FairDbReader<CbmStsDbQaIvPar>* r_ivpar = ivpar.GetParamReader();  
  fReader_Iv = ivpar.GetParamReader();  
  // Activate reading from previous Context
  fReader_Iv->Activate(context, GetVersion());

  Int_t numIvMeas = fReader_Iv->GetNumRows(); 
  for (int i = 0; i <= numIvMeas; ++i) {
    CbmStsDbQaIvPar* iv_par = (CbmStsDbQaIvPar*) fReader_Iv->GetRow(i);
    if (!iv_par) { continue; }
    // !!!Change to addat !!!
    fIvMeas->AddAtAndExpand(iv_par, iv_par->GetUID());
    cout << " fill  fIvMeas at -----> " << iv_par->GetUID() << endl;
  }

}


void CbmStsDbQaPar::store(UInt_t rid)
{

 //<DB> store the lists
  cout << "-I- CbmStsDbQaPar::store() called with RID# " << rid << endl; 

 // CbmStsDbQaSensorPar stored row-wized
  CbmStsDbQaSensorPar sPar;
  FairDbWriter<CbmStsDbQaSensorPar>* sW = sPar.ActivateWriter(rid); 

  if (sW) 
  {
      // List of Qa Objects
      for(Int_t i=0;i<fSensors->GetSize();i++)
      {
         CbmStsDbQaSensorPar* s_par = (CbmStsDbQaSensorPar*) fSensors->At(i);
         if (s_par) *sW << *s_par; 
      }
  
  // Reset the Writer
    if(!sW->Close())
    {
     cout << " -W- CbmStsDbQaPar:: IO Error writing  sensor parameters ******** "<< endl;  
    }

  }//! (aWriter)
   else {
   cout << " -W- CbmStsDbQaPar:: Writer for CbmStsDbQaSensorPar not created "<< endl;  
  }


// CbmStsDbQaIvPar stored row-wized
  CbmStsDbQaIvPar ivPar;
  FairDbWriter<CbmStsDbQaIvPar>* iW = ivPar.ActivateWriter(rid); 

  if (iW) 
  {
      // List of Qa Objects
      for(Int_t i=0;i<fIvMeas->GetSize();i++)
      {
         CbmStsDbQaIvPar* iv_par = (CbmStsDbQaIvPar*) fIvMeas->At(i);
         if (iv_par) *iW << *iv_par; 
      }
  
   // Reset the Writer
   if(!iW->Close())
   {
    cout << " -W- CbmStsDbQaPar:: IO Error writing  IV meas. parameters ******** "<< endl;  
   }

  }//! (aWriter)
   else {
   cout << " -W- CbmStsDbQaPar:: Writer for CbmStsDbQaIvPar not created "<< endl;  
  }

}

void CbmStsDbQaPar::Print()
{

  std::cout<<" -----------  STS Qa Parameters -------------  "<<std::endl;

  
  for(Int_t i=0;i<fSensors->GetSize(); i++)
  {
    CbmStsDbQaSensorPar* s_par = (CbmStsDbQaSensorPar*) fSensors->At(i);  
    cout << "----------------------------------------------------------------------" << endl;
    if (s_par) s_par->Print();
  }

  for(Int_t i=0;i<fIvMeas->GetSize(); i++)
  {
    CbmStsDbQaIvPar* iv_par = (CbmStsDbQaIvPar*) fIvMeas->At(i);  
    cout << "----------------------------------------------------------------------" << endl;
    if (iv_par) iv_par->Print();
  }

}

void CbmStsDbQaPar::ImportFromCsvFile(const string& fname)
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
			  // cout <<" tokens <  " << *it <<  " > " <<  endl;
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

    for (vector< vector<string> >::const_iterator it = csv_values.begin()+1; it != csv_values.end(); ++it)
    {
     const vector<string>& values = *it;
     CbmStsDbQaSensorPar *par = new CbmStsDbQaSensorPar();
     if(par->Import(values)) fSensors->AddAtAndExpand(par,par->GetUID());  
    }

     cout << " FSensors array  entries ----- >" << fSensors->GetEntries() << endl;
    
return;
}


void CbmStsDbQaPar::ImportIvFilesFromDir()
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
     CbmStsDbQaIvPar* par = (CbmStsDbQaIvPar*) fIvMeas->At(i);
     if (par) {par->Print();} 
   }
 
  return;  
}   

void CbmStsDbQaPar::LoadDataFromIvFile(const string& fname)
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
       cout << "-E- CbmStsDbQaPar::LoadDataFromIvFile error opening iv file:  " <<  fname << endl; 
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
  CbmStsDbQaIvPar * par = new CbmStsDbQaIvPar();
  par->SetUID(s_id[0]);
  par->SetFileName(fname);
  par->SetN(n_meas);
  par->SetI(a);
  par->SetV(b);
  
  fIvMeas->AddAtAndExpand(par,par->GetUID());
  
  
 return;
} 

std::istream& CbmStsDbQaPar::safeGetline(std::istream& is, std::string& t)
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



