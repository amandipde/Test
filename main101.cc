#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/FastJet3.h"
#include "TH1.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TVector3.h"

using namespace Pythia8;

int main() {


  TH1F* histmass_fatjet = new TH1F("mass_matched fatjet", "Mass of the matched fatjet", 25,10.0,250);
  TH1F* deltaR_top_J= new TH1F("delta R of top and Jet", "delta R of top and Jet", 40,0.0,4);
  TH1F* deltaR_b1_J= new TH1F("delta R of b1 and Jet", "delta R of b1 and Jet", 40,0.0,4);
  TH1F* deltaR_b2_J= new TH1F("delta R of b2 and Jet", "delta R of b2 and Jet", 40,0.0,4);
  TH1F* deltaR_tau_J= new TH1F("delta R of tau and Jet", "delta R of tau and Jet", 40,0.0,4);
  
TH1F *histnfatjets = new TH1F("nfatjets", "Number of fat jets", 10, 0.0, 10.0);
TH1F *histnfatjets1 = new TH1F("nfatjets1", "Number of fat jets", 10, 0.0, 10.0);

TH1F *histsortedjets = new TH1F("nsortedfatjets", "Number of sorted fat jets", 4, 0.0, 4.0);
TH1F *hist_mass_b_nu_tau = new TH1F("mass_b_nu_tau", "Mass_b_nu_tau", 25,10.0,250);

TH1F *hist_mass_b_tau = new TH1F("mass_b_tau", "Mass_b_tau", 25,10.0,250);

TH1F* hist_delta_t_b1= new TH1F("gen delta R of top and b1", "gen delta R of top and b1", 40,0.0,4);
TH1F* hist_delta_t_b2= new TH1F("gen delta R of top and b2", "gen delta R of top and b2", 40,0.0,4);
TH1F* hist_delta_t_tau= new TH1F("gen delta R of top and tau", "en delta R of top and tau", 40,0.0,4);





  // Generator
  Pythia pythia;
  Event& event = pythia.event;
  
  pythia.readString("PartonLevel:ISR = on");
  pythia.readString("PartonLevel:FSR = on");
  pythia.readString("PartonLevel:MPI = on");
  pythia.readString("HadronLevel:Hadronize = on");

  // Initialize Les Houches Event File run.
  pythia.readString("Beams:frameType = 4");
  pythia.readString("Beams:LHEF = wprime_10k.lhe");
  pythia.init();

  // Allow for possibility of a few faulty events.
  int nAbort = 10, nEvent=1000;
  int iAbort = 0;
   int ncount = 0 ;

 double R = 1.0, Rsub = 0.3;
  fastjet::RecombinationScheme recomb_scheme=fastjet::E_scheme;
  fastjet::Strategy strategy = fastjet::Best;
  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, R,recomb_scheme,strategy);
  //fastjet::JetDefinition jet_def(fastjet::cambridge_algorithm, R);
  fastjet::JetDefinition jet_def_subjet(fastjet::cambridge_algorithm, Rsub);


