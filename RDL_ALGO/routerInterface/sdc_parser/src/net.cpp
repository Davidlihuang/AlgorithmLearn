#include "net.h"
#define Max_double 999999.0
namespace SDC
{
Net::Net(string Netname, double clock_period) : netname(Netname), output_delay_max_fall(clock_period),
                                                output_delay_min_fall(clock_period), output_delay_max_rise(clock_period), output_delay_min_rise(clock_period)
{
    if (Netname == "NRST" || Netname == "SWCLKTCK" || Netname == "SWDIOTMS")
    {
        this->set_output_delaytime_max_fall(Max_double);
        this->set_output_delaytime_max_rise(Max_double);
        this->set_output_delaytime_min_fall(Max_double);
        this->set_output_delaytime_min_rise(Max_double);
    }
}
// Net::Net(string Netname) : netname(Netname), input_delay_max_fall(0), input_delay_min_fall(0),
//                            input_delay_max_rise(0), input_delay_min_rise(0), output_delay_max_fall(0),
//                            output_delay_min_fall(0), output_delay_max_rise(0), output_delay_min_rise(0)
// {
// }

string Net::get_netname()
{
    return this->netname;
}

Signal *Net::create_signal(string chipname)
{
    Signal *new_signal = new Signal(chipname, this);
    this->signals.push_back(new_signal);
    return new_signal;
}

Net *is_netname_exists(vector<Net *> vec_net, string new_netname)
{
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        string temp_netname = vec_net[i]->get_netname();
        if (temp_netname == new_netname)
        {
            return vec_net[i];
        }
    }
    return nullptr;
}

bool is_contains_clock(vector<string> vec_str)
{
    return any_of(vec_str.begin(), vec_str.end(), [](string i)
                  { return i == "-clock"; });
}

bool is_contains_fall(vector<string> vec_str)
{
    return any_of(vec_str.begin(), vec_str.end(), [](string i)
                  { return i == "-fall"; });
}

bool is_contains_rise(vector<string> vec_str)
{
    return any_of(vec_str.begin(), vec_str.end(), [](string i)
                  { return i == "-rise"; });
}

bool is_contains_max(vector<string> vec_str)
{
    return any_of(vec_str.begin(), vec_str.end(), [](string i)
                  { return i == "-max"; });
}

bool is_contains_min(vector<string> vec_str)
{
    return any_of(vec_str.begin(), vec_str.end(), [](string i)
                  { return i == "-min"; });
}

