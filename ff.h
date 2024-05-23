#ifndef _FlipFlop_.H
#define _FlipFlop_.H
#include <iostream>
#include <map>
#include "pin.h"
using namespace std;

class FF{
    private:
        int bits;
        int Width;
        int Height;
        int pinCount;
        map<string,Pins>PinList;
    public:
        FF(){

        }
        void add(int bit,int w,int h,int pC){
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