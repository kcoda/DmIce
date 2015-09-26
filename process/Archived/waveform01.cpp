#include "TFile.h"
#include "waveform.h"
#include <iostream>


/* VERSION 01 */



using namespace std;

Waveform::Waveform(Float_t data[]){

	Int_t minLocation;
	Int_t zero_counter = 0;
	Int_t pulse_start;
	Int_t pulse_end;
	Int_t meantime_start;
	Float_t offavg = 0;
	Float_t corrected_data[RECORD_LENGTH];
	Float_t summed_area = 0;
	Float_t weight;
	Float_t avg;
	Bool_t found_pulse_start = false;
	Bool_t found_pulse_end   = false;
	Bool_t meantime_valid    = true;
	for(Int_t i = 0; i < 200; i++){
		offavg += data[i];
	}
	offavg = offavg/200;
	for(Int_t i = 0; i < RECORD_LENGTH; i++){

		corrected_data[i] = data[i] - offavg;
		
		if(i < RECORD_LENGTH - 2 && !_saturated) {
			if(data[i] == data[i+1] && data[i] == data[i+2] && corrected_data[i] < -1){
				_saturated = true;
			}
		}

		if(corrected_data[i] < _min){
			_min = corrected_data[i];
			minLocation = i;
		}

		if(i > 3){
			Float_t local_avg = corrected_data[i-4] 
					  + corrected_data[i-3] 
					  + corrected_data[i-2] 
					  + corrected_data[i-1] 
					  + corrected_data[i];
			
			if(local_avg < -.05 && !found_pulse_start){
				pulse_start = i - 4;
				found_pulse_start = true;
				summed_area +=(corrected_data[i-4]
					      +corrected_data[i-3]
					      +corrected_data[i-2]
					      +corrected_data[i-1]);
			}
			
			if(found_pulse_start && !found_pulse_end){
				summed_area += corrected_data[i];
			}

			if(local_avg >=0 && !found_pulse_end && found_pulse_start){
				zero_counter++;
				if(zero_counter == 15){
					pulse_end = i-2;
					found_pulse_end = true;
					summed_area -=(corrected_data[i-1] + corrected_data[i]);
				}
					
			}
		}

	}
	_area = -(summed_area);

	_energy = (_area*69.674)+1.8148;

	for(Int_t i = minLocation; i > 0; i--){
		
		if(corrected_data[i] < _min/2){
			meantime_start = i;
		}
	}

	for(Int_t i = meantime_start; i < (meantime_start +100) && i < RECORD_LENGTH; i++){
		
		weight += (i-meantime_start)*corrected_data[i];
		avg    += corrected_data[i];
		
		if(i == RECORD_LENGTH-1){
			meantime_valid = false;
		}
	}

	if(meantime_valid){
		_meantime = (weight/avg)*4;
	}
	else{
		_meantime = 0;
	}
	
	
}


Float_t Waveform::GetMin(){
	return _min;
}

Float_t Waveform::GetArea(){
	return _area;
}

Float_t Waveform::GetEnergy(){
	return _energy;
}

Float_t Waveform::GetMeanTime(){
	return _meantime;
}

Bool_t Waveform::GetSaturated(){
	return _saturated;
}

	








	







