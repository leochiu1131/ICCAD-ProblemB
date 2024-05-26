#include <iostream>
using namespace std;#ifndef _instance_.H
#define _instance_.H
#include <iostream>
#include <map>
#include "pin.h"
using namespace std;
/*
FlipFlop 1 FF1 5 10 2
Pin D 0 8
Pin Q 5 8*/
class instance{
    private:
        double leftdownx ,leftdowny;
        FF* fftype;
        gate* gatetype;
        string name;
    public:
        instance(){

        }
        void add(string names,string fftype ,double x,double y,FF* ff)
        {
            name=names;
            fftype=ff;
            leftdownx=x;
            leftdowny=y;
        }
};



#endif