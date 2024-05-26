#ifndef _Gate_.H
#define _Gate_.H
#include <iostream>
#include <map>
#include "pin.h"
using namespace std;

class Gate{
    private:
        double Width;
        double Height;
        int pinCount;
        map<string,Pins>PinList;
    public:
        Gate(){
        }
        void add(double w,double h,int pC){
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