#include <iostream>
#include "pin.h"
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
double distance(Pins& a ,Pins& b)
{
    double ans=absd(a.getx()-b.getx())+absd(a.gety()-b.gety());
    return ans;
}