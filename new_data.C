/* 

			New data Analysis 
			First look without track matching  
			Author: Amandip De
*/ 
#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"


void new_data() {
  
Int_t n = 18;
int Runs[18] = {2661,2663,2664,2665,2666,2667,2668,2670,2671,2672,2673,2674,2677,2678,2679,2680,2682,2683};

Double_t angle[18] = {0,5,10,15,20,25,14, 16, 17, 18, 19, 21, 22, 23, 24, 18.5, 20.5,19.5};

Double_t stub_eff[18];

for(int run_no = 0 ; run_no < n ; run_no++ ) 
{
/*
TH1F *h_hit0 = new TH1F("Hit0", "Hit0", 10, 0 ,10);
TH1F *h_hit1 = new TH1F("Hit1", "Hit1", 10, 0 ,10);
TH1F *h_stub = new TH1F("Stub", "Stub", 10, 0 ,10);
*/

TString dasetname; 
dasetname.Form("/home/amandip/MEGA/MEGAsync/TRACKER/Results/New_Data/new_ntuple/run%d.root", Runs[run_no]);

TFile *f = new TFile(dasetname);
//TFile * f = new TFile ("run2419.root");
TTree *t1 = (TTree*)f->Get("flatTree");

std::vector<Int_t> *hit0 = new std::vector<Int_t>();
std::vector<Int_t> *hit1 = new std::vector<Int_t>();
std::vector<Int_t> *stub = new std::vector<Int_t>();

t1->SetBranchAddress("hit0", &hit0);
t1->SetBranchAddress("hit1", &hit1);
t1->SetBranchAddress("stub", &stub);

Int_t nentries = (Int_t)t1->GetEntries();

double n_hit0 = 0., den = 0. , n_hit1= 0., n_stub = 0. ; 

// Start the event loop 
for (Int_t i = 0 ; i<nentries; i++) {
t1->GetEntry(i);
//cout << "size of hit "<< hit0->size()<<endl;
/*
h_hit0->Fill(hit0->size());
h_hit1->Fill(hit1->size());
h_stub->Fill(stub->size());
*/
den++;

bool hit_at_0 = false; 
bool hit_at_1 = false; 
bool stub_at_DUT = false; 

	for (int j = 0 ; j < hit0->size(); ++j) {
	Int_t hit = 0;
	//std::cout<<"evt"<<i<<"hit["<< j << "]:" << hit0->at(j)<<std::endl;
	hit_at_0 = true;
    }

	for (int k = 0 ; k < hit1->size(); ++k) {
	Int_t hit = 0;
	//std::cout<<"evt"<<i<<"hit["<< k << "]:" << hit1->at(k)<<std::endl;
	hit_at_1 = true;
    }

	for (int l = 0 ; l < stub->size(); ++l) {
	Int_t hit = 0;
	//std::cout<<"evt"<<i<<"stub["<< l << "]:" << stub->at(l)<<std::endl;
	stub_at_DUT = true;
    }

if (hit_at_0) n_hit0++;
if (hit_at_1) n_hit1++;
if (stub_at_DUT) n_stub++;


}
// End of the Event Loop
//TFile output("Result.root","recreate");
/*
TCanvas *c1 =new TCanvas();
h_hit0->Draw();
c1->SaveAs("Hit_on_0th_sensor.png");

c1->Update();
h_hit1->Draw();
c1->SaveAs("Hit_on_1st_sensor.png");

c1->Update();
h_stub->Draw();
c1->SaveAs("No_of_stubs.png"); 
*/


std::cout<<"angle->"<<angle[run_no]<<" Den->" <<den<< " hit0->"<< n_hit0 << " hit1->"<< n_hit1<<" stubs->"<< n_stub<< " eff->"<< n_stub / den<<endl;
Double_t num_by_deno = n_stub / den ;
std::cout<< " stub eff->"<<num_by_deno<<endl; 
stub_eff[run_no]= num_by_deno; 

}

TCanvas *c1 = new TCanvas("c1","Graph Draw Options", 200,10,600,400);
TGraph *gr1 = new TGraph (n, angle, stub_eff);
gr1->SetMarkerStyle(20);
gr1->SetMarkerSize(0.9);
gr1->SetMarkerColor(kRed);
gr1->SetTitle("Stub Efficiency");

TMultiGraph * mg = new TMultiGraph();
mg->Add(gr1);
mg->Draw("AP");
mg->GetXaxis()->SetRangeUser(-1,26);
mg->GetYaxis()->SetRangeUser(0.0, 1.2);
mg->GetXaxis()->SetTitle("Angle of rotation");
mg->GetYaxis()->SetTitle("Stub Efficiency");
c1->BuildLegend();
}










