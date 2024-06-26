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
#include "estimate.cpp"
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
    vector<vector<instance>>New_FF,Old_FF;
    
}
   
int clique_test(clique& nowclique,map<string,pinpair>& totest_Pinpair, map<string, FF>&FF_lib2,double a,double b,double c,double displacemaentdelay)
{

        bool success=0;
        cout<<"xx"<<endl;
        double x=nowclique.centerx*nowclique.clique_member.size();
        double y=nowclique.centery*nowclique.clique_member.size();
        for(auto it=totest_Pinpair.begin();it!=totest_Pinpair.end();it++)
        {
            
            
            x+=it->second.todpin.getx();
            y+=it->second.toqpin.gety();   
        }
        x/=(nowclique.clique_member.size()+totest_Pinpair.size());
        y/=(nowclique.clique_member.size()+totest_Pinpair.size());
        double maxgain=0;
        FF mergeff;
        string mergeffname;
        Pins center;
        center.SetX(x);
        center.SetY(y);
        //cout<<nowclique.ffname<<"FFNAME"<<endl;
        cout<<FF_lib2.size();
        //cout<<nowclique.flipflop.getarea()<<endl;
        double prevcost=nowclique.flipflop.Getcostperbit(b,c)*(nowclique.clique_member.size()+totest_Pinpair.size());
        //cout<<"prevcost="<<prevcost<<endl;
        for(auto fit=FF_lib2.begin();fit!=FF_lib2.end();fit++)
        {
            double nowffcost=fit->second.Getcost(b,c);
            cout<<"nowcost="<<nowffcost<<endl;
            double totalslackcost=0;
            map<string,int> corespond=set_corespond_pin(nowclique,totest_Pinpair,fit->second);
            cout<<"tt";
            for(auto it=corespond.begin();it!=corespond.end();it++)
            {
                double displacement=0;
                //cout<<it->first<<endl;
                Pins d=fit->second.qdpinpair.at(it->second).first;
                d.addX(center.getx()-(fit->second.getwidth()/2));
                d.addY( center.gety()-(fit->second.getheight()/2));
    
                auto topair=nowclique.clique_member.find(it->first);
                if(topair!=nowclique.clique_member.end())
                {
                   
                  
                    double difference=(distance(topair->second.todpin,d)-topair->second.GetDdistance());
                    if(difference>0)
                    {
                    displacement+=difference;
                    }   
                }
                else
                {
                    auto topair=totest_Pinpair.find(it->first);
                     double difference=(distance(topair->second.todpin,d)-topair->second.GetDdistance());
                    if(difference>0)
                    {
                    displacement+=difference;
                    }   
                    
                }
                
        
                double slack =topair->second.Getslack()-displacemaentdelay*displacement-fit->second.Getdelay()+topair->second.getdelay();
                if(slack<0)
                {
                    totalslackcost-=a*slack;
                }



            }
            if(totalslackcost>prevcost)
                {
                    success=-1;
                }
             double nowgain= prevcost-fit->second.Getcost(b,c)-totalslackcost;
                if(nowgain>maxgain)
                {
                    maxgain=nowgain;
                    mergeffname=fit->first;
                    mergeff=fit->second;
                    success=1;
                }
       
             
            
               // cout<<fit->first<<endl;

        }
        if(success==1)
        {
            nowclique.merge_clique(totest_Pinpair,mergeff);
            nowclique.ffname=(mergeffname);
           // cout<<mergeffname<<"mergesucces"<<endl;
        }
        
       
        return  success;



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
    cout<<"start"<<endl;
    infile.open("C:\\Users\\Yeh\\Desktop\\class\\eda\\Fp\\ICCAD-ProblemB\\testcase1_0614.txt");
    //sample.txt
    //testcase1.txt
    //testcase1_0614.txt
    if(infile.is_open())
    {

    }
    else
    {
        cout<<"notopen";
        return 3;
    }
    infile >> s; //Alpha
    cout<<s;
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
    cout<<s;
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
    cout<<endl<<s<<endl;
    while (s == "FlipFlop" || s == "Gate") {
        if (s == "FlipFlop") {
            FF tempFF;
            infile >> num;   //bits數量
            tempFF.Setbits(num);

            string ffname;
            infile >> ffname;//flipflop name
            cout<<ffname<<endl;

            infile >> num;   //flipflop Width
            tempFF.SetWidth(num);
            cout<<"width="<<num<<endl;

            infile >> num;   //flipflop Height
            tempFF.SetHeight(num);
            cout<<"height="<<num<<endl;
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
    int maxff=FF_lib2.rbegin()->first;
    cout<<"maxff="<<maxff;
    cout << "inst" << endl;
    
    //s:NumInstance
    
    int instanceCount;
   
    infile >> instanceCount; //instanceCount
    // cout<<instanceCount<<endl;
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
   
    cout << "nettd" << endl;
    vector<vector<string>>FF_same_CLK;     //找出有相同clk signal的FF
    bool CLK_ok;

    infile >> s; //NumNets
    int netCount;
    infile >> netCount;
    cout<<netCount<<"ne"<<endl;
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
        //placement_check[temp_x][temp_y] = 1;SEGMENTAION FAULT
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

    cout << FF_same_CLK.size()<<endl;
     int inst_num=inst_lib.size();
    for (int i = 0; i < FF_same_CLK.size(); i++)
    {
        cout <<"ffclk" <<i<<" " <<FF_same_CLK[i].size()<<endl;
        map<string,pinpair> topin;
         
        for(int j=0;j<FF_same_CLK[i].size();j++)
        {
            
            string nowinstname=FF_same_CLK.at(i).at(j);
            instance nowinst=inst_lib[nowinstname];
            cout<<nowinstname<<endl;
            FF nowff=FF_lib[nowinst.Getlibname()];
            if(nowff.getbit()>1)
            {
                for(int k=0;k<nowff.getbit();k++)
                {   
                
                    string pinindex= to_string(k);
                    Pins temptoD =nowinst.todpin["D"+pinindex];
                    Pins temptoQ =nowinst.toqpin["Q"+pinindex];
                    pinpair temppinpair(temptoD,temptoQ,nowinst,pinindex);
                    cout<<temptoQ.getx()<<"toq,"<<temptoQ.gety()<<endl;
                    cout<<temptoD.getx()<<"tod,"<<temptoD.gety()<<endl;
                    topin.insert(pair<string,pinpair>(nowinstname+"/"+to_string(k),temppinpair));
                }
            }
            else
            {
              
                    Pins temptoD =nowinst.todpin["D"];
                    Pins temptoQ =nowinst.toqpin["Q"];
                    string pinindex= to_string(0);
                    pinpair temppinpair(temptoD,temptoQ,nowinst,pinindex);
                    cout<<temptoQ.getx()<<"toq,"<<temptoQ.gety()<<endl;
                    cout<<temptoD.getx()<<"tod,"<<temptoD.gety()<<endl;
                    topin.insert(pair<string,pinpair>(nowinstname+"/"+"0",temppinpair));
            }
                
               
            
             
        
        
            
        }
        /*
        for(auto it=topin.begin();it!=topin.end();it++)
        {
            cout<<it->first<<"xx"<<endl;
        }*/
        vector<clique> newff;
        vector<map<string,int>> cores_pin;
        map<string,pinpair>to_test;
       
        while(topin.size())
        {   
            size_t pos = topin.begin()->first.find('/');
            string inst_name;
            string pin_name;
            if (pos != string::npos) {
                inst_name = topin.begin()->first.substr(0, pos);
                pin_name = topin.begin()->first.substr(pos + 1);
            }
            FF tempff=inst_lib[inst_name].getff();
            map<string,pinpair> temppair;
            temppair.insert(pair<string,pinpair>(topin.begin()->first,topin.begin()->second));
            clique tempclique(temppair,tempff);
            tempclique.ffname=inst_lib[inst_name].Getlibname();
           // cout<<tempclique.ffname<<"tt";
            double pincount=1;
           
            int success;
            
            do
            {
                string nearst=find_nearst_pinpair_outof_clique(topin,tempclique);
                
                if(topin.find(nearst)!=topin.end())
                {
                    pinpair nearst_pinpair=pinpair(topin[nearst]);
                    string nearst_neearst=find_nearst_pinpair(topin,nearst_pinpair.todpin,nearst_pinpair.toqpin);
                    auto inclique=tempclique.clique_member.find(nearst_neearst);
                    cout<<nearst<<nearst_neearst<<endl;
                    if(inclique!=tempclique.clique_member.end())
                    {
                      to_test.insert(pair<string,pinpair>(nearst,nearst_pinpair));
                      pincount++;

                        
                    }
                    else
                    {
                        if(pincount+2<=maxff)
                        {
                            to_test.insert(pair<string,pinpair>(nearst,nearst_pinpair));
                            to_test.insert(pair<string,pinpair>(nearst_neearst,topin[nearst_neearst]));
                            pincount+=2;
                        }
                        else
                        {
                            to_test.insert(pair<string,pinpair>(nearst,nearst_pinpair));
                            pincount++;
                        }
                        
                    }
                    
                    for(auto it=tempclique.clique_member.begin();it!=tempclique.clique_member.end();it++)
                    {
                        cout<<it->first<<"inclique"<<endl;
                    }
                    for(auto it=to_test.begin();it!=to_test.end();it++)
                    {
                        cout<<it->first<<"to_test"<<endl;
                    }
                    tempclique.setdq();
                    
                    cout<<pincount<<endl;
                    success=clique_test(tempclique,to_test,FF_lib2[pincount],Alpha,Beta,Gamma,DisplacementDelay);
                   // cout<<"soccess="<<success<<endl;
                }
                else
                {
                    cout<<nearst<<"nearstnotfound"<<endl;
                    break;
                }
                

            } while (success!=-1&&pincount<maxff);
            to_test.clear();
        
            newff.push_back(tempclique);
            map<string,int> temp=set_corespond_pin(tempclique,tempclique.flipflop);
            cores_pin.push_back(temp);

            
            for(auto it=tempclique.clique_member.begin();it!=tempclique.clique_member.end();it++)
            {
                auto included=topin.find(it->first);
                if(included!=topin.end())
                {
                    
                    topin.erase(included);
                }
            }
          
        }
        auto cit=cores_pin.begin();
       // cout<<"maxff="<<maxff<<endl;
        cout<<"newff---------------------" <<endl; 
        for(auto it=newff.begin();it!=newff.end();it++)
        {
            cout<<"inst C"<<inst_num<<" "<<it->ffname<<endl;
            for(auto pit=cit->begin();pit!=cit->end();pit++)
            {
              
              cout<<pit->first<<"map"<<"C"<<inst_num<<"/D"<<pit->second<<endl;
              cout<<pit->first<<"map"<<"C"<<inst_num<<"/Q"<<pit->second<<endl;
            }
            cit++;
            inst_num++;
            cout<<endl;

        }
            
         
            //cout<<"to"<<it->first<<" "<<it->second.todpin.getx()<<","<<it->second.todpin.gety()<<"-"<<it->second.toqpin.getx()<<","<<it->second.toqpin.gety()<<endl;
            //COMBINATIAL GATE 顯示不出待查
        
       
 

    }

   



    infile.close();
    outfile.close();
    return 0;
}
