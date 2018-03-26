// Loads text strings from file QrInput.txt in current directory and
// creates an A4 paper sized canvas filled with QR codes

void QrPage()
{
  Int_t n = 0;
  const Int_t max = 1000;
  std::vector< std::string > ids;
  std::string line;

  const char *file = "QrInput.txt";

  ifstream data("QrInput.txt");
  if (!data.good())
  {
    std::cout << "File does not exist, not plotting anything" << std::endl;
    return;
  }

  while(!data.eof())
  {
    std::getline(data, line);

    ids.emplace_back(std::string(line));
  }

  gStyle->SetPalette(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadLeftMargin(0.0);
  gStyle->SetPadRightMargin(0.00);
  gStyle->SetPadTopMargin(0.25);
  gStyle->SetPadBottomMargin(0.00);

  TCanvas *qrCanvas = new TCanvas("qrCanvas", "qrCanvas", 210*2, 297*2);
  qrCanvas->SetWindowSize(210*2, 297*2);
  qrCanvas->DivideSquare(64);

  for (Int_t i = 0; i < ids.size(); i++)
  {
    qrCanvas->cd(i+1);

    TH2F *hcol1 = FairDbQr::EncodeString(ids[i]);
    hcol1->SetName(TString::Format("QR%d", i).Data());

    hcol1->Draw("COL A");
    TPaveText *pt = new TPaveText(0.0,1.0,1.0,0.85, "NDC");
    pt->AddText(ids[i].c_str());
    pt->SetFillColor(kWhite);
    pt->SetBorderSize(0);
    pt->Draw();
  }

  qrCanvas->SaveAs("QrPage.pdf");
}