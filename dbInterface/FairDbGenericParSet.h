/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef FAIRDBGENERICPARSET_H
#define FAIRDBGENERICPARSET_H

#include "FairDbLogService.h"
#include "FairDbLogFormat.h"
#include "FairDbExceptionLog.h"       
#include "FairDbConnectionPool.h"
#include "FairDbStatement.h"
#include "FairDbParSet.h"
#include "FairDbReader.h"
#include "FairDbWriter.h"
#include "FairDbUtilType.h"
#include "FairDbOutTableBuffer.h"    
#include "FairDbStatement.h"       
#include "FairDbParFactory.h"
#include "DataType.h"         
#include "ValCondition.h"     
#include "ValTimeStamp.h"     


#include "TString.h"                   
#include "Rtypes.h" 
#include <iosfwd>                    
#include <iostream> 
#include <string>   
#include <sstream>  
#include <stdlib.h> 
#include <memory>   
#include <vector>   

/// TODO: find better place for this helper
template< class T >
std::unique_ptr<T> copy_unique(const std::unique_ptr<T>& source)
{
  return source ? std::unique_ptr<T>(new T(*source)) : nullptr;
}

template <typename T>
class FairDbGenericParSet : public FairDbParSet
{
  public:
  
    FairDbGenericParSet();
    FairDbGenericParSet(FairDbDetector::Detector_t detid, DataType::DataType_t dataid, 
                        const char* name, const char* title, const char* context, Bool_t ownership=kFALSE);
    FairDbGenericParSet(const FairDbGenericParSet& from);
    FairDbGenericParSet& operator=(const FairDbGenericParSet&);
    virtual ~FairDbGenericParSet();
    
    // Composite Data Model Flag 
    FairDbDetector::Detector_t GetDetectorId() {return fDet_id;}
    DataType::DataType_t GetDataType()  {return fData_id;}
    void SetCompId(Int_t x)  {SetComboNo(x); fCompId=x;}
    Int_t GetCompId() const {return fCompId;}
        
    // Create the FairDb tables (data + metatdata) in DB
    void CreateDbTable(Int_t db_entry);
     
    // RuntimeDb IO 
    virtual void clear();
    virtual void fill(UInt_t rid=0);
#ifndef __CINT__
    static std::vector<T> GetBy(std::function<bool(const T&)> condition, UInt_t rid=ValTimeStamp());
#endif
    static unique_ptr<T> GetByIndex(Int_t index, UInt_t rid=ValTimeStamp());
    static std::vector<T> GetAll(UInt_t rid=ValTimeStamp());
    std::vector<T> GetAllVersions();
    virtual void store(UInt_t rid=0);
    static void StoreArray(std::vector<T>, UInt_t rid=0, std::string = "");

    // Standard Validity frame definition
    virtual ValCondition GetContext(UInt_t rid) {
      return ValCondition(fDet_id,
                          fData_id,
                          ValTimeStamp(rid));
    }

    // Custom Table Name 
    void SetTableName(string name ) { fTableName = name ;}
    
    // Standard Table Name (Generated) 
    string GetTableName() {
      std::transform(fTableName.begin(), fTableName.end(),fTableName.begin(), ::toupper);
      return fTableName;}
 
    virtual FairDbObjTableMap* CreateObjTableMap() const {
      // GenericParSet create table with the object ClassName  
      // <DB> 25.01.2017
      T a;
      return new T(a.GetDetectorId(),a.GetDataType(),a.GetTableName().data(),a.GetTitle(),a.getParamContext(),kTRUE);        
    }
    
    // SQL-IO Meta-Class Getters
    FairDbReader<T>* GetParamReader();
    FairDbWriter<T>* GetParamWriter();
    FairDbWriter<T>* ActivateWriter(Int_t rid);

    // Get Values of ptr
    FairDbReader<T>& GetReader() {return *(GetParamReader());} 
    FairDbWriter<T>& GetWriter() {return *(GetParamWriter());} 

  protected:
    // Meta Data (added in ctor)
    FairDbDetector::Detector_t fDet_id; //!
    DataType::DataType_t fData_id; //!
    // Associated Table Name
    string fTableName; //!
    // Data Aggregation Id
    Int_t fCompId; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbWriter<T>* fParam_Writer; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbReader<T>* fParam_Reader; //!
    // Connection Pool
    FairDbConnectionPool* fMultConn;  //!

    
    ClassDefT(FairDbGenericParSet<T>,0)   // FairDbGenericParSet: the FairDb Generic Parameter Interface
};
ClassDefT2(FairDbGenericParSet,T)

template<class T>
inline  FairDbReader<T>* FairDbGenericParSet<T>::GetParamReader()
{
  if (fParam_Reader) { return fParam_Reader; }
  else {
    fParam_Reader = new  FairDbReader<T>(GetTableName());
    return fParam_Reader;
  }
}
template<class T>
inline FairDbWriter<T>* FairDbGenericParSet<T>::GetParamWriter()
{
  if (fParam_Writer) { return fParam_Writer; }
  else {
    fParam_Writer = new  FairDbWriter<T>(GetTableName());
    return fParam_Writer;
  }
}


#endif  // FAIRDBGENERICPARSET_H
