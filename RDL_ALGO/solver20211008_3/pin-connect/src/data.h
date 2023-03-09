#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;

class net
{
public:
    string netname;
    vector<string> pinnames;
    double delay;
    void update_delay(double new_delay);

    net(string new_netname) : netname(new_netname) {}
};

class data
{
public:
    vector<net *> netlist;
    void add_net(net *new_net);
    void readline();
    void sort_by_delay();
    void insert_new_pins(string netname_new, string new_pin1, string new_pin2);
    bool find_net(string netname);
    void print_netlist();
};

void net::update_delay(double new_delay)
{
    if (new_delay > this->delay)
        this->delay = new_delay;
}

void data::add_net(net *new_net)
{

    this->netlist.push_back(new_net);
}

bool data::find_net(string netname_new)
{
    for (size_t i = 0; i < this->netlist.size(); i++)
    {

        if (this->netlist[i]->netname == netname_new)
            return true;
    }
    return false;
}

void data::insert_new_pins(string netname_new, string new_pin1, string new_pin2)
{
    for (size_t i = 0; i < this->netlist.size(); i++)
    {
        net *temp_net = this->netlist[i];
        if (temp_net->netname == netname_new)
        {
            bool flag_same_pin1 = false, flag_same_pin2 = false;
            for (size_t j = 0; j < temp_net->pinnames.size(); j++)
            {
                if (new_pin1 == temp_net->pinnames[j])
                    flag_same_pin1 = true;
                if (new_pin2 == temp_net->pinnames[j])
                    flag_same_pin2 = true;
            }
            if (!flag_same_pin1)
                temp_net->pinnames.push_back(new_pin1);
            if (!flag_same_pin2)
                temp_net->pinnames.push_back(new_pin2);
        }
    }
}

void data::sort_by_delay()
{
    sort(this->netlist.begin(), this->netlist.end(), [](net *const &lhs, net *const &rhs)
         { return lhs->delay > rhs->delay; });
}

void data::print_netlist()
{
    for (size_t i = 0; i < this->netlist.size(); i++)
    {
        net *temp_net = this->netlist[i];
        cout << temp_net->netname << " ";
        if (temp_net->pinnames.size() == 3)
            cout << " *****  3 *****  " << endl;
        for (size_t j = 0; j < temp_net->pinnames.size(); j++)
        {
            cout << temp_net->pinnames[j] << " ";
        }
        cout << temp_net->delay << endl;
    }
}