void parse(string filename, vector<Net *> &vec_net, double &clock_period)
{
    cout << "[file input]" << filename << endl;
    string chiplet_name = filename;
    chiplet_name.erase(chiplet_name.begin(), chiplet_name.begin() + 9);
    chiplet_name.erase(chiplet_name.end() - 4, chiplet_name.end());

    ifstream infile(filename);
    if (!infile.is_open())
    {
        cout << "error: cannot open file " << filename << endl;
        return;
    }

    string str;
    string line;

    //string string_set_input_delay = "set_input_delay";
    //int size_str = string_set_input_delay.size();
    //cout << size_str << endl;

    while (getline(infile, line))
    {
        stringstream ss(line);
        vector<string> strs;
        Net *temp_net = nullptr;

        //collecting strings from a line
        while (getline(ss, str, ' '))
        {
            strs.push_back(str);

            //cout << str << endl;
        }
        //end of string collection

        if (strs[0] == "set_input_delay")
        {
            //cout << "set_intput_delay" << endl;
            string netname = get_netname_in_strs(strs);
            Net *is_net_exists = is_netname_exists(vec_net, netname);

            //assign Net pointer
            if (is_net_exists)
            {
                //cout << "is netname: " << netname << endl;
                temp_net = is_net_exists;
            }
            else
            {
                //cout << "not netname: " << netname << endl;
                Net *new_net = new Net(netname, clock_period);
                temp_net = new_net;
                vec_net.push_back(new_net);
            }

            if (is_contains_clock(strs))
            {
                Signal *temp_signal = temp_net->is_signal_exist(chiplet_name); //if signal already exists, assign delay time
                if (temp_signal)
                {
                    if (is_contains_max(strs) && is_contains_fall(strs))
                        temp_signal->set_input_delaytime_max_fall(get_delay_time(strs));
                    else if (is_contains_min(strs) && is_contains_fall(strs))
                        temp_signal->set_input_delaytime_min_fall(get_delay_time(strs));
                    else if (is_contains_max(strs) && is_contains_rise(strs))
                        temp_signal->set_input_delaytime_max_rise(get_delay_time(strs));
                    else if (is_contains_min(strs) && is_contains_rise(strs))
                        temp_signal->set_input_delaytime_min_rise(get_delay_time(strs));
                    else if (is_contains_fall(strs))
                    {
                        temp_signal->set_input_delaytime_max_fall(get_delay_time(strs));
                        temp_signal->set_input_delaytime_min_fall(get_delay_time(strs));
                    }
                    else if (is_contains_rise(strs))
                    {
                        temp_signal->set_input_delaytime_max_rise(get_delay_time(strs));
                        temp_signal->set_input_delaytime_min_rise(get_delay_time(strs));
                    }
                    else if (is_contains_max(strs))
                    {
                        temp_signal->set_input_delaytime_max_fall(get_delay_time(strs));
                        temp_signal->set_input_delaytime_max_rise(get_delay_time(strs));
                    }
                    else if (is_contains_min(strs))
                    {
                        temp_signal->set_input_delaytime_min_fall(get_delay_time(strs));
                        temp_signal->set_input_delaytime_min_rise(get_delay_time(strs));
                    }
                    else
                        cout << "error: this line of strings doesn't have feasible parameters for delay time" << endl;
                }
                //if signal doens't exist, create one
                else
                {
                    temp_signal = temp_net->create_signal(chiplet_name);

                    if (is_contains_max(strs) && is_contains_fall(strs))
                        temp_signal->set_input_delaytime_max_fall(get_delay_time(strs));
                    else if (is_contains_min(strs) && is_contains_fall(strs))
                        temp_signal->set_input_delaytime_min_fall(get_delay_time(strs));
                    else if (is_contains_max(strs) && is_contains_rise(strs))
                        temp_signal->set_input_delaytime_max_rise(get_delay_time(strs));
                    else if (is_contains_min(strs) && is_contains_rise(strs))
                        temp_signal->set_input_delaytime_min_rise(get_delay_time(strs));
                    else if (is_contains_fall(strs))
                    {
                        temp_signal->set_input_delaytime_max_fall(get_delay_time(strs));
                        temp_signal->set_input_delaytime_min_fall(get_delay_time(strs));
                    }
                    else if (is_contains_rise(strs))
                    {
                        temp_signal->set_input_delaytime_max_rise(get_delay_time(strs));
                        temp_signal->set_input_delaytime_min_rise(get_delay_time(strs));
                    }
                    else if (is_contains_max(strs))
                    {
                        temp_signal->set_input_delaytime_max_fall(get_delay_time(strs));
                        temp_signal->set_input_delaytime_max_rise(get_delay_time(strs));
                    }
                    else if (is_contains_min(strs))
                    {
                        temp_signal->set_input_delaytime_min_fall(get_delay_time(strs));
                        temp_signal->set_input_delaytime_min_rise(get_delay_time(strs));
                    }
                    else
                        cout << "error: this line of strings doesn't have feasible parameters for delay time" << endl;
                }
                // if (is_contains_max(strs) && is_contains_fall(strs))
                //     temp_net->create_signal(get_delay_time(strs));
                // else if (is_contains_min(strs) && is_contains_fall(strs))
                //     temp_net->set_input_delaytime_min_fall(get_delay_time(strs));
                // else if (is_contains_max(strs) && is_contains_rise(strs))
                //     temp_net->set_input_delaytime_max_rise(get_delay_time(strs));
                // else if (is_contains_min(strs) && is_contains_rise(strs))
                //     temp_net->set_input_delaytime_min_rise(get_delay_time(strs));
                // else
                //     cout << "error: this line of strings doesn't have feasible parameters for delay time" << endl;
            }
        }

        else if (strs[0] == "set_output_delay")
        {
            //cout << "set_output_delay" << endl;

            string netname = get_netname_in_strs(strs);
            Net *is_net_exists = is_netname_exists(vec_net, netname);

            //assign Net pointer
            if (is_net_exists)
            {
                //cout << "is netname: " << netname << endl;
                temp_net = is_net_exists;
            }
            else
            {
                //cout << "not netname: " << netname << endl;
                Net *new_net = new Net(netname, clock_period);
                temp_net = new_net;
                vec_net.push_back(new_net);
            }

            if (temp_net->get_output_chipletname().empty())
            {
                temp_net->set_output_chipletname(chiplet_name);
                if (is_contains_clock(strs))
                {
                    if (is_contains_max(strs) && is_contains_fall(strs))
                        temp_net->set_output_delaytime_max_fall(get_delay_time(strs));
                    else if (is_contains_min(strs) && is_contains_fall(strs))
                        temp_net->set_output_delaytime_min_fall(get_delay_time(strs));
                    else if (is_contains_max(strs) && is_contains_rise(strs))
                        temp_net->set_output_delaytime_max_rise(get_delay_time(strs));
                    else if (is_contains_min(strs) && is_contains_rise(strs))
                        temp_net->set_output_delaytime_min_rise(get_delay_time(strs));
                    else
                        cout << "error: this line of strings doesn't have feasible parameters for delay time" << endl;
                }
            }
            else
            {
                if (is_contains_clock(strs))
                {
                    if (is_contains_max(strs) && is_contains_fall(strs))
                        temp_net->set_output_delaytime_max_fall(get_delay_time(strs));
                    else if (is_contains_min(strs) && is_contains_fall(strs))
                        temp_net->set_output_delaytime_min_fall(get_delay_time(strs));
                    else if (is_contains_max(strs) && is_contains_rise(strs))
                        temp_net->set_output_delaytime_max_rise(get_delay_time(strs));
                    else if (is_contains_min(strs) && is_contains_rise(strs))
                        temp_net->set_output_delaytime_min_rise(get_delay_time(strs));
                    else
                        cout << "error: this line of strings doesn't have feasible parameters for delay time" << endl;
                }
            }

            // if (is_contains_clock(strs))
            // {
            //     Signal *temp_signal = temp_net->is_signal_exist(chiplet_name);
            //     //if signal already exists, assign output delay time to net
            //     if (temp_signal)
            //     {
            //         if (is_contains_max(strs) && is_contains_fall(strs))
            //             temp_signal->set_input_delaytime_max_fall(get_delay_time(strs));
            //         else if (is_contains_min(strs) && is_contains_fall(strs))
            //             temp_signal->set_input_delaytime_min_fall(get_delay_time(strs));
            //         else if (is_contains_max(strs) && is_contains_rise(strs))
            //             temp_signal->set_input_delaytime_max_rise(get_delay_time(strs));
            //         else if (is_contains_min(strs) && is_contains_rise(strs))
            //             temp_signal->set_input_delaytime_min_rise(get_delay_time(strs));
            //         else
            //             cout << "error: this line of strings doesn't have feasible parameters for delay time" << endl;
            //     }
            //     //if signal doens't exist, create one
            //     else
            //     {
            //         temp_signal = temp_net->create_signal(chiplet_name);

            //         if (is_contains_max(strs) && is_contains_fall(strs))
            //             temp_signal->set_input_delaytime_max_fall(get_delay_time(strs));
            //         else if (is_contains_min(strs) && is_contains_fall(strs))
            //             temp_signal->set_input_delaytime_min_fall(get_delay_time(strs));
            //         else if (is_contains_max(strs) && is_contains_rise(strs))
            //             temp_signal->set_input_delaytime_max_rise(get_delay_time(strs));
            //         else if (is_contains_min(strs) && is_contains_rise(strs))
            //             temp_signal->set_input_delaytime_min_rise(get_delay_time(strs));
            //         else
            //             cout << "error: this line of strings doesn't have feasible parameters for delay time" << endl;
            //     }
            // }
        }

        else if (strs[0] == "set_case_analysis")
        {
            // string netname = get_netname_in_strs(strs);

            // if (is_netname_exists(vec_net, netname))
            // {
            //     //cout << "is netname: " << netname << endl;
            //     temp_net = is_netname_exists(vec_net, netname);
            // }
            // else
            // {
            //     //cout << "not netname: " << netname << endl;
            //     Net *new_net = new Net(netname, clock_period);
            //     temp_net = new_net;
            //     vec_net.push_back(new_net);
            //     //temp_net = new_net;
            // }

            // if (chiplet_name == "u_core_context")
            //     temp_net->set_output_chipletname(chiplet_name);
            // else
            // {
            //     temp_net->create_signal(chiplet_name);
            // }
        }

        else if (strs[0] == "create_clock")
        {
            string netname = get_netname_in_strs(strs);

            clock_period = get_clock_time(strs);
            //cout << "[clock time] " << clock_period << endl;
            if (is_netname_exists(vec_net, netname))
            {
                //cout << "is netname: " << netname << endl;
                temp_net = is_netname_exists(vec_net, netname);
            }
            else
            {
                //cout << "not netname: " << netname << endl;
                Net *new_net = new Net(netname, clock_period);
                temp_net = new_net;
                vec_net.push_back(new_net);
                // temp_net->set_input_delaytime_max_fall(-1);
                // temp_net->set_input_delaytime_min_fall(-1);
                // temp_net->set_input_delaytime_max_rise(-1);
                // temp_net->set_input_delaytime_min_rise(-1);
            }

            Signal *new_signal = temp_net->create_signal(chiplet_name);
            new_signal->set_input_delaytime_max_fall(-1);
            new_signal->set_input_delaytime_min_fall(-1);
            new_signal->set_input_delaytime_max_rise(-1);
            new_signal->set_input_delaytime_min_rise(-1);
        }
    }
}

