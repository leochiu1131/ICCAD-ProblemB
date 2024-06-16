#ifndef _Nets_H
#define _Nets_H
#include <iostream>
#include <map>
#include "pin.h"
#include "instance.h"
#include "ff.h"
using namespace std;

class Nets{
    private:
        map<string,Pins>netlist;
        map<string,Pins> Dnetlist;
        map<string,Pins> Qnetlist;
        map<string,Pins> INnetlist;
        map<string,Pins> OUTnetlist;
        map<string,Pins> CLKnetlist;
        map<string, instance>instNet;
    public:
        Nets(){

        }
         void Setnet(string s, Pins p) {
            netlist.insert(pair<string, Pins>(s, p));
        }
         void SetDnet(string s, Pins p) {
            Dnetlist.insert(pair<string, Pins>(s, p));
        }
         void SetQnet(string s, Pins p) {
            Qnetlist.insert(pair<string, Pins>(s, p));
        }
         void SetINnet(string s, Pins p) {
            INnetlist.insert(pair<string, Pins>(s, p));
        }
        void SetOUTnet(string s, Pins p) {
            OUTnetlist.insert(pair<string, Pins>(s, p));
        }
        void SetCLKnet(string s, Pins p) {
            CLKnetlist.insert(pair<string, Pins>(s, p));
        }

        void add(map<string,instance>&inst,string s){ 
            size_t pos = s.find('/');

            if (pos != string::npos) 
            {
                string inst_name = s.substr(0, pos);
                string pin_name = s.substr(pos + 1);
                
            } 
            else 
            {
                cout << "The string does not contain '/'" << std::endl;
            }
            instance& ins=inst.find(inst_name);
            Pins& p=  ins.GetPins(pin_name);
            bool added=0;
            if(pin_name[0]=='D')
            {
                SetDnet(s,p);
                added=1;
            }
            if(pin_name[0]=='Q')
            {
                SetQnet(s,p);
                added=1;
            }
            if(pin_name[0]=='I')
            {
                SetINnet(s,p);
                added=1;
            }
            if(pin_name[0]=='O')
            {
                SetOUTnet(s,p);
                added=1;
            }
            if(pin_name[0]=='C'||pin_name[0]=='c')
            {
                SetCLKnet(s,p);
                added=1;
            }
            if(!added)
            {

                cout<<inst_name<<pin_name<<"bug"<<endl;
            }


            
            //s代表C1/D or C1/Q
            //取出斜線後的pin點，前面是instance的名字，後面是pin的名字

            //用前面的名字去找出instance，再從後面的名字去找出對應的pin點

            //找到之後加到netlist裡面
        }
       
};



#endif