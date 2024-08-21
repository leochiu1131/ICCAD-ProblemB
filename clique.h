#ifndef _clique_H
#define _clique_H

#include<vector>
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
        bool haveq;

    public:
    Pins todpin;
    Pins toqpin;
    pinpair()
    {

    }
    pinpair(Pins& dpin,vector<Pins>& qpins,instance& inst,string& s,string& instname)
    {
        todpin=dpin;
        
        Ddistance=distance(inst.GetPins("D"+s),todpin);
        double qx,qy;
        qx=qy=0;
        for(auto it=qpins.begin();it!=qpins.end();it++)
        {   
            qx+=it->getx();
            qy+=it->gety();

        }
        qx/=qpins.size();
        qy/=qpins.size();
        toqpin=Pins();
        toqpin.SetX(qx);
        toqpin.SetY(qy);
        Qdistance=distance(inst.GetPins("Q"+s),toqpin);
        slack=inst.Getslack();
        inst_name=instname;
        oldffname=inst.Getlibname();
        haveq=qpins.size();
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
   void sethaveq()
   {
     haveq=1;
   }
   bool gethaveq()
   {
    return haveq;
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
    
    int haveqnum;
    clique (map<string,pinpair>&member,FF& fl)
    {
      
        haveqnum=0;
        for(auto it=member.begin();it!=member.end();it++)
        {
            if(it->second.gethaveq())
            {
                
                haveqnum++;
            }
        }
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
          if(haveqnum>0&&it->second.gethaveq())
          {
            qx+=it->second.toqpin.getx();
          
            qy+=it->second.toqpin.gety();
             if(it->second.toqpin.getx()>right)
            {
                right=it->second.toqpin.getx();
            }
            if(it->second.toqpin.getx()<left)
            {
                left=it->second.toqpin.getx();
            }
             if(it->second.toqpin.gety()>up)
            {
                up=it->second.toqpin.gety();
            }
              if(it->second.toqpin.gety()<down)
            {
                down=it->second.toqpin.gety();
            }
          }
            
            dx+=it->second.todpin.getx();
           
            dy+=it->second.todpin.gety();
           
             if(it->second.todpin.getx()>right)
            {
                right=it->second.todpin.getx();
            }
             
             if(it->second.todpin.getx()<left)
            {
                left=it->second.todpin.getx();
            }
            
            if(it->second.todpin.gety()>up)
            {
                up=it->second.todpin.gety();
            }
            
            if(it->second.todpin.gety()<down)
            {
                down=it->second.todpin.gety();
            }

        }
        aver_d_x=dx/clique_member.size();;
        aver_d_y=dy/clique_member.size();;
        if(haveqnum)
        {
            aver_q_x=qx/haveqnum;
            aver_q_y=qy/haveqnum;
            centerx=(dx+qx)/(haveqnum+clique_member.size());
            centery=(dy+qy)/(haveqnum+clique_member.size());
        }
        else
        {
            centerx=(aver_d_x);
            centery=(aver_d_y);

        }
        
        
        
    // cout<<"sl"<<left<<"r"<<right<<"u"<<up<<"d"<<down<<endl;

    }
void merge_clique (map<string,pinpair>&new_member ,FF& fl)
    {
        flipflop=fl;
        double dx=aver_d_x*clique_member.size();
        double dy=aver_d_y*clique_member.size();
        double qx=aver_q_x*haveqnum;
        double qy=aver_q_y*haveqnum;

        for(auto it=new_member.begin();it!=new_member.end();it++)
        {
            
            clique_member.insert(*it);
            if(it->second.gethaveq())
            {
                qx+=it->second.toqpin.getx();
          
                qy+=it->second.toqpin.gety();
                 if(it->second.toqpin.getx()>right)
                {
                    right=it->second.toqpin.getx();
                }
                 if(it->second.toqpin.getx()<left)
                {
                    left=it->second.toqpin.getx();
                }
                 if(it->second.toqpin.gety()>up)
                {
                    up=it->second.toqpin.gety();
                }
                   if(it->second.toqpin.gety()<down)
                {
                down=it->second.toqpin.gety();
                }
                haveqnum++;
            } 
            
            dx+=it->second.todpin.getx();
           
            dy+=it->second.todpin.gety();
           
             if(it->second.todpin.getx()>right)
            {
                right=it->second.todpin.getx();
            }
            
             if(it->second.todpin.getx()<left)
            {
                left=it->second.todpin.getx();
            }
            
            if(it->second.todpin.gety()>up)
            {
                up=it->second.todpin.gety();
            }
           
            if(it->second.todpin.gety()<down)
            {
                down=it->second.todpin.gety();
            }
        }
        
        aver_d_x=dx/clique_member.size();;
        aver_d_y=dy/clique_member.size();;
        if(haveqnum)
        {
            aver_q_x=qx/haveqnum;
            aver_q_y=qy/haveqnum;
            centerx=(aver_q_x+aver_d_x)/2;
            centery=(aver_q_y+aver_d_y)/2;
        }
        else
        {
            centerx=(aver_d_x);
            centery=(aver_d_y);

        }


    }
};   
 #endif