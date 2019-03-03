/* 

			New data Analysis 
			First look without track matching inserting cluster logic  
			Author: Amandip De
*/ 
#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"
#include  "math.h"
#include <vector>

class cluster
{
      public: 
	float x;
	int size;    
};

// ---------Formation of the cluster-------------

std::vector < cluster*> Clusterization (std::vector<Int_t> *hit)
{ 
sort(hit->begin(),hit->end());	
vector < cluster*> Cluster;

 //if (hit->empty()) return ; 
    if (hit->size()<1){
        return(Cluster);
    }

   float x0=hit->at(0);
    int size=1;
    int edge = 250;
  cout<< "hit at "<<x0<<endl;  
for (unsigned int k=1; k<hit->size(); k++){
        if (hit->at(k)==x0+size && !(hit->at(k)==edge)){
            size++;
        }
	else{
	  cluster *clust = new cluster;  	
	  clust->x = x0+(size-1)/2.;
	  clust->size = size;
	  Cluster.push_back(clust);
	  x0=hit->at(k);
	  size= 1;
}}

cluster *clust = new cluster;  	
	  clust->x = x0+(size-1)/2.;
	  clust->size = size;
	  Cluster.push_back(clust);

return Cluster;

}
// ----------End of Cluster Formation--------------

void new_data_test_cluster_1() {

//Data For angular Scan

Int_t n = 18;
int Runs[18] = {2661,2663,2664,2665,2666,2667,2668,2670,2671,2672,2673,2674,2677,2678,2679,2680,2682,2683};
Double_t angle[18] = {0,5,10,15,20,25,14, 16, 17, 18, 19, 21, 22, 23, 24, 18.5, 20.5,19.5};
Double_t stub_eff[18];
/*
//Data for VCTH Scan 
Int_t n = 33;
int Runs[33] = {2419,2421,2422,2424,2425,2426,2427,2428,2429,2430,2431,2432,2433,2434,2435,2436,2437,2438, 2439,2440,2441,2442,2444,2445,2446,2447,2448,2449,2450,2452,2453,2454,2457};
*/
// Define the Histograms
TH1F *h_hit0 = new TH1F("Hit0", "Hit0", 10, 0 ,10);
TH1F *h_hitmap0 = new TH1F("Hitmap0", "Hitmap0",300,0,300);
TH1F *h_hit1 = new TH1F("Hit1", "Hit1", 10, 0 ,10);
TH1F *h_stub = new TH1F("Stub", "Stub", 10, 0 ,10);
TH1F *h_stub_hitmap = new TH1F("Stub_hitmap", "Stub_hitmap",300,0,300);
//TH1F *stub_by_hit = new TH1F("Stub_by_hit", "Stub_by_hit",300,0,300);
TH2D *correlation = new TH2D("Hit_Correlation","Hit_Correlation", 300,0,300,300,0,300);
TH1F *h_cluster0 = new TH1F("cluster0", "cluster0", 10, 0 ,10);
TH1F *h_cluster1 = new TH1F("cluster1", "cluster1", 10, 0 ,10);
TH1F *h_clustersize0 = new TH1F("Cls_size0", "Cls_size0", 10, 0 ,10);
TH1F *h_clustersize1 = new TH1F("Cls_size1", "Cls_size1", 10, 0 ,10);
TH1F *h_clusterpos0 = new TH1F("Cluster_position0", "Cluster Position 0", 300,0,300);
TH1F *h_clusterpos1 = new TH1F("Cluster_position1", "Cluster Position 1", 300,0,300);

for(int run_no = 0 ; run_no < n ; run_no++ ) 
{

TString dasetname; 
dasetname.Form("/home/amandip/MEGA/MEGAsync/TRACKER/Results/New_Data/new_ntuple/run%d.root", Runs[run_no]);

TFile *f = new TFile(dasetname);

//TFile * f = new TFile ("/home/amandip/MEGA/MEGAsync/TRACKER/Results/New_Data/new_ntuple/run2419.root");
TTree *analysisTree = (TTree*)f->Get("flatTree");

std::vector<Int_t> *hit0 = new std::vector<Int_t>();
std::vector<Int_t> *hit1 = new std::vector<Int_t>();
std::vector<Int_t> *stub = new std::vector<Int_t>();

analysisTree->SetBranchAddress("hit0", &hit0);
analysisTree->SetBranchAddress("hit1", &hit1);
analysisTree->SetBranchAddress("stub", &stub);
Int_t nentries = (Int_t)analysisTree->GetEntries();

double n_hit0 = 0., den = 0. , n_hit1= 0., n_stub = 0. ; 


// Start the event loop 
for (Int_t i = 0 ; i<nentries; i++) {

analysisTree->GetEntry(i);
Long64_t jentry = analysisTree->GetEntry(i);
if (jentry < 0) break;
if (i%1000 == 0) {
      cout << " Events processed. " << std::setw(8) << i
	   << endl;
}

double Hit0_size = hit0->size();
double Hit1_size = hit1->size();
double center_0 = 0. , center_1=0.;

// Fill the corresponding hits and CBC stub
h_hit0->Fill(hit0->size());
h_hit1->Fill(hit1->size());
h_stub->Fill(stub->size());

den++;

bool hit_at_0 = false; 
bool hit_at_1 = false; 
bool stub_at_DUT = false; 

vector < cluster*> Cluster0;
vector < cluster*> Cluster1;
Cluster0 =Clusterization(hit0);
Cluster1 =Clusterization(hit1);

h_cluster0->Fill(Cluster0.size());
h_cluster1->Fill(Cluster1.size());
//for Cluster 0
for (auto& Cls0 : Cluster0)
{

//cout<< Cls0->x<<"	"<< Cls0->size<<endl; 	
h_clustersize0->Fill(Cls0->size);
h_clusterpos0->Fill(Cls0->x);

}
// for Cluster 1
for (auto& Cls1 : Cluster1)
{
//cout<< Cls1->x<<"	"<< Cls1->size<<endl; 	
h_clustersize1->Fill(Cls1->size);
h_clusterpos1->Fill(Cls1->x);
}



	Float_t Hit_0 = 0., Hit_1 = 0., stub_0 = 0. ;
	Int_t h_index = -1 ;

//Hit0 loop

	for (int j = 0 ; j < hit0->size(); ++j) {
	hit_at_0 = true;
	Int_t hit = 0;
	//std::cout<<"evt"<<i<<"hit["<< j << "]:" << (*hit0)[j]<<std::endl;
	//cout << "size of hit0 "<< hit0->GetEntries() <<endl;
	//h_hit0->Fill(hit0->size());
	Hit_0 = (*hit0)[j] ;
	//cout<< "strip values"<<Hit_0<<endl;
	center_0 = Hit_0 + (0.5 * (Hit0_size - 1.));
	//cout<<"Center of the hit: "<<center_0<<endl;
	float x_position = -1. * (center_0 -  250.5/2.)* 0.090 ;	

	//cout<< "strip position in terms of x value: "<< x_position<<endl; 	
	h_index = j ;
	h_hitmap0->Fill(Hit_0); 

		for (int k = 0 ; k < hit1->size(); ++k) {
		Hit_1 = (*hit1)[k] ;	
		correlation->Fill(Hit_0,Hit_1);


    }}
	//h_hitmap0->Fill((*hit0)[h_index]); 

// Hit1 loop 

	for (int k = 0 ; k < hit1->size(); ++k) {
	Int_t hit = 0;
	//std::cout<<"evt"<<i<<"hit["<< k << "]:" << hit1->at(k)<<std::endl;
	hit_at_1 = true;
    }

//CBC stub loop

	for (int l = 0 ; l < stub->size(); ++l) {
	Int_t hit = 0;
	//std::cout<<"evt"<<i<<"stub["<< l << "]:" << stub->at(l)<<std::endl;
	stub_at_DUT = true;
	stub_0 = (*stub)[l] ;
	h_stub_hitmap->Fill(stub_0);
    }

if (hit_at_0) n_hit0++;
if (hit_at_1) n_hit1++;
if (stub_at_DUT) n_stub++;


}// End of the Event Loop

TString dasetname_output;
dasetname_output.Form("/home/amandip/MEGA/MEGAsync/TRACKER/Results/New_Data/Result/Result_%d.root",Runs[run_no]);
//dasetname_output.Form("Result_2419_test.root");
TFile *output = new TFile(dasetname_output, "recreate");
h_hit0->Write();
h_hit0->Write();
h_stub->Write();
h_hitmap0->Write();
h_stub_hitmap ->Write();
h_cluster0->Write();
h_cluster1->Write();
h_clustersize0->Write();
h_clustersize1->Write();
h_clusterpos0->Write();
h_clusterpos1->Write();


TH1F *stub_by_hit = (TH1F*) h_stub_hitmap->Clone();
stub_by_hit->Divide(h_hitmap0);
stub_by_hit->Write();
correlation->Write();
output->Close();


/*
// to find the max strip no 
double x;
Double_t x_values[300] , mymaximum = 0.0;
 int i ; 
for (i = 0 ; i < 300; i++)
{
if( h_hitmap0->GetBinContent(i) != 0)
{
 x_values[i] = ((TAxis*)h_hitmap0->GetXaxis())->GetBinCenter(i); 
//cout << "stip: "<<x_values[i]<<endl;
}
}
for(int i = 0; i < 300; i++){
    if(x_values[i] > mymaximum){
        mymaximum = x_values[i];
    }
}
cout<<"	max strip no "<<mymaximum<<endl;
// End of max strip find
*/



std::cout<<" Den->" <<den<< " hit0->"<< n_hit0 << " hit1->"<< n_hit1<<" stubs->"<< n_stub<< " eff->"<< n_stub / den<<endl;
Double_t num_by_deno = n_stub / den ;
std::cout<< " stub eff->"<<num_by_deno<<endl; 

}
}








