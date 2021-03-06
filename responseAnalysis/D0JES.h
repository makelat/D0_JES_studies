////////////////////////////////////////////////////////////////
// This class has been automatically generated on             //
// Thu Feb  1 12:21:30 2018 by ROOT version 6.13/01           //
// from TTree Pythia6Jets/Pythia6 particle data               //
// found on files: P6_dijet_*.root, H7_dijet_*.root, etc.     //
//                                                            //
// The code has been modified further along the way by        //
// toni.makela@cern.ch of Helsinki Institute of Physics, 2018 //
////////////////////////////////////////////////////////////////

#ifndef D0JES_h
#define D0JES_h

// C/C++
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ios>
#include <pthread.h>
#include <time.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

// ROOT
#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TMatrix.h"
#include "TMatrixD.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TThread.h"
#include "TVector.h"
#include "TVector3.h"
#include "TVectorD.h"
#include "Riostream.h"

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::fstream;
using std::ifstream;
using std::max;
using std::min;
using std::ofstream;
using std::stringstream;

//#define NIJ //If uncommented, sample must have prtclsnij_ -branches

class D0JES {
public :
  string ReadName;	//ROOT filename to read
  string OpenName;	// -||- with ".root" -suffix

  /* Uncomment the subdirectory of input_ROOT_files to fetch input from */
  string inSubDir = "";
  //string inSubDir = "can_repr_pprplots_photon_tag_2/";
  //string inSubDir = "190318/";
  //string inSubDir = "after_gen_handle_renov/";

  TTree* fChain;	//Pointer to the analyzed TTree or TChain
  Int_t  fCurrent;	//Current Tree number in a TChain

  string gjFile="";	//gamma+jet filename compatible with this object
  string djFile="";	//EM+jet -||-
  string gjFile_b="";	//Same as the two above but...
  string djFile_b="";	//...for b-enriched samples

// Fixed size dimensions of array or collections stored in the TTree if any.

  // Declaration of leaf types
  Float_t         weight;
  //Particle lvl
  vector<unsigned char> *prtcl_jet;
  vector<int>     *prtcl_pdgid;
  vector<float>   *prtcl_pt;
  vector<float>   *prtcl_eta;
  vector<float>   *prtcl_phi;
  vector<float>   *prtcl_e;
  //Particles not associated with jets
  #ifdef NIJ
  vector<int>     *prtclnij_pdgid;
  vector<float>   *prtclnij_pt;
  vector<float>   *prtclnij_eta;
  vector<float>   *prtclnij_phi;
  vector<float>   *prtclnij_e;
  #endif
  //Parton lvl
  vector<char>    *prtn_jet;
  vector<int>     *prtn_pdgid;
  vector<char>    *prtn_tag;
  vector<float>   *prtn_pt;
  vector<float>   *prtn_eta;
  vector<float>   *prtn_phi;
  vector<float>   *prtn_e;
  vector<float>   *prtn_dr;
  vector<float>   *jet_pt;
  vector<float>   *jet_eta;
  vector<float>   *jet_phi;
  vector<float>   *jet_e;
  vector<int>     *jet_constituents;
  vector<float>   *jet_ptd;
  vector<float>   *jet_sigma2;
  Float_t         met;

  // List of branches
  //Particle lvl
  TBranch        *b_weight;
  TBranch        *b_prtcl_jet;
  TBranch        *b_prtcl_pdgid;
  TBranch        *b_prtcl_pt;
  TBranch        *b_prtcl_eta;
  TBranch        *b_prtcl_phi;
  TBranch        *b_prtcl_e;
  //Particles not in jets
  #ifdef NIJ
  TBranch        *b_prtclnij_pdgid;
  TBranch        *b_prtclnij_pt;
  TBranch        *b_prtclnij_eta;
  TBranch        *b_prtclnij_phi;
  TBranch        *b_prtclnij_e;
  #endif
  //Parton lvl
  TBranch        *b_prtn_jet;
  TBranch        *b_prtn_pdgid;
  TBranch        *b_prtn_tag;
  TBranch        *b_prtn_pt;
  TBranch        *b_prtn_eta;
  TBranch        *b_prtn_phi;
  TBranch        *b_prtn_e;
  TBranch        *b_prtn_dr;
  //Jet lvl
  TBranch        *b_jet_pt;
  TBranch        *b_jet_eta;
  TBranch        *b_jet_phi;
  TBranch        *b_jet_e;
  TBranch        *b_jet_constituents;
  TBranch        *b_jet_ptd;
  TBranch        *b_jet_sigma2;
  TBranch        *b_met;

