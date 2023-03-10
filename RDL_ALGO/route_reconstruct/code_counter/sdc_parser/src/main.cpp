#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>

#include "net.h"

using namespace std;

vector<Net *> nets;
vector<Chiplet *> chiplets;
vector<string> sdc_filenames;
double clock_period;

int main(int argc, char *argv[])
{

    for (int i = 1; i < argc; i++)
    {
        sdc_filenames.push_back(argv[i]);
        string chiplet_name = argv[i];
        chiplet_name.erase(chiplet_name.begin(), chiplet_name.begin() + 9);
        chiplet_name.erase(chiplet_name.end() - 4, chiplet_name.end());
        Chiplet *new_chiplet = new Chiplet(chiplet_name, argv[i]);
        chiplets.push_back(new_chiplet);
    }

    for (size_t i = 0; i < sdc_filenames.size(); i++)
    {
        string chiplet_name = sdc_filenames[i];
        chiplet_name.erase(chiplet_name.begin(), chiplet_name.begin() + 9);
        chiplet_name.erase(chiplet_name.end() - 4, chiplet_name.end());

        parse(sdc_filenames[i], nets, clock_period);
        //cout << "num_net: " << chiplets[i]->nets.size() << endl;
    }

    // cout << "[num net] " << nets.size() << endl;
    // for (size_t i = 0; i < nets.size(); i++)
    // {
    //     cout << "net#" << i << " ";
    //     if (nets[i]->get_output_chipletname().empty())
    //         cout << "IO"
    //              << "/" << nets[i]->get_netname() << ": " << nets[i]->get_output_delay_avgOf4() << endl;
    //     else
    //         cout << nets[i]->get_output_chipletname() << "/" << nets[i]->get_netname() << ": " << nets[i]->get_output_delay_avgOf4() << endl;
    //     for (size_t j = 0; j < nets[i]->signals.size(); j++)
    //     {
    //         cout << "Signal" << i << " " << nets[i]->signals[j]->input_chipletname << "/" << nets[i]->signals[j]->parent_net->get_netname() << ": " << nets[i]->signals[j]->get_input_delay_avgOf4();
    //         cout << endl;
    //     }
    //     cout << endl;
    // }

    //output_csv(sdc_filenames, nets);

    set_all_wire_budget(nets, clock_period);
    sort_netlist_by_ascending_wire_budget(nets, clock_period);
    output_netlist_with_delay(sdc_filenames, nets, clock_period);
    return 0;
}