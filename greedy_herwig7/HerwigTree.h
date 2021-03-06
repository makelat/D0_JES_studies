/////////////////////////////////////////////////////////////////////
/// @file HerwigTree.h
///
/// @author Hannu Siikonen (errai- \@GitHub, hsiikone \@GitLab)
/// @date 1.11.2017 (Last modification; fun times)
/// @date 1.11.2017 (Finishing touches for the greedy Herwig 7 code)
/// @date 17.7.2018 (Fixed particle adding -Toni Mäkelä, tmakela \@GitLab)
/////////////////////////////////////////////////////////////////////

#ifndef HERWIGTREE_H
#define HERWIGTREE_H

/* Herwig stuff */
#include "ThePEG/Handlers/AnalysisHandler.h"
#include "ThePEG/EventRecord/Event.h"
#include "ThePEG/EventRecord/Particle.h"
#include "ThePEG/EventRecord/StandardSelectors.h"
#include "ThePEG/Interface/ClassDocumentation.h"
#include "ThePEG/Repository/CurrentGenerator.h"
#include "ThePEG/Repository/EventGenerator.h"
#include "ThePEG/Repository/UseRandom.h"
#include "ThePEG/Utilities/DescribeClass.h"
#include "ThePEG/PDT/StandardMatchers.h"
#include "ThePEG/PDT/EnumParticles.h"
#include "ThePEG/Config/Unitsystem.h"
#include "ThePEG/Persistency/PersistentOStream.h"
#include "ThePEG/Persistency/PersistentIStream.h"

/* ROOT stuff */
#include "TROOT.h"
#include "TChain.h"
#include "TTree.h"
#include "TMatrix.h"
#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"
#include "TMath.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TVector2.h"
#include "TBranch.h"

#include "../generic/help_functions.h"
#include "../events/PrtclEvent.h"

/* stdlib */
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <typeinfo>

/* Fastjet */
#include "fastjet/config.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/GridMedianBackgroundEstimator.hh"
#include "fastjet/D0RunIIConePlugin.hh"
#include "fastjet/internal/numconsts.hh"

using std::string;
using std::vector;
using std::pair;
using std::map;
using std::cerr;
using std::cout;
using std::endl;
using std::runtime_error;

// Configuration settings (the program is short enough to define them simply here)
namespace cfg {
    /* Debugging tools */
    constexpr bool Debug        = false; // Mostly print some specific info
    constexpr bool DebugPartons = false; // Print parton info
    constexpr bool DebugGViz    = false; // Print graphviz event info

    /* Generic flags and settings */
    constexpr bool CutMode = true;  // For PTD and Sigma2: do we apply detector-like cuts

    /* Do we add corrected momentum values for partons (idx 1)? */
    constexpr bool DOCorrParton   = false;
    /* Do we add uncorrected momentum values for signal leptons (idx 3)? */
    constexpr bool DOUnCorrLepton = false;

    /* Specific settings for Toni's D0 studies (uncomment if to be used) */
    constexpr bool DOD0      = true;  // General D0 settings (energy & so on)
    constexpr bool DOD0RIIC  = true;  // Usage of D0 RunII Cone
    constexpr bool BEnriched = false;  // Produce a b-jet enriched sample?
    constexpr bool EtaCut    = false; // Demand min amount of |eta|<0.4 jets
    constexpr bool PtCut     = false; // Reject events w/ high-pT extra jets
    constexpr bool PSCut     = false; // Use prtn lvl phase-space eta cut

    /* Usage of final-state particles */
    #define STORE_PRTCLS             // Do we store the final state particles?
    constexpr bool StoreNIJ = true;  // Store particles not in jets (NIJ), REQUIRES STORE_PRTCLS

    /* Coefficient for scaling the momentum of "ghosts" */
    constexpr double GhostCoeff = pow(10,-18);

    constexpr double RCone = DOD0 ? 0.5 : 0.4;

    /* Tag photon cuts for JES studies */
    constexpr double GammaEtaD0Max = 1.0;
    constexpr double GammaPtD0Min  = 7.0;

    /* Photon isolation parameters (gamma+jets) */
    constexpr double GammaDR  = 0.1;
    constexpr double GammaLim = 0.1;
    constexpr double GammaPt  = 30;
    constexpr double GammaEta = 2.1;

