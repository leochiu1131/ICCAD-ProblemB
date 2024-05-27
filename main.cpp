#include <iostream>
#include <map>
#include "pin.h"
#include "ff.h"
#include "gate.h"
#include "net.h"
#include "instance.h"
using namespace std;
int main(){
    double Alpha,Beta,Gamma,Lambda;

    double Die_LLeftX,Die_LLeftY,Die_URightX,Die_URightY;
    bool **Die_check;

    map<string,Pins>Input_pins;
    map<string,Pins>Output_pins;

    map<string,FF>FF_lib;
    map<string,Gate>GG_lib;

    map<string,instance>inst_lib;
    map<string,Nets>Net_lib;

    double BinWidth,BinHeight,BinMaxUtil;

    double DisplacementDelay;
    map<string,double>TimingSlack;
    return 0;
}