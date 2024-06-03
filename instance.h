#ifndef _instance_H
#define _instance_H
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
        FF fftype;
        Gate gatetype;
        bool type; //0:FF,1:Gate
        string libname;
    public:
        instance(){

        }
        string Getname(){
            return libname;
        }
        bool Gettype() {
            return type;
        }
        void Setname(string s,map<string, instance>inst_lib, map<string, FF>FF_lib,map<string, Gate>GG_lib) {
            libname = s; 
            auto it = FF_lib.find(libname);
            if (it != FF_lib.end()) {
                type = 0;
                return;
            }
            else {
                type = 1;
            }
        }
        void SetX(float x) {
            leftdownx = x;
            map<string, Pins> templist;
            if (!type)templist = fftype.GetPinlist();
            else templist=gatetype.GetPinlist();
                
            for (auto& P : templist) {
                P.second.addX(x);
            }
        }
        void SetY(float y) {
            leftdowny = y;
            map<string, Pins> templist;
            if (!type)templist = fftype.GetPinlist();
            else templist = gatetype.GetPinlist();

            for (auto& P : templist) {
                P.second.addY(y);
            }
        }
        void SetFF(FF f) {
            fftype = f;
        }
        void SetGate(Gate g) {
            gatetype = g;
        }
        Pins GetPins(string s) {
            if (!type) {
                return fftype.GetPins(s);
            }
            else {
                return gatetype.GetPins(s);
            }
        }
        void SetSlack(string s, double x) {
            if (!type) {
                fftype.SetSlack(s,x);
            }
            else {
                gatetype.SetSlack(s, x);
            }
        }
};



#endif