    /* Muon isolation parameters (Zmumu+jets) */
    constexpr double MuonDR  = 0.1;
    constexpr double MuonLim = 0.1;
    constexpr double MuonPt  = 30;
    constexpr double MuonEta = 2.1;

    /* Lepton isolation parameters (ttbarlepton+jets) */
    constexpr double ElDR = 0.3;
    constexpr double MuDR = 0.4;
    constexpr double ElLim = 0.12; /* Loose version: 0.20 */
    constexpr double MuLim = 0.15; /* Loose version: 0.25 */
    /* Worst case kind of cuts for bonus leptons */
    constexpr double LeptPt = 5;
    constexpr double LeptEta = 2.4;

    /* General limits for "nuisance leptons/photons" that are taken into account for isolation. */
    constexpr double NuisancePt  = 7.0;
    constexpr double NuisanceEta = 5.0;

    /* Threshold for PtCut (D0 studies) */
    constexpr double JetPtThres = 11;

    /* Pt limit to be used while checking the isolation vs. nearby jets */
    constexpr double MinJetVisiblePt = 15.0;
    /* Pt limit to be used for fastjet clustering. */
    constexpr double MinJetClustPt   = 6.0;

}

/////////////////////////////////////////////////////////////////////
/// A struct for storing parton information
/////////////////////////////////////////////////////////////////////
struct PartonHolder {
/// Parton four momentum.
    fastjet::PseudoJet p4;
/// Parton pdg id.
    int id;
/// The reason for which the parton was saved.
/// - 0: ME Parton
/// - 1: ME Parton with final-state momentum
/// - 2: ME Lepton or photon
/// - 3: ME Lepton or photon with final-state momentum
/// - 4: Descendant of ME parton
/// - 5: Descendant of ME parton with final-state momentum
/// - 6: Subjects for isolation (photons or leptons)
/// - 7: bottom neutrinos
/// - 8: Other neutrinos
/// - 9: Ghost bottom hadrons
    char tag;
/// The index of the parent parton in the program flow (0 if no parent).
    int ptnid;
/// The index of the current parton in the program flow.
    int ownid;
/// Indicate whether the parton is already in the use of a jet.
    bool used;
};



namespace Herwig {


using namespace ThePEG;

bool IsLastInShower(const Particle & p) {
    return p.children().size() > 1 and p.children()[0]->id() != p.id() and p.children()[1]->id() != p.id();
}

struct TTBar {
    static bool AllCollisions() {return false;}
    static bool AllSteps() {return true;}
    static bool FinalState() {return false;}
    static bool Intermediate() {return true;}
    static bool Check(const Particle &p) {return abs(p.id())== ParticleID::t and IsLastInShower(p);}
};

/////////////////////////////////////////////////////////////////////
/// Class structure for storing Herwig7 particle data into trees.
///
/// Modes of operation:
///
///     0: Generic events
///         - Normal QCD production for general studies
///
///     1: Dijet events
///         - Normal QCD production with dijet-specific settings
///
///     2: Photon+Jet events
///         - Photon+Jet production
///
///     3: Zmumu+Jet events
///         - Z+Jet production with a Z -> mumu setting
///
///     4: Ttbarlepton+jet events
///         - Ttbar production with WW -> qqbarllbar
///
///     5: Ttbarlepton+jet events \@POWHEG
///         - Ttbar production with WW -> qqbarllbar
///
///     6: Ttbarlepton+jet events \@MADGRAPH5aMC\@NLO
///         - Ttbar production with WW -> qqbarllbar
/////////////////////////////////////////////////////////////////////
class HerwigTree: public AnalysisHandler {

public:

    HerwigTree() {}

    /** Analyze a given Event.
     * @param event Pointer to the Event to be analyzed.
     * @param ieve The event number.
     * @param loop The number of times this event has been presented.
     * @param state Nonzero if the event has been manipulated. */
    virtual void analyze(tEventPtr event, long ieve, int loop, int state);

    /** Standard Init function, called exactly once. */
    static void Init() { static ClassDocumentation<HerwigTree> documentation("The HerwigTree class is intended for saving gen-level data into tree structures."); }

protected:

    virtual IBPtr clone() const { return new_ptr(*this); }
    virtual IBPtr fullclone() const { return new_ptr(*this); }