Signal *Net::is_signal_exist(string input_chipletname)
{
    for (size_t i = 0; i < this->signals.size(); i++)
    {
        if (input_chipletname == this->signals[i]->input_chipletname)
        {
            return this->signals[i];
            break;
        }
    }
    return nullptr;
}

double get_delay_time(vector<string> vec_str)
{
    int idx_clock = -1;

    for (size_t i = 0; i < vec_str.size(); i++)
    {
        if (vec_str[i] == "-clock")
        {
            idx_clock = i;
            break;
        }
    }

    string str_delay_time = vec_str[idx_clock - 1];
    return stod(str_delay_time);
}

double get_clock_time(vector<string> vec_str)
{
    int idx_clock = -1;

    for (size_t i = 0; i < vec_str.size(); i++)
    {
        if (vec_str[i] == "-period")
        {
            idx_clock = i;
            break;
        }
    }

    string str_clock_time = vec_str[idx_clock + 1];
    return stod(str_clock_time);
}

string get_netname_in_strs(vector<string> vec_str)
{

    int idx_get_port = -1;
    for (size_t i = 0; i < vec_str.size(); i++)
    {
        if (vec_str[i] == "[get_ports")
        {
            idx_get_port = i;
            break;
        }
    }

    string netname = vec_str[idx_get_port + 1];
    netname.erase(netname.begin(), netname.begin() + 1);
    netname.erase(netname.end() - 2, netname.end());

    return netname;
}

