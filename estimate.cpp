#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <set>
#include "pin.h"
#include "ff.h"
#include "gate.h"
#include "net.h"
#include "instance.h"
using namespace std;
double absd(double x)
{
    if(x>0)
    {
        return x*-1;
    }
    else
    {
        return x;
    }
}
double distance(Pins a ,Pins& b)
{
    double ans=absd(a.getx()-b.getx())+absd(a.gety()-b.gety());
    return ans;
}
class pinpair
{
    private:
        double qpindelay;
        double Qdistance;
        double Ddistance;
        double slack;

    public:
    Pins todpin;
    Pins toqpin;
    pinpair()
    {

    }
    pinpair(Pins& dpin,Pins& qpin,instance& inst,string& s)
    {
        todpin=dpin;
        toqpin=qpin;
        Ddistance=distance( inst.GetPins("D"+s),todpin);
        Qdistance=distance(inst.GetPins("Q"+s),toqpin);
        slack=inst.Getslack();
        qpindelay=inst.getff().Getdelay();
    }
    double GetQdistance()
    {
        return Qdistance;
    }
    double GetDdistance()
    {
        return Ddistance;
    }
    double GetQdistance() const
    {
        return Qdistance;
    }
    double GetDdistance() const
    {
        return Ddistance;
    }
    
   double Getslack() const
   {
     return slack;
   }
   double Getslack() 
   {
     return slack;
   }
   double getdelay()
   {
    return qpindelay;
   }
    double getdelay() const
   {
    return qpindelay;
   }

};

class clique
{
    public:
    double centerx;
    double centery;
    FF flipflop;
    string ffname;
    map<string,pinpair>clique_member;
    double aver_d_x, aver_d_y, aver_q_x, aver_q_y;

    clique (map<string,pinpair>&member,FF& fl)
    {
        clique_member=member;
        flipflop=fl;
       
        setdq();
    }
 
    void setdq()
    {
        double dx=0;
        double dy=0;
   
        double qx=0;
        double qy=0;
         for(auto it=clique_member.begin();it!=clique_member.end();it++)
        {
            
            qx+=it->second.toqpin.getx();
          
            qy+=it->second.toqpin.gety();
            dx+=it->second.todpin.getx();
           
            dy+=it->second.todpin.gety();


        }
        aver_q_x=qx/clique_member.size();;
        aver_q_y=qy/clique_member.size();;
        aver_d_x=dx/clique_member.size();;
        aver_d_y=dy/clique_member.size();;
        centerx=(aver_q_x+aver_d_x)/2;
        centery=(aver_q_y+aver_d_y)/2;


    }
    
    void merge_clique (map<string,pinpair>&new_member,FF& fl)
    {
        flipflop=fl;
        double dx=aver_d_x*clique_member.size();
        double dy=aver_d_y*clique_member.size();
         double qx=aver_q_x*clique_member.size();
        double qy=aver_q_y*clique_member.size();

        for(auto it=new_member.begin();it!=new_member.end();it++)
        {
            
            clique_member.insert(*it);
            
             qx+=it->second.toqpin.getx();
          
            qy+=it->second.toqpin.gety();
            dx+=it->second.todpin.getx();
           
            dy+=it->second.todpin.gety();
        }
       aver_q_x=qx/clique_member.size();;
        aver_q_y=qy/clique_member.size();;
        aver_d_x=dx/clique_member.size();;
        aver_d_y=dy/clique_member.size();;
        centerx=(aver_q_x+aver_d_x)/2;
        centery=(aver_q_y+aver_d_y)/2;


    }
};
 string find_nearst_pinpair_outof_clique(map<string,pinpair>& topin,clique&nowclique)
    {
        double mindistance=__DBL_MAX__;
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
            if(vit==nowclique.clique_member.end())
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
    /*
    string find_nearst_pinpair(map<string,pinpair>& topin,clique&nowclique)
    {
        double mindistance=__DBL_MAX__;
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
    string find_nearst_pinpair(map<string,pinpair>& topin,Pins& d,Pins&q)
    {
        double mindistance=__DBL_MAX__;
        string nearstpair=" ";
        
        for(auto it=topin.begin();it!=topin.end();it++)
        {
        

            
                double nowdistance=distance(it->second.todpin,d)+distance(it->second.toqpin,q);
                if(nowdistance<mindistance)
                {
                    mindistance=nowdistance;
                    nearstpair=it->first;
                }
            

        }
        return nearstpair;

    }