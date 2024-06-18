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
        map<string,Pins> todpin;
        map<string,Pins> toqpin;
        instance(){

        }
        string Getlibname(){
            return libname;
        }
        bool Gettype() {
            return type;
        }
        void Setname(string s, bool t) {
            libname = s; 
            type = t;
        }
  void SetX(float x) 
        {
            leftdownx = x;
            
            if (!type)
            {
                map<string, Pins>& templist = fftype.GetPinlist();
                for (auto P = templist.begin();P!=templist.end();P++) {
                
                //cout << P->second.getx()<<"to";
                P->second.addX(x);
               // cout << P->second.getx()<<endl;
                }

            }
            else 
            {
                map<string, Pins>& templist = gatetype.GetPinlist();
                for (auto P = templist.begin();P!=templist.end();P++) {
                
               // cout << P->second.getx()<<"to";
                P->second.addY(x);
               // cout << P->second.getx()<<endl;
                }

            }
            

            
        }
        void SetY(float y) 
        {
            leftdowny = y;
            
            if (!type)
            {
                map<string, Pins>& templist = fftype.GetPinlist();
                for (auto P = templist.begin();P!=templist.end();P++) {
                
               // cout << P->second.gety()<<"to";
                P->second.addY(y);
                //cout << P->second.gety()<<endl;
                }

            }
            else 
            {
                map<string, Pins>& templist = gatetype.GetPinlist();
                for (auto P = templist.begin();P!=templist.end();P++) {
                
                //cout << P->second.gety()<<"to";
                P->second.addY(y);
               // cout << P->second.gety()<<endl;
                }

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
        double GetX() {
            return leftdownx;
        }
        double GetY() {
            return leftdowny;
        }
};



#endif