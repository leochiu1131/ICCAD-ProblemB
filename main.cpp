#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <sstream>
#include <iomanip>
#include "pin.h"
#include "ff.h"
#include "gate.h"
#include "net.h"
#include "instance.h"
#include "estimate.cpp"
#include<cfloat>
using namespace std;
struct X_And_Y {
    double x;
    double y;
    double Out_X () const { return x; }
    double Out_Y () const { return y; }
};
struct placement {
    double startX;
    double startY;
    double siteWidth;
    double siteHeight;
    int NumofSite;
};
bool position_check(FF after, double**& placement_check, vector<placement>& placementRow,
    int after_x, int after_y, int W, int H) {
    int startX = placementRow[0].startX, startY = placementRow[0].startY;
    int after_x_right = (after_x + after.getwidth()) / W, after_y_right = (after_y + after.getheight()) / H;
    if (after_x_right > placementRow[0].NumofSite - 1)return false;
    else if (after_y_right > placementRow.size() - 1)return false;

    for (int i = after_x; i <= after_x_right; i++) {
        for (int j = after_y; j <= after_y_right; j++) {
            if (placement_check[i][j] != 0) {
                return false;
            }
        }
    }
    return true;
}
bool BFS(FF after, instance& ok_instance, double**& placement_check, vector<placement>& placementRow, int average_x, int average_y, int W, int H) {
    if (position_check(after, placement_check, placementRow, average_x, average_y, W, H)) {
        ok_instance.SetX(average_x * placementRow[0].siteWidth + placementRow[0].startX);
        ok_instance.SetY(average_y * placementRow[0].siteHeight + placementRow[0].startY);
        return true;
    }
    int x_max = placementRow[0].NumofSite;
    int y_max = placementRow.size();
    queue<int>x_q; x_q.push(average_x);
    queue<int>y_q; y_q.push(average_y);
    bool** ok = new bool* [x_max];
    for (int i = 0; i < x_max; i++) {
        ok[i] = new bool[y_max];
    }
    for (int i = 0; i < x_max; i++) {
        for (int j = 0; j < y_max; j++) {
            ok[i][j] = 1;
        }
    }
    ok[average_x][average_y] = 0;
    //             下 左 上 右
    int go_x[4] = { 0,-1,0,1 };
    int go_y[4] = { 1,0,-1,0 };
    bool find = 0;
    while (!x_q.empty() && !find) {
        int now_x = x_q.front(); x_q.pop();
        int now_y = y_q.front(); y_q.pop();
        //根據下右上左的順序check
        for (int i = 0; i < 4; i++) {
            int after_x = now_x + go_x[i];
            int after_y = now_y + go_y[i];
            //找到終點了，跳出迴圈
            if (position_check(after, placement_check, placementRow, after_x, after_y, W, H)) {
                ok_instance.SetX(after_x * placementRow[0].siteWidth + placementRow[0].startX);
                ok_instance.SetY(after_y * placementRow[0].siteHeight + placementRow[0].startY);
                find = 1;
                return true;
            }
            else if (after_x >= 0 && after_x < x_max && after_y >= 0 && after_y < y_max) {
                if (ok[after_x][after_y] == 1) {
                    x_q.push(after_x); y_q.push(after_y);
                    ok[after_x][after_y] = 0;
                }
            }

        }
    }
    return false;
}
X_And_Y find_the_position(map<string, pinpair>clique_member, map<string, FF>& FF_lib, double**& placement_check,
    vector<placement>& placementRow, unordered_map<string, instance>& inst_lib, string name //name代表需要改成的Flip Flop library name
) {
    vector<string>old_ff;
    vector<instance>Old_FF;
    for (auto it = clique_member.begin(); it != clique_member.end(); it++) {
        string s = it->first;
        size_t pos = s.find('/');
        string inst_name = s.substr(0, pos);
        if (old_ff.size() == 0) {
            Old_FF.push_back(inst_lib[inst_name]);
        }
        else {
            bool ok = 1;
            for (int i = 0; i < old_ff.size(); i++) {
                if (Old_FF[i].Getlibname() == inst_name) {
                    ok = 0;
                }
            }
            if (ok)Old_FF.push_back(inst_lib[inst_name]);
        }
    }
    int average_x = 0, average_y = 0;
    int startX = placementRow[0].startX, startY = placementRow[0].startY;
    //W:寬的間隔長度 H:高的間隔長度
    int W = placementRow[0].siteWidth, H = placementRow[0].siteHeight;
    //after:修改後的FF種類
    FF after = FF_lib[name];

    //先解除原本FF的位置限制
    for (int i = 0; i < Old_FF.size(); i++) {
        int temp_x = (Old_FF[i].GetX() - startX) / W, temp_y = (Old_FF[i].GetY() - startY) / H;
        placement_check[temp_x][temp_y] = 0;
        int temp_x_right = Old_FF[i].getff().getwidth() / W;
        int temp_y_right = Old_FF[i].getff().getheight() / H;
        for (int i = temp_x; i <= temp_x + temp_x_right; i++) {
            for (int j = temp_y; j <= temp_y + temp_y_right; j++) {
                placement_check[i][j] = 0;
            }
        }
        average_x += Old_FF[i].GetX();
        average_y += Old_FF[i].GetY();
    }

    //area_x:x的平均位置，area_y:y的平均位置
    double area_x = static_cast<float>(average_x) / Old_FF.size();
    double area_y = static_cast<float>(average_y) / Old_FF.size();

    average_x /= Old_FF.size();
    average_y /= Old_FF.size();
    //average_x:代表所占用的row的寬，average_y:代表所占用的row的高
    average_x = (average_x - startX) / W;
    average_y = (average_y - startY) / H;
    bool ok = 0;
    instance temp;
    temp.SetFF(after);

    if (BFS(after, temp, placement_check, placementRow, average_x, average_y, W, H)) {
        int temp_x = temp.GetX(); temp_x = (temp_x - startX) / W;
        int temp_y = temp.GetY(); temp_y = (temp_y - startY) / H;
        int temp_x_right = after.getwidth() / W;
        int temp_y_right = after.getheight() / H;
        for (int i = temp_x; i <= temp_x + temp_x_right; i++) {
            for (int j = temp_y; j <= temp_y + temp_y_right; j++) {
                double l;
                if (i == temp_x + temp_x_right)l = after.getwidth() - temp_x_right * placementRow[0].siteWidth;
                else l = placementRow[0].siteWidth;
                double r;
                if (j == temp_y + temp_y_right)r = after.getheight() - temp_y_right * placementRow[0].siteHeight;
                else r = placementRow[0].siteHeight;
                placement_check[i][j] = l * r;
            }
        }
        X_And_Y re;
        re.x = temp.GetX(); re.y = temp.GetY();
        return re;
    }
    else {
        X_And_Y re; re.x=0; re.y=0;
        return re;
    }
}
double compute_area(double**& placement_check, int x, int y) {
    double area = 0.0;
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            area += placement_check[i][j];
        }
    }
    return area;
}
int clique_test(clique& nowclique, map<string, pinpair>& totest_Pinpair, map<string, FF>& FF_lib2, double a, double b, double c, double displacemaentdelay)
{

    bool success = 0;
    //cout << "xx" << endl;
    double x = nowclique.centerx * nowclique.clique_member.size();
    double y = nowclique.centery * nowclique.clique_member.size();
    for (auto it = totest_Pinpair.begin(); it != totest_Pinpair.end(); it++)
    {


        x += it->second.todpin.getx();
        y += it->second.toqpin.gety();
    }
    x /= (nowclique.clique_member.size() + totest_Pinpair.size());
    y /= (nowclique.clique_member.size() + totest_Pinpair.size());
    double maxgain = 0;
    FF mergeff;
    string mergeffname;
    Pins center;
    center.SetX(x);
    center.SetY(y);
    // cout<<nowclique.ffname<<"FFNAME"<<endl;
    // cout<<nowclique.clique_member.size()+totest_Pinpair.size();
     //cout<<FF_lib2.size()<<"ss";
     //cout<<nowclique.flipflop.getarea()<<endl;
    double prevcost = nowclique.flipflop.Getcostperbit(b, c) * (nowclique.clique_member.size() + totest_Pinpair.size());
    //cout<<"prevcost="<<prevcost<<endl;
    for (auto fit = FF_lib2.begin(); fit != FF_lib2.end(); fit++)
    {
        double nowffcost = fit->second.Getcost(b, c);
        //cout<<"nowcost="<<nowffcost<<endl;
        double totalslackcost = 0;
        // cout<<fit->first<<endl;
        map<string, int> corespond = set_corespond_pin(nowclique, totest_Pinpair, fit->second);

        for (auto it = corespond.begin(); it != corespond.end(); it++)
        {
            double displacement = 0;
            // cout<<it->first<<"ss"<<endl;
            Pins d = fit->second.qdpinpair.at(it->second).first;
            d.addX(center.getx() - (fit->second.getwidth() / 2));
            d.addY(center.gety() - (fit->second.getheight() / 2));

            auto topair = nowclique.clique_member.find(it->first);
            if (topair != nowclique.clique_member.end())
            {

                double difference = (distance(topair->second.todpin, d) - topair->second.GetDdistance());
                if (difference > 0)
                {
                    displacement += difference;
                }
                //   cout<<"tt";
            }
            else
            {
                topair = totest_Pinpair.find(it->first);
                // cout<<"ss";
                if (topair == totest_Pinpair.end())
                {
                    //cout << it->first << "error";
                }
                double difference = (distance(topair->second.todpin, d) - topair->second.GetDdistance());
                if (difference > 0)
                {
                    displacement += difference;
                }

            }

            //cout<<displacement<<endl;
            double slack = topair->second.Getslack() - displacemaentdelay * displacement - fit->second.Getdelay() + topair->second.getdelay();
            if (slack < 0)
            {
                totalslackcost -= a * slack;
            }



        }
        if (totalslackcost > prevcost)
        {
            success = -1;
        }
        double nowgain = prevcost - fit->second.Getcost(b, c) - totalslackcost;
        if (nowgain > maxgain)
        {
            maxgain = nowgain;
            mergeffname = fit->first;
            mergeff = fit->second;
            success = 1;
        }



        // cout<<fit->first<<endl;

    }
    if (success == 1)
    {
        nowclique.merge_clique(totest_Pinpair, mergeff);
        nowclique.ffname = (mergeffname);
        // cout<<mergeffname<<"mergesucces"<<endl;
    }


    return  success;



}

