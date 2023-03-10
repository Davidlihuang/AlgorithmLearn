#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstdlib>
#include "data.h"
using namespace std;

//global var
data *D = new data;
//

void parsecsv(char *filename)
{
    cout << filename << endl;
    ifstream infile;
    infile.open(filename);
    string line, field;
    string temp_netname, temp_p1, temp_p2;
    getline(infile, line);
    while (getline(infile, line))
    {
        bool flag = false;
        istringstream iss(line);

        getline(iss, temp_netname, ',');
        //cout << temp_netname << endl;
        net *new_net = new net(temp_netname);
        flag = D->find_net(temp_netname);

        if (!flag) //is new netname
        {
            getline(iss, temp_p1, ',');
            //cout << temp_p1 << endl;
            new_net->pinnames.push_back(temp_p1);
            getline(iss, temp_p2, ',');
            //cout << temp_p2 << endl;
            new_net->pinnames.push_back(temp_p2);

            getline(iss, field, ','); //load delay
            //cout << field << endl;
            const char *str_c = field.c_str();
            char **ptr;
            //char* str_c = con_str_c;
            double delay = strtod(str_c, ptr);
            new_net->delay = delay;
            //cout << "delay: " << delay << endl;
            D->add_net(new_net);
        }
        else //found the same netname
        {
            getline(iss, temp_p1, ',');
            //cout << temp_p1 << endl;
            new_net->pinnames.push_back(temp_p1);
            getline(iss, temp_p2, ',');
            //cout << temp_p2 << endl;
            new_net->pinnames.push_back(temp_p2);
            D->insert_new_pins(temp_netname, temp_p1, temp_p2);

            getline(iss, field, ','); //load delay
            //cout << field << endl;
            const char *str_c = field.c_str();
            char **ptr;
            double delay = strtod(str_c, ptr);
            new_net->update_delay(delay);
            //cout << "delay: " << delay << endl;
        }
    }
    //cout << endl;
}

void output_netname_by_descending_delay(char *filename)
{
    ofstream outfile;
    outfile.open(filename);
    for (size_t i = 0; i < D->netlist.size(); i++)
    {
        outfile << D->netlist[i]->netname << " " << D->netlist[i]->delay << endl;
        //cout << D->netlist[i]->netname << " " << D->netlist[i]->delay << endl;
    }
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc - 1; i++)
    {
        cout << "input file " << i << ": ";
        parsecsv(argv[i]);
    }
    //D->print_netlist();
    D->sort_by_delay();
    cout << "output file: " << argv[argc - 1] << endl;
    output_netname_by_descending_delay(argv[argc - 1]);
    //D->print_netlist();
    return 0;
}