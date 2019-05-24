#include "TCanvas.h"
#include "TF1.h"

//A handy function to read in parameter values
void reader(string file, double eta, TF1* func, double A, double B, double C) {

  //Open file for reading
  ifstream in;
  in.open(file);
  if (!in.is_open()) {
    cout << "Error opening file!" << endl;
    return;
  }

  //Init
  string read;
  int linesRead = 0;	//Stepper
  double p0=0,p1=0,p2=0,p3=0,p4=0,p5=0;	//Init dummies to read into
  while (getline(in,read) && linesRead != (int(eta*10)-1)) ++linesRead;
  stringstream stream;
  stream << read;

  //Read params
  string name = func->GetName();
  //1st string on line is eta region low. bd. so read p0 twice to get the param.
  if (name.find("S0")!=string::npos) {
    stream >>p0>>p0>>p1>>p2;	//Hadron style param. file
    func->SetParameters(p0, pow((1.192/0.75),1-p2), p2,A,B,C);
  } else if (name.find("Sm")!=string::npos) {
    cout << "Sm" << endl;
    stream >>p0>>p0>>p1>>p2;	//Hadron style param. file
    func->SetParameters(p0, pow((1.197/0.75),1-p2), p2,A,B,C);
  } else if (name.find("Sp")!=string::npos) {
    stream >>p0>>p0>>p1>>p2;	//Hadron style param. file
    func->SetParameters(p0, pow((1.189/0.75),1-p2), p2,A,B,C);
  } else if (name.find("Xi0")!=string::npos) {
    cout << "Xi0" << endl;
    stream >>p0>>p0>>p1>>p2;	//Hadron style param. file
    func->SetParameters(p0, pow((1.315/0.75),1-p2), p2,A,B,C);
  } else if (name.find("Xim")!=string::npos) {
    stream >>p0>>p0>>p1>>p2;	//Hadron style param. file
    func->SetParameters(p0, pow((1.322/0.75),1-p2), p2,A,B,C);
  } else if (name.find("Om")!=string::npos) {
    stream >>p0>>p0>>p1>>p2;	//Hadron style param. file
    func->SetParameters(p0, pow((1.67245/0.75),1-p2), p2,A,B,C);
  } else {
    stream >>p0>>p0>>p1>>p2;
    func->SetParameters(p0,p1,p2,A,B,C);
  }

  in.close();

}

/* THE MAIN FUNCTION */

