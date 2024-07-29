#ifndef _clique_H
#define _clique_H


#include "pin.h"
#include "ff.h"
#include "gate.h"
#include "net.h"
#include "distance.cpp "



class pinpair
{
    private:
        
        double Qdistance;
        double Ddistance;
        double slack;
        string inst_name;
        string oldffname;
      

    public:
    Pins todpin;
    Pins toqpin;
    pinpair()
    {

    }
    pinpair(Pins& dpin,Pins& qpin,instance& inst,string& s,string& instname)
    {
        todpin=dpin;
        toqpin=qpin;
        Ddistance=distance( inst.GetPins("D"+s),todpin);
        Qdistance=distance(inst.GetPins("Q"+s),toqpin);
        slack=inst.Getslack();
        inst_name=instname;
        oldffname=inst.Getlibname();
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
   string getinstname()
   {
     return inst_name;
   }
   string getoldffname()
   {
     return oldffname;
   }
    

};
class clique;
map<string,int> set_corespond_pin(clique& nowclique,FF& flipflop);
class clique
{
    public:
    double up;
    double down;
    double left;
    double right;
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
    map<string, pinpair> Getmember() {
        return clique_member;
    }
    string GetName() {
        return ffname;
    }
    void setdq()
    {
        double dx=0;
        double dy=0;
   
        double qx=0;
        double qy=0;
        down=DBL_MAX;
        left=DBL_MAX;
        up=DBL_MIN;
        right=DBL_MIN;
         for(auto it=clique_member.begin();it!=clique_member.end();it++)
        {
           //cout<<"qin"<<it->second.toqpin.getx()<<it->second.toqpin.gety()<<endl;
          // cout<<"din"<<it->second.todpin.getx()<<it->second.toqpin.gety()<<endl;
            qx+=it->second.toqpin.getx();
          
            qy+=it->second.toqpin.gety();
            dx+=it->second.todpin.getx();
           
            dy+=it->second.todpin.gety();
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
        aver_q_x=qx/clique_member.size();;
        aver_q_y=qy/clique_member.size();;
        aver_d_x=dx/clique_member.size();;
        aver_d_y=dy/clique_member.size();;
        centerx=(aver_q_x+aver_d_x)/2;
        centery=(aver_q_y+aver_d_y)/2;
    // cout<<"sl"<<left<<"r"<<right<<"u"<<up<<"d"<<down<<endl;

    }
void merge_clique (map<string,pinpair>&new_member ,FF& fl)
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
       aver_q_x=qx/clique_member.size();;
        aver_q_y=qy/clique_member.size();;
        aver_d_x=dx/clique_member.size();;
        aver_d_y=dy/clique_member.size();;
        centerx=(aver_q_x+aver_d_x)/2;
        centery=(aver_q_y+aver_d_y)/2;


    }
};   
 #endif