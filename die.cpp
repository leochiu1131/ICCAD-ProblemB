#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "pin.h"
#include "ff.h"
#include "gate.h"
#include "net.h"
#include "instance.h"
using namespace std;

class dieunit
{
    public:
    string pintype;
    bool occupied;
};

double xindex=Die_URightX-Die_LLeftX+1;
double yindex=Die_URighty-Die_LLefty+1;
vector<vector<dieunit>> die(xindex, vector<dieunit>(yindex));

void pinD(Pins &p)
{

    die.at(p.getx()-Die_LLeftX).at(p.gety-Die_LLefty).pintype=p.getpintype();
}





