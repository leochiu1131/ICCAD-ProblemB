#ifndef _Pins_H
#define _Pins_H
#include <iostream>
#include <map>
using namespace std;

class Pins{
    friend class FF;
    friend class Gate;
    private:
        double LocationX;
        double LocationY;
        double slack;
        string type;//d q in out 
        //net?
    public:
        Pins(){
        }
        void SetX(float x) {
            LocationX = x;
        }
        void SetY(float y) {
            LocationY = y;
        }
        void addX(float x) {
            LocationX += x;
        }
        void addY(float y) {
            LocationY += y;
        }
        void SetSlack(double s) {
            slack = s;
        }
        double getx ()
         
        {
            return LocationX;
        }
        double gety()
        {
            return LocationY;
        }
        double getx ()
        const 
        {
            return LocationX;
        }
        double gety()
        const
        {
            return LocationY;
        }
        void setpintype(string s)
        {
                type=s[0];    
        } 
        string getpintype()
        {
            return type;
        }
};



#endif