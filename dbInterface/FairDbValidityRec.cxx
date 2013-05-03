#include "FairDbValidityRec.h"

#include "FairDbBinaryFile.h"           // for FairDbBinaryFile
#include "FairDbOutRowStream.h"         // for FairDbOutRowStream
#include "FairDbResPtr.h"               // for string, FairDbResultPtr
#include "FairDbResult.h"               // for FairDbResultSet
#include "ValContext.h"                 // for ValContext
#include "ValRange.h"                   // for ValRange
#include "ValTimeStamp.h"               // for ValTimeStamp, operator>, etc

#include "TString.h"                    // for operator<<

#include <iostream>                     // for operator<<, basic_ostream, etc

using std::ostringstream;
using std::hex;

ClassImp(FairDbValidityRec)


#include "FairDbResPtr.tpl"
template class  FairDbResultPtr<FairDbValidityRec>;


ostream& operator<<(ostream& os, const FairDbValidityRec& vRec)
{

  const ValRange&           range(vRec.GetValRange());

  ostringstream os2;  //Use local to avoid manipulators from making permanent change to os.
  os2 << "-I- FairDbValidityRec:: SeqNo: " << vRec.GetSeqNo()
      << "  AggNo: " << vRec.GetAggregateNo()
      << "  DbNo: " << vRec.GetDbNo();
  if ( vRec.IsGap() ) { os2 << " (gap)"; }
  os2  << "  ValRange: |"
       << hex  << "0x"  << range.GetDetectorMask() << "|"
       << "0x" << range.GetSimMask() << "|"
       << " " << range.GetTimeStart().AsString("s") << " .. "
       << range.GetTimeEnd().AsString("s")
       << " from: " << range.GetDataSource();
  os << os2.str();
  return os;

}


void FairDbValidityRec::AndTimeWindow(const ValTimeStamp& startOther,
                                      const ValTimeStamp& endOther)
{

  ValTimeStamp start = fValRange.GetTimeStart();
  ValTimeStamp end   = fValRange.GetTimeEnd();

  SetTimeWindow( start > startOther ? start : startOther,
                 end   < endOther   ? end   : endOther );
}

//.....................................................................

FairDbValidityRec::FairDbValidityRec(Int_t dbNo,Bool_t isGap)
  : FairDbTableRow(),
    fAggregateNo(-2),
    fCreationDate(),
    fDbNo(dbNo),
    fInsertDate(),
    fIsGap(isGap),
    fVersion(0),
    fSeqNo(0),
    fTableProxy(0),
    fValRange()
{
}
//.....................................................................

FairDbValidityRec::FairDbValidityRec(const FairDbValidityRec& from)
  : FairDbTableRow(from),
    fAggregateNo(from.fAggregateNo),
    fCreationDate(from.fCreationDate),
    fDbNo(from.fDbNo),
    fInsertDate(from.fInsertDate),
    fIsGap(from.fIsGap),
    fVersion(from.fVersion),
    fSeqNo(from.fSeqNo),
    fTableProxy(from.fTableProxy),
    fValRange(from.fValRange)

{
  *this = from;
}
//.....................................................................

FairDbValidityRec::FairDbValidityRec(const ValRange& range,
                                     FairDb::Version task,
                                     Int_t aggNo,
                                     UInt_t seqNo,
                                     Int_t dbNo,
                                     Bool_t isGap,
                                     ValTimeStamp time) :
  fAggregateNo(aggNo),
  fCreationDate(time),
  fDbNo(dbNo),
  fInsertDate(),
  fIsGap(isGap),
  fVersion(task),
  fSeqNo(seqNo),
  fTableProxy(0),
  fValRange(range)
{

}

//.....................................................................
FairDbValidityRec& FairDbValidityRec::operator=(const FairDbValidityRec& from)
{
  if (this == &from) { return *this; }

  FairDbTableRow::operator=(from);
  fAggregateNo=from.fAggregateNo;
  fCreationDate=from.fCreationDate;
  fDbNo=from.fDbNo;
  fInsertDate=from.fInsertDate;
  fIsGap=from.fIsGap;
  fVersion=from.fVersion;
  fSeqNo=from.fSeqNo;
  fTableProxy=from.fTableProxy;
  fValRange=from.fValRange;

  return *this;
}

//.....................................................................

FairDbValidityRec::~FairDbValidityRec()
{

}


void FairDbValidityRec::Fill(FairDbResultSet& rs,
                             const FairDbValidityRec* /* vrec */)
{

  ValTimeStamp start, end;
  Int_t detMask, simMask;


  fTableProxy = rs.GetTableProxy();
  fDbNo       = rs.GetDbNo();

//  Read data from row.

  rs >> fSeqNo >> start >> end >> detMask >> simMask >> fVersion
     >> fAggregateNo    >> fCreationDate  >> fInsertDate;

  fIsGap = kFALSE;

  ValRange vr(detMask, simMask, start, end, "From Database");
  fValRange = vr;

  cout
      << "FairDbValidityRec for row " << rs.CurRowNum()
      << ": " << fValRange.AsString()
      << " seq num: " << fSeqNo
      << " agg no: "  << fAggregateNo
      << " version: " << fVersion <<  endl;
}


