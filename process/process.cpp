#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"
#include "dirent.h"
#include "waveform.h"
#include "process.h"
#include "TBranch.h"

#include <stdio.h>
#include <vector>
#include <iostream>

/* CURRENT VERSION IS 02 :::: DONT FORGET TO ALTER WAVEFORM.CPP */

/* process will return 1 if the tree was created sucessfully and 0 otherwise */


using namespace std;

int process(string ROOTFILEOUT, string ROOTFILEIN, bool verbose){

	Float_t data[1024], min, area, energy, meantime;
	Bool_t saturated;
	
	TChain t("tree");
		if(t.AddFile(ROOTFILEIN.c_str(), 0) == 0){
			cout << ROOTFILEIN << " does not exist!" << endl;
			return 0;
		}
		TObjArray* branch_list = t.GetListOfBranches();
		if(branch_list->FindObject("Waveforms") == 0){
			cout << ROOTFILEIN << " does not contain the correct branch name. You may need to reprocess it" << endl;
			return 0;
		}
		t.SetBranchAddress("Waveforms", data);
	TFile *fout = TFile::Open(ROOTFILEOUT.c_str(), "recreate");
		TTree *tree = new TTree("tree", "NaI Detector Data");
			tree->Branch("Waveforms", data, "data[1024]/F");
			tree->Branch("Height", &min, "min/F");
			tree->Branch("Area", &area, "area/F");
			tree->Branch("Saturated", &saturated, "saturated/B");
			tree->Branch("Energy", &energy, "energy/F");
			tree->Branch("MeanTime", &meantime, "meantime/F");

	for(Int_t e = 0; t.GetEntry(e) > 0; e++){
	
		Waveform waveform(data);
		min		= waveform.GetMin();
		area   	  	= waveform.GetArea();
		saturated 	= waveform.GetSaturated();
		energy 		= waveform.GetEnergy();
		meantime	= waveform.GetMeanTime();

		tree->Fill();
	}

	tree->Write();
	if(verbose)
		tree->Print();
	fout->Close();
	delete fout;
	return 1;

}
int process(string ROOTFILEOUT, vector<string> ROOTFILEIN, bool verbose){

	Float_t data[1024], min, area, energy, meantime;
	Bool_t saturated;
	
	TChain t("tree");
		for(int f = 0; f < ROOTFILEIN.size(); f++){
			if(t.AddFile(ROOTFILEIN[f].c_str(), 0) == 0){
				cout << ROOTFILEIN[f] << " does not exist!" << endl;
				return 0;
			}
		}
		TObjArray* branch_list = t.GetListOfBranches();
		if(branch_list->FindObject("Waveforms") == 0){
			cout << " oen of the files does not contain the correct branch name. You may need to reprocess it" << endl;
			return 0;
		}
		t.SetBranchAddress("Waveforms", data);

	TFile *fout = TFile::Open(ROOTFILEOUT.c_str(), "recreate");
		TTree *tree = new TTree("tree", "NaI Detector Data");
			tree->Branch("Waveforms", data, "data[1024]/F");
			tree->Branch("Height", &min, "min/F");
			tree->Branch("Area", &area, "area/F");
			tree->Branch("Saturated", &saturated, "saturated/B");
			tree->Branch("Energy", &energy, "energy/F");
			tree->Branch("MeanTime", &meantime, "meantime/F");

	for(Int_t e = 0; t.GetEntry(e) > 0; e++){
	
		Waveform waveform(data);
		min		= waveform.GetMin();
		area   	  	= waveform.GetArea();
		saturated 	= waveform.GetSaturated();
		energy 		= waveform.GetEnergy();
		meantime	= waveform.GetMeanTime();

		tree->Fill();
	}

	tree->Write();
	if(verbose)
		tree->Print();
	fout->Close();
	delete fout;
	return 1;
}

int updateall(string parent, string version, bool keep, bool verbose){

DIR *dir;
struct dirent *ent;
size_t foundprocessed;
size_t foundroot;
size_t length;
int updated = 0;
if((dir = opendir(parent.c_str())) != NULL) {
        while((ent = readdir(dir)) != NULL) {
                string child = ent->d_name;
                string pathtochild = parent;
                pathtochild.append(child);
                foundprocessed = pathtochild.find("Processed");
                foundroot = pathtochild.find(".root");
                if(foundprocessed != string::npos && foundroot != string::npos){
                        cout << "Updating " << pathtochild << endl;
                        length = foundroot - foundprocessed;
                        string rootfileout = pathtochild;
                        string newstring = "Processed_v";
                        newstring.append(version);
                        rootfileout.replace(foundprocessed, length, newstring);
			if(rootfileout.compare(pathtochild) == 0){
				cout << rootfileout << " already exists!" << endl;
				cout << "skipping " << rootfileout << endl;
			}
			else {
                        	if(process(rootfileout, pathtochild, verbose) == 0)
					cout << pathtochild << " was not updated" << endl;
				else { 
					updated += 1;
					if(!keep)
						remove(pathtochild.c_str());
				}
			}
			cout << endl;
                }
                if(child.substr(0,1).compare(".") != 0){
                        pathtochild.append("/");
                        updated += updateall(pathtochild, version, keep, verbose);
                }
        }
        closedir(dir);
	return updated;
}
else {
        return 0;
}


}




