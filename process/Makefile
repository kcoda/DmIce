CXX = g++
CXXFLAGS = -I/projects/physics/apps/root-5.34/include/root -L/projects/physics/apps/root-5.34/lib/root -std=c++11 -lCore -lRIO -lTree -lHist


all: tree ntree process update find

tree:
	g++ tree.cpp -I/projects/physics/apps/root-5.34/include/root -L/projects/physics/apps/root-5.34/lib/root -lCore -lRIO -lTree -lHist -o tree
ntree:
	g++ ntree.cpp -I/projects/physics/apps/root-5.34/include/root -L/projects/physics/apps/root-5.34/lib/root -lCore -lRIO -lTree -lHist -o ntree

process: waveform.o process.o process_main.o
	$(CXX) $(CXXFLAGS) waveform.o process.o process_main.o -o process

process_main.o: process_main.cpp 
	$(CXX) $(CXXFLAGS) -c process_main.cpp 

process.o: process.cpp process.h
	$(CXX) $(CXXFLAGS) -c process.cpp process.h

waveform.o: waveform.cpp waveform.h 
	$(CXX) $(CXXFLAGS) -c waveform.cpp waveform.h

update: waveform.o process.o update_main.o
	$(CXX) $(CXXFLAGS) waveform.o process.o update_main.o -o update

update_main.o: update_main.cpp
	$(CXX) $(CXXFLAGS) -c update_main.cpp

find:
	$(CXX) $(CXXFLAGS) find.cpp -o find


	

clean : 
	-rm *.o *.gch process update 