std::string FairDbValidityRec::GetL2CacheName() const
{
  return FairDbValidityRec::GetL2CacheName(this->GetSeqNo(),
         this->GetSeqNo(),
         this->GetCreationDate());

}

std::string FairDbValidityRec::GetL2CacheName(UInt_t seqLo,
    UInt_t seqHi,
    ValTimeStamp ts)
{

  ostringstream oss;
  oss << seqLo << "_";
  if ( seqLo != seqHi ) { oss << seqHi << "_"; }
  oss << ts.AsString("s");
  std::string str(oss.str());
  // Convert white space to underscore.
  int i = str.size();
  while ( i-- ) if ( str[i] == ' ' ) { str[i] = '_'; }
  return str;

}


Bool_t FairDbValidityRec::HasExpired(const FairDbValidityRec& other) const
{

  const ValRange& otherVR =other.GetValRange();

  return
    (       other.GetVersion() == fVersion
            && ( otherVR.GetDetectorMask() & fValRange.GetDetectorMask() )
            && ( otherVR.GetSimMask()      & fValRange.GetSimMask() )
            && (    otherVR.GetTimeStart() >= fValRange.GetTimeEnd()
                    || otherVR.GetTimeEnd()   <= fValRange.GetTimeStart() )
    );

}


Bool_t FairDbValidityRec::HasExpired(const ValContext& vc,
                                     const FairDb::Version& task) const
{

  ValTimeStamp    ts = vc.GetTimeStamp();
  const ValRange& vr = this->GetValRange();

  return
    (       task             == this->GetVersion()
            && ( vc.GetDetector() &  vr.GetDetectorMask() )
            && ( vc.GetSimFlag()  &  vr.GetSimMask() )
            && (    ts            >= vr.GetTimeEnd()
                    || ts            <  vr.GetTimeStart() )
    );

}



Bool_t FairDbValidityRec::IsCompatible(const ValContext& vc,
                                       const FairDb::Version& task) const
{

  cout  << " FairDbValidityRec::IsCompatible : tasks:"
        << task << "," << fVersion
        << " is compat: " << fValRange.IsCompatible(vc) << endl
        << "   range " << fValRange.AsString() << endl
        << "   context " << vc.AsString() << endl;

  return task == fVersion  && fValRange.IsCompatible(vc);

}

void FairDbValidityRec::SetTimeWindow(const ValTimeStamp& start,
                                      const ValTimeStamp& end)
{
  ValRange  range(fValRange.GetDetectorMask(),
                  fValRange.GetSimMask(),
                  start,
                  end,
                  fValRange.GetDataSource() );
  fValRange = range;

}


void FairDbValidityRec::Store(FairDbOutRowStream& ors,
                              const FairDbValidityRec* /* vrec */) const
{

  ors << fSeqNo
      << fValRange.GetTimeStart()
      << fValRange.GetTimeEnd()
      << fValRange.GetDetectorMask()
      << fValRange.GetSimMask()
      << fVersion
      << fAggregateNo
      << fCreationDate
      << fInsertDate;
}

void FairDbValidityRec::Streamer(FairDbBinaryFile& file)
{

  if ( file.IsReading() ) {
    file >> fAggregateNo
         >> fCreationDate
         >> fDbNo
         >> fInsertDate
         >> fIsGap
         >> fVersion
         >> fSeqNo
         >> fValRange;
    fTableProxy = 0;
  } else if ( file.IsWriting() ) {
    file << fAggregateNo
         << fCreationDate
         << fDbNo
         << fInsertDate
         << fIsGap
         << fVersion
         << fSeqNo
         << fValRange;
  }
}


void FairDbValidityRec::Trim(const ValTimeStamp& queryTime,
                             const FairDbValidityRec& other)
{

  if ( fAggregateNo != other.fAggregateNo || other.IsGap() ) { return; }

//  If this record is not a gap then the other record can be ignore
//  as it is of lower priority.

  if ( ! IsGap() ) { return; }

  ValTimeStamp start      = fValRange.GetTimeStart();
  ValTimeStamp end        = fValRange.GetTimeEnd();
  ValTimeStamp startOther = other.GetValRange().GetTimeStart();
  ValTimeStamp endOther   = other.GetValRange().GetTimeEnd();

// If entry brackets query date, then use it but with a validity that
// is trimmed by the current record.

  if ( startOther <= queryTime && endOther > queryTime ) {
    if ( start < startOther ) { start = startOther; }
    if ( end   > endOther   ) { end   = endOther; }
    *this = other;
    SetTimeWindow(start,end);
  }

// It doesn't bracket, so use it to trim the window

  else {

    if ( endOther <= queryTime ) {
      if ( start < endOther   ) { SetTimeWindow(endOther,end); }
    } else if ( startOther > queryTime ) {
      if ( end   > startOther ) { SetTimeWindow(start, startOther); }
    }
  }

}