    /** Initialization (open files etc.). */
    virtual void doinitrun();
    /** Finishing (close files etc.). */
    virtual void dofinish();

    /** @name Fragmentation of the analysis process. */
    //@{
    /** Clear storages etc. before analysis */
    void Initialize();
    /** Analyze the hard process.
     * @return true if Hard process is good, false otherwise. */
    bool HardProc();
    /** Analyze the final state.
     * @return true if the final state is good, false otherwise. */
    bool FinalState();
    /** Process and analyze the jets.
     * @return true if the jet loop goes well, false otherwise. */
    bool JetLoop();
    /** Process and isolation conditions.
     * @return true if everything goes well, false otherwise. */
    bool IsolationProc();
    /** Isolation from photons.
     * @return true if everything goes well, false otherwise. */
    bool IsolationPhotons(int iprt, fastjet::PseudoJet &ipart);
    /** Isolation from muons.
     * @return true if everything goes well, false otherwise. */
    bool IsolationMuons  (int iprt, fastjet::PseudoJet &ipart);
    /** Isolation from leptons.
     * @return true if everything goes well, false otherwise. */
    bool IsolationLeptons(int iprt, fastjet::PseudoJet &ipart);
    /** Check the eta-phi distance of isolated objects to jets.
    * @return true if everything went ok. */
    bool IsolationFromJets();
    //@}

    /** @name Event-type specific analysis actions. */
    //@{
    /** Add a photon in a gamma+jets event.
     * @param gamma The signal photon. */
    bool GammaAdd(tPPtr gamma);
    /** Add a muon in a Zmumu+jets event.
     * @param muon A signal muon. */
    bool MuonAdd(tPPtr muon);
    /** Add a lepton in a ttbarlepton+jets event.
     * @param lepton A lepton associated with W decay. */
    bool LeptonAdd(tPPtr lepton, int parent = -1);
    //@}

    /** @name Helper methods for the analysis */
    //@{
    /** A function for finding b neutrinos
     * @param parrt The b-parton to study. */
    void BNeutrinos(const tPPtr& part);
    /** ThePEG does not provide useful status codes and the status has to be studied manually.
     * This method is a mock-up of the CMSSW-way to calculate the status code.
     * @param part The particle to study.
     * @return 1 for final state, 2 for intermediate and 3 for unusable intermediate.*/
    int GetStatusCode(const tPPtr& part) const;

    /** A function that checks whether a photon is originated from a pi0 and that
      * the energy of the photon-pair corresponds to the pion. returns 0 if
      * the origin is not a pion with good energy and 1 if it is.
      * @param gamma The photon to study.
      * @return false if not pi0 photon, otherwise true.*/
    bool GammaChecker(const tPPtr& gamma) const;

    /** Does the current hadron have a daughter of the given flavor.
      * See HadronAndPartonSelector.cc in cmssw for reference.
      * @param part The given hadron.
      * @param quarkId The Quark Id to study (1,2,3,4,5) .
      * @return -1 when the given hadron is not of the given flavor, 1 when there is a daughter and 0 when not. */
    int IsExcitedHadronState(const tPPtr& part, int quarkId) const;
    //@}

    /** @name Functions for adding stuff to be saved */
    //@{
    /** Add a final-state particle to the output file.
     * @param part The particle to add.
     * @param jetid Index of the jet with which the particle is associated. */
    void ParticleAdd(const tPPtr& part, char jetid);
    /** Add a parton to the output file.
     * @param part The parton to add.
     * @param jetid Index of the jet with which the particle is associated.
     * @param tag The reason for which the particle was saved (explained in PartonHolder).
     * @param ptnid The number of the parent parton. Zero if no parent.
     * @param ownid The number of the current parton.
     * @see PartonHolder */
    void PartonAdd(const tPPtr& part, char jetid, char tag, int ptnid = -1, int ownid = -1);
    /** Add a parton to the output file.
     * @param num The parton to add (index in mPartonInfo).
     * @param jetid Index of the jet with which the particle is associated.
     * @see PartonHolder */
    void PartonAdd(unsigned num, char jetid);
    /** Add a jet to the output file.
     * @param jet The index of the jet.
     * @param spoil A special function is triggered. */
    void JetAdd(unsigned jet, int spoil = 0);
    /** Add a parton to mPartonInfo.
     * @param p4 Four momentum of the parton.
     * @param part The parton to be added.
     * @param tag The reason for saving the parton.
     * @param ptnid The number of the parent of the current parton (-1 if none).*/
    void PartonAppend(fastjet::PseudoJet p4, tPPtr part, char tag, int ptnid=-1);
    //@}

