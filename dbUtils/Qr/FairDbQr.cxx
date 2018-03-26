#include "FairDbQr.h"

using qrcodegen::QrCode;
using qrcodegen::QrSegment;

TH2F* FairDbQr::EncodeString(std::string string)
{
  const char *text = string.c_str();
  const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;
  const QrCode qr = QrCode::encodeText(text, errCorLvl);
  Int_t size = qr.getSize();

  TH2F *hist = new TH2F("QR Code", text, size, 0, size, size, 0, size);
  for (Int_t x = 0; x < size; x++) {
     for (Int_t y = 0; y < size; y++) {
        if (qr.getModule(x, y)) {
           hist->Fill(x,y);
        }
     }
  }
  hist->SetStats(0);
  hist->SetMinimum(0);
  hist->SetMaximum(1e5);

  return hist;
}
