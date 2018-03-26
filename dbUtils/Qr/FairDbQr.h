#ifndef FAIRDBQR_H
#define FAIRDBQR_H

#include "Rtypes.h"
#include "TObject.h"
#include "TH2F.h"

#include "qr/BitBuffer.hpp"
#include "qr/QrCode.hpp"
#include "qr/QrSegment.hpp"

class FairDbQr: public TObject
{
  public:
    static TH2F* EncodeString(std::string string);
    ClassDef(FairDbQr, 0);
};

#endif // FAIRDBQR_H
