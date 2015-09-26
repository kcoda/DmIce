#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "TFile.h"

class Waveform {

public:

	Waveform(Float_t data[]);
	Float_t GetMin();
	Float_t GetArea();
	Float_t GetEnergy();
	Float_t GetMeanTime();
	Bool_t 	GetSaturated();



private:
	const Int_t RECORD_LENGTH = 1000;
	Float_t  _min = 0;
	Float_t  _area;
	Float_t  _energy;
	Float_t  _meantime;
	Bool_t	 _saturated = false;
};

#endif
