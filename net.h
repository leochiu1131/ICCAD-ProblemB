#ifndef _Nets_H
#define _Nets_H
#include <iostream>
#include <map>
#include <cstring>
#include "pin.h"
#include "instance.h"
#include "ff.h"
using namespace std;

class Nets{
    private:
        map<string, instance>instNet;
        map<string, Pins> Dnetlist;
        map<string, Pins> Qnetlist;
        map<string, Pins> INnetlist;
        map<string, Pins> OUTnetlist;
    public:
        map<string, Pins> CLKnetlist;
        Nets(){

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
        void add(unordered_map<string, instance>&inst, string s,string inst_name,string pin_name,size_t pos, map<string, Pins>&Input_pins, map<string, Pins>&Output_pins) {
            if (pos == string::npos) {
                if (s[0] == 'I' || s[0] == 'i') {
                    SetINnet(s, Input_pins[s]);
                    return ;
                }
                else {
                    SetOUTnet(s, Output_pins[s]);
                    return ;
                }
            } 
            instance& ins = inst.find(inst_name)->second;
            Pins p=  ins.GetPins(pin_name);
            bool added = 0;
            if (pin_name[0] == 'D')
            {
                SetDnet(s, p);
                added = 1;
            }
            else if (pin_name[0] == 'Q')
            {
                SetQnet(s, p);
                added = 1;
            }
            else if (pin_name[0] == 'I')
            {
                SetINnet(s, p);
                added = 1;
            }
            else if (pin_name[0] == 'O')
            {
                SetOUTnet(s, p);
                added = 1;
            }
            else if (pin_name[0] == 'C' || pin_name[0] == 'c')
            {
                SetCLKnet(s, p);
                added = 1;
            }
            if (!added){
                cout << inst_name << pin_name << "bug" << endl;
            }
        }
        int GetSize() {
            return Dnetlist.size() + Qnetlist.size() + CLKnetlist.size() + INnetlist.size() + OUTnetlist.size();
        }
        int GetCLkSize() {
            return CLKnetlist.size();
        }
};



#endif