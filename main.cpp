#include <iostream>
#include <map>
#include <unordered_map>
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
void find_the_position(vector<instance>front, vector<instance>end, map<string, FF>FF_lib,bool**placement_check,
    vector<placement>placementRow) {
    //從給予的FF_Lib資訊以及front，找出一個最佳且可以放置的位置，放到end
}
void Merge(vector<instance>FF_same_CLK, bool** placement_check, vector<placement>placementRow, map<string, FF>FF_lib,
    map<int, map<string, FF>>FF_lib2) {
    //找出需要合併的FF，先將其FF存在front，並給予合併之後的FF_Lib，傳到find_the_position
    vector<instance>front, end;
    for (int i = 0; i < FF_same_CLK.size(); i++) {
        front.push_back(FF_same_CLK[i]);
    }


    find_the_position(front, end, FF_lib,placement_check,placementRow);
}

int main() {
    double Alpha, Beta, Gamma, Lambda;

    double Die_LLeftX, Die_LLeftY, Die_URightX, Die_URightY;

    map<string, Pins>Input_pins;
    map<string, Pins>Output_pins;

    map<string, FF>FF_lib;
    map<int, map<string, FF>>FF_lib2;
    map<string, Gate>GG_lib;

    unordered_map<string, instance>inst_lib;
    map<string, Nets>Net_Lib;

    double BinWidth, BinHeight, BinMaxUtil;

    vector<placement> placementRow;
    bool** placement_check;
    double DisplacementDelay;
    map<string, double>TimingSlack;

    ifstream infile;
    ofstream outfile;

    string s;
    double num;
    infile.open("testcase1.txt");
    //sample.txt
    //testcase1.txt

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
    
    while (s == "FlipFlop" || s == "Gate") {
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
            auto bitnums = FF_lib2.find(tempFF.Getpin());
            if (bitnums != FF_lib2.end())
            {
                bitnums->second.insert(pair<string, FF>(ffname, tempFF));
            }
            else
            {
                map<string, FF> tmpmap;
                tmpmap.insert(pair<string, FF>(ffname, tempFF));
                FF_lib2.insert(pair<int, map<string, FF>>(tempFF.Getpin(), tmpmap));

            }

        }
        else {
            Gate tempGate;
            string gatename;
            infile >> gatename; //Gate name

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

    cout << "inst" << endl;
    //s:NumInstance
    int instanceCount;
    infile >> instanceCount; //instanceCount
    for (int i = 0; i < instanceCount; i++) {
        if (i % 10000 == 0)cout << i << endl;
        //cout << i << endl;
        instance tempinst;
        infile >> s; //Inst
        string instName;
        infile >> instName; //instName
        infile >> s;        //libCellname
        auto it = FF_lib.find(s);
        bool type = (it==FF_lib.end());  //type=1:代表沒找到->Gate
        tempinst.Setname(s,type);
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

    cout << "net" << endl;
    vector<vector<string>>FF_same_CLK;     //找出有相同clk signal的FF
    bool CLK_ok;

    infile >> s; //NumNets
    int netCount;
    infile >> netCount;
    vector<string>FF_same_CLK_temp;
    for (int i = 0; i < netCount; i++) {
        if (i % 10000 == 0)cout << i << endl;
        //For record
        CLK_ok = 0;
        infile >> s; //Net
        string netname;
        int numPins;
        infile >> netname; //Netname
        infile >> numPins; //numPins
        Nets tempnet;
        for (int i = 0; i < numPins; i++) {
            infile >> s; //Pin
            infile >> s;
            size_t pos = s.find('/');
            string inst_name;
            string pin_name;
            if (pos != string::npos) {
                inst_name = s.substr(0, pos);
                pin_name = s.substr(pos + 1);
            }
            tempnet.add(inst_lib, s,inst_name,pin_name,pos,Input_pins,Output_pins);
            /*F = s.c_str();
            len = sscanf_s(F, "%[^/]%c%s", part1, 100, slash, 5, part2, 100);
            if (len == 3) {
                instance tempinst;
                Pins temppin;
                tempinst = inst_lib[part1];
                temppin = tempinst.GetPins(part2);
                if (part2[0] == "D")tempnet.SetDnet(s, temppin);
                else if (part2 == "Q")tempnet.SetQnet(s, temppin);
                else tempnet.SetCLKnet(s, temppin);
                if (!strcmp(part2, "CLK")) {
                    CLK_ok = 1;
                }
            }
            else {
                auto it = Input_pins.find(s);
                if (it != Input_pins.end()) {
                    tempnet.Setnet(s, Input_pins[s]);
                }
                else {
                    tempnet.Setnet(s, Output_pins[s]);
                }
            }*/
            if (pin_name=="CLK" && s != "CLK") {
                CLK_ok = 1;
                FF_same_CLK_temp.push_back(inst_name);
            }
        }
        /*if (tempnet.GetCLkSize() != 0) {
            auto it = tempnet.CLKnetlist.begin();
            while (it != tempnet.CLKnetlist.end()) {
                FF_same_CLK_temp.push_back(inst_lib[it->first]);
            }
        }*/
        Net_Lib.insert(pair<string, Nets>(netname, tempnet));
        if (CLK_ok) {
            FF_same_CLK.push_back(FF_same_CLK_temp);
            FF_same_CLK_temp.clear();
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
    //初始定義
    placement_check = new bool* [placementRow[0].NumofSite];
    for (int i = 0; i < placementRow[0].NumofSite; i++) {
        placement_check[i] = new bool[placementRow.size()];
    }
    //初始化
    for (int i = 0; i < placementRow[0].NumofSite; i++) {
        for (int j = 0; j < placementRow.size(); j++) {
            placement_check[i][j] = 0;
        }
    }
    cout << "placement_check" << endl;
    for (auto& it : inst_lib) {
        int temp_x = it.second.GetX();
        int temp_y = it.second.GetY();
        temp_x = (temp_x - placementRow[0].startX) / placementRow[0].siteWidth;
        temp_y=(temp_y-placementRow[0].startY)/ placementRow[0].siteHeight;
        placement_check[temp_x][temp_y] = 1;
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
        infile >> s;
        infile >> num;
        FF_lib[s].SetPower(num);
        infile >> s;
    }

    cout << FF_same_CLK.size()<<endl;
    for (int i = 0; i < FF_same_CLK.size(); i++) {
        cout << FF_same_CLK[i].size()<<endl;
    }

    



    infile.close();
    outfile.close();
    return 0;
}