  double R_cone;//Jet algorithm cone radius
  //Tensors to contain response function parameters
  //  1st index: (0) runIIa or or default D0 style runIIb* (1) runIIb*-P20ToP17
  //  2nd index: |eta| region. 32 entries for this index
  //  3rd index: the parameters p^(i)_{particle type} in the SPR functions
  //             3rd index has 3 entries unless otherwise stated
  vector<vector<vector<double>>> params_e;	//3rd index 5 entries
  vector<vector<vector<double>>> params_K;	
  vector<vector<vector<double>>> params_KL;	
  vector<vector<vector<double>>> params_KS;	
  vector<vector<vector<double>>> params_L;	
  vector<vector<vector<double>>> params_mu;	//3rd index 4 entries
  vector<vector<vector<double>>> params_n;	
  vector<vector<vector<double>>> params_gam;	//3rd index 6 entries
  vector<vector<vector<double>>> params_pi;	
  vector<vector<vector<double>>> params_p;	
  //For storing D0 dijet / EM+jet data points and errors
  static int const nD0data = 10;	//#Data points available from D0
  static int const nEpochs = 4;		//#Epochs in run IIa and IIb altogether
  //D0 pT-balance data points and errors. dj for dijet, gj for gamma+jet
  double djEpII[nEpochs][nD0data];
  double djD0II[nEpochs][nD0data];
  double djERII[nEpochs][nD0data];
  double gjEpII[nEpochs][nD0data];
  double gjD0II[nEpochs][nD0data];
  double gjERII[nEpochs][nD0data];
  //RunIIa D0 PTB MC
  static int const nD0MC = 13;		//#MC sim. points available from D0
  double djMCEpII[nEpochs][nD0MC];	//Dijet
  double djD0MCII[nEpochs][nD0MC];
  double gjMCEpII[nEpochs][nD0MC];	//Gamma+jet
  double gjD0MCII[nEpochs][nD0MC];
  //D0 gamma+jet MPF MC (run unknown) R_cone=0.7
  static int const nD0MC_MPF_R07=16;	//#MC MPF points R=0.7 from D0
  double gjMCEp_MPF_R07[nD0MC_MPF_R07];
  double gjD0MC_MPF_R07[nD0MC_MPF_R07];
  //D0 gamma+jet MPF data
  static int const nD0_MPF_R07 = 11;  //#MC MPF data points available from D0
  double gjEp_MPF_R07[nD0_MPF_R07];
  double gjD0_MPF_R07[nD0_MPF_R07];
  //D0 gamma+jet MPF ALT MC (run and R_cone unknown)
  static int const nD0MC_MPF=11;	//#MC MPF points from D0
  double gjMCEp_MPF[nD0MC_MPF];
  double gjD0MC_MPF[nD0MC_MPF];
  double djMCEp_MPF[nD0MC_MPF];
  double djD0MC_MPF[nD0MC_MPF];

  //Flags etc. for changing calculation properties
  int const nABCvars=25;	//#Neighboring points to calculate respFit at
  vector<bool> passedCuts;	//Flags for all evts if they passed cuts before
  unsigned int maxIter=500;	//Maximum #iterations in fitting
  bool verbose = false;		//Print additional information in Loop
  bool bEnrichedFiles = false;	//Use separate files for getting F^b_corr?
  bool contHistos = true;	//Produce probe particle content histograms
  bool MPFmode = false;		//Fit to MPF data? If false, use pT-bal.
  bool runIIa = false; 		//Use runIIa response parameters
  bool runIIb = true;	 	//-||-runIIb -||-
  bool P20ToP17	= true; 	//P20ToP17 params for MC-reco instead of default
  //string run = "RunIIb1";	//Different options...
  //string run = "RunIIb2";	//...which run IIb...
  string run = "RunIIb34";	//... epoch to use
  //string run = "ZS";		//Parameters from samples w/ zero suppression
  bool StrangeB = true;		//Use Ansätze for strange baryon response
  //string Ansatz = "pn";   	//If StrangeB=true, use p & n  based Ansätze
  //string Ansatz = "L";  	//        -||-         Lambda     -||-
  string Ansatz = "pi";  	//        -||-         pion       -||-
  bool printProg = true;	//Print info on Loop() progress?
  bool useEarlierCuts  = false;	//True if events chosen based on readEvt
  bool useD0ABC        = false;	//Use fit param A,B,C values given in D0 ANs.
  bool useInitGuessABC = false;	//Use initial guess A,B,C; for starting fits
  bool djFitting = true;	//Use EM+jet ("dijet") data points for fitting
  bool gjFitting = true;	//Use gamma+jet data points for fitting
  double epsilon = 0.01;	//Small step to take along gradient
  double epsilonMin = 1e-6;	//Fit converged when step smaller than this
  double A=1,B=0,C=1;		//Fit reco hadron response fit parameters
  double Aer=0,Ber=0,Cer=0;	//Fit parameter uncertainties
  double ABer=0,ACer=0,BCer=0;	//Off-diag. elem.s of covariance matrix
  double E_O[2] = {0.15,0.45};	//Approx. offset energy: [0] IIa, [1] IIb
  #ifdef NIJ
  bool recoMissing = false;	//Fully reconstruct also particles not in jets
  #endif

  //Setters
  void SetA(double val) {A = val;}
  void SetB(double val) {B = val;}
  void SetC(double val) {C = val;}
  void SetABC(double Ain,double Bin,double Cin) {A=Ain;  B=Bin;  C=Cin;}
  void SetAer( double val) {Aer  = val;}
  void SetBer( double val) {Ber  = val;}
  void SetCer( double val) {Cer  = val;}
  void SetABer(double val) {ABer = val;}
  void SetACer(double val) {ACer = val;}
  void SetBCer(double val) {BCer = val;}
  void SetABCer(double Ain,double Bin,double Cin) {Aer=Ain;  Ber=Bin;  Cer=Cin;}
  void Setepsilon(   double val) {epsilon = val;}
  void SetepsilonMin(double val) {epsilonMin = val;}
  void SetuseEarlierCuts(bool flag) {useEarlierCuts = flag;}
  void SetuseD0ABC(  bool flag) {useD0ABC  = flag;}
  void SetdjFitting( bool flag) {djFitting = flag;}
  void SetgjFitting( bool flag) {gjFitting = flag;}
  void SetprintProg( bool flag) {printProg = flag;}
  void SetbEnrichedFiles(bool flag) {bEnrichedFiles=flag;}
  void SetcontHistos(bool flag) {contHistos=flag;}
  void SetMPFmode( bool flag ) {MPFmode = flag;}
  void Setrun(     string val) {run    = val; }
  void SetrunIIa(  bool flag ) {runIIa = flag;}
  void SetrunIIb(  bool flag ) {runIIb = flag;}
  void SetP20ToP17(bool flag ) {P20ToP17 = flag;}
  void SetStrangeB(bool flag ) {StrangeB = flag;}
  void SetAnsatz(  string val) {
   if (val=="pn" || val=="L" || val=="pi") Ansatz = val;
   else cout << "SetAnsatz: value "<< val << " unknown, wont apply" << endl;
  }
  #ifdef NIJ
  void SetrecoMissing(bool flag) {recoMissing = flag;}
  #endif
  void SetmaxIter(unsigned int val) {maxIter = val;}
  void Setverbose(bool flag) {verbose = flag;}

