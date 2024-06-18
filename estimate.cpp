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

double distance(Pins&a ,Pins&b)

{
    double ans=abs(a.getx()-b.getx())+abs(a.gety()-b.gety());
    return ans;
}