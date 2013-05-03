#ifndef FAIRDBMULTCONNECTOR_H
#define FAIRDBMULTCONNECTOR_H

#include "FairDbConnection.h"           // for FairDbConnection

#include "Riosfwd.h"                    // for ostream
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "Rtypes.h"                     // for Int_t, UInt_t, Bool_t, etc
#endif

#include <map>                          // for map
#include <ostream>                      // for ostream
#include <string>                       // for string
#include <vector>                       // for vector

class FairDbStatement;

class FairDbMultConnector
{
    friend ostream& operator<<(ostream& s, const FairDbMultConnector& cascader);

  public:


    enum Status { kFailed, kClosed, kOpen };

    FairDbStatement* CreateStatement(UInt_t dbNo) const;

/// Return associated FairDbConnection. FairDbMultConnector retains ownership.
    const FairDbConnection* GetConnection(UInt_t dbNo) const;
    FairDbConnection* GetConnection(UInt_t dbNo) ;

    std::string GetDbName(UInt_t dbNo) const;
    Int_t GetDbNo(const std::string& dbName) const;
    Int_t GetStatus(UInt_t dbNo) const {
      if ( dbNo >= GetNumDb() || ! fConnections[dbNo]  ) { return kFailed; }
      return fConnections[dbNo]->IsClosed() ? kClosed : kOpen;
    }
    std::string GetStatusAsString(UInt_t dbNo) const ;
    std::string GetURL(UInt_t dbNo) const {
      return ( dbNo < GetNumDb() ) ? fConnections[dbNo]-> GetUrl(): "";
    }
    Bool_t IsTemporaryTable(const std::string& tableName,
                            Int_t dbNo) const;
// Cascade-wide getters.

    Int_t AllocateSeqNo(const std::string& tableName,
                        Int_t requireGlobal = 0,
                        Int_t dbNo = 0) const;
    Int_t GetAuthorisingDbNo() const { return fGlobalSeqNoDbNo; }
    UInt_t GetNumDb() const {return fConnections.size();}
    Int_t  GetTableDbNo(const std::string& tableName, Int_t selectDbNo = -1) const;
    Bool_t TableExists(const std::string& tableName, Int_t selectDbNo = -1) const {
      return this->GetTableDbNo(tableName,selectDbNo) >= 0;
    }


    Int_t CreateTemporaryTable(const std::string& tableName,
                               const std::string& tableDescr);
    void HoldConnections();
    void ReleaseConnections();
    void SetPermanent(UInt_t dbNo, Bool_t permanent = true);
    // Constructors and destructors.
    FairDbMultConnector();
    virtual ~FairDbMultConnector();
    FairDbMultConnector(const FairDbMultConnector& conn);

    void SetAuthorisingEntry(Int_t entry) {fGlobalSeqNoDbNo = entry;}

  protected:

  private:

    Int_t ReserveNextSeqNo(const std::string& tableName,
                           Bool_t isGlobal,
                           UInt_t dbNo) const;


    Int_t fGlobalSeqNoDbNo;
    std::vector<FairDbConnection*> fConnections;
    std::map<std::string,Int_t> fTemporaryTables;



    class BLock
    {

      public:
        BLock(FairDbStatement* stmtDB, const std::string& seqnoTable, const std::string& dataTable);
        ~BLock();

        Bool_t IsBLocked() const { return fLocked; }

      private:

        BLock(const BLock&);
        BLock operator=(const BLock&);

        void SetBLock(Bool_t setting = kTRUE);

        FairDbStatement* fStmt;            // Statement to be used to issue lock
        std::string fSeqnoTableName;  // The SEQNO table that is locked
        std::string fDataTableName;   // The data table that is locked
        Bool_t fLocked;          // Lock status

    };



    ClassDef(FairDbMultConnector,0)     // A priority list of Db connections

};

#endif  //
