#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TGraph.h"
#include "TF1.h"
#include <iostream>

using namespace std;

int main(){
	
	float data[1024], x[1024], min, area, energy, timing;
	int start, cut, count;
	TFile *fout = TFile::Open("/projects/physics/dmice/Fermilab/Co60_Temp.root", "recreate");
		TTree *tree = new TTree("tree", "NaI Detector Data");
		tree->Branch("Waveforms", data, "data[1024]/F");
		tree->Branch("Height", &min, "min/F");
		tree->Branch("Area", &area, "area/F");
		tree->Branch("Saturated", &cut, "cut/I");
		tree->Branch("Energy", &energy, "energy/F");
		tree->Branch("Timing" , &timing, "timing/F");
	
	TChain t("tree");
		t.Add("/projects/physics/dmice/Fermilab/sourcedata/Co60raw/Waveforms_0.root");
		//t.Add("/projects/physics/dmice/Fermilab/1V_runs/Fermilab_1V_raw.root");
		t.SetBranchAddress("Waveforms", data);
	
	for(int e = 0; t.GetEntry(e) > 0; e++){  //&& e < 10
		Float_t weight=0, avg = 0, y[1024], offavg=0, bin_sum = 0;
		int minLoc, n0;
		min = 0;
		for(int i = 0; i < 200; i++)
			offavg += data[i];
		offavg = offavg/200;
		for(int i = 0; i < 1024; i++){
			y[i] = data[i] - offavg;
			x[i] = i;
			if(i < 1021 && cut == 0)
				if(data[i] == data[i+1] && data[i] == data[i+2] && data[i] < -.2)
					cut = 1;
			if(y[i] < min){
				min = y[i];
				minLoc = i;
			}

		}
		int zero_counter = 0, start, end;
		bool foundstart = false, foundend = false;
		for(int i = 2; i < 997; i++){
			float local_avg = y[i-2] + y[i-1] + y[i] + y[i+1] + y[i+2];
			if(local_avg < -.05 && !foundstart){
				start = i-2;
				foundstart = true;
			}
			if(local_avg >= 0 && !foundend && foundstart){
				zero_counter++;
				if(zero_counter == 15){
					end = i;
					foundend = true;
				}
			}
		
		}
		//cout <<"Pulse Start: " << start << endl;
		//cout <<"Pulse End: " << end << endl;
		

	//	for(inti i = start; i <= end; i++){
	//		bin_sum += y[i];
	//	}

		TGraph gr(1024, x, y);
		area = gr.Integral(start, end);
	//	area = -bin_sum;

		energy = (area*69.674)+1.8148;
		
		for(int i = minLoc; i > 0; i--){
			if(y[i] < min/2)
				n0 = i;
		}		
		for(int i = n0; i < (n0+100); i++)
		{
			weight += (i-n0)*y[i];
			avg += y[i];
			if(i == 1023)
			{
				weight = 0;
				avg = 0;
				break;
			}
		}
		if(avg != 0)
			timing = (weight/avg)*4;
		tree->Fill();
		//cout << "min: " << min << endl;
		//cout << "Area: "<< area << endl;
		//cout << "Saturated: " << cut << endl;
		//cout << "Meantime: " << timing << endl;
		//cout << endl;

		count++;
	}
	tree->Write();
	tree->Print();
	fout ->Close("R");
	delete fout;
}






