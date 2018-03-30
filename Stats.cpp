/******************************
 * Ryan Thompson
 * CS 3339 - Spring 2018 Section 263
 ******************************/
#include "Stats.h"

Stats::Stats() {
  cycles = PIPESTAGES - 1; // pipeline startup cost
  flushes = 0;
  bubbles = 0;
  memops = 0;
  branches = 0;
  taken = 0; 
  totalRAWHazards = 0;

  //pipeline iitialization
  for(int i = IF1; i < PIPESTAGES; i++) {
    resultReg[i]   = -1;
	resultStage[i] = -1;
  }
}

void Stats::clock() {
  cycles++;

  // advance all pipeline flops
  for(int i = WB; i > IF1; i--)
  { 
	  resultReg[i]   = resultReg[i-1];
	  resultStage[i] = resultStage[i-1];
  }
  
  resultReg[IF1]   = -1;
  resultStage[IF1] = -1;
}

///////////////////////////////////////////////////////////
// Checks for destination registers still being processesed
// in the pipeline identical to the source register
///////////////////////////////////////////////////////////
void Stats::registerSrc(int r, PIPESTAGE needed) {
//cycles through stages
    for( int i = EXE1; i < WB; i++ )
	{
		//checks to see if destination stage matches a source stage
       if( resultReg[i] == r )
	   {
		   //Pre-bubble pipeline print
		   //cout << "Source register == " << r << endl;
		   //cout << "Result Stage == " << resultStage[i] << endl;
	       //cout << "Before: ";
		   //printPipeline();
		   //cout << endl;
		   
		   totalRAWHazards++;
		   numRAWHazards[i]++;
		   
			//injects bubbles
            for( int j = i; j < resultStage[i]-1; j++ )
			{
				bubble();
				//cout << "        ";
				//printPipeline();
				//cout << endl;
            }
			
			
			//prints final pipeline stage
			//cout << "After:  ";
			//printPipeline();
			//cout << endl << endl;
			break; //stops the check loop from looping
        }
    }
}

void Stats::registerDest(int r, PIPESTAGE valid) {
	resultReg[ID] = r;
	resultStage[ID] = valid;
}

//Overwrites the the fetch instructions with a noop
void Stats::flush(int count) { // count == how many ops to flush
    for(int i = 0; i < count; i++){ 
		clock();
		flushes++;
	}
}

//Injects a bubbles after the ID stage
void Stats::bubble() {
    cycles++;
    bubbles++;
    // advance pipeline flops in front if ID
    for(int i = WB; i > EXE1; i--){ 
		resultReg[i] = resultReg[i-1];
	}
	resultReg[EXE1] = -1;
}

void Stats::printPipeline(){
	for(int k = IF1; k < PIPESTAGES; k++) 
		{ cout << resultReg[k] << " "; }
}

void Stats::getHazardReport(){
	cout << "RAW hazards: "  << totalRAWHazards << " ( 1 per every " 
		                     << instructions/(float)totalRAWHazards << " instructions)" << endl;
	cout << "  On EXE1 op: " << numRAWHazards[EXE1] << " (" 
		                     << (int)((numRAWHazards[EXE1]/(float)totalRAWHazards) * 100) << " %)" << endl;
	cout << "  On EXE2 op: " << numRAWHazards[EXE2] << " ("
	                         << (int)((numRAWHazards[EXE2]/(float)totalRAWHazards) * 100) << " %)" << endl;
	cout << "  On MEM1 op: " << numRAWHazards[MEM1] << " ("
	                         << (int)((numRAWHazards[MEM1]/(float)totalRAWHazards) * 100) << " %)" << endl;
	cout << "  On MEM2 op: " << numRAWHazards[MEM2] << " ("
		                     << (int)((numRAWHazards[MEM2]/(float)totalRAWHazards) * 100) << " %)" << endl;
	//cout << "Temp = " << getTemp() << endl;
}