void AnsatzResponses() {

  //N.B. Set upper limit of the eta region to plot in 0.1 steps
  for (double etaMax = 0.1; etaMax < 0.15/*3.2*/; etaMax += 0.1) {
    if (etaMax < 0.1 || etaMax > 3.2) {
      cout << "Particle outside good eta region" << endl;
      return;
    }

    //Instantiate and divide canvas
    TCanvas* canv = new TCanvas("c","",600,450);

    /* Response functions. Energy is x here due to ROOT conventions */

    //Hadron response functions (for the main plot)
    string pwrlaw = "[5]*[0]*(1-[3]*[1]*pow(x/0.75,[2]+[4]-1))";
    TF1* frpi = new TF1("pi",  pwrlaw.c_str(), 0, 251);
    TF1* frp  = new TF1("p",   pwrlaw.c_str(), 0, 251);
    TF1* frXi = new TF1("Xi0", pwrlaw.c_str(), 0, 251);
    TF1* frS  = new TF1("Sm",  pwrlaw.c_str(), 0, 251);
    TF1* frOm = new TF1("Om",  pwrlaw.c_str(), 0, 251);

    //A workaround to set axis range
    TF1* lim = new TF1("lim","0",0.3,251);    

    //Higher resolution
    int reso = 2000;     frpi->SetNpx(reso);  frp->SetNpx(reso);
    frXi->SetNpx(reso);  frS->SetNpx( reso);  frOm->SetNpx(reso);

    /* Set params for different |eta| values and draw */
    string dir = "./../../spr_mc/";
    string run = "RunIIa";
    //string run = "RunIIb1-P20ToP17";
    double ourABC[3] = {1.45924, 0.0039269, 1.00393};	//Init to P6 IIa
    if (run.find("RunIIb1")!=string::npos) {	//Switch parameters to P6 IIb1
      cout << "Switching to IIb1 fit param.s" << endl;
      ourABC[0]=1.41964;  ourABC[1]=-0.12901;  ourABC[2]=1.00905;
    }
    reader(dir+run+"/proton.txt",etaMax, frp,         1,        0,        1);
    reader(dir+run+"/pion.txt",  etaMax, frpi,        1,        0,        1);
    reader(dir+run+"/pion.txt",  etaMax, frS,         1,        0,        1);
    reader(dir+run+"/pion.txt",  etaMax, frXi,        1,        0,        1);
    reader(dir+run+"/pion.txt",  etaMax, frOm,        1,        0,        1);
/*  //Fitted
    reader(dir+run+"/pion.txt",  etaMax, frS, ourABC[0],ourABC[1],ourABC[2]);
    reader(dir+run+"/pion.txt",  etaMax, frXi,ourABC[0],ourABC[1],ourABC[2]);
    reader(dir+run+"/pion.txt",  etaMax, frOm,ourABC[0],ourABC[1],ourABC[2]);
*/
    //Legend
    TLegend* lg = new TLegend(0.12,0.45,0.35,0.845);
    lg->SetBorderSize(0);   lg->SetFillStyle(0); 
    lg->AddEntry(frp,  "#font[12]{p}",          "l");
    lg->AddEntry(frpi, "#font[132]{#pi^{#pm}}", "l");
    lg->AddEntry(frXi, "#font[12]{#Xi}",        "l");
    lg->AddEntry(frS,  "#font[12]{#Sigma}",     "l");
    lg->AddEntry(frOm, "#font[12]{#Omega}^{-}", "l");

    //Colors
    lim->SetLineColor(0);         frOm->SetLineColor(kOrange);
    frp->SetLineColor( kBlack);   frpi->SetLineColor(kGray+1);
    frXi->SetLineColor(kRed  );   frS->SetLineColor( kBlue  );

    //General plot setup
    canv->SetLogx();
    lim->GetYaxis()->SetTitle("#font[12]{R}");
    lim->GetYaxis()->SetTitleSize(0.05);
    lim->GetYaxis()->SetTitleOffset(0.7);
    lim->SetMaximum(1.05);		//Set vertical...
    lim->SetMinimum(0);		//...axis range
    lim->GetXaxis()->SetTitle("#font[132]{#font[12]{E}_{gen} [GeV]}");
    lim->GetXaxis()->SetTitleSize(0.05);
    lim->GetXaxis()->SetTitleOffset(0.8);
    lim->GetXaxis()->SetNoExponent(1);

    //Construct plot title and figure filename
    string title = "";
    lim->SetTitle(title.c_str());


    //Plot
    lim->Draw();         frp->Draw("SAME");  frpi->Draw("SAME");
    frXi->Draw("SAME");  frS->Draw("SAME");  frOm->Draw("SAME");
    lg->Draw(  "SAME");

    //Name the runs above the legend
    TLatex latex;
    latex.SetTextSize(0.05);
    latex.SetTextAlign(13);  //align at top
    latex.DrawLatex(0.015,1.04,	"#font[132]{IIa}");
    latex.DrawLatex(0.03,1.04,	"#font[132]{IIb}");
    latex.SetTextSize(0.025);
    latex.DrawLatex(0.025,0.99,	"P20ToP17");
    //Save plot
    stringstream figName;
    figName << "Ansatz_" << run << "_"
	    << int(etaMax-0.1) << int(10*etaMax-1) % 10 << "_eta_"
	    << int(etaMax)     << int(10*etaMax  ) % 10
	    << ".eps";
    canv->Print(figName.str().c_str());

    //Free memory
    delete canv;  delete lg;
    delete frp;   delete frpi;  delete frXi;  delete frS;  delete frOm;

  } //Loop eta regions

}
