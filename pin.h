#ifndef _Pins_.H
#define _Pins_.H
#include <iostream>
#include <map>
using namespace std;

class Pins{
    friend class FF;
    friend class Gate;
    private:
        double LocationX;
        double LocationY;
        //net?
    public:
        Pins(float X,float Y){
            LocationX=X;
            LocationY=Y;
        }
};



#endif