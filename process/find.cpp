#include "dirent.h"
#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std;


float parse_trigger(string the_file){
	size_t trigger_idx = the_file.find("trigger");
	if(trigger_idx == string::npos)
		trigger_idx = the_file.find("Trigger");
	if(trigger_idx == string::npos)
		trigger_idx = the_file.find("trig");
	if(trigger_idx == string::npos)
		return 0;
	size_t colon_idx = the_file.find(":", trigger_idx);
	if(colon_idx - trigger_idx > 9)
		return 0;
	string the_number = the_file.substr(colon_idx+1, 10);
	if(the_number.substr(0, 1).compare(" ") == 0)
		the_number.erase(0, 1);
	return stof(the_number);


}
float parse_resolution(string the_file){

	string accepted[6] = {"voltrange", "Voltrange", "range", "Range", "resolution", "Resolution"};
	size_t reso_idx = string::npos;
	for(int i = 0; i < 6 && reso_idx == string::npos; i++){
		reso_idx = the_file.find(accepted[i]);
	}	
	size_t colon_idx = the_file.find(":", reso_idx);
	if(colon_idx - reso_idx > 13)
		return 0;
	string the_number = the_file.substr(colon_idx+1, 10);
	if(the_number.substr(0, 1).compare(" ") == 0)
		the_number.erase(0, 1);
	return stof(the_number);


}


string file_to_string(string filename){
	FILE* the_file = fopen(filename.c_str(), "r");
	if(the_file == NULL){
		cout << "Error attempting to open " << filename << endl;
		return "";
	}	
	fseek(the_file, 0, SEEK_END);
	long file_length = ftell(the_file);
	rewind(the_file);
	char file_text[file_length];
	fread(file_text, 1, file_length, the_file);
	fclose(the_file);
	string rtrn(file_text);
	return rtrn;

}

void print_file(string filename){
	FILE* the_file = fopen(filename.c_str(), "r");
	char line[250];
	cout << filename << endl;
	while( fgets(line, 100, the_file) != NULL)
		cout << "\t" << line;
	cout << endl;
	fclose(the_file);
}



int find_info(string parent, int bias, float trigger, float resolution){

DIR *dir;
struct dirent *ent;
size_t foundinfo;
long file_length;
int updated = 0;
if((dir = opendir(parent.c_str())) != NULL) {
        while((ent = readdir(dir)) != NULL) {
                string child = ent->d_name;
                string pathtochild = parent;
		pathtochild.append("/" + child);
                foundinfo = pathtochild.find("Info.txt");
                if(foundinfo != string::npos) {
			string file_text_str = file_to_string(pathtochild);
			if(file_text_str.size() == 0)
				return 0;
			bool correct_resolution = parse_resolution(file_text_str) == resolution || resolution == 0;
			bool correct_trigger	= parse_trigger(file_text_str) == trigger || trigger == 0;
			if(correct_resolution && correct_trigger)
				print_file(pathtochild);

		}
		else if(child.substr(0, 1).compare(".") != 0)
			find_info(pathtochild, bias, trigger, resolution);

        }
        closedir(dir);
	return updated;
}
else {
        return 0;
}


}


int main(int argc, char* argv[]){

	if(argc < 2){
		cout << "find needs at least a path to the parent directory to be searched" << endl;
		cout << "See find.cpp for argument syntax" << endl;
		return 0;
	
	}

	string arg1(argv[1]), arg2_str(""), arg3_str(""), arg4_str("");
	float arg2 = 0, arg3 = 0, arg4 = 0;

	for(int i = 2; i < argc; i++){
		string temp(argv[i]);
		size_t length = temp.size();
		size_t trigger_idx = temp.find("trigger:");
		size_t resolution_idx = temp.find("resolution:");
		size_t bias_idx = temp.find("bias:");
		if(trigger_idx != string::npos){
			arg3 = parse_trigger(temp);
		}
		if(resolution_idx != string::npos){
			arg4 = parse_resolution(temp);
		}
		if(bias_idx != string::npos){
			arg2 = 0;     
		}
	}

	//cout << "passing arguments" << endl << arg2 << endl << arg3 << endl << arg4 << endl;

	
	find_info(arg1, arg2, arg3, arg4);
}