int main(int argc,char* argv[]) {
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
    double** placement_check;
    double DisplacementDelay;
    map<string, double>TimingSlack;

    ifstream infile;
    ofstream outfile;

    string s;
    double num;

    // infile.open("testcase1_0614.txt");
    // outfile.open("output0614.txt");
    infile.open(argv[1]);
    outfile.open(argv[2]);
    //sample.txt
    //testcase1.txt
    //testcase1_0614.txt

    if (infile.is_open())
    {

    }
    else
    {
        cout << "notopen";
        return 0;
    }
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
            tempFF.setqdpinpair();
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
    int maxff = FF_lib2.rbegin()->first;

    //s:NumInstance

    int instanceCount;

    infile >> instanceCount; //instanceCount
    // cout<<instanceCount<<endl;
    for (int i = 0; i < instanceCount; i++) {
        instance tempinst;
        infile >> s; //Inst
        string instName;
        infile >> instName; //instName
        infile >> s;        //libCellname
        auto it = FF_lib.find(s);
        bool type = (it == FF_lib.end());  //type=1:代表沒找到->Gate
        tempinst.Setname(s, type);
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


    vector<vector<string>>FF_same_CLK;     //找出有相同clk signal的FF
    bool CLK_ok;

    infile >> s; //NumNets
    int netCount;
    infile >> netCount;
    vector<string>FF_same_CLK_temp;
    for (int i = 0; i < netCount; i++) {
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
            tempnet.add(inst_lib, s, inst_name, pin_name, pos, Input_pins, Output_pins);

            tempnet.settopin(inst_lib);
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
            if (pin_name == "CLK" && s != "CLK") {
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
    //cout<<s;
    infile >> BinWidth;
    infile >> s; //BinHeight
    infile >> BinHeight;
    infile >> s; //BinMaxUtil
    infile >> BinMaxUtil;
    // cout<<s;
    infile >> s; //PlacementRows;
    // cout<<s;
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
    placement_check = new double* [placementRow[0].NumofSite];
    for (int i = 0; i < placementRow[0].NumofSite; i++) {
        placement_check[i] = new double[placementRow.size()];
    }
    //初始化
    for (int i = 0; i < placementRow[0].NumofSite; i++) {
        for (int j = 0; j < placementRow.size(); j++) {
            placement_check[i][j] = 0.0;
        }
    }

  
    for (auto& it : inst_lib) {
        int temp_x = it.second.GetX();
        int temp_y = it.second.GetY();
        temp_x = (temp_x - placementRow[0].startX) / placementRow[0].siteWidth;
        temp_y = (temp_y - placementRow[0].startY) / placementRow[0].siteHeight;
        int area = 0;
        int temp_x_right, temp_y_right;
        if (!it.second.Gettype()) {
            FF tempFF = it.second.getff();
            temp_x_right = tempFF.getwidth() / placementRow[0].siteWidth;
            temp_y_right = tempFF.getheight() / placementRow[0].siteHeight;
            for (int i = temp_x; i <= temp_x + temp_x_right; i++) {
                for (int j = temp_y; j <= temp_y + temp_y_right; j++) {
                    double l;
                    if (i == temp_x + temp_x_right)l = tempFF.getwidth() - (temp_x_right)*placementRow[0].siteWidth;
                    else l = placementRow[0].siteWidth;
                    double r;
                    if (j == temp_y + temp_y_right)r = tempFF.getheight() - (temp_y_right)*placementRow[0].siteHeight;
                    else r = placementRow[0].siteHeight;
                    placement_check[i][j] = l * r;
                }
            }
        }
        else {
            Gate tempGate = it.second.getgate();
            temp_x_right = tempGate.getwidth() / placementRow[0].siteWidth;
            temp_y_right = tempGate.getheight() / placementRow[0].siteHeight;
            area = tempGate.getheight() * tempGate.getwidth();
            for (int i = temp_x; i <= temp_x + temp_x_right; i++) {
                for (int j = temp_y; j <= temp_y + temp_y_right; j++) {
                    double l;
                    if (i == temp_x + temp_x_right)l = tempGate.getwidth() - (temp_x_right)*placementRow[0].siteWidth;
                    else l = placementRow[0].siteWidth;
                    double r;
                    if (i == temp_y + temp_y_right)r = tempGate.getheight() - (temp_y_right)*placementRow[0].siteHeight;
                    else r = placementRow[0].siteHeight;
                    placement_check[i][j] = l * r;
                }
            }
        }
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
        inst_lib[s].SetSlack(num);
        infile >> s;
    }

    //"Now" s is GatePower
    while (s == "GatePower") {
        infile >> s;
        infile >> num;
        FF_lib[s].SetPower(num);
        infile >> s;
    }
    vector<string>new_inst;
    vector<vector<string>>new_map_list;

    int inst_num = inst_lib.size();
    for (int i = 0; i < FF_same_CLK.size(); i++)
    {
        //cout << "ffclk" << i << " " << FF_same_CLK[i].size() << endl;
        map<string, pinpair> topin;

        for (int j = 0; j < FF_same_CLK[i].size(); j++)
        {

            string nowinstname = FF_same_CLK.at(i).at(j);
            instance nowinst = inst_lib[nowinstname];
            //cout<<nowinstname<<endl;
            FF nowff = FF_lib[nowinst.Getlibname()];
            if (nowff.getbit() > 1)
            {
                for (int k = 0; k < nowff.getbit(); k++)
                {

                    string pinindex = to_string(k);
                    Pins temptoD = nowinst.todpin["D" + pinindex];
                    Pins temptoQ = nowinst.toqpin["Q" + pinindex];
                    pinpair temppinpair(temptoD, temptoQ, nowinst, pinindex);
                    //cout<<temptoQ.getx()<<"toq,"<<temptoQ.gety()<<endl;
                    //cout<<temptoD.getx()<<"tod,"<<temptoD.gety()<<endl;
                    topin.insert(pair<string, pinpair>(nowinstname + "/" + to_string(k), temppinpair));
                }
            }
            else
            {

                Pins temptoD = nowinst.todpin["D"];
                Pins temptoQ = nowinst.toqpin["Q"];
                string pinindex = to_string(0);
                pinpair temppinpair(temptoD, temptoQ, nowinst, pinindex);
                //cout<<temptoQ.getx()<<"toq,"<<temptoQ.gety()<<endl;
               // cout<<temptoD.getx()<<"tod,"<<temptoD.gety()<<endl;
                topin.insert(pair<string, pinpair>(nowinstname + "/", temppinpair));
            }







        }
        /*
        for(auto it=topin.begin();it!=topin.end();it++)
        {
            cout<<it->first<<"xx"<<endl;
        }*/
        vector<clique> newff;
        vector<map<string, int>> cores_pin;
        map<string, pinpair>to_test;

        while (topin.size())
        {
            size_t pos = topin.begin()->first.find('/');
            string inst_name;
            string pin_name;
            if (pos != string::npos) {
                inst_name = topin.begin()->first.substr(0, pos);
                pin_name = topin.begin()->first.substr(pos + 1);
            }
            FF tempff = inst_lib[inst_name].getff();
            map<string, pinpair> temppair;
            temppair.insert(pair<string, pinpair>(topin.begin()->first, topin.begin()->second));
            clique tempclique(temppair, tempff);
            tempclique.ffname = inst_lib[inst_name].Getlibname();
            // cout<<tempclique.ffname<<"tt";
            double pincount = 1;

            int success;

            do
            {
                string nearst = find_nearst_pinpair_outof_clique(topin, tempclique, to_test);

                if (topin.find(nearst) != topin.end())
                {
                    pinpair nearst_pinpair = pinpair(topin[nearst]);
                    string nearst_neearst = find_nearst_pinpair(topin, nearst_pinpair.todpin, nearst_pinpair.toqpin);
                    auto inclique = tempclique.clique_member.find(nearst_neearst);
                    //cout << nearst << nearst_neearst << endl;
                    pincount = tempclique.clique_member.size() + to_test.size();
                    if (pincount > maxff)
                    {
                        break;
                    }
                    if (inclique != tempclique.clique_member.end())
                    {
                        to_test.insert(pair<string, pinpair>(nearst, nearst_pinpair));
                        // pincount++;
                    }
                    else
                    {
                        if (pincount + 2 <= maxff)
                        {
                            to_test.insert(pair<string, pinpair>(nearst, nearst_pinpair));
                            to_test.insert(pair<string, pinpair>(nearst_neearst, topin[nearst_neearst]));

                        }
                        else
                        {
                            to_test.insert(pair<string, pinpair>(nearst, nearst_pinpair));



                        }

                    }

                    for (auto it = tempclique.clique_member.begin(); it != tempclique.clique_member.end(); it++)
                    {
                        //cout << it->first << "inclique" << endl;
                    }
                    for (auto it = to_test.begin(); it != to_test.end(); it++)
                    {
                        //cout << it->first << "to_test" << endl;
                    }
                    tempclique.setdq();
                    pincount = tempclique.clique_member.size() + to_test.size();
                    if (pincount > maxff)
                    {
                        break;
                    }
                    //cout << pincount << endl;
                    success = clique_test(tempclique, to_test, FF_lib2[pincount], Alpha, Beta, Gamma, DisplacementDelay);
                    //cout << "soccess=" << success << endl;
                    if (success == 1)
                    {
                        to_test.clear();
                    }
                }
                else
                {
                    //cout << nearst << "nearstnotfound" << endl;
                    break;
                }


            } while (success != -1 && pincount <= maxff);
            to_test.clear();

            newff.push_back(tempclique);
            map<string, int> temp = set_corespond_pin(tempclique, tempclique.flipflop);
            cores_pin.push_back(temp);


            for (auto it = tempclique.clique_member.begin(); it != tempclique.clique_member.end(); it++)
            {
                auto included = topin.find(it->first);
                if (included != topin.end())
                {

                    topin.erase(included);
                }
            }

        }
        auto cit = cores_pin.begin();
        // cout<<"maxff="<<maxff<<endl;
        //cout << "newff---------------------" << endl;
        
        for (auto it = newff.begin(); it != newff.end(); it++)
        {
            stringstream ss;
            string temp_inst;
            X_And_Y temp_XY;
            temp_XY=find_the_position(it->Getmember(), FF_lib, placement_check, placementRow, inst_lib, it->GetName());
            ss << "Inst C" << inst_num << " " << it->ffname << " " << temp_XY.x << " " << temp_XY.y;
            new_inst.push_back(ss.str());

            vector<string>temp_map_list_one;
            stringstream ss_2;
            if ((*cit).size() > 1)
            {
                // cout<<"aa";
                set<string> oldff;
                for (auto pit = cit->begin(); pit != cit->end(); pit++)
                {

                    size_t pos = pit->first.find('/');
                    string inst_name;
                    string pin_name;
                    if (pos != string::npos) {
                        inst_name = pit->first.substr(0, pos);
                        pin_name = pit->first.substr(pos + 1);
                    }
                    oldff.insert(inst_name);
                    ss_2 << inst_name << "/" << "D" << pin_name << " map " << "C" << inst_num << "/D" << pit->second;
                    temp_map_list_one.push_back(ss_2.str());
                    ss_2.str("");

                    ss_2 << inst_name << "/" << "Q" << pin_name << " map " << "C" << inst_num << "/Q" << pit->second;
                    temp_map_list_one.push_back(ss_2.str());
                    ss_2.str("");
                    
                }
                for (auto sit = oldff.begin(); sit != oldff.end(); sit++)
                {
                    ss_2 << *sit << "/" << "CLK" << " map " << "C" << inst_num << "/CLK";
                    temp_map_list_one.push_back(ss_2.str());
                    ss_2.str("");
                }
            }
            else
            {
                for (auto pit = cit->begin(); pit != cit->end(); pit++)
                {

                    size_t pos = pit->first.find('/');
                    string inst_name;
                    string pin_name;
                    if (pos != string::npos) {
                        inst_name = pit->first.substr(0, pos);
                        pin_name = pit->first.substr(pos + 1);
                    }
                    ss_2 << inst_name << "/" << "D" << pin_name << " map " << "C" << inst_num << "/D";
                    temp_map_list_one.push_back(ss_2.str());
                    ss_2.str(""); //ss_2.clear();

                    ss_2 << inst_name << "/" << "Q" << pin_name << " map " << "C" << inst_num << "/Q";
                    temp_map_list_one.push_back(ss_2.str());
                    ss_2.str("");//ss_2.clear();

                    ss_2 << inst_name << "/" << "CLK" << " map " << "C" << inst_num << "/CLK";
                    temp_map_list_one.push_back(ss_2.str());
                    ss_2.str(""); //ss_2.clear();
                }
            }
            new_map_list.push_back(temp_map_list_one);
            cit++;
            inst_num++;
            //cout << endl;

        }
    }
    outfile << "CellInst " << new_inst.size() << endl;
    for (int i = 0; i < new_inst.size(); i++) {
        outfile << new_inst[i] << endl;
    }
    for (int i = 0; i < new_map_list.size(); i++) {
        for (int j = 0; j < new_map_list[i].size(); j++) {
            outfile << new_map_list[i][j]<<endl;

        }
    }




    infile.close();
    outfile.close();
    return 0;
}