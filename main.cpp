#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "pin.h"
#include "ff.h"
#include "gate.h"
#include "net.h"
#include "instance.h"
using namespace std;
struct placement {
    double startX;
    double startY;
    double siteWidth;
    double siteHeight;
    int NumofSite;
};
int main() {
    double Alpha, Beta, Gamma, Lambda;

    double Die_LLeftX, Die_LLeftY, Die_URightX, Die_URightY;
    bool** Die_check;

    map<string, Pins>Input_pins;
    map<string, Pins>Output_pins;

    map<string, FF>FF_lib;
    map<string, Gate>GG_lib;

    map<string, instance>inst_lib;
    map<string, Nets>Net_lib;

    double BinWidth, BinHeight, BinMaxUtil;

    vector<placement> placementRow;
    double DisplacementDelay;
    map<string, double>TimingSlack;

    ifstream infile;
    ofstream outfile;

    string s;
    double num;
    infile.open("sample.txt");

    infile >> s; //Alpha
    infile >> Alpha; 
    infile >> s; //Beta
    infile >> Beta;
    infile >> s; //Gamma
    infile >> Gamma;
    infile >> s; //Lambda
    infile >> Lambda;

    infile >> s; //Diesize 
    infile >> Die_LLeftX;
    infile >> Die_LLeftY;
    infile >> Die_URightX;
    infile >> Die_URightY;

    infile >> s; //Number of Input
    infile >> num;
    for (int i = 0; i < num; i++) {
        int coodinate;
        Pins tempPin;
        infile >> s; //Input
        infile >> s; //InputName
        infile >> coodinate;
        tempPin.SetX(coodinate);
        infile >> coodinate;
        tempPin.SetY(coodinate);

        Input_pins.insert(pair<string, Pins>(s, tempPin));
    }

    infile >> s; //Number of Output

    infile >> num;
    for (int i = 0; i < num; i++) {
        int coodinate;
        Pins tempPin;
        infile >> s; //Output
        infile >> s; //OutputName
        infile >> coodinate;
        tempPin.SetX(coodinate);
        infile >> coodinate;
        tempPin.SetY(coodinate);

        Output_pins.insert(pair<string, Pins>(s, tempPin));
    }

    infile >> s; //flip_flop

    while (s == "FlipFlop" || s=="Gate") {
        if (s == "FlipFlop") {
            FF tempFF;
            infile >> num;   //bits數量
            tempFF.Setbits(num);

            string ffname;
            infile >> ffname;//flipflop name

            infile >> num;   //flipflop Width
            tempFF.SetWidth(num);
            infile >> num;   //flipflop Height
            tempFF.SetHeight(num);
            infile >> num;   //flipflop pinCount
            tempFF.SetpinCount(num);

            for (int i = 0; i < tempFF.GetpinCount(); i++) {
                Pins tempPin;
                infile >> s;  //Pin

                infile >> s;  //Pin name
                infile >> num;//Pin x
                tempPin.SetX(num);
                infile >> num;//Pin y
                tempPin.SetY(num);
                tempFF.add_Pin(s, tempPin);
            }
            FF_lib.insert(pair<string, FF>(ffname, tempFF));
        }
        else {
            Gate tempGate;
            string gatename;
            cin >> gatename; //Gate name

            infile >> num;   //flipflop Width
            tempGate.SetWidth(num);
            infile >> num;   //flipflop Height
            tempGate.SetHeight(num);
            infile >> num;   //flipflop pinCount
            tempGate.SetpinCount(num);
            for (int i = 0; i < tempGate.GetpinCount(); i++) {
                Pins tempPin;
                infile >> s;  //Pin

                infile >> s;  //Pin name
                infile >> num;//Pin x
                tempPin.SetX(num);
                infile >> num;//Pin y
                tempPin.SetY(num);
                tempGate.add_Pin(s, tempPin);
            }
            GG_lib.insert(pair<string, Gate>(gatename, tempGate));
        }
        infile >> s;
    }
    
    //s:NumInstance
    int instanceCount;
    infile >> instanceCount; //instanceCount
    for (int i = 0; i < instanceCount; i++) {
        instance tempinst;
        infile >> s; //Inst
        string instName;
        infile >> instName; //instName
        infile >> s; //libCellname
        tempinst.Setname(s,inst_lib,FF_lib,GG_lib);
        if (!tempinst.Gettype()) { //代表是flip flop
            tempinst.SetFF(FF_lib[s]);
        }
        else { //代表是Gate
            tempinst.SetGate(GG_lib[s]);
        }
        infile >> num;//x coodinate
        tempinst.SetX(num);
        infile >> num;//y coodinate
        tempinst.SetY(num);

        inst_lib.insert(pair<string, instance>(instName, tempinst));
    }

    infile >> s; //NumNets
    int netCount;
    infile >> netCount;
    const char* F;
    char part1[100], part2[100];
    int len;
    char slash[2];
    for (int i = 0; i < netCount; i++) {
        infile >> s; //Net
        string netname;
        int numPins;
        infile >> netname; //Netname
        infile >> numPins; //numPins
        Nets tempnet;
        for (int i = 0; i < numPins; i++) {
            infile >> s; //Pin
            infile >> s;
            F = s.c_str();
            len=sscanf_s(F, "%[^/]%c%s", part1,100, slash,5, part2,100);
            if (len == 3) {
                instance tempinst;
                Pins temppin;
                tempinst = inst_lib[part1];
                temppin = tempinst.GetPins(part2);
                tempnet.Setnet(s, temppin);
            }
            else {
                auto it = Input_pins.find(s);
                if (it != Input_pins.end()) {
                    tempnet.Setnet(s, Input_pins[s]);
                }
                else {
                    tempnet.Setnet(s, Output_pins[s]);
                }
            }
            //cout << part1 << "," <<len<<"," << part2 << endl;
        }
    }

    infile >> s; //BinWidth
    infile >> BinWidth;
    infile >> s; //BinHeight
    infile >> BinHeight;
    infile >> s; //BinMaxUtil
    infile >> BinMaxUtil;

    infile >> s; //PlacementRows;
    while (s == "PlacementRows") {
        placement temp;
        infile >> temp.startX;
        infile >> temp.startY;
        infile >> temp.siteWidth;
        infile >> temp.siteHeight;
        infile >> temp.NumofSite;
        placementRow.push_back(temp);
        infile >> s;
    }

    //"Now" s is DisplacementDelay
    infile >> DisplacementDelay;
    infile >> s; //QpinDelay
    while (s == "QpinDelay") {
        infile >> s;
        infile >> num;
        FF_lib[s].SetQpinDelay(num);
        infile >> s;
    }
    //"Now" s is TimingSlack
    while (s == "TimingSlack") {
        string temp;
        infile >> s; //instance name
        infile >> temp;
        infile >> num;
        inst_lib[s].SetSlack(temp, num);
        infile >> s;
    }

    //"Now" s is GatePower
    while (s == "GatePower") {
        cout << "hi" << endl;
        infile >> s;
        infile >> num;
        FF_lib[s].SetPower(num);
        infile >> s;
    }


    infile.close();
    outfile.close();
    return 0;
}