    /** @name Miscellaneous helper functions */
    //@{

    /** Cuts for jet property calculations. */
    void                Cuts();
    /** Fragmentation function calculation. */
    double              PTD();
    /** Calculation of jet inner radius. */
    double              Sigma2();
    /** Helper function for Cuts. */
    bool                IsCharged(int pdgid) const;
    /** Helper function for Cuts. */
    bool                IsHadron(int pdgid) const;
    /** Helper function for PartonDescend. */
    bool                IsParton(int pdgid) const;
    /** Helper function for OptimalParton. */
    bool                IsLepton(int pdgid) const;
    /** Find the partonic descendants of the current parton.
     * @param part The initial parton. */
    void PartonDescend(tPPtr part);
    /** Find the last descendant before a splitting.
     * @param part The original parton. */
    tPPtr OptimalParton(tPPtr part) const;
    /** Calculate corrected momentum from final-state descendants.
     * @param psum The value of the momentum sum.
     * @param parent The current parent to study. */
    Lorentz5Momentum FinalMomenta(tPPtr parent) const;
    /** Calculate corrected momentum from final-state parton descendants.
     * @param psum The value of the momentum sum.
     * @param parent The current parent to study. */
    Lorentz5Momentum FinalPMomenta(tPPtr parent) const;
    /** Print the parents of particle part
     * @param part The particle to study.*/
    void Print_parents(const tPPtr& part) const;
    /** Iterative printing function
     * @param part The current parton.
     * @param gen The generation of the current parton. */
    void Iter_print(tPPtr part, unsigned gen) const;
    /** Is the given value absent from mSpecialIndices?
     * @param num The current particle number. */
    bool Absent(unsigned int num) const;
    /** Problems in the creation.
     * @param msg The "problem".
     * @param wgt The current event weight. */
    void AddMessage(string msg, double wgt);
    /** A TLorentzVector from particle pointer.
     * @param part The particle to TLorentzify */
    TLorentzVector TLorentzify(const tPPtr& part) const;
    /** A PseudoJet from particle pointer.
     * @param part The particle to PseudoJettify. */
    fastjet::PseudoJet PseudoJettify(const tPPtr& part) const;
    /** A PsudoJet from a TLorentzVector.
     * @param p4 The TLorentzVector to PseudoJettify. */
    fastjet::PseudoJet PseudoJettify(TLorentzVector p4) const;
    //@}
private:

    /** The static object used to initialize the description of this class. */
    static NoPIOClassDescription<HerwigTree> initHerwigTree;

    /** The assignment operator is private and must never be called. */
    HerwigTree & operator=(const HerwigTree &);

    /** Find the descendants of t partons
     * @param part The given t parton.
     * @param lvl The level of nesting (starts at 1). */
    vector<tPPtr> Probe(tPPtr part, int lvl=1) const;

    /* To be called only just before "return". */
    void Repeat() {
        if (!generator()->repeatEvent())
            throw runtime_error("Repeating events does not work.");
    }

protected:

    /** @name Variables for the analysis */
    //@{

    /* The hardest event */
    tEventPtr                    mEvent;
    tcEventBasePtr               mHardHandler;
    vector<tPPtr>                mFinals;

    TFile                       *mFile;
    TTree                       *mTree;

    int                          mNumEvents;
    int                          mMode;
    int                          mTimerStep;
    Timer                        mTimer;

    vector<unsigned>             mSpecialIndices;

