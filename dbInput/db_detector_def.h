/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

////////////////////////////////////////////////////////////////////////////
// FairDbDetector
//
// FairDbDetector defines Detector_t which is an enumeration of the
// legal detector configurations
//
////////////////////////////////////////////////////////////////////////////

#ifndef FAIRDBDETECTOR_H
#define FAIRDBDETECTOR_H

#ifndef ROOT_Rtypes
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "Rtypes.h"
#endif
#endif

#include "TString.h"

class FairDbDetector
{
  public:
    public:
     typedef enum EDetector {
       kUnknown          = 0x000,
       kMagnet           = 0x001,
       kMvd              = 0x002,
       kSts              = 0x004,
       kMuch             = 0x008,
       kRich             = 0x010,
       kTrd              = 0x020,
       kTof              = 0x040,
       kEcal             = 0x080,
       kPsd              = 0x100,
     } Detector_t;

    static Int_t       FullMask() {
      return kMagnet|kMvd|kSts|kMuch|kRich|kTrd|kTof|kEcal|kPsd;
    }

    static const Char_t*  AsString(Detector_t detector) {
      switch (detector) {
      case kUnknown:
        return "Unknown";
        break;
      case kMagnet:
        return "Magnet";
        break;
      case kMvd:
        return "Mvd";
        break;
      case kSts:
        return "Sts";
        break;
      case kMuch:
        return "Much";
        break;
      case kRich:
        return "Rich";
        break;
      case kTrd:
        return "Trd";
        break;
      case kTof:
        return "Tof";
        break;
      case kEcal:
        return "Ecal";
        break;
      case kPsd:
        return "Psd";
        break;
      default:
        return "?Unknown?";
        break;
      }
    }

    static FairDbDetector::Detector_t   CharToEnum(Char_t c) {
      return (FairDbDetector::Detector_t) c;
    }

    static Char_t*  MaskToString(Int_t mask) {
      static Char_t newstring[255] = "";
      Char_t* ptr = newstring;
      *ptr = 0;
      Int_t fullmask = FairDbDetector::FullMask();

      for (Int_t i=0; i<32; i++) {
        FairDbDetector::Detector_t adet = (FairDbDetector::Detector_t)(1<<i);
        if (mask & adet & fullmask) {
          const Char_t* toadd = FairDbDetector::AsString(adet);
          if (ptr != newstring) { *ptr++ = ','; }
          strcpy(ptr,toadd);
          ptr += strlen(toadd);
        }
      }
      *ptr++ = 0;

      return newstring;
    }

    static Int_t  StringToMask(const Char_t* chars, Int_t maxChar=0) {
      Int_t mask  = 0;
      TString thestring(chars);
      if (maxChar>0 && maxChar<thestring.Length()) { thestring.Resize(maxChar); }

      if (thestring.Contains("Magnet")) { mask |= kMagnet; }
      if (thestring.Contains("Mvd")) { mask |= kMvd; }
      if (thestring.Contains("Sts")) { mask |= kSts; }
      if (thestring.Contains("Much")) { mask |= kMuch; }
      if (thestring.Contains("Rich")) { mask |= kRich; }
      if (thestring.Contains("Trd")) { mask |= kTrd; }
      if (thestring.Contains("Tof")) { mask |= kTof; }
      if (thestring.Contains("Ecal")) { mask |= kEcal; }
      if (thestring.Contains("Psd")) { mask |= kPsd; }

      return mask;
    }

    static FairDbDetector::Detector_t  StringToEnum(const Char_t* chars, Int_t maxChar=0) {
      Int_t mask = FairDbDetector::StringToMask(chars,maxChar);

      switch (mask) {
      case kUnknown:
        return kUnknown;
        break;
      case kMagnet:
        return kMagnet;
        break;
      case kMvd:
        return kMvd;
        break;
      case kSts:
        return kSts;
        break;
      case kMuch:
        return kMuch;
        break;
      case kRich:
        return kRich;
        break;
      case kTrd:
        return kTrd;
        break;
      case kTof:
        return kTof;
        break;
      case kEcal:
        return kEcal;
        break;
      case kPsd:
        return kPsd;
        break;
      default:
        return kUnknown;
        break;
      }
    }

    static FairDbDetector::Detector_t  GetDetType(Int_t detMask) {
      FairDbDetector::Detector_t fDetType = FairDbDetector::kUnknown;

      if      ( detMask & FairDbDetector::kMagnet)    { fDetType = FairDbDetector::kMagnet; }
      else if ( detMask & FairDbDetector::kMvd)       { fDetType = FairDbDetector::kMvd; }
      else if ( detMask & FairDbDetector::kSts)       { fDetType = FairDbDetector::kSts; }
      else if ( detMask & FairDbDetector::kMuch)      { fDetType = FairDbDetector::kMuch; }
      else if ( detMask & FairDbDetector::kRich)      { fDetType = FairDbDetector::kRich; }
      else if ( detMask & FairDbDetector::kTrd)       { fDetType = FairDbDetector::kTrd; }
      else if ( detMask & FairDbDetector::kTof)       { fDetType = FairDbDetector::kTof; }
      else if ( detMask & FairDbDetector::kEcal)      { fDetType = FairDbDetector::kEcal; }
      else if ( detMask & FairDbDetector::kPsd)       { fDetType = FairDbDetector::kPsd; }

      return fDetType;
    }
};


#endif // FAIRDBDETECTOR_H

