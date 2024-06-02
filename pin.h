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
};



#endif