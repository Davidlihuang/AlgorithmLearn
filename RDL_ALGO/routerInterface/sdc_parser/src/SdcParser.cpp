#include "SdcParser.h"

void SdcParser::run(const string &strInputSdc0, const string &strInputSdc1, const string &strInputSdc2, const string &strOutPutFile)
{
    vecInput.push_back(strInputSdc0);
    vecInput.push_back(strInputSdc1);
    vecInput.push_back(strInputSdc2);

    for (unsigned int i = 0; i < vecInput.size(); i++)
    {
        sdc_filenames.push_back(vecInput[i]);
        string chiplet_name = vecInput[i];
        chiplet_name.erase(chiplet_name.begin(), chiplet_name.begin() + 9);
        chiplet_name.erase(chiplet_name.end() - 4, chiplet_name.end());
        SDC::Chiplet *new_chiplet = new SDC::Chiplet(chiplet_name, vecInput[i]);
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

    set_all_wire_budget(nets, clock_period);
    sort_netlist_by_ascending_wire_budget(nets, clock_period);
    vector<string> vecOutPutFile;
    vecOutPutFile.push_back(strOutPutFile);
    output_netlist_with_delay(vecOutPutFile, nets, clock_period);
}