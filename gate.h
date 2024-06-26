#ifndef _Gate_H
#define _Gate_H
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
        void add_Pin(string s, Pins p) {
            PinList.insert(pair<string, Pins>(s, p));
        }
        void SetWidth(float x) {
            Width = x;
        }
        void SetHeight(float x) {
            Height = x;
        }
        void SetpinCount(int x) {
            pinCount = x;
        }
        void SetSlack(string s, double x) {
            PinList[s].SetSlack(x);
        }
        int GetpinCount() {
            return pinCount;
        }
        map<string, Pins>& GetPinlist() {
            return PinList;
        }
        Pins GetPins(string s) {
            return PinList[s];
        }
        double getheight()
        {
            return Height;
        }
        double getwidth()
        {
            return Width;
        }
};



#endif