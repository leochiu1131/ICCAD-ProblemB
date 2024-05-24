#ifndef _Pins_.H
#define _Pins_.H
#include <iostream>
#include <map>
using namespace std;

class Pins{
    friend class FF;
    friend class Gate;
    private:
        int LocationX;
        int LocationY;
        //net?
    public:
        Pins(int X,int Y){
            LocationX=X;
            LocationY=Y;
        }
};



#endif