void output_csv(vector<string> sdc_filenames, vector<Net *> vec_net)
{
    for (size_t i = 0; i < sdc_filenames.size(); i++)
    {
        string chiplet_name = sdc_filenames[i];
        chiplet_name.erase(chiplet_name.begin(), chiplet_name.begin() + 9);
        chiplet_name.erase(chiplet_name.end() - 4, chiplet_name.end());
        if (chiplet_name == "u_core_context")
            continue;

        string output_filename = "../output/" + chiplet_name + "-LPD.csv";
        cout << output_filename << endl;

        ofstream outfile(output_filename);
        if (!outfile.is_open())
        {
            cout << "error: " << output_filename << " cannot be opened!" << endl;
            break;
        }

        outfile << "Package-net,p1_name,p2_name,LPD (ns)\n";

        for (size_t j = 0; j < vec_net.size(); j++)
        {
            Net *temp_net = vec_net[j];
            Signal *temp_signal = temp_net->is_signal_exist(chiplet_name);
            if (temp_net->get_output_chipletname() == chiplet_name)
            {
                Signal *another_signal = temp_net->signals[0];
                string netname = temp_net->get_netname();
                outfile << netname << "," << chiplet_name << "/" << netname << ","
                        << temp_net->get_output_chipletname() << "/" << netname << "," << another_signal->get_input_delay_avgOf4() << "\n";
            }
            else if (temp_signal)
            {
                string netname = temp_net->get_netname();
                Signal *another_signal = nullptr;
                if (temp_net->get_output_chipletname().empty())
                {
                    for (size_t k = 0; k < temp_net->signals.size(); k++)
                    {
                        if (temp_net->signals[k] != temp_signal)
                            another_signal = temp_net->signals[k];
                    }
                    outfile << netname << "," << another_signal->input_chipletname << "/" << netname << "," << chiplet_name << "/" << netname << "," << temp_signal->get_input_delay_avgOf4() << "\n";
                }
                else
                {
                    outfile << netname << "," << temp_net->get_output_chipletname() << "/" << netname << "," << chiplet_name << "/" << netname << "," << temp_signal->get_input_delay_avgOf4() << "\n";
                }
            }
        }

        /* code */
    }
}