    char                         mBNuCount;
    char                         mONuCount;
    char                         mNuOB;
    char                         mNuOC;
    char                         mNuOLept;
    char                         mNuOOther;
    char                         mNuB;
    char                         mNuC;
    char                         mNuLept;
    char                         mNuOther;
    char                         mInfo;
    float                        mWeight;
    float                        mPtHat;
    vector<float>                mIsolation;
    /* Particle level for the particles within jets. */
    vector<unsigned char>        mJetId;
    vector<int>                  mPDGID;
    vector<float>                mPt;
    vector<float>                mEta;
    vector<float>                mPhi;
    vector<float>                mE;
    /* Particle level for the particles not in jets (nij). */
    vector<int>                  mNIJPDGID;
    vector<float>                mNIJPt;
    vector<float>                mNIJEta;
    vector<float>                mNIJPhi;
    vector<float>                mNIJE;
    /* Parton level. */
    vector<char>                 mPJetId;
    vector<int>                  mPPtnId;
    vector<int>                  mPOwnId;
    vector<int>                  mPPDGID;
    vector<char>                 mPTag;
    vector<float>                mPPt;
    vector<float>                mPEta;
    vector<float>                mPPhi;
    vector<float>                mPE;
    vector<float>                mDR;
    /* Jet level. */
    vector<float>                mJPt;
    vector<float>                mJEta;
    vector<float>                mJPhi;
    vector<float>                mJE;
    vector<int>                  mJPtnId;
    vector<int>                  mConst;
    vector<float>                mPTD;
    vector<float>                mSigma2;
    /* MET level. */
    float                        mMet;
    fastjet::PseudoJet           mMetVect;

    vector<PartonHolder>         mPartonInfo;

    fastjet::JetDefinition::Plugin *mPlugin;
    fastjet::JetDefinition          mJetDef;
    vector<fastjet::PseudoJet>      mJetInputs;
    vector<fastjet::PseudoJet>      mNJetInputs;
    vector<fastjet::PseudoJet>      mPJetInputs;
    vector<fastjet::PseudoJet>      mSortedJets;
    vector<fastjet::PseudoJet>      mJetParts;
    vector<fastjet::PseudoJet>      mCutJetParts;

    vector<int>                  mAncestry;
    vector<int>                  mNAncestry;
    vector<int>                  mWs;

    double                       mTotWgt;
    double                       mSelWgt;
    map<string,double>           mErrorList;
    unsigned                     mHardProcCount;
    unsigned                     mCounter;

    vector<pair<fastjet::PseudoJet,int>>  mGammas;
    vector<pair<fastjet::PseudoJet,int>>  mChargeds;
    vector<pair<fastjet::PseudoJet,int>>  mMuons;
    int                                   mLeptonFriend;