//------------------------------Starting of event------------
for (int iEvent = 0; iEvent < nEvent; ++iEvent) {

      // Generate next event.
    // Break out of event loop if at end of LHE file.
    if ( !pythia.next() ) continue;
    if (iEvent ==0) pythia.event.list() ;


int indx=0 , top_indx =0 , W_indx=0, w=0, index_b1=0, index=0 ,truth_top=0; 
// Add final-state particles to FastJet input vector
std::vector <fastjet::PseudoJet> fjInputs, subjet_inputs, TOPLIKE;
 //  fjInputs.resize(0); TOPLIKE.resize(0);


  



//------------------------------------pythia event loop start --------------------
    for (int i = 0; i < pythia.event.size(); ++i)
    {
 

 /*
 // Final state only
  if (pythia.event[i].isFinal() ){
//if (pythia.event[i].idAbs() == 5  &&  pythia.event[event[i].mother1()].idAbs() == 34) continue ; 
  fastjet::PseudoJet particle(pythia.event[i].px(),
          pythia.event[i].py(),
          pythia.event[i].pz(),
          pythia.event[i].e() );
  particle.set_user_index(i);
  fjInputs.push_back( particle);

}
*/

if (pythia.event[i].isFinal() ){//&& event[i].isVisible() ){
   fjInputs.push_back(fastjet::PseudoJet(
        event[i].px(), event[i].py(), event[i].pz(), event[i].e() ) );
}



 if (pythia.event[i].idAbs() == 6 && fabs(pythia.event[i].status()) ==22) truth_top = i ;

 

// top

        if (pythia.event[i].idAbs() == 6) { 

          
            index = i ;  
/*  
cout<<"Line NO: "<<i<< " ID: "<< pythia.event[i].id()<< " status: "<<pythia.event[i].status()<< "mother1 ID: "
            <<pythia.event[event[i].mother1()].id()<< "  mother2 ID: "<<
            pythia.event[event[i].mother2()].id()<< "   daughter1 id:"<<
            pythia.event[event[i].daughter1()].id()<< "   daughter2 id:"<<
            pythia.event[event[i].daughter2()].id()<< "       "<< "  px:  "<<
            pythia.event[i].px()<< "    py:  "<<
            pythia.event[i].py()<< "    pz:  "<<
            pythia.event[i].pz()<< "    Energy:  "<<
            pythia.event[i].e()<<  "    Mass:  "<<
            pythia.event[i].m()<<endl; 
*/
            //top recoil
            while(pythia.event[i].idAbs() ==6 && fabs(pythia.event[i].status()) ==52)
            {
              indx= i ;
              int topdaughter1 = pythia.event[i].daughter1();
              i  = topdaughter1;
            }
            top_indx=indx;

}

if (  pythia.event[i].idAbs() == 24) {
 int topdaughter = pythia.event[top_indx].daughter1();
    if (pythia.event[topdaughter].idAbs() != 24) continue ;
          //w recoil
            while(pythia.event[i].idAbs() ==24)
            {
              w= i ;
              int topdaughter1 = pythia.event[i].daughter1();
              i  = topdaughter1;
            }
            W_indx=w;

}


// Wprime 
        if (pythia.event[i].idAbs() == 34) { 
int Wprimedaughter2 = pythia.event[i].daughter2();
if (pythia.event[Wprimedaughter2].idAbs() == 5) index_b1 = Wprimedaughter2 ;
/*
cout<<"Line NO: "<<i<< " ID: "<< pythia.event[i].id()<< " status: "<<pythia.event[i].status()<< "mother1 ID: "
            <<pythia.event[event[i].mother1()].id()<< "  mother2 ID: "<<
            pythia.event[event[i].mother2()].id()<< "   daughter1 id:"<<
            pythia.event[event[i].daughter1()].id()<< "   daughter2 id:"<<
            pythia.event[event[i].daughter2()].id()<< "       "<< "  px:  "<<
            pythia.event[i].px()<< "    py:  "<<
            pythia.event[i].py()<< "    pz:  "<<
            pythia.event[i].pz()<< "    Energy:  "<<
            pythia.event[i].e()<<  "    Mass:  "<<
            pythia.event[i].m()<<endl; 
*/
}




    } // pythia event loop 

 int topdaughter1 = pythia.event[top_indx].daughter1(); // w from tau 
 int topdaughter2 = pythia.event[top_indx].daughter2();  // b from tau 

 int wdaughter1 = pythia.event[W_indx].daughter1(); // w from tau 
 int wdaughter2 = pythia.event[W_indx].daughter2();  // b from tau 


// Get the vector for top 

if (pythia.event[top_indx].idAbs() !=6 ) continue ;
if (pythia.event[wdaughter1].idAbs() !=15 ) continue ;
if (pythia.event[topdaughter2].idAbs() !=5 ) continue ;
if (pythia.event[index_b1].idAbs() !=5 ) continue ;
if (pythia.event[wdaughter2].idAbs() <12 ) continue ;


   fastjet::PseudoJet toplike(pythia.event[truth_top].px(),
                              pythia.event[truth_top].py(),
                              pythia.event[truth_top].pz(),
                              pythia.event[truth_top].e());
   
   fastjet::PseudoJet toplike1(pythia.event[top_indx].px(),
                              pythia.event[top_indx].py(),
                              pythia.event[top_indx].pz(),
                              pythia.event[top_indx].e());

    fastjet::PseudoJet nu  (pythia.event[wdaughter2].px(),
                              pythia.event[wdaughter2].py(),
                              pythia.event[wdaughter2].pz(),
                              pythia.event[wdaughter2].e());

   fastjet::PseudoJet taulike(pythia.event[wdaughter1].px(),
                              pythia.event[wdaughter1].py(),
                              pythia.event[wdaughter1].pz(),
                              pythia.event[wdaughter1].e());

   fastjet::PseudoJet b1like(pythia.event[index_b1].px(),
                              pythia.event[index_b1].py(),
                              pythia.event[index_b1].pz(),
                              pythia.event[index_b1].e());

   fastjet::PseudoJet b2like(pythia.event[topdaughter2].px(),
                              pythia.event[topdaughter2].py(),
                              pythia.event[topdaughter2].pz(),
                              pythia.event[topdaughter2].e());

vector <fastjet::PseudoJet> inclusive_jets , sortedjets , subjets;             
  double ptmin = 200.0;
  bool jetsorting = false; 
  // run the jet clustering with the above jet definition
  fastjet::ClusterSequence clust_seq(fjInputs, jet_def);
  inclusive_jets = sorted_by_pt(clust_seq.inclusive_jets(ptmin));

  if (inclusive_jets.size() < 1) continue ;
histnfatjets1->Fill(inclusive_jets.size());
int ijet ;

for (unsigned int i = 0; i < inclusive_jets.size(); i++) {

if (inclusive_jets.size() == 1 ){
double d0 =  toplike1.delta_R(inclusive_jets[0]) ;
if (d0 < 0.8 ) continue;
ijet = 0;
}
 
else if (inclusive_jets.size() >=2 ){
double d0 = toplike1.delta_R(inclusive_jets[0]);
double d1 = toplike1.delta_R(inclusive_jets[1]);
if (d0<d1){
if (d0 >0.8) continue ;
ijet=0;
} 
if (d1<d0)
{
if (d1>0.8) continue;
ijet= 1;
}
//cout<<"d0:  "<<d0<<"   "<<ijet<<endl;
//cout<<"d1:  "<<d1<<"     "<<ijet<<endl;
 }
// cout<<inclusive_jets[ijet].m()<<endl;

histmass_fatjet->Fill(inclusive_jets[ijet].m());

deltaR_b1_J->Fill(b1like.delta_R(inclusive_jets[ijet]));
deltaR_b2_J->Fill(b2like.delta_R(inclusive_jets[ijet]));
deltaR_tau_J->Fill(taulike.delta_R(inclusive_jets[ijet]));
deltaR_top_J->Fill(toplike1.delta_R(inclusive_jets[ijet]));
}

hist_delta_t_b1->Fill(b1like.delta_R(toplike1));
hist_delta_t_b2->Fill(b2like.delta_R(toplike1));
hist_delta_t_tau->Fill(taulike.delta_R(toplike1));
hist_mass_b_nu_tau->Fill((b2like + nu +taulike).m());
hist_mass_b_tau->Fill((b2like + taulike).m());

histnfatjets->Fill(nfatjets);
histsortedjets->Fill(nsorted_jets);

  }// End of event loop.
  // Give statistics. Print histogram.
  pythia.stat();

TFile *fout = TFile::Open("Result.root","RECREATE");
fout->cd();

histmass_fatjet->Write();
deltaR_b1_J->Write();
deltaR_b2_J->Write();
deltaR_top_J->Write();
deltaR_tau_J->Write();
hist_mass_b_nu_tau->Write();
hist_delta_t_b1->Write();
hist_delta_t_tau->Write();
hist_delta_t_b2->Write();
histnfatjets->Write();
histsortedjets->Write();
histnfatjets1->Write();
hist_mass_b_tau->Write();
delete fout;


  // Done.
  return 0;
}
