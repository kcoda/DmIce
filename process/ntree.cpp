#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TGraph.h"
#include "TF1.h"
#include <iostream>
#include "TCanvas.h"


//ntree.cpp
//Version Number 01**

using namespace std;

int main(){

	float data[1024], x[1024], min, area, energy, timing;
	int start, cut, count, end;
	
	TFile *fout = TFile::Open("/projects/physics/dmice/Fermilab/Cs137_NProcessed01.root", "recreate");
		TTree *tree = new TTree("tree", "NaI Detector Data");
		tree->Branch("Waveforms", data, "data[1024]/F");
		tree->Branch("Height", &min, "min/F");
		tree->Branch("Area", &area, "area/F");
		tree->Branch("Saturated", &cut, "cut/I");
		tree->Branch("Energy", &energy, "energy/F");
		tree->Branch("Timing" , &timing, "timing/F");
		tree->Branch("start" , &start, "start/F");
		tree->Branch("end" , &end, "end/F");
	
	TChain t("tree");
		t.Add("/projects/physics/dmice/Fermilab/sourcedata/Cs137raw/Waveforms_0.root");
		//t.Add("/projects/physics/dmice/Fermilab/1V_runs/Fermilab_1V_raw.root");
		t.SetBranchAddress("Waveforms", data);
	Float_t offavg = .000024;

	for(int e = 0; t.GetEntry(e) > 0; e++){    //Loop over each entry

	Float_t y[1024];
		for(int i = 0; i< 1024; i++){
			y[i] = data[i] - offavg;
			x[i] = i;
		}
		

		Float_t weight=0, avg = 0;
		int minLoc, n0;
		int numpulse = 0;
		Float_t startloc[10], endloc[10];
		min = 0;
		int zero_counter = 0;
		bool foundstart = false, foundzero = false;
		for(int i = 2; i < 997; i++){   //Loop through the data to find each startloc and endloc
			float local_avg = (y[i-2] + y[i-1] + y[i] + y[i+1] + y[i+2]);  
			
			if(local_avg >= 0 && !foundstart){
				zero_counter++;
				if(zero_counter == 5){
					zero_counter = 0;
					foundzero = true;
				}
			}
			
			if(local_avg < -.1 && !foundstart && foundzero){  //play around with .01 (previously not /5 and < -.05)
				start = i-2;
				foundzero = false;
				foundstart = true;
				startloc[numpulse] = start;
				zero_counter = 0;

			}
			if(local_avg >= 0 && !foundzero && foundstart && i >= (start + 100 )){  
				zero_counter++;
				if(zero_counter == 6){
					end = i;
					foundzero = true;
					foundstart = false;
					endloc[numpulse] = end;
					numpulse ++;
				}

			}

		}
 

		

		for(int i = 0; i < numpulse; i++){    //Loop through each pulse
			Float_t bin_sum = 0;	
			if(endloc[i] == 0)
				startloc[i] = 0;
			for(int k = startloc[i]; k < endloc[i]; k++){    
				if(k < 1021 && cut == 0)           
					if(data[k] == data[k+1] && data[k] == data[k+2] && data[k] < -.2)  
						cut = 1;
				if(y[k] < min){ 
					min = y[k];  
					minLoc = k;					 
				}
			}

				
				
			//TGraph gr(1024, x, y);
			//area = gr.Integral(startloc[i], endloc[i]);
			for(int v = startloc[i]; v <= endloc[i]; v++){
				bin_sum += y[v];
			}
			area = -bin_sum;

			energy = (area*69.674)+1.8148;
		
			for(int z = minLoc; z > 0; z--){
				if(y[z] < min/2)
				n0 = z;
			}
			
			for(int x = n0; x< (n0+100); x++){
				weight += (x-n0)*y[x];
				avg += y[x];
				if(x == 1023){
					weight = 0;
					avg = 0;
					break;
				}
			if(avg != 0)
				timing = (weight/avg)*4;
			
			
			
			count++;
				
                	
			}

			tree->Fill();
		}	
		
		}
		tree->Write();
		tree->Print();
		fout->Close("R");
		delete fout;


	
}








