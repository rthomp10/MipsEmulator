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

  for(int i = IF1; i < PIPESTAGES; i++) {
    resultReg[i] = -1;
  }
}

void Stats::clock() {
  cycles++;

  // advance all pipeline flops
  for(int i = WB; i > IF1; i--) {
    resultReg[i] = resultReg[i-1];
  }
  // inject no-op into IF1
  resultReg[IF1] = -1;
}

void Stats::registerSrc(int r /* register index */) {
    //cycles thruogh the stages to see if the destination hasn't been written yet
    for( int i = IF1; i < PIPESTAGES; i ++ ){
        if( resultReg[r] == resultReg[i] ){
            //adds the necessary amount of bubbles required to reach WB
            for( int j = i; j < PIPESTAGES; j++ ){
                bubble();
            }
        }
    }
}

void Stats::registerDest(int r /* register index */) {
    resultReg[IF1] = r;
}

void Stats::flush(int count) { // count == how many ops to flush
    flushes += count;
}

void Stats::bubble() {
    bubbles++;
    cycles++;
}
