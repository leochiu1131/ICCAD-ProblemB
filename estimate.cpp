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
    if(x<0)
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
    
    void setdq()
    {
        double dx=0;
        double dy=0;
   
        double qx=0;
        double qy=0;
        down=__DBL_MAX__;
        left=__DBL_MAX__;
        up=__DBL_MIN__;
        right=__DBL_MIN__;
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

 string find_nearst_pinpair_outof_clique(map<string,pinpair>& topin,clique&nowclique,map<string,pinpair>& to_test)
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
            auto tit=to_test.find(it->first);
            if(vit==nowclique.clique_member.end()&&tit==to_test.end())
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
    //cout<<"uu";
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
        Pins d=flipflop.qdpinpair[k].first;
        Pins q=flipflop.qdpinpair[k].second;
        double dx=d.getx()/flipflop.getwidth();
        double dy=d.gety()/flipflop.getheight();
        double qx=q.getx()/flipflop.getwidth();
         double qy=q.gety()/flipflop.getheight();
        //cout<<dx<<" ,"<<dy<<qx<<" ,"<<qy<<endl;
        double min=100;
        string corepinpair;
        for(auto it=tobematch.begin();it!=tobematch.end();it++)
        {

        Pins td=it->second.todpin;    
        Pins tq=it->second.toqpin;
            
        double tdx=(td.getx()-left)/right;
        double tdy=(td.gety()-down)/up;
        double tqx=(tq.getx()-left)/right;
         double tqy=(tq.gety()-down)/up;
       // cout<<it->first<<"c "<<" "<<tdx<<" ,"<<tdy<<" "<<tqx<<" ,"<<tqy<<endl;
        double core_num=(absd(tdx-dx)+absd(tdy-dy))+absd(tqy-qy)+absd(tqx-qx);
        if(core_num<min&&corespond.find(it->first)==corespond.end())
            {
                min=core_num;
                corepinpair=it->first;
            }

        }
       
       // cout<<corepinpair<<"cpin"<<endl;
        corespond.insert(pair<string,int>(corepinpair,k));
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

        Pins td=it->second.todpin;    
        Pins tq=it->second.toqpin;
            
        double tdx=(td.getx()-left)/right;
        double tdy=(td.gety()-down)/up;
        double tqx=(tq.getx()-left)/right;
         double tqy=(tq.gety()-down)/up;
      //  cout<<it->first<<"c "<<" "<<tdx<<" ,"<<tdy<<" "<<tqx<<" ,"<<tqy<<endl;
        double core_num=(absd(tdx-dx)+absd(tdy-dy))+absd(tqy-qy)+absd(tqx-qx);
        if(core_num<min&&corespond.find(it->first)==corespond.end())
            {
                min=core_num;
                corepinpair=it->first;
            }

        }
       

        corespond.insert(pair<string,int>(corepinpair,k));
      // cout<<corepinpair<<"core"<<k<<endl;
    }
    return  corespond;


}
