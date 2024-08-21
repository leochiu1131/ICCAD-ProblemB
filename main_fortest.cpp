#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <sstream>
#include <iomanip>
#include <set>
#include<cfloat>
#include "pin.h"
#include "ff.h"
#include "gate.h"
#include "net.h"
#include "instance.h"
#include "clique.h"
using namespace std;
double totalestslack=0;
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




 string find_nearst_pinpair_outof_clique(ofstream& log,map<string,pinpair>& topin,clique&nowclique,map<string,pinpair>& to_test)
    {
        double mindistance=DBL_MAX;
        string nearstpair;
        Pins d;
        Pins q;
        d.SetX(nowclique.aver_d_x);
        d.SetY(nowclique.aver_d_y);
        q.SetX(nowclique.aver_q_x);
        q.SetY(nowclique.aver_q_y);

        for(auto it=topin.begin();it!=topin.end();it++)
        {
            auto vit=nowclique.clique_member.find(it->first);
            auto tit=to_test.find(it->first);
            if(vit==nowclique.clique_member.end()&&tit==to_test.end())
            {
                double nowdistance;
                if(it->second.gethaveq())
                {
                    nowdistance=distance(it->second.todpin,d)+distance(it->second.toqpin,q);
                }
                else
                {
                    nowdistance=distance(it->second.todpin,d)*2;
                }
               
                if(nowdistance<mindistance)
                {
                    mindistance=nowdistance;
                    nearstpair=it->first;
                    log<<"outclique"<<it->first<<nowdistance<<endl;
                }
            }
            
        }
        return nearstpair;

    }
    
    
    /*
    string find_nearst_pinpair(map<string,pinpair>& topin,clique&nowclique)
    {
        double mindistance=DBL_MAX;
        string nearstpair;
        Pins d;
        Pins q;
        d.SetX(nowclique.aver_d_x);
        d.SetY(nowclique.aver_d_y);
        q.SetX(nowclique.aver_q_x);
        q.SetY(nowclique.aver_q_y);
        for(auto it=topin.begin();it!=topin.end();it++)
        {
        
            {
                double nowdistance=distance(it->second.todpin,d)+distance(it->second.toqpin,q);
                if(nowdistance<mindistance)
                {
                    mindistance=nowdistance;
                    nearstpair=it->first;
                }
            }

        }
        return nearstpair;

    }
    */
