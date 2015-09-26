#include "process.h"
#include <iostream>

using namespace std;


/* argument syntax: parentdirectory version keep verbose
 * parentdirectory is the highest directory to search for files to update. example: /projects/physics/dmice/
 * version is the version of the new file. Created files with have this version  
 * keep is an optional parameter to specify to not remove the original file. type keep in this field
 * verbose is an optional parameter to specify to print out root tree data. By default update will not print tree data
 * 
 * example syntax: 	./update /projects/physics/dmice/Fermilab/ 02 keep
 *			./update /projects/physics/dmice/ test verbose
 */

int main(int argc, char* argv[]){
	
	bool keep;
	bool verbose = false;
	if(argc > 5){
		cout << "ERROR: too many arguements!" << endl;
		return 0;
	}
	if(argc < 3){
		cout << "ERROR: upate needs at least 2 arguments" << endl;
		cout << "Refer to update_main.cpp for syntax definition" << endl;
		return 0;
	}
	if(argc == 4){
		string arg3(argv[3]);
		if(arg3.substr(0,4).compare("keep") == 0){
			keep = true;
		}
		else if(arg3.substr(0,7).compare("verbose") == 0){
			verbose = true;	
		}
		else {
			cout << "3rd argument must be blank, 'keep' , or 'verbose'" << endl;
			return 0;
		}
	}
	else {
		keep = false;
	}
	if(argc == 5){
		string arg4(argv[4]);
		if(arg4.substr(0,7).compare("verbose") == 0){
			verbose = true;	
		}
	}
		
	string arg1(argv[1]);
	string arg2(argv[2]);
	int total = updateall(arg1, arg2, keep, verbose);
	cout << "Updated " << total << " file(s) to version " << arg2 << endl;
	return 1;
}