double Net::get_avg_input_delay_avgOf4()
{
    double sum_delay = 0;
    for (size_t i = 0; i < this->signals.size(); i++)
    {
        sum_delay += this->signals[i]->get_input_delay_avgOf4();
    }
    double divider = this->signals.size();

    if (this->signals.size() == 0)
        return 0;
    else
        return sum_delay / divider;
}

void sort_netlist_by_delay(vector<Net *> &vec_net)
{
    sort(vec_net.begin(), vec_net.end(), [](Net *const &lhs, Net *const &rhs)
         { return lhs->get_avg_input_delay_avgOf4() > rhs->get_avg_input_delay_avgOf4(); });
}

void sort_netlist_by_ascending_wire_budget(vector<Net *> &vec_net, double clock_period)
{
    sort(vec_net.begin(), vec_net.end(), [](Net *const &lhs, Net *const &rhs)
         { return lhs->get_wire_budget() < rhs->get_wire_budget(); });
}

void set_all_wire_budget(vector<Net *> &vec_net, double clock_period)
{
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        temp_net->set_wire_budget(clock_period);
    }
}

void Net::set_wire_budget(double clock_time)
{
    double wire_timing_budget = Max_double;
    if (this->get_avg_input_delay_avgOf4() < 0)
        wire_timing_budget = -Max_double;
    else
        wire_timing_budget = clock_time - ((clock_time - this->get_avg_input_delay_avgOf4()) + (clock_time - this->get_output_delay_avgOf4()));
    this->wire_budget = wire_timing_budget;
}

void output_netlist_with_delay(vector<string> sdc_filenames, vector<Net *> vec_net, double clock_period)
{
    //sort_netlist_by_delay(vec_net);

    //string output_filename = "../output/sdc_netlist_with_descending_delay_inputdelay.out";
    //string output_filename_outputdelay = "../output/sdc_netlist_with_descending_delay_outputdelay.out";
    //string output_filename_wirebudget = "../output/sdc_netlist_with_ascending_delay_wirebudget.out";
    string output_filename_wirebudget = sdc_filenames[0];

    cout << "[file output] " << output_filename_wirebudget << endl;

    //ofstream outfile(output_filename);
    //ofstream outfile_od(output_filename_outputdelay);
    ofstream outfile_wb(output_filename_wirebudget);
    if (!outfile_wb.is_open())
    {
        cout << "error: " << output_filename_wirebudget << " cannot be opened!" << endl;
        return;
    }

    for (size_t i = 0; i < vec_net.size(); i++)
    {
        //outfile << vec_net[i]->get_netname() << " " << vec_net[i]->get_avg_input_delay_avgOf4() << endl;
        //outfile_od << vec_net[i]->get_netname() << " " << vec_net[i]->get_output_delay_avgOf4() << endl;
        outfile_wb << vec_net[i]->get_netname() << " " << vec_net[i]->get_wire_budget() << endl;
    }
}
}