string find_nearst_pinpair(ofstream& log,map<string,pinpair>& topin,Pins& d,Pins&q,string& self)
    {
        double mindistance=DBL_MAX;
        string nearstpair=" ";
        //log<<q.getx()<<"Q"<<q.gety()<<endl;
        for(auto it=topin.begin();it!=topin.end();it++)
        {

                if(it->first==self)
                {
                    continue;
                }
                double nowdistance;
                log<<it->first<<"--"<<it->second.gethaveq();
                
                if(it->second.gethaveq())
                {
                    nowdistance=distance(it->second.todpin,d)+distance(it->second.toqpin,q);
                    
                   /// log<<it->second.toqpin.getx()<<"TOQ"<<it->second.toqpin.gety()<<endl;
                  ///  log<<"tod"<<distance(it->second.todpin,d)<<"toq"<<distance(it->second.toqpin,q)<<endl;
                   
                }
                else
                {
                    nowdistance=distance(it->second.todpin,d)*2;
                  //  log<<"tod"<<distance(it->second.todpin,d)<<endl;
                } 
               // log<<"tod"<<distance(it->second.todpin,d)<<"toq"<<distance(it->second.toqpin,q)<<endl;
                //log<<it->first<<nowdistance<<endl;
                
                if(nowdistance<mindistance&&nowdistance!=0)
                {
                    mindistance=nowdistance;
                    nearstpair=it->first;
                }
            

        }
        return nearstpair;

}
string find_nearst_pinpair(ofstream& log,map<string,pinpair>& topin,Pins& d,string& self)
    {
        double mindistance=DBL_MAX;
        string nearstpair="";
        
        for(auto it=topin.begin();it!=topin.end();it++)
        {

                 if(it->first==self)
                {
                    continue;
                }
                double nowdistance;
                {
                    nowdistance=distance(it->second.todpin,d)*2;
                    //log<<"tod"<<distance(it->second.todpin,d)<<endl;
                } 
               // log<<"tod"<<distance(it->second.todpin,d)<<"toq"<<distance(it->second.toqpin,q)<<endl;
                //log<<it->first<<nowdistance<<endl;
                
                if(nowdistance<mindistance&&nowdistance!=0)
                {
                    mindistance=nowdistance;
                    nearstpair=it->first;
                }
            

        }
        return nearstpair;

}
map<string,int> set_corespond_pin(clique& nowclique,map<string,pinpair>& totest_Pinpair,FF& flipflop)
{
    double h=flipflop.getheight();
    double w=flipflop.getwidth();
    double up=nowclique.up;
    double down=nowclique.down;
    double left=nowclique.left;
    double right=nowclique.right;

    map<string,int> corespond;
   
    map<string,pinpair> tobematch=nowclique.clique_member;
    //cout<<"l"<<left<<"r"<<right<<"u"<<up<<"d"<<down<<endl;
    for(auto it=totest_Pinpair.begin();it!=totest_Pinpair.end();it++)
    {
        tobematch.insert(pair<string,pinpair>(it->first,it->second));
    }
    for(auto it=totest_Pinpair.begin();it!=totest_Pinpair.end();it++)
    {
           if(it->second.toqpin.getx()>right)
            {
                right=it->second.toqpin.getx();
            }
             if(it->second.todpin.getx()>right)
            {
                right=it->second.todpin.getx();
            }
             if(it->second.toqpin.getx()<left)
            {
                left=it->second.toqpin.getx();
            }
             if(it->second.todpin.getx()<left)
            {
                left=it->second.todpin.getx();
            }
             if(it->second.toqpin.gety()>up)
            {
                up=it->second.toqpin.gety();
            }
            if(it->second.todpin.gety()>up)
            {
                up=it->second.todpin.gety();
            }
              if(it->second.toqpin.gety()<down)
            {
                down=it->second.toqpin.gety();
            }
            if(it->second.todpin.gety()<down)
            {
                down=it->second.todpin.gety();
            }
    }
    //cout<<"l"<<left<<"r"<<right<<"u"<<up<<"d"<<down<<endl;
    //cout<<"qdsize"<<flipflop.qdpinpair.size()<<endl;
   
    for(int k=0;k<flipflop.qdpinpair.size();k++)
    {
        //cout<<k<<endl;
        Pins d=flipflop.qdpinpair[k].first;
        Pins q=flipflop.qdpinpair[k].second;
        double dx=d.getx()/flipflop.getwidth();
        double dy=d.gety()/flipflop.getheight();
        double qx=q.getx()/flipflop.getwidth();
        double qy=q.gety()/flipflop.getheight();
      //  cout<<dx<<" ,"<<dy<<qx<<" ,"<<qy<<endl;
        double min=4;
        string corepinpair;
        for(auto it=tobematch.begin();it!=tobematch.end();it++)
        {
       // cout<<"tomatch"<<it->first<<endl;  
        Pins td=it->second.todpin;  
         double tdx=(td.getx()-left)/right;
        double tdy=(td.gety()-down)/up;
         double core_num;  
        if(it->second.gethaveq())
        {
             Pins tq=it->second.toqpin;
            
       
            if(nowclique.haveqnum)
            {
                double tqx=(tq.getx()-left)/right;
                double tqy=(tq.gety()-down)/up;
                core_num=(absd(tdx-dx)+absd(tdy-dy))+absd(tqy-qy)+absd(tqx-qx);

            }
            else
            {
                core_num=(absd(tdx-dx)+absd(tdy-dy))*2;

            }   
            
        }
        else 
        {
            core_num=(absd(tdx-dx)+absd(tdy-dy))*2;

        }   
        //cout<<core_num<<endl;
        
      //  cout<<it->first<<"c "<<" "<<tdx<<" ,"<<tdy<<" "<<tqx<<" ,"<<tqy<<endl;
        
        if(core_num<min&&corespond.find(it->first)==corespond.end())
            {
                min=core_num;
                corepinpair=it->first;
            }

        
       

        
    }
       
       // cout<<corepinpair<<"cpin"<<endl;
       corespond.insert(pair<string,int>(corepinpair,k));
        //cout<<corepinpair<<"cores"<<k<<endl;
      // cout<<corepinpair<<"core"<<k<<endl;
    }
    return  corespond;


}
map<string,int> set_corespond_pin(clique& nowclique,FF& flipflop)
{
    double h=flipflop.getheight();
    double w=flipflop.getwidth();
    double up=nowclique.up;
    double down=nowclique.down;
    double left=nowclique.left;
    double right=nowclique.right;
    map<string,int> corespond;
    map<string,pinpair> tobematch=nowclique.clique_member;
    //cout<<"l"<<left<<"r"<<right<<"u"<<up<<"d"<<down<<endl;
    
    for(int k=0;k<flipflop.qdpinpair.size();k++)
    {
        Pins d=flipflop.qdpinpair[k].first;
        Pins q=flipflop.qdpinpair[k].second;
        double dx=d.getx()/flipflop.getwidth();
        double dy=d.gety()/flipflop.getheight();
        double qx=q.getx()/flipflop.getwidth();
        double qy=q.gety()/flipflop.getheight();
      //  cout<<dx<<" ,"<<dy<<qx<<" ,"<<qy<<endl;
        double min=4;
        string corepinpair;
        for(auto it=tobematch.begin();it!=tobematch.end();it++)
        {
          //cout<<"tomatch"<<it->first<<endl;  
        Pins td=it->second.todpin;  
         double tdx=(td.getx()-left)/right;
        double tdy=(td.gety()-down)/up;
         double core_num;  
        if(it->second.gethaveq())
        {
             Pins tq=it->second.toqpin;
            
       
            if(nowclique.haveqnum)
            {
                double tqx=(tq.getx()-left)/right;
                double tqy=(tq.gety()-down)/up;
                core_num=(absd(tdx-dx)+absd(tdy-dy))+absd(tqy-qy)+absd(tqx-qx);

            }
            else
            {
                core_num=(absd(tdx-dx)+absd(tdy-dy))*2;

            }   
            
        }
        else 
        {
            core_num=(absd(tdx-dx)+absd(tdy-dy))*2;

        }   
       
        
      //  cout<<it->first<<"c "<<" "<<tdx<<" ,"<<tdy<<" "<<tqx<<" ,"<<tqy<<endl;
        
        if(core_num<min&&corespond.find(it->first)==corespond.end())
            {
                min=core_num;
                corepinpair=it->first;
            }

        }
       

        corespond.insert(pair<string,int>(corepinpair,k));
       //cout<<corepinpair<<"core"<<k<<endl;
    }
    return  corespond;


}

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
    vector<placement>& placementRow, unordered_map<string, instance>& inst_lib, string name,//name代表需要改成的Flip Flop library name
    instance &temp_re,double DisplacementDelay) {
    vector<string>old_ff;
    vector<instance>Old_FF;
    double old_slack=0;
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
        old_slack += inst_lib[inst_name].Getslack();
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
        temp_re = temp;
        double sum = 0;
        for (int i = 0; i < Old_FF.size(); i++) {
            double temp_x = (Old_FF[i].GetX() - temp.GetX()) * DisplacementDelay; double temp_y = (Old_FF[i].GetY() - temp.GetY()) * DisplacementDelay;
            sum += temp_x; sum += temp_y;
        }
        sum += old_slack; sum += temp.getff().Getdelay();
        temp_re.SetSlack(sum);
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
        cout << "can't" << endl;
        X_And_Y re;
        re.x = 0; re.y = 0;
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
int clique_test(ofstream& log, clique& nowclique, map<string, pinpair>& totest_Pinpair,map<string, FF>& FF_lib, map<string, FF>& FF_lib2, double a, double b, double c, double displacemaentdelay)
{

    bool success = 0;
    log<<"c have q"<<nowclique.haveqnum<<endl;
    //cout << "xx" << endl;
    double x = nowclique.centerx * (nowclique.clique_member.size()+nowclique.haveqnum);
    double y = nowclique.centery * (nowclique.clique_member.size()+nowclique.haveqnum);
    log<<"ori clique center"<<x<<" "<<y<<endl;
    for (auto it = totest_Pinpair.begin(); it != totest_Pinpair.end(); it++)
    {


        x += it->second.todpin.getx();
        y += it->second.todpin.gety();
        if(it->second.gethaveq())
        {
            x += it->second.toqpin.getx();
            y += it->second.toqpin.gety();
        }
        else
        {
            
        }
    }
    x /= (nowclique.clique_member.size() + totest_Pinpair.size());
    y /= (nowclique.clique_member.size() + totest_Pinpair.size());
    double maxgain = 0;
    FF mergeff;
    string mergeffname;
    Pins center;
    center.SetX(x);
    center.SetY(y);
    log<<"newclique center"<<center.getx()<<" "<<center.gety()<<endl;
    // cout<<nowclique.ffname<<"FFNAME"<<endl;
     // cout<<FF_lib[nowclique.ffname].getpoewer()<<"FFpow"<<endl;
     // cout<<nowclique.flipflop.getarea()<<"FFar"<<endl;
    // cout<<nowclique.clique_member.size()+totest_Pinpair.size();
     //cout<<FF_lib2.size()<<"ss";
     
     //cout<<nowclique.flipflop.Getcostperbit(b, c)<<endl;
    double prevcost =( nowclique.flipflop.getarea()*b+ c*FF_lib[nowclique.ffname].getpoewer())*(nowclique.clique_member.size() + totest_Pinpair.size());
    //cout<<"prevcost="<<nowclique.flipflop.Getcost(b,c)<<endl;
    for (auto fit = FF_lib2.begin(); fit != FF_lib2.end(); fit++)
    {
        double nowffcost = fit->second.Getcost(b, c);
       // cout<<"nowcost="<<nowffcost<<endl;
        double totalslackcost = 0;
        // cout<<fit->first<<endl;
        //cout<<"start core"<<endl;
        map<string, int> corespond = set_corespond_pin(nowclique, totest_Pinpair, fit->second);
        double displacement = 0;
        for (auto it = corespond.begin(); it != corespond.end(); it++)
        {
            
            // cout<<it->first<<"ss"<<endl;
            log<<it->first<<"coreto"<<fit->first<<" "<<it->second<<endl;
            Pins d = fit->second.qdpinpair.at(it->second).first;
            d.addX(center.getx() - (fit->second.getwidth() / 2));
            d.addY(center.gety() - (fit->second.getheight() / 2));
            Pins q = fit->second.qdpinpair.at(it->second).second;
            q.addX(center.getx() - (fit->second.getwidth() / 2));
            q.addY(center.gety() - (fit->second.getheight() / 2));

            auto topair = nowclique.clique_member.find(it->first);
            double difference ;
            if (topair != nowclique.clique_member.end())
            {
                //cout<<topair->second.todpin.getx()<<","<<topair->second.todpin.gety()<<endl;
               // cout<<topair->second.toqpin.getx()<<","<<topair->second.toqpin.gety()<<endl;
              
                
                 if(topair->second.gethaveq())
                 {
                    difference= (distance(topair->second.todpin, d) - topair->second.GetDdistance())+(distance(topair->second.toqpin, q) - topair->second.GetQdistance());
                 }
                 else
                 {
                    difference= 2*(distance(topair->second.todpin, d) - topair->second.GetDdistance());
                 }   
                
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
                    cout << it->first << "error"<<endl;
                }
                if(topair->second.gethaveq())
                 {
                    difference= (distance(topair->second.todpin, d) - topair->second.GetDdistance())+(distance(topair->second.toqpin, q) - topair->second.GetQdistance());
                 }
                 else
                 {
                    difference= 2*(distance(topair->second.todpin, d) - topair->second.GetDdistance());
                 }
                if (difference > 0)
                {
                    displacement += difference;
                }

            }

           // cout<<displacement<<endl;
            double slack = topair->second.Getslack() - displacemaentdelay * displacement - fit->second.Getdelay() + FF_lib[topair->second.getoldffname()].Getdelay();
          //  cout<<topair->second.getoldffname()<<endl;
           // cout<<FF_lib[topair->second.getoldffname()].Getdelay()<<endl;
            //cout<<fit->second.Getdelay()<<endl;
            if(slack>=topair->second.Getslack())
            {

                if(slack<0)
                {
                    totalslackcost = a*(topair->second.Getslack()-slack);
                   // cout<< totalslackcost<<endl;
                }
                else
                {
                    if(topair->second.Getslack()>=0)
                    {
                        totalslackcost =0;
                    }
                    else
                    {
                        totalslackcost =a*topair->second.Getslack();
                    }
                    
                   // cout<< totalslackcost<<endl;
                }
            }
            else
            {
                if (slack < 0)
                {
               // cout<<"slack="<<topair->second.Getslack()<<endl;
               /// cout<<"delay="<<fit->second.Getdelay()<<endl;
                    totalslackcost = -a * slack;
                }

            }
            



        }
        if (totalslackcost > prevcost)
        {
            success = -1;
        }
        double nowgain = prevcost - fit->second.Getcost(b, c) - totalslackcost;
        if (nowgain > maxgain)
        {
            //cout<<totalslackcost/a<<endl;
            totalestslack+=totalslackcost;
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

int main(int argc, char** argv) {
    double Alpha, Beta, Gamma, Lambda;

    double Die_LLeftX, Die_LLeftY, Die_URightX, Die_URightY;

    map<string, Pins>Input_pins;
    map<string, Pins>Output_pins;

    map<string, FF>FF_lib;
    map<int, map<string, FF>>FF_lib2;
    map<string, Gate>GG_lib;
    double oldarea=0;
    double oldslack=0;
    double oldpower=0; 
    double newarea=0;
    double newslack=0;
    double newpower=0; 


    unordered_map<string, instance>inst_lib;
    unordered_map<string, instance>inst_lib_new;
    map<string, Nets>Net_Lib;

    double BinWidth, BinHeight, BinMaxUtil;

    vector<placement> placementRow;
    double** placement_check;
    double DisplacementDelay;
    map<string, double>TimingSlack;

    ifstream infile;
    ofstream outfile,logfile;

    string s;
    double num;
    infile.open(argv[1]);
    outfile.open(argv[2]);
    logfile.open("log.txt");


     //infile.open("C:\\Users\\Yeh\\Desktop\\class\\eda\\Fp\\ICCAD-ProblemB\\testcase1_0614.txt");
    // outfile.open("C:\\Users\\Yeh\\Desktop\\class\\eda\\Fp\\ICCAD-ProblemB\\output0614.txt");
   // infile.open();
    
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
    if (logfile.is_open())
    {

    }
    else
    {
        cout << "lognotopen";
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
            tempinst.Setname(s,0);
            oldarea+=FF_lib[s].getarea();
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
    map<string,string> frompin;//second街道first ex:(reg1/d,in) 
    map<string,double> fromdist;

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
            tempnet.setfrompin(frompin);
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
    for(auto it=frompin.begin();it!=frompin.end();it++)
    {
       // cout<<it->first<<it->second<<endl;
        if((it->first).find("D")!=string::npos)
        {
            size_t pos = it->first.find('/');
                    string inst_name;
                    string pin_name;
                    if (pos != string::npos) {
                        inst_name = it->first.substr(0, pos);
                        pin_name = it->first.substr(pos + 1);
                    }
                Pins d=inst_lib[inst_name].GetPins(pin_name);
            if((it->second).find("Q")!=string::npos)
            {
                
                size_t pos = it->second.find('/');
                    string inst_name;
                    string pin_name;
                    if (pos != string::npos) {
                        inst_name = it->second.substr(0, pos);
                        pin_name = it->second.substr(pos + 1);
                    }
                    else
                    {
                        Pins q=Input_pins[it->second];
                        fromdist.insert(pair<string,double>(it->first,distance(d,q)));

                        continue;
                    }  
                Pins q=inst_lib[inst_name].GetPins(pin_name);
                fromdist.insert(pair<string,double>(it->first,distance(d,q)));
            }
            else
            {

                size_t pos = it->second.find('/');
                    string inst_name;
                    string pin_name;
                    if (pos != string::npos) {
                        inst_name = it->second.substr(0, pos);
                        pin_name = it->second.substr(pos + 1);
                    }
                    else
                    {
                        Pins q=Input_pins[it->second];
                        fromdist.insert(pair<string,double>(it->first,distance(d,q)));

                        continue;
                    }    
                Pins mid=inst_lib[inst_name].GetPins(pin_name);
                string qs=frompin[it->second];
                     pos = qs.find('/');
                   
                    if (pos != string::npos) {
                        inst_name = qs.substr(0, pos);
                        pin_name = qs.substr(pos + 1);
                    }
                Pins q=inst_lib[inst_name].GetPins(pin_name);
                fromdist.insert(pair<string,double>(it->first,distance(d,mid)+distance(mid,q)));

            }
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
        FF_lib2[FF_lib[s].getbit()][s].SetQpinDelay(num);
        infile >> s;
    }
    //"Now" s is TimingSlack
    while (s == "TimingSlack") {
        string temp;
        infile >> s; //instance name
        infile >> temp;
        infile >> num;
        inst_lib[s].SetSlack(num);
        if(num<0)
        {
            oldslack-=num;
        }
        infile >> s;
    }

    //"Now" s is GatePower
    while (s == "GatePower") {
        infile >> s;
        infile >> num;
        FF_lib[s].SetPower(num);
        FF_lib2[ FF_lib[s].getbit()][s].SetPower(num);
        infile >> s;
    }
    vector<string>new_inst;
    vector<vector<string>>new_map_list;
    map<string,string> oldtonew;
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
            //logfile<<"now"<<nowinstname<<endl;
            FF nowff = FF_lib[nowinst.Getlibname()];
            oldpower+=nowff.getpoewer();
            //logfile<<nowinstname<<nowinst.toqpin.size()<<endl;
            if (nowff.getbit() > 1)
            {
                
                for (int k = 0; k < nowff.getbit(); k++)
                {

                    string pinindex = to_string(k);
                    Pins temptoD = nowinst.todpin["D" + pinindex];
                    vector<Pins> temptoQ ;
                    if(nowinst.toqpin.find("Q" + pinindex)!=nowinst.toqpin.end())
                    {
                        
                        //<<"GOTQ";
                        temptoQ = nowinst.toqpin["Q" + pinindex];
                    
                    }
                    pinpair temppinpair(temptoD, temptoQ, nowinst, pinindex,nowinstname);
                    
                    //logfile<<temppinpair.toqpin.getx()<<nowinstname + "/" + to_string(k)<<temppinpair.toqpin.gety()<<endl;
                    //cout<<temptoQ.getx()<<"toq,"<<temptoQ.gety()<<endl;
                    //cout<<temptoD.getx()<<"tod,"<<temptoD.gety()<<endl;
                   // cout<<temppinpair.gethaveq();
                    topin.insert(pair<string, pinpair>(nowinstname + "/" + to_string(k), temppinpair));
                    //logfile<<nowinstname<<"haveq"<<topin[nowinstname + "/"+ to_string(k)].gethaveq()<<endl;
                   // logfile<<"to"<<nowinstname + "/" + to_string(k)<<endl;
                }
            }
            else
            {

                Pins temptoD = nowinst.todpin["D"];
                
                string pinindex = to_string(0);
                vector<Pins> temptoQ ;
                if(nowinst.toqpin.find("Q")!=nowinst.toqpin.end())
                {
                    
                   // logfile<<"GOTQ";
                    temptoQ = nowinst.toqpin["Q"];
                }
                pinpair temppinpair(temptoD, temptoQ, nowinst, pinindex,nowinstname);
                if(temppinpair.gethaveq())
                {
                    //   logfile<<temppinpair.toqpin.getx()<<nowinstname<<temppinpair.toqpin.gety()<<endl;
                }
                
                //cout<<temptoQ.getx()<<"toq,"<<temptoQ.gety()<<endl;
               // cout<<temptoD.getx()<<"tod,"<<temptoD.gety()<<endl;
               // cout<<temppinpair.gethaveq();
                topin.insert(pair<string, pinpair>(nowinstname + "/", temppinpair));
                //logfile<<nowinstname<<"haveq"<<topin[nowinstname + "/"].gethaveq()<<endl;
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
           // cout<<topin.begin()->first<<endl;
            temppair.insert(pair<string, pinpair>(topin.begin()->first, topin.begin()->second));
            
            //cout<<"xx"<<temppair[topin.begin()->first].todpin.getx()+temppair[topin.begin()->first].toqpin.getx()<<endl;
          //  cout<<"yy"<<temppair[topin.begin()->first].todpin.gety()+temppair[topin.begin()->first].toqpin.gety()<<endl;
            clique tempclique(temppair, tempff);
            tempclique.ffname = inst_lib[inst_name].Getlibname();
            // cout<<tempclique.ffname<<"tt";
            double pincount = 1;

            int success;

            do
            {
                string nearst = find_nearst_pinpair_outof_clique(logfile, topin, tempclique, to_test);
                logfile<<"near"<<nearst<<endl;
                if (topin.find(nearst) != topin.end())
                {
                    string nearst_neearst;
                    pinpair nearst_pinpair = pinpair(topin[nearst]);
                    if(nearst_pinpair.gethaveq())
                    {
                         nearst_neearst = find_nearst_pinpair(logfile,topin, nearst_pinpair.todpin, nearst_pinpair.toqpin,nearst);
                    }
                    else
                    {
                         nearst_neearst = find_nearst_pinpair(logfile,topin, nearst_pinpair.todpin,nearst);
                    }
                    logfile<<"near2"<<nearst_neearst<<endl;
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
                    logfile<<"test"<<endl;
                    success = clique_test(logfile,tempclique, to_test,  FF_lib,FF_lib2[pincount], Alpha, Beta, Gamma, DisplacementDelay);
                    //cout << "soccess=" << success << endl;
                    logfile<<"testdone"<<endl;
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
           //cout<<it->ffname<<endl;
            instance temp_re;
            
            temp_XY=find_the_position(it->Getmember(), FF_lib, placement_check, placementRow, inst_lib, it->GetName(),temp_re,DisplacementDelay);
            ss << "Inst C" << inst_num << " " << it->ffname << " " << temp_XY.x << " " << temp_XY.y;

//            cout << "C" << inst_num;
            temp_re.Setlibname(it->ffname);
           // cout<<"ss"<<temp_re.Getlibname()<<endl;
            inst_lib_new.insert(pair<string, instance>("C"+to_string(inst_num),temp_re));
           // cout<<inst_lib_new["C"+to_string(inst_num)].Getlibname()<<"tt"<<endl;
            //inst_lib_new["C"+to_string(inst_num)].SetFF(it->flipflop);
            //outfile<<temp_re.Getlibname()<<endl;
            new_inst.push_back(ss.str());
            newarea+=it->flipflop.getarea();
            newpower+=FF_lib[it->ffname].getpoewer();
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
                    oldtonew.insert(pair<string,string>(inst_name + "/" + "D" +pin_name,"C" +to_string(inst_num) + "/D" + to_string(pit->second)));
                    temp_map_list_one.push_back(ss_2.str());
                    ss_2.str("");

                    ss_2 << inst_name << "/" << "Q" << pin_name << " map " << "C" << inst_num << "/Q" << pit->second;
                    oldtonew.insert(pair<string,string>(inst_name + "/" + "Q" +pin_name,"C" +to_string(inst_num)  + "/Q" + to_string(pit->second)));
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
                    oldtonew.insert(pair<string,string>(inst_name + "/" + "D" ,"C" +to_string(inst_num ) + "/D" ));
                    temp_map_list_one.push_back(ss_2.str());
                    ss_2.str(""); //ss_2.clear();

                    ss_2 << inst_name << "/" << "Q" << pin_name << " map " << "C" << inst_num << "/Q";
                    oldtonew.insert(pair<string,string>(inst_name + "/" + "Q" ,"C" +to_string(inst_num )  + "/Q" ));
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
    }/*
    for(auto it=oldtonew.begin();it!=oldtonew.end();it++)
    {
        cout<<it->first<<"t0"<<it->second<<endl;
    }
    */
    for(auto it=frompin.begin();it!=frompin.end();it++)
    

    {
        //cout<<it->first<<"xx"<<it->second<<endl;
        double newdist=0;
        if((it->first).find("D")!=string::npos)//所有D PIN
        {
            
             size_t dpos = it->first.find('/');
                    string dinst_name;
                    string dpin_name;
                    if (dpos != string::npos) {
                        dinst_name =  it->first.substr(0, dpos);
                        dpin_name =  it->first.substr(dpos + 1);
                    }
            string s=oldtonew[it->first];
             //cout<<s<<endl;
            size_t pos = s.find('/');
                    string ndinst_name;
                    string pin_name;
                    if (pos != string::npos) {
                        ndinst_name = s.substr(0, pos);
                        pin_name = s.substr(pos + 1);
                    }
                Pins d=inst_lib_new[ndinst_name].GetPins(pin_name);
                //cout<<"newff="<<inst_lib_new[inst_name].Getlibname()<<endl;
            if((it->second).find("Q")!=string::npos)
            {
                string s=oldtonew[it->second];   
               // cout<<s<<endl;
                size_t pos = s.find('/');
                    string inst_name;
                    string pin_name;
                    if (pos != string::npos) {
                        inst_name = s.substr(0, pos);
                        pin_name = s.substr(pos + 1);
                    }
                Pins q=inst_lib_new[inst_name].GetPins(pin_name);
                double dispace=distance(d,q);
              //  cout<<d.getx()<<","<<d.gety()<<endl;
              //  cout<<q.getx()<<","<<q.gety()<<endl;

                //cout<<"newff="<<inst_lib_new[inst_name].Getlibname()<<endl;
               // cout<<dispace<<"-"<<fromdist[it->first]<<endl;
                double tmp=inst_lib[dinst_name].Getslack()-DisplacementDelay*(dispace-fromdist[it->first])+FF_lib[inst_lib[dinst_name].Getlibname()].Getdelay()-FF_lib[inst_lib_new[ndinst_name].Getlibname()].Getdelay();
             //    
               // cout<<dinst_name<<ndinst_name<<endl;
                if(inst_lib_new.find(ndinst_name)==inst_lib_new.end())
                {
                    cout<<"not"<<endl;
                }
                // cout<<inst_lib[dinst_name].Getlibname()<<inst_lib_new[ndinst_name].Getlibname()<<endl;
               // cout<<FF_lib[inst_lib[dinst_name].Getlibname()].Getdelay()<<FF_lib[inst_lib_new[ndinst_name].Getlibname()].Getdelay()<<endl;
                if(tmp<0)
                {
                    newslack-=tmp;
                   // cout<<dispace<<"-"<<fromdist[it->first]<<endl;
                  //  cout<<"s"<<inst_lib[dinst_name].Getslack()<<endl;
                   // cout<<DisplacementDelay*(dispace-fromdist[it->first])<<endl;
             //    
                 //   cout<<"tq"<<tmp<<endl;        
                     //       cout<<"de"<<inst_lib[it->first].Getdelay()<<endl;
                   //         cout<<"de2"<<inst_lib_new[inst_name].Getdelay()<<endl;
                       //     cout<<"slackq"<< newslack<<endl;
                }
            }
            else
            {
                string s=oldtonew[it->second];   
                size_t pos = s.find('/');
                    string inst_name;
                    string pin_name;
                    if (pos != string::npos) {
                        inst_name = s.substr(0, pos);
                        pin_name = s.substr(pos + 1);
                        
                    }
                     else
                    {
                        //frompin.second為IN
                        Pins q=Input_pins[it->second];
                        //fromdist.insert(pair<string,double>(it->first,distance(d,q)));
                 
                        double dispace=distance(d,q);
                      //  cout<<dispace<<"-"<<fromdist[it->first]<<endl;
                        double tmp=inst_lib[dinst_name].Getslack()-DisplacementDelay*(dispace-fromdist[it->first])+FF_lib[inst_lib[dinst_name].Getlibname()].Getdelay()-FF_lib[inst_lib_new[ndinst_name].Getlibname()].Getdelay();
               //       cout<<inst_lib[it->first].Getdelay()<<inst_lib_new[inst_name].Getdelay();
                    //cout<<dinst_name<<ndinst_name<<endl;
                     if(inst_lib_new.find(ndinst_name)==inst_lib_new.end())
                {
                    cout<<"not"<<endl;
                }
                  //  cout<<inst_lib[dinst_name].Getlibname()<<inst_lib_new[ndinst_name].Getlibname()<<endl;
                  //  cout<<FF_lib[inst_lib[dinst_name].Getlibname()].Getdelay()<<FF_lib[inst_lib_new[ndinst_name].Getlibname()].Getdelay()<<endl;
                        if(tmp<0)
                        {
                          //  cout<<DisplacementDelay*(dispace-fromdist[it->first])<<endl;
                            newslack-=tmp;//cout<<"ti"<<tmp<<endl;
                         //   cout<<dispace<<"-"<<fromdist[it->first]<<endl;
                          //  cout<<"s"<<inst_lib[dinst_name].Getslack()<<endl;
                           // cout<<"de"<<inst_lib[it->first].Getdelay()<<endl;
                           //cout<<"de2"<<inst_lib_new[inst_name].Getdelay()<<endl;
                           // cout<<"slacki"<< newslack<<endl;
                        }
                        continue;
                    }  
                Pins mid=inst_lib_new[inst_name].GetPins(pin_name);

                string qs=oldtonew[frompin[it->second]];
                 pos = qs.find('/');
                    
                    if (pos != string::npos) {
                        inst_name = qs.substr(0, pos);
                        pin_name = qs.substr(pos + 1);
                    }
                Pins q=inst_lib_new[inst_name].GetPins(pin_name);
                double dispace=distance(d,mid)+distance(mid,q);
                //cout<<dispace<<"-"<<fromdist[it->first]<<endl;
                double tmp=inst_lib[dinst_name].Getslack()-DisplacementDelay*(dispace-fromdist[it->first])+FF_lib[inst_lib[dinst_name].Getlibname()].Getdelay()-FF_lib[inst_lib_new[ndinst_name].Getlibname()].Getdelay();
                //cout<<inst_lib[it->first].Getdelay()<<inst_lib_new[inst_name].Getdelay();
              //  cout<<dinst_name<<ndinst_name<<endl;
                  if(inst_lib_new.find(ndinst_name)==inst_lib_new.end())
                {
                    cout<<"not"<<endl;
                }
              //   cout<<inst_lib[dinst_name].Getlibname()<<inst_lib_new[ndinst_name].Getlibname()<<endl;
              //  cout<<FF_lib[inst_lib[dinst_name].Getlibname()].Getdelay()<<FF_lib[inst_lib_new[ndinst_name].Getlibname()].Getdelay()<<endl;
                //
                if(tmp<0)
                {
                    newslack-=tmp;
                    //cout<<dispace<<"-"<<fromdist[it->first]<<endl;
                  //  cout<<"tm"<<tmp<<endl;
                   // cout<<DisplacementDelay*(dispace-fromdist[it->first])<<endl;
                   // cout<<"s"<<inst_lib[dinst_name].Getslack()<<endl;
                   // cout<<"de"<<inst_lib[it->first].Getdelay()<<endl;
                    //cout<<"de2"<<inst_lib_new[inst_name].Getdelay()<<endl;
                    //cout<<"slackm"<< newslack<<endl;
                }
                

            }
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



    cout<<"oldpower="<<oldpower<<"oldarea="<<oldarea<<"oldslack="<<oldslack<<"newpower="<<newpower<<"newarea="<<newarea<<"newslack="<<newslack<<endl;
    cout<<"est="<< totalestslack;
    infile.close();
    outfile.close();
    logfile.close();
    return 0;
}
