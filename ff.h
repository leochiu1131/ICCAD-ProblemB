#ifndef _FlipFlop_.H
#define _FlipFlop_.H
#include <iostream>
#include <map>
#include "pin.h"
using namespace std;
/*
FlipFlop 1 FF1 5 10 2
Pin D 0 8
Pin Q 5 8*/
class FF{
    private:
        int bits;
        double Width;
        double Height;
        int pinCount;      
        map<string,Pins>PinList;
    public:
        FF(){

        }
        void add(int bit,float w,float h,int pC){
            bits=bit;
            Width=w;
            Height=h;
            pinCount=pC;
            for(int i=0;i<pinCount;i++){
                string name;
                int X,Y;
                Pins temp(X,Y);
                PinList.insert(pair<string,Pins>(name,temp));
            }
        }
};



#endif