  //Getters
  double GetA() {return A;}
  double GetB() {return B;}
  double GetC() {return C;}
  double GetAer() {return Aer;}
  double GetBer() {return Ber;}
  double GetCer() {return Cer;}
  double GetABer() {return ABer;}
  double GetACer() {return ACer;}
  double GetBCer() {return BCer;}
  double Getepsilon()    {return epsilon;}
  double GetepsilonMin() {return epsilonMin;}
  bool GetuseEarlierCuts() {return useEarlierCuts;}
  bool GetuseD0ABC()  {return useD0ABC; }
  bool GetdjFitting() {return djFitting;}
  bool GetgjFitting() {return gjFitting;}
  bool GetprintProg() {return printProg;}
  bool GetbEnrichedFiles() {return bEnrichedFiles;}
  bool GetcontHistos() {return contHistos;}
  bool GetMPFmode()  {return MPFmode;}
  string Getrun()    {return run;}
  bool GetrunIIa()   {return runIIa;}
  bool GetrunIIb()   {return runIIb;}
  bool GetP20ToP17() {return P20ToP17;}
  bool GetStrangeB() {return StrangeB;}
  string GetAnsatz() {return Ansatz;}
  #ifdef NIJ
  bool GetrecoMissing() {return recoMissing;}
  #endif
  unsigned int GetmaxIter() {return maxIter;}
  bool Getverbose() {return verbose;}

  //Constructor and destructor
  D0JES(TTree *tree=0, string="");
  virtual ~D0JES();

  //ROOT TTree specific functions
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);

  //Other memeber functions
  virtual void     InputNameConstructor();
  virtual void     Loop();
  virtual void     FindFJtoMC();
  virtual void     Plot2D();
  virtual void     StudyTree();
  virtual void     PrintEvt();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
  void   axisSetupFJtoMC(TProfile2D* FJtoMC, string titleAdd);
  void   FitGN();		//Gauss-Newton fit function
  void   MultiLoop(D0JES* dj_in=NULL, D0JES* gj_in=NULL, bool fitPars=true);
  void   plotConvPT();
  void   plotPT(int gen=0,int alg=0,int rad=0,int ct=-1,int Nevt=0,int run=0,
                int P=0,  int XS=0, bool MConly=false, bool fitOnly=false    );
  void   plotSepPT();
  void   plotMPF(int gen=0,  int alg=0, int rad=0, int ct=-1,
                 int Nevt=0, int run=0, int P=0,   int XS=0  );
  void   plotD0();
  void   Response(int id, double pseudorap, double energy,   double pT,
	          TF1* frE, TF1* frMU, TF1* frG, TF1* frH, bool pos,
                  double fA, double fB, double fC, bool MC, bool FIT, bool EM,
                  vector<double>& retMC, vector<double>& retFIT,
                  vector<double>& retEM                                       );
  void   ParamReader(string file, int n1, int n2, int n3,
                     vector<vector<vector<double>>> &params);
  void   flavCorr(bool plot=true, int gen=0, int alg=0,
                  int rad=0,int ct=-1,int Nevt=0,int run=0,int P=0,int XS=0);
  void   FFplot();
  void   plotQuery(string& respStr, string& djstr,  string& gjstr,
                                    string& djstrb, string& gjstrb,
                   int& gen,  int& alg,  int& rad,  int& ct,
                   int& Nevt, int& run,  int& P,    int& XS        );
  bool   fidCuts(int id, double pT);
  bool   isNeutrino(int id);  //PDGID is for Neutrino
  bool   isStrangeB(int id);  //Check if PDGID is for Xi, Sigma or Omega^-
};

#endif

#ifdef D0JES_cxx

//A function to read hadron response function parameters from files
//Params:	file		The filename to read as a string
//		n1,n2,n3	Dimensions of the params tensor
//		params		Reference to the tensor to read parameters into
void D0JES::ParamReader(string file, int n1, int n2, int n3,
                        vector<vector<vector<double>>> &params)
{
  //Init temps to read into
  double p=0;
  vector<double> v;
  vector<vector<double>> M;		//Temp matrix
  vector<vector<vector<double>>> T;	//Temp tensor

  //Read the parameters from files
  ifstream in;
  string paramFile0, paramFile1;
  paramFile0 = "./spr_mc/" + (runIIa ? "RunIIa" : run) + file; 
  paramFile1 = "./spr_mc/" + run + "-P20ToP17" + file; 
  for (int paramSet=0; paramSet!=n1; ++paramSet) {
    if (paramSet==0) in.open(paramFile0);
    else             in.open(paramFile1);
    if (!in.is_open()) {
      cout << "Error opening " << (paramSet==0 ? paramFile0:paramFile1) << endl;
      return;
    }
    for (int lines=0; lines!=n2; ++lines) {
      in >> p;		//1st string on line is eta region low. bd., omit it
      for (int i=0; i!=n3; ++i) {
        in >> p;  v.push_back(p);
      }
      M.push_back(v);
      v.clear();
    }
    T.push_back(M);
    M.clear();
    in.close();
  }
  params = T;
  T.clear();

} //ParamReader

