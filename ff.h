#ifndef _FlipFlop_H
#define _FlipFlop_H
#include <iostream>
#include <map>
#include "pin.h"
using namespace std;
/*
FlipFlop 1 FF1 5 10 2
Pin D 0 8
Pin Q 5 8*/

class FF{
    private:
        int bits;
        double Width;
        double Height;
        int pinCount;
        double cost;
        double QpinDelay;
        double Power;
        map<string,Pins>PinList;    
        
        
    public:
    
    map<int,pair<Pins,Pins>> qdpinpair;  
        FF(){

        }
        double getheight()
        {
            return Height;
        }
        double getwidth()
        {
            return Width;
        }
        double getpoewer()
        {
            return Power;
        }
        void setqdpinpair()
        {
            if(bits>1)
            {
                for(int i=0;i<bits;i++)
                {
                    Pins d=PinList.find("D"+to_string(i))->second;
                    Pins q=PinList.find("Q"+to_string(i))->second;
                    pair<Pins,Pins> tmp(d,q);
                    qdpinpair.insert(pair<int,pair<Pins,Pins>>(i,tmp));
                }
            }
            else
            {
                Pins d=PinList.find("D")->second;
                Pins q=PinList.find("Q")->second;
                pair<Pins,Pins> tmp(d,q);
                qdpinpair.insert(pair<int,pair<Pins,Pins>>(0,tmp));
            }
            
        }
        void add_Pin(string s, Pins p){
            
            PinList.insert(pair<string, Pins>(s, p));
        }
        
        void SetWidth(double x) {
            Width = x;
        }
        void SetHeight(double x) {
            Height = x;
        }
        void Setbits(int x) {
            bits = x;
        }
        void SetpinCount(int x) {
            pinCount = x;
        }
        void SetQpinDelay(double x) {
            QpinDelay = x;
        }
        void SetPower(double x) {
            Power = x;
        }
        void SetSlack(string s,double x) {
            PinList[s].SetSlack(x);
        }
        int GetpinCount() {
            return pinCount;
        }
        int Getpin() {
            return bits;
        }
        map<string, Pins>& GetPinlist() {
            return PinList;
        }
        Pins GetPins(string s) {
            return PinList[s];
        }
        double Getcost(double beta ,double gamma){
            return beta*Width*Height+gamma*Power;
        }
        double Getcostperbit(double beta ,double gamma){
            return (beta*Width*Height+gamma*Power)/bits;
        }
        int getbit()
        {
            return bits;
        }
        double Getdelay()
        {
            return QpinDelay;
        }
        double getarea()
        {
            return Width*Height;
        }

};



#endif