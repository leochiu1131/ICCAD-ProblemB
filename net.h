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
        map<string, instance>instNet;
    public:
        Nets(){

        }
        void add(map<string,instance>&inst,string s){ //s代表C1/D or C1/Q
            //取出斜線後的pin點，前面是instance的名字，後面是pin的名字

            //用前面的名字去找出instance，再從後面的名字去找出對應的pin點

            //找到之後加到netlist裡面
        }
        void Setnet(string s, Pins p) {
            netlist.insert(pair<string, Pins>(s, p));
        }
};



#endif