//Constructor
D0JES::D0JES(TTree *tree, string toRead) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  if (tree == 0) {	//No pointer to a tree given

    if (toRead != "") ReadName = toRead;
    else {
      //Add here all the files one should be able to study
      vector<string> files;	//To contain studiable filenames
      files.push_back(              "P6_dijet_D0rIIc_R05_ct10mm_1000000");
      files.push_back(           "P6_gammajet_D0rIIc_R05_ct10mm_1000000");
      files.push_back(   "P6_dijet_D0rIIc_R05_ct10mm_b-enriched_1000000");
      files.push_back("P6_gammajet_D0rIIc_R05_ct10mm_b-enriched_1000000");
      files.push_back(              "H7_dijet_D0rIIc_R05_ct10mm_2000000");
      files.push_back(           "H7_gammajet_D0rIIc_R05_ct10mm_2000000");

      //User interface
      printf("No filename given, choose file (y = default file):\n");
      for (int i=0; i!=files.size(); ++i) {
        if (i%4==0) printf("\n");
        printf("%2d:%56s\n", i, files[i].c_str());
      }
      printf("Want something else? Add it to the constructor in header\n");
      //Read input
      int input;	//For cin
      cin >> input;
      if (input < 0 || input >= files.size()) {	//Check for erroneous input
	cout<< "Invalid input! Chose 0" <<endl;
        input = 0;
      }
      ReadName = files[input];			//Pick the filename to study
    }
    OpenName = "./input_ROOT_files/" + inSubDir + ReadName + ".root";
    printf("Opening file: %s\n", OpenName.c_str());
    TFile *f = (TFile*)gROOT->GetListOfFiles()
			    ->FindObject(OpenName.c_str());
    if (!f || !f->IsOpen()) f = new TFile(OpenName.c_str());
    if (ReadName.find("P6")!=string::npos) f->GetObject("Pythia6Jets",tree);
    else f->GetObject("HerwigTree",tree);

  }
  Init(tree);	//Setup branch adresses etc.

  if (runIIa && runIIb) {
    cout << "Error: both run IIa and IIb enabled" << endl;
    return;
  }

  /* Read D0 data and MC points */

  ifstream inPTBdata_dj_a;   ifstream inPTBMC_dj_a;   //RunIIa dijet
  ifstream inPTBdata_gj_a;   ifstream inPTBMC_gj_a;   // -||-  gammajet
  ifstream inPTBdata_dj_b1;  ifstream inPTBMC_dj_b1;  //RunIIb1 dijet
  ifstream inPTBdata_gj_b1;  ifstream inPTBMC_gj_b1;  // -||-  gammajet
  ifstream inPTBdata_dj_b2;  ifstream inPTBMC_dj_b2;  //RunIIb1 dijet
  ifstream inPTBdata_gj_b2;  ifstream inPTBMC_gj_b2;  // -||-  gammajet
  ifstream inPTBdata_dj_b34; ifstream inPTBMC_dj_b34; //RunIIb1 dijet
  ifstream inPTBdata_gj_b34; ifstream inPTBMC_gj_b34; // -||-  gammajet
  ifstream inMPFdata_gj;     ifstream inMPFMC_gj;     //MPF gammajet, run NA
  ifstream inMPFdata_dj;     ifstream inMPFMC_dj;
  ifstream inMPFdata_gj_R07; ifstream inMPFMC_gj_R07;
  //RunIIa pT-balance
  inPTBdata_dj_a.open("./data_and_MC_input/pTbal/runIIa/dijet_data");
  inPTBdata_gj_a.open("./data_and_MC_input/pTbal/runIIa/gammajet_data");
  inPTBMC_dj_a.open("./data_and_MC_input/pTbal/runIIa/dijet_MC");
  inPTBMC_gj_a.open("./data_and_MC_input/pTbal/runIIa/gammajet_MC");
  //RunIIb1 pT-balance
  inPTBdata_dj_b1.open("./data_and_MC_input/pTbal/runIIb1/dijet_data"   );
  inPTBdata_gj_b1.open("./data_and_MC_input/pTbal/runIIb1/gammajet_data");
  inPTBMC_dj_b1.open(  "./data_and_MC_input/pTbal/runIIb1/dijet_MC"     );
  inPTBMC_gj_b1.open(  "./data_and_MC_input/pTbal/runIIb1/gammajet_MC"  );
  //RunIIb2 pT-balance
  inPTBdata_dj_b2.open("./data_and_MC_input/pTbal/runIIb2/dijet_data"   );
  inPTBdata_gj_b2.open("./data_and_MC_input/pTbal/runIIb2/gammajet_data");
  inPTBMC_dj_b2.open(  "./data_and_MC_input/pTbal/runIIb2/dijet_MC"     );
  inPTBMC_gj_b2.open(  "./data_and_MC_input/pTbal/runIIb2/gammajet_MC"  );
  //RunIIb3-4 pT-balance
  inPTBdata_dj_b34.open("./data_and_MC_input/pTbal/runIIb3-4/dijet_data"   );
  inPTBdata_gj_b34.open("./data_and_MC_input/pTbal/runIIb3-4/gammajet_data");
  inPTBMC_dj_b34.open(  "./data_and_MC_input/pTbal/runIIb3-4/dijet_MC"     );
  inPTBMC_gj_b34.open(  "./data_and_MC_input/pTbal/runIIb3-4/gammajet_MC"  );
  //MPF
  inMPFdata_gj_R07.open("./data_and_MC_input/MPF/MPF_gammajet_data_R07");
  inMPFMC_gj_R07.open("./data_and_MC_input/MPF/MPF_gammajet_MC_R07");
  inMPFMC_dj.open("./data_and_MC_input/MPF/MPF_dijet_MC");
  inMPFMC_gj.open("./data_and_MC_input/MPF/MPF_gammajet_MC");
	//Whe D0 MPF detector data and MC points for different run epochs found,
	//support for them can be added here as above

  if (!inPTBdata_dj_a.is_open()   || !inPTBMC_dj_a.is_open()   ||
      !inPTBdata_gj_a.is_open()   || !inPTBMC_gj_a.is_open()   ||
      !inPTBdata_dj_b1.is_open()  || !inPTBMC_dj_b1.is_open()  ||
      !inPTBdata_gj_b1.is_open()  || !inPTBMC_gj_b1.is_open()  ||
      !inPTBdata_dj_b2.is_open()  || !inPTBMC_dj_b2.is_open()  ||
      !inPTBdata_gj_b2.is_open()  || !inPTBMC_gj_b2.is_open()  ||
      !inPTBdata_dj_b34.is_open() || !inPTBMC_dj_b34.is_open() ||
      !inPTBdata_gj_b34.is_open() || !inPTBMC_gj_b34.is_open() ||
      !inMPFMC_dj.is_open()       || !inMPFMC_gj.is_open()     ||
      !inMPFdata_gj_R07.is_open() || !inMPFMC_gj_R07.is_open()   )
  {
    cout << "Error opening D0 data/MC point files!" << endl;
    return;
  }
  
  for (int step=0; step != nD0data; ++step) {	//D0 pT-balance data
    inPTBdata_dj_a   >> djEpII[0][step] >> djD0II[0][step] >> djERII[0][step];
    inPTBdata_gj_a   >> gjEpII[0][step] >> gjD0II[0][step] >> gjERII[0][step];
    inPTBdata_dj_b1  >> djEpII[1][step] >> djD0II[1][step] >> djERII[1][step];
    inPTBdata_gj_b1  >> gjEpII[1][step] >> gjD0II[1][step] >> gjERII[1][step];
    inPTBdata_dj_b2  >> djEpII[2][step] >> djD0II[2][step] >> djERII[2][step];
    inPTBdata_gj_b2  >> gjEpII[2][step] >> gjD0II[2][step] >> gjERII[2][step];
    inPTBdata_dj_b34 >> djEpII[3][step] >> djD0II[3][step] >> djERII[3][step];
    inPTBdata_gj_b34 >> gjEpII[3][step] >> gjD0II[3][step] >> gjERII[3][step];
  }
  for (int step=0; step != nD0MC; ++step) {	//D0 pT-balance MC points
    inPTBMC_dj_a   >> djMCEpII[0][step] >> djD0MCII[0][step];
    inPTBMC_gj_a   >> gjMCEpII[0][step] >> gjD0MCII[0][step];
    inPTBMC_dj_b1  >> djMCEpII[1][step] >> djD0MCII[1][step];
    inPTBMC_gj_b1  >> gjMCEpII[1][step] >> gjD0MCII[1][step];
    inPTBMC_dj_b2  >> djMCEpII[2][step] >> djD0MCII[2][step];
    inPTBMC_gj_b2  >> gjMCEpII[2][step] >> gjD0MCII[2][step];
    inPTBMC_dj_b34 >> djMCEpII[3][step] >> djD0MCII[3][step];
    inPTBMC_gj_b34 >> gjMCEpII[3][step] >> gjD0MCII[3][step];
  }
  for (int step=0; step != nD0_MPF_R07; ++step) {	//MPF data points
    inMPFdata_gj_R07 >> gjEp_MPF_R07[step] >> gjD0_MPF_R07[step];
  }
  for (int step=0; step != nD0MC_MPF_R07; ++step) {	//MPF MC points
    inMPFMC_gj_R07 >> gjMCEp_MPF_R07[step] >> gjD0MC_MPF_R07[step];
  }
  for (int step=0; step != nD0MC_MPF; ++step) {	//MPF MC points
    inMPFMC_gj >> gjMCEp_MPF[step] >> gjD0MC_MPF[step];
    inMPFMC_dj >> djMCEp_MPF[step] >> djD0MC_MPF[step];
  }

  //Close the D0 MC / data point files
  inPTBdata_dj_a.close();    inPTBdata_dj_b1.close();
  inPTBdata_gj_a.close();    inPTBdata_gj_b1.close();
  inPTBMC_dj_a.close();      inPTBMC_dj_b1.close();
  inPTBMC_gj_a.close();      inPTBMC_gj_b1.close();
  inPTBdata_dj_b2.close();   inPTBdata_dj_b34.close();
  inPTBdata_gj_b2.close();   inPTBdata_gj_b34.close();
  inPTBMC_dj_b2.close();     inPTBMC_dj_b34.close();
  inPTBMC_gj_b2.close();     inPTBMC_gj_b34.close();
  inMPFdata_gj_R07.close();  inMPFMC_gj_R07.close();
  inMPFMC_dj.close();        inMPFMC_gj.close();

  /* Read params from files to matrices */

  //Open file and initializations for reading
  ifstream in0;	//RunIIa
  ifstream in1;	//RunIIb
  int line = 0;	//Stepper

  //Init dummies to read into
  double ap0=0,ap1=0,ap2=0,ap3=0,ap4=0,ap5=0;
  double bp0=0,bp1=0,bp2=0,bp3=0,bp4=0,bp5=0;

  //Param reading works the same way for all hadrons
  ParamReader("/photon.txt",   2, 32, 6, params_gam);
  ParamReader("/electron.txt", 2, 32, 5, params_e  );
  ParamReader("/muon.txt",     2, 32, 4, params_mu );
  ParamReader("/kaon.txt",     2, 32, 3, params_K  );
  ParamReader("/klong.txt",    2, 32, 3, params_KL );
  ParamReader("/kshort.txt",   2, 32, 3, params_KS );
  ParamReader("/lambda.txt",   2, 32, 3, params_L  );
  ParamReader("/neutron.txt",  2, 32, 3, params_n  );
  ParamReader("/pion.txt",     2, 32, 3, params_pi );
  ParamReader("/proton.txt",   2, 32, 3, params_p  );

  /* Plug fit parameter values here for fit reco */
  if (runIIa) {
    //D0 runIIa 0<|eta|0.4 fit params
    if      (useD0ABC       ) {A  =1.409;     B  =0.0017;      C  =0.9973;
                               Aer=0.020468;  Ber=0.00710429;  Cer=0.0929739;}
    //Initial guess to start fitting from
    else if (useInitGuessABC) {A   = 1.4;     B   = 0.0;       C = 1.0;
                               Aer = 0.0;     Ber = 0.0;       Cer = 0.0;    }
    //Default: use our A,B,C depending on generator
    else {
      if (ReadName.find("P6")!=string::npos) {
/*
        //P6 G-N: chi2/n_d0f=1.84271 RunIIa	LC=1/pow(0.01,2), LB=0, LA=0
        A    =  1.48853;        B    = -0.00438068;     C    = 1.00328;
        Aer  =  0.0580336;	Ber  =  0.0261882;      Cer  = 0.00704791;
        ABer = -0.00143444;	ACer = -6.22493e-05;	BCer = 8.43637e-05;
*/
/*
        //P6G-N: chi2/n_d0f=2.2961 RunIIa 190327 dijet tag.eta<0.4
        A    = 1.45924;		B    = 0.0039269;	C = 1.00393;
        Aer  = 0.0576814;	Ber  = 0.0263632;	Cer = 0.00704882;
        ABer = -0.00143583;	ACer =-5.93105e-05;	BCer = 8.37236e-05;
*/
        //P6 G-N: chi2/n_d0f=2.28613 RunIIa dijet tag.eta<0.4 C constr 0.015
        A    = 1.45399;	B    = 0.0115405;	C    = 1.00865;
        Aer  = 0.0572361;	Ber  = 0.0287636;	Cer  = 0.0105337;
        ABer = -0.00148885;	ACer = -0.000124997;	BCer = 0.000180898;
      } else if (ReadName.find("H7")!=string::npos) {
/*
        //H7G-N: chi2/n_d0f=2.39969 RunIIa 190327 dijet tag.eta<0.4
        A = 1.23648;		B = 0.0290488;		C = 1.00211;
        Aer = 0.0563003;	Ber = 0.0296926;	Cer = 0.00706082;
        ABer = -0.0015739;	ACer = -3.61654e-05;	BCer = 8.54972e-05;
*/
        //H7 G-N: chi2/n_d0f=2.39668 RunIIa tag.eta<0.4 C constr 0.015
        A    = 1.23482;	B    = 0.0333262;	C    = 1.0047;
        Aer  = 0.0559627;	Ber  = 0.0321562;	Cer  = 0.0105726;
        ABer = -0.00160909;	ACer = -7.75965e-05;	BCer = 0.00018794;

      } else cout << "\nWARNING: unknown fit parameters!\n" << endl;
    }
  } else if (runIIb) {
    //D0 runIIb 0<|eta|0.4 fit params
    if (useD0ABC) {
      if      (run == "RunIIb1" ) {A   = 2.1682;  B   =-0.310;   C   = 1.0228;
                                   Aer = 0.2854;  Ber = 0.1005;  Cer = 0.0142;}
      else if (run == "RunIIb2" ) {A   = 1.9325;  B   =-0.1611;  C   = 1.0585;
                                   Aer = 0.1335;  Ber = 0.0571;  Cer = 0.0153;} 
      else if (run == "RunIIb34") {A   = 1.4687;  B   = 0.0040;  C   = 1.0729;
                                   Aer = 0.0500;  Ber = 0.0301;  Cer = 0.0143;}
    //Initial guess to start fitting from
    } else if (useInitGuessABC  ) {
      if (ReadName.find("P6")!=string::npos) {
        A   = 2.17;       B   = -0.31;   C   = 1.0;
        Aer = 0.0;        Ber = 0.0;     Cer = 0.0;
      }  else if (ReadName.find("H7")!=string::npos) {
        A   = 0.83;       B   = 0.07;    C   = 1.00;
        Aer = 0.0;        Ber = 0.0;     Cer = 0.0;
      }
    //Default: use our A,B,C depending on generator and epoch
    } else {
      if (ReadName.find("P6")!=string::npos) {
        if (run == "RunIIb1") {
/*
          //P6 G-N: chi2/n_d0f=6.98874 RunIIb1-P20ToP17 sigma_C=0.01
          A    =  1.44718;      B    = -0.132045;       C    = 1.00937;
          Aer  =  0.0633509;    Ber  =  0.035115;       Cer  = 0.0070287;
          ABer = -0.00208156;   ACer = -0.000135351;	BCer = 0.000150421;
*/
/*
          //P6G-N: chi2/n_d0f=7.71008 RunIIb1-P20ToP17 190327 dijet tag.eta<0.4
          A    = 1.41964;	B    =-0.12901;		C    = 1.00905;
          Aer  = 0.0634915;	Ber  = 0.035804;	Cer  = 0.00703395;
          ABer =-0.00212723;	ACer =-0.000134499;	BCer = 0.000152899;
*/
          //P6 G-N: chi2/n_d0f=7.64468 RunIIb1-P20ToP17 tag.eta<0.4 C con. 0.015
          A    = 1.38622;	B    = -0.0965625;	C    = 1.0196;
          Aer  = 0.0620556;	Ber  = 0.0397675;	Cer  = 0.0104943;
          ABer = -0.00224026;	ACer = -0.000258386;	BCer = 0.000308332;

        } else if (run == "RunIIb2") {
/*
          //P6 G-N: chi2/n_d0f=9.19018 RunIIb2-P20ToP17 sigma_C=0.01
          A    = 1.75924;      B    =-0.23483;        C    = 1.01171;
          Aer  = 0.0714513;    Ber  = 0.0363645;      Cer  = 0.00700499;
          ABer =-0.00245272;	ACer =-0.000219493;    BCer = 0.000178072;
*/
/*
          //P6G-N: chi2/n_d0f=10.1442 RunIIb2-P20ToP17 190327 dijet tag.eta<0.4
          A    = 1.75202;       B    =-0.241152;	C    = 1.01107;
          Aer  = 0.0727587;	Ber  = 0.037401;	Cer  = 0.0070121;
          ABer =-0.00257112;	ACer =-0.000225806;	BCer = 0.000183722;
*/
          //P6 G-N: chi2/n_d0f=10.0504 RunIIb2-P20ToP17 tag.eta<0.4 C con. 0.015
          A    = 1.67477;	B    = -0.192474;	C    = 1.02349;
          Aer  = 0.0722089;	Ber  = 0.0424363;	Cer  = 0.0104384;
          ABer = -0.00285822;	ACer = -0.000419891;	BCer = 0.000359969;
        } else if (run == "RunIIb34") {
/*
          //P6 G-N: chi2/n_d0f=5.6846 RunIIb34-P20ToP17 sigma_C=0.01
          A    =  1.68785;      B    = -0.214582;       C    = 1.00871;
          Aer  =  0.0685093;    Ber  =  0.0357547;      Cer  = 0.00700611;
          ABer = -0.00230188;   ACer = -0.000205659;    BCer = 0.000175358;
*/
/*
          //P6 G-N: chi2/n_d0f=6.4696 RunIIb34-P20ToP17 190327 dijet tag.eta<0.4
          A    = 1.6596;	B    =-0.213225;	C    = 1.00838;
          Aer  = 0.0687585;	Ber  = 0.0364689;	Cer  = 0.00701444;
          ABer =-0.00235616;	ACer =-0.000205617;	BCer = 0.000178719;
*/
          //P6 G-N: chi2/n_d0f=6.41513 RunIIb34-P20ToP17 tag.eta<0.4 C con 0.015
          A    = 1.60543;	B    = -0.176375;	C    = 1.01797;
          Aer  = 0.0695504;	Ber  = 0.0423024;	Cer  = 0.0104403;
          ABer = -0.00272743;	ACer = -0.000396289;	BCer = 0.000359408;

        }
      } else if (ReadName.find("H7")!=string::npos) {
        /* H7 sigma_C = 0.01 */
        if (run == "RunIIb1") {
/*
          //H7G-N: chi2/n_d0f=3.51587 RunIIb1-P20ToP17
          A    = 1.26877;	B    =-0.143465;	C    = 1.00265;
          Aer  = 0.0648385;	Ber  = 0.0414433;	Cer  = 0.00705229;
          ABer =-0.00250486;	ACer =-0.0001222;	BCer = 0.000171442;
*/
/*
          //H7G-N: chi2/n_d0f=7.82295 RunIIb1-P20ToP17 190327 dijet tag.eta<0.4
          A    = 1.08654;	B    =-0.0684344;	C    = 1.00327;
          Aer  = 0.0568077;	Ber  = 0.0396991;	Cer  = 0.00705623;
          ABer =-0.00208236;	ACer =-8.22724e-05;	BCer = 0.000155064;
*/
          //H7 G-N: chi2/n_d0f=7.81315 RunIIb1-P20ToP17 tag.eta<0.4 C con 0.015
          A    = 1.07966;	B    = -0.0564157;	C    = 1.00719;
          Aer  = 0.0567488;	Ber  = 0.045132;	Cer  = 0.010559;
          ABer = -0.00225655;	ACer = -0.000172125;	BCer = 0.00033327;
        } else if (run == "RunIIb2") {
/*
          //H7G-N: chi2/n_d0f=5.15999 RunIIb2-P20ToP17
          A    = 1.51114;	B    =-0.234668;	C    = 1.00358;
          Aer  = 0.06988;	Ber  = 0.0417084;	Cer  = 0.00703734;
          ABer =-0.0027265;	ACer =-0.000188845;	BCer = 0.00019768;
*/
/*
          //H7G-N: chi2/n_d0f=10.2327 RunIIb2-P20ToP17 190327 dijet tag.eta<0.4
          A    = 1.31592;	B    =-0.173279;	C    = 1.0036;
          Aer  = 0.0626151;	Ber  = 0.0407405;	Cer  = 0.00704417;
          ABer =-0.00236347;	ACer =-0.000148036;	BCer = 0.000187418;
*/
          //H7 G-N: chi2/n_d0f=10.2183 RunIIb2-P20ToP17 tag.eta<0.4 C con 0.015
          A    = 1.29986;	B    = -0.156661;	C    = 1.00784;
          Aer  = 0.0643688;	Ber  = 0.0483232;	Cer  = 0.0105222;
          ABer = -0.0028136;	ACer = -0.000308217;	BCer = 0.000398856;

        } else if (run == "RunIIb34") {
/*
          //H7G-N: chi2/n_d0f=2.46422 RunIIb34-P20ToP17
          A    = 1.40175;	B    =-0.18957;		C    = 1.00206;
          Aer  = 0.0642744;	Ber  = 0.0396785;	Cer  = 0.00704051;
          ABer =-0.00236792;	ACer =-0.000161143;	BCer = 0.000185885;
*/
/*
          //H7G-N: chi2/n_d0f=6.09385 RunIIb34-P20ToP17 190327 dijet tag.eta<0.4
          A    = 1.22079;	B    =-0.12888;		C    = 1.00236;
          Aer  = 0.0574284;	Ber  = 0.0386396;	Cer  = 0.00704679;
          ABer =-0.00203803;	ACer =-0.000122733;	BCer = 0.000175041;
*/
          //H7 G-N: chi2/n_d0f=6.08783 RunIIb34-P20ToP17 tag.eta<0.4 C con 0.015
          A    = 1.21223;	B    = -0.11859;	C    = 1.00518;
          Aer  = 0.0591748;	Ber  = 0.0462266;	Cer  = 0.0105287;
          ABer = -0.00243865;	ACer = -0.000261209;	BCer = 0.000378679;
        }
      } else cout << "\nWARNING: unknown fit parameters!\n" << endl;
/*
    //P6 sigma_C=0.02
    if (run == "RunIIb1") {
      //G-N: chi2/n_d0f=6.8445 RunIIb1-P20ToP17
      A = 1.37688;	B = -0.062586;		C = 1.03392;
      Aer = 0.059592;	Ber = 0.0418771;	Cer = 0.0138767;
    } else if (run == "RunIIb2") {
      //G-N: chi2/n_d0f=8.99029 RunIIb2-P20ToP17
      A = 1.61032;	B = -0.136087;		C = 1.04016;
      Aer = 0.0684915;	Ber = 0.0443471;	Cer = 0.0137736;
    } else if (run == "RunIIb34") {
      //G-N: chi2/n_d0f=5.56895 RunIIb34-P20ToP17
      A = 1.57573;	B = -0.137047;		C = 1.03069;
      Aer = 0.0686895;	Ber = 0.0457585;	Cer = 0.0137586;
    }
    //P6 sigma_C=0.05
    if (run == "RunIIb1") {
      //G-N: chi2/n_d0f=6.21719 RunIIb1-P20ToP17
      A = 1.23686;	B = 0.134131;		C = 1.15244;
      Aer = 0.0377558;	Ber = 0.0400128;	Cer = 0.0333264;
    } else if (run == "RunIIb2") {
      //G-N: chi2/n_d0f=8.24218 RunIIb2-P20ToP17
      A = 1.35843;		B = 0.0978804;		C = 1.16306;
      Aer = 0.0426116;	Ber = 0.0422117;	Cer = 0.0331325;
    } else if (run == "RunIIb34") {
      //G-N: chi2/n_d0f=5.10582 RunIIb34-P20ToP17
      A = 1.3504;		B = 0.0714475;		C = 1.12938;
      Aer = 0.0478388;	Ber = 0.0484399;	Cer = 0.0329338;
    }
*/

    }
  } else cout << "\n\n\tERROR: neither run IIa nor IIb activated!\n\n" << endl;
} //Constructor
//-----------------------------------------------------------------------------
//Destructor
D0JES::~D0JES()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}
//-----------------------------------------------------------------------------
Int_t D0JES::GetEntry(Long64_t entry)
{
// Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
//-----------------------------------------------------------------------------
Long64_t D0JES::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
     fCurrent = fChain->GetTreeNumber();
     Notify();
  }
  return centry;
}
//-----------------------------------------------------------------------------
void D0JES::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set object pointers initially to NULL
  //Particle lvl
  prtcl_jet = 0;
  prtcl_pdgid = 0;
  prtcl_pt = 0;
  prtcl_eta = 0;
  prtcl_phi = 0;
  prtcl_e = 0;
  //Particles not in jets
  #ifdef NIJ
  prtclnij_pdgid = 0;
  prtclnij_pt = 0;
  prtclnij_eta = 0;
  prtclnij_phi = 0;
  prtclnij_e = 0;
  #endif
  //Parton lvl
  prtn_jet = 0;
  prtn_pdgid = 0;
  prtn_tag = 0;
  prtn_pt = 0;
  prtn_eta = 0;
  prtn_phi = 0;
  prtn_e = 0;
  prtn_dr = 0;
  //Jet lvl
  jet_pt = 0;
  jet_eta = 0;
  jet_phi = 0;
  jet_e = 0;
  jet_constituents = 0;
  jet_ptd = 0;
  jet_sigma2 = 0;
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("weight", &weight, &b_weight);	//Event weight
  //Particle lvl  
  fChain->SetBranchAddress("prtcl_jet", &prtcl_jet, &b_prtcl_jet);
  fChain->SetBranchAddress("prtcl_pdgid", &prtcl_pdgid, &b_prtcl_pdgid);
  fChain->SetBranchAddress("prtcl_pt", &prtcl_pt, &b_prtcl_pt);
  fChain->SetBranchAddress("prtcl_eta", &prtcl_eta, &b_prtcl_eta);
  fChain->SetBranchAddress("prtcl_phi", &prtcl_phi, &b_prtcl_phi);
  fChain->SetBranchAddress("prtcl_e", &prtcl_e, &b_prtcl_e);
  //Particles not in jets lvl
  #ifdef NIJ
  fChain->SetBranchAddress("prtclnij_pdgid",&prtclnij_pdgid, &b_prtclnij_pdgid);
  fChain->SetBranchAddress("prtclnij_pt", &prtclnij_pt, &b_prtclnij_pt);
  fChain->SetBranchAddress("prtclnij_eta", &prtclnij_eta, &b_prtclnij_eta);
  fChain->SetBranchAddress("prtclnij_phi", &prtclnij_phi, &b_prtclnij_phi);
  fChain->SetBranchAddress("prtclnij_e", &prtclnij_e, &b_prtclnij_e);
  #endif
  //Parton lvl
  fChain->SetBranchAddress("prtn_jet", &prtn_jet, &b_prtn_jet);
  fChain->SetBranchAddress("prtn_pdgid", &prtn_pdgid, &b_prtn_pdgid);
  fChain->SetBranchAddress("prtn_tag", &prtn_tag, &b_prtn_tag);
  fChain->SetBranchAddress("prtn_pt", &prtn_pt, &b_prtn_pt);
  fChain->SetBranchAddress("prtn_eta", &prtn_eta, &b_prtn_eta);
  fChain->SetBranchAddress("prtn_phi", &prtn_phi, &b_prtn_phi);
  fChain->SetBranchAddress("prtn_e", &prtn_e, &b_prtn_e);
  fChain->SetBranchAddress("prtn_dr", &prtn_dr, &b_prtn_dr);
  //Jet lvl
  fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
  fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
  fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
  fChain->SetBranchAddress("jet_e", &jet_e, &b_jet_e);
  fChain->SetBranchAddress("jet_constituents", &jet_constituents,
						&b_jet_constituents);
  fChain->SetBranchAddress("jet_ptd", &jet_ptd, &b_jet_ptd);
  fChain->SetBranchAddress("jet_sigma2", &jet_sigma2, &b_jet_sigma2);
  fChain->SetBranchAddress("met", &met, &b_met);
  Notify();
}
//-----------------------------------------------------------------------------
Bool_t D0JES::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}
//-----------------------------------------------------------------------------
void D0JES::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
//-----------------------------------------------------------------------------
Int_t D0JES::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
  return 1;
}
#endif // #ifdef D0JES_cxx