    fastjet::PseudoJet           mGamma; // Prompt photon in gamma+jets events
    //@}
};

/* Initialization, closing and administrative stuff: */

inline void HerwigTree::doinitrun()
{
    AnalysisHandler::doinitrun();
    try {
        /* Commenting the settings! */
        if (cfg::DOD0 and cfg::DOD0RIIC) cerr << "  Using D0 RunII cone algorithm plugin";
        else cerr << "  Using anti-kT jet algorithm";
        cerr << " -> Cone radius = " << cfg::RCone << endl;
        if (cfg::BEnriched) cerr << "Making a b-enriched sample" << endl;
        #ifdef STORE_PRTCLS
        cerr << "  Storing Final-State particles found in jets." << endl;
        if (cfg::StoreNIJ) cerr << "  Storing also particles not in jets." << endl;
        #endif

        string fileName = "pjets_herwig7";

        /* In a general multithread-case, generate a thread-unique root file name */
        fileName += "_";
        fileName += generator()->runName();
        fileName += ".root";

        size_t pos = fileName.find("jet_");
        string modeName = fileName.substr(14,pos-14);
        if (modeName=="generic") {
            mMode = 0;
        } else if (modeName=="di") {
            mMode = 1;
        } else if (modeName=="gamma") {
            mMode = 2;
        } else if (modeName=="Z") {
            mMode = 3;
        } else if (modeName=="ttbar") {
            mMode = 4;
        } else if (modeName=="powhegtt") {
            mMode = 5;
        } else if (modeName=="madgraphtt") {
            mMode = 6;
        } else {
            throw runtime_error("Bad mode");
        }

        mTotWgt = 0;
        mSelWgt = 0;
        mCounter = 0;

        /* Setup a root file */
        mFile = new TFile (fileName.c_str(),"RECREATE");
        if (!mFile) throw runtime_error("Creating an output file failed");
        mFile->SetCompressionLevel(1);

        /* Setup a root tree */
        mTree = new TTree ("HerwigTree","Herwig7 particle data.");
        if (!mTree) throw runtime_error("Creating a tree failed");
        mTree->SetAutoSave(100000000);  /* 0.1 GBytes */
        mTree->SetCacheSize(10000000);  /* 10 MBytes */
        TTree::SetBranchStyle(1); /* new style */

        /** Connect an event handle with the tree **/
        mTree->Branch("info",          &mInfo     )->SetAutoDelete(kFALSE);
        mTree->Branch("bnucount",      &mBNuCount )->SetAutoDelete(kFALSE);
        mTree->Branch("onucount",      &mONuCount )->SetAutoDelete(kFALSE);
        mTree->Branch("nuob",          &mNuOB     )->SetAutoDelete(kFALSE);
        mTree->Branch("nuoc",          &mNuOC     )->SetAutoDelete(kFALSE);
        mTree->Branch("nuolept",       &mNuOLept  )->SetAutoDelete(kFALSE);
        mTree->Branch("nuoother",      &mNuOOther )->SetAutoDelete(kFALSE);
        mTree->Branch("nub",           &mNuB      )->SetAutoDelete(kFALSE);
        mTree->Branch("nuc",           &mNuC      )->SetAutoDelete(kFALSE);
        mTree->Branch("nulept",        &mNuLept   )->SetAutoDelete(kFALSE);
        mTree->Branch("nuother",       &mNuOther  )->SetAutoDelete(kFALSE);
        mTree->Branch("weight",        &mWeight   )->SetAutoDelete(kFALSE);
        mTree->Branch("pthat",         &mPtHat    )->SetAutoDelete(kFALSE);
        #ifdef STORE_PRTCLS
        /* Particle lvl (jets) */
        mTree->Branch("prtcl_jet",       &mJetId    )->SetAutoDelete(kFALSE);
        mTree->Branch("prtcl_pdgid",     &mPDGID    )->SetAutoDelete(kFALSE);
        mTree->Branch("prtcl_pt",        &mPt       )->SetAutoDelete(kFALSE);
        mTree->Branch("prtcl_eta",       &mEta      )->SetAutoDelete(kFALSE);
        mTree->Branch("prtcl_phi",       &mPhi      )->SetAutoDelete(kFALSE);
        mTree->Branch("prtcl_e",         &mE        )->SetAutoDelete(kFALSE);
        /* Particle lvl for particles not in jets (nij) */
        mTree->Branch("prtclnij_pdgid",  &mNIJPDGID )->SetAutoDelete(kFALSE);
        mTree->Branch("prtclnij_pt",     &mNIJPt    )->SetAutoDelete(kFALSE);
        mTree->Branch("prtclnij_eta",    &mNIJEta   )->SetAutoDelete(kFALSE);
        mTree->Branch("prtclnij_phi",    &mNIJPhi   )->SetAutoDelete(kFALSE);
        mTree->Branch("prtclnij_e",      &mNIJE     )->SetAutoDelete(kFALSE);
        #endif
        /* Isolation info is saved as partons, even though these are final-state particles. */
        mTree->Branch("isolation",       &mIsolation)->SetAutoDelete(kFALSE);
        /* Parton lvl */
        mTree->Branch("prtn_jet",        &mPJetId   )->SetAutoDelete(kFALSE);
        mTree->Branch("prtn_ptn",        &mPPtnId   )->SetAutoDelete(kFALSE);
        mTree->Branch("prtn_own",        &mPOwnId   )->SetAutoDelete(kFALSE);
        mTree->Branch("prtn_pdgid",      &mPPDGID   )->SetAutoDelete(kFALSE);
        mTree->Branch("prtn_tag",        &mPTag     )->SetAutoDelete(kFALSE);
        mTree->Branch("prtn_pt",         &mPPt      )->SetAutoDelete(kFALSE);
        mTree->Branch("prtn_eta",        &mPEta     )->SetAutoDelete(kFALSE);
        mTree->Branch("prtn_phi",        &mPPhi     )->SetAutoDelete(kFALSE);
        mTree->Branch("prtn_e",          &mPE       )->SetAutoDelete(kFALSE);
        mTree->Branch("prtn_dr",         &mDR       )->SetAutoDelete(kFALSE);
        /* Jet lvl */
        mTree->Branch("jet_pt",          &mJPt      )->SetAutoDelete(kFALSE);
        mTree->Branch("jet_eta",         &mJEta     )->SetAutoDelete(kFALSE);
        mTree->Branch("jet_phi",         &mJPhi     )->SetAutoDelete(kFALSE);
        mTree->Branch("jet_e",           &mJE       )->SetAutoDelete(kFALSE);
        mTree->Branch("jet_ptn",         &mJPtnId   )->SetAutoDelete(kFALSE);
        mTree->Branch("jet_constituents",&mConst    )->SetAutoDelete(kFALSE);
        mTree->Branch("jet_ptd",         &mPTD      )->SetAutoDelete(kFALSE);
        mTree->Branch("jet_sigma2",      &mSigma2   )->SetAutoDelete(kFALSE);

        mTree->Branch("met",&mMet)->SetAutoDelete(kFALSE);

        mTree->BranchRef();

        if (cfg::DOD0 and cfg::DOD0RIIC) { // D0RunIIConePlugin params: R_cone, min_jet_Et, split_ratio
            mPlugin = new fastjet::D0RunIIConePlugin(cfg::RCone,6,0.5);
            mJetDef = fastjet::JetDefinition(mPlugin);
        } else { // Normal anti-kt, JetDefinition params: algorithm,radius,recombination scheme,strategy
            mJetDef = fastjet::JetDefinition(fastjet::antikt_algorithm, cfg::RCone, fastjet::E_scheme, fastjet::Best);
        }

        /* Timing functions */
        mNumEvents = generator()->N();
        mTimerStep = 1000;
        mTimer.setParams(mNumEvents,mTimerStep);
        mTimer.startTiming();

    } catch (std::exception& e) {
        cout << "An error occurred: " << e.what() << endl;
    }
}

inline void HerwigTree::dofinish()
{
    AnalysisHandler::dofinish();

    mTree->GetCurrentFile();
    mTree->AutoSave("Overwrite");
    mFile->Close();

    cout << "A tree has been written into a .root file" << endl;

    cerr << "Total cross-section: " << generator()->integratedXSec()/picobarn << " pm " << generator()->integratedXSecErr()/picobarn << " pb." << endl;
    cerr << "Passed cross-section: " << (generator()->integratedXSec()/picobarn)*(mSelWgt/mTotWgt) << " pm " << (generator()->integratedXSecErr()/picobarn)*(mSelWgt/mTotWgt) << " pb." << endl;
//     cerr << mPythia.info.nSelected() << endl;
//     cerr << "Passed cross-section: " << mPythia.info.sigmaLHEF(0)*(mSelWgt/mTotWgt) << endl;
//     cerr << "Passed cross-section: " << (mSelWgt/mPythia.info.nSelected()) << endl;
//     cerr << "Testing " << mPythia.info.sigmaLHEF(0) << endl;

    cerr << "Possible errors:" << endl;
    for (auto &err : mErrorList) {
        cerr << "Error: " << err.first << " weight of total cross section: " << err.second/mTotWgt << endl;
    }

    if (cfg::DOD0 and cfg::DOD0RIIC) delete mPlugin;

    if (mCounter != 0) cerr << "Non-zero counter value: " << mCounter << endl;
}

NoPIOClassDescription<HerwigTree> HerwigTree::initHerwigTree;
// Definition of the static class description member.

}



#include "ThePEG/Utilities/ClassTraits.h"

namespace ThePEG {

/** @cond TRAITSPECIALIZATIONS */

/** This template specialization informs ThePEG about the
 *  base classes of HerwigTree. */
template <>
struct BaseClassTrait<Herwig::HerwigTree,1> {
    /** Typedef of the first base class of HerwigTree. */
    typedef AnalysisHandler NthBase;
};

/** This template specialization informs ThePEG about the name of
 *  the HerwigTree class and the shared object where it is defined. */
template <>
struct ClassTraits<Herwig::HerwigTree>
    : public ClassTraitsBase<Herwig::HerwigTree> {
    /** Return a platform-independent class name */
    static string className() { return "Herwig::HerwigTree"; }
    /** Return the name(s) of the shared library (or libraries) be loaded to get
    *  access to the HerwigTree class and any other class on which it depends
    *  (except the base class). */
    static string library() { return "libGHerwigTree.so"; }
};

/** @endcond */

}

#endif /* HERWIGTREE_H */
