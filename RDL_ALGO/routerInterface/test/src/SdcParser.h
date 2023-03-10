#ifndef SDCPARSER_H
#define SDCPARSER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>

#include "net.h"

using namespace std;

class SdcParser
{
private:
    vector<SDC::Net *> nets;
    vector<SDC::Chiplet *> chiplets;
    vector<string> sdc_filenames;
    double clock_period;

    vector<string> vecInput;
public:
    void run(const string &strInputSdc0, const string &strInputSdc1, const string &strInputSdc2, const string &strOutPutFile);
};

#endif