#ifndef _instance_.H
#define _instance_.H
#include <iostream>
using namespace std;
#include <map>
#include "pin.h"
#include "ff.h"
#include "Gate.h"
using namespace std;

class instance{
    private:
        double leftdownx ,leftdowny;
        FF* fftype;
        Gate* gatetype;
        string name;
    public:
        instance(){

        }
        void add(string names,double x,double y,FF* ff){
            name=names;
            fftype=ff;
            leftdownx=x;
            leftdowny=y;
        }
        string Getname(){
            return name;
        }
        
};



#endif