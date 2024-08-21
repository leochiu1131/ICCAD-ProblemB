#ifndef _instance_H
#define _instance_H
#include <iostream>

#include <map>
#include "pin.h"
#include "ff.h"
#include "gate.h"

using namespace std;
class clique;
class instance{
    private:
        double leftdownx ,leftdowny;
        FF fftype;
        Gate gatetype;
        bool type; //0:FF,1:Gate
        string libname;
        double slack;
        
    
        
    public:
        map<string,Pins> todpin;
        map<string,vector<Pins>> toqpin;
        instance(){

        }
        instance(string s)
        {
            libname=s;
        }
        

        
        string Getlibname(){
            return libname;
        }
        bool Gettype() {
            return type;
        }
        void Setlibname(string s) {
            libname = s; 
            
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
        Pins& GetPins(string s) {
            if (!type) {
                return fftype.GetPins(s);
            }
            else {
                return gatetype.GetPins(s);
            }
        }
        void SetSlack( double x) {
            
            slack=x;
        }
        double GetX() {
            return leftdownx;
        }
        double GetY() {
            return leftdowny;
        }
       
        double Getslack()
        {
            return slack;
        }
        FF getff()
        {
                return fftype;
            
        }
        Gate getgate()
        {
            return gatetype;

        }
};



#endif