#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

int process(std::string ROOTFILEOUT, std::string ROOTFILEIN, bool verbose);

int process(std::string ROOTFILEOUT, std::vector<std::string> ROOTFILEIN, bool verbose);

int updateall(std::string parentdirectory, std::string version, bool keep, bool verbose);


#endif
