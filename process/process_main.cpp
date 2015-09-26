#include "process.h"

#include <vector>
#include <iostream>

using namespace std;

/* arguments syntax: quiet rootfileout rootfilein
 *
 * rootfileout is the full path to the new file to be created
 * rootfilein is the full path to the old file to be read
 * quiet is an optional argument used to specify to not print out the tree information. By default, process prints out the tree info
 *
 *
 * example syntax: 	./process /projects/physics/dmice/new.root /projects/physics/dmice/oldfile.root
 * 			./process quiet /projects/physics/dmice/new.root /projects/physics/dmice/oldfile.root 
 * 			./process quiet /projects/physics/dmice/new.root /projects/physics/dmice/Waveforms*.root (wildcarding)
 * */

int main(int argc, char* argv[]){
	
	bool quiet = false;
	if(argc < 3){
		cout << "ERROR: process needs at least 2 arguments" << endl;
		cout << "See process_main.cpp for arguement syntax" << endl;
		return 0;
	
	}
	string arg1(argv[1]);
	if(arg1.substr(0,5).compare("quiet") == 0){
		quiet = true;
	}
	if(!quiet){
		if(argc == 3){
			string fileout(argv[1]);
			string filein(argv[2]);
			return process(fileout, filein, !quiet);
			
		}
		else {
			string fileout(argv[1]);
			vector<string> filein_list;
			for(int i = 2; i < argc; i++){
				string filein(argv[i]);
				filein_list.push_back(filein);
			}
			return process(fileout, filein_list, !quiet);
			
		}
	}
	else {			
		if(argc == 3){
			cout << "ERROR: process needs both a filname to read in and a filename to output" << endl;
			return 0;
		}
		else if(argc == 4){
			string fileout(argv[2]);
			string filein(argv[3]);
			return process(fileout, filein, !quiet);			
		
		}
		else {
			string fileout(argv[2]);
			vector<string> filein_list;
			for(int i = 3; i < argc; i++){
				string filein(argv[i]);
				filein_list.push_back(filein);
			}
			return process(fileout, filein_list, !quiet);
			
		}
	}
}
			
