#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

namespace SDC
{
class Signal;
class Net
{
private:
    string netname;
    string output_chipletname;
    vector<string> pinnames;
    // double input_delay_max_fall;
    // double input_delay_min_fall;
    // double input_delay_max_rise;
    // double input_delay_min_rise;

    double output_delay_max_fall;
    double output_delay_min_fall;
    double output_delay_max_rise;
    double output_delay_min_rise;

    double wire_budget;

public:
    vector<Signal *> signals;
    Net(string Netname, double clock_period);
    //double get_input_delay_avgOf4() { return (this->input_delay_max_fall + this->input_delay_min_fall + this->input_delay_max_rise + this->input_delay_min_rise) / 4.0; }

    double get_output_delay_avgOf4() { return (this->output_delay_max_fall + this->output_delay_min_fall + this->output_delay_max_rise + this->output_delay_min_rise) / 4.0; }
    double get_avg_input_delay_avgOf4();
    double get_wire_budget() { return this->wire_budget; }

    void set_output_chipletname(string _output_chipletname) { this->output_chipletname = _output_chipletname; }
    string get_output_chipletname() { return this->output_chipletname; }

    // void set_input_delaytime_max_fall(double delaytime) { this->input_delay_max_fall = delaytime; }
    // void set_input_delaytime_min_fall(double delaytime) { this->input_delay_min_fall = delaytime; }
    // void set_input_delaytime_max_rise(double delaytime) { this->input_delay_max_rise = delaytime; }
    // void set_input_delaytime_min_rise(double delaytime) { this->input_delay_min_rise = delaytime; }

    void set_output_delaytime_max_fall(double delaytime) { this->output_delay_max_fall = delaytime; }
    void set_output_delaytime_min_fall(double delaytime) { this->output_delay_min_fall = delaytime; }
    void set_output_delaytime_max_rise(double delaytime) { this->output_delay_max_rise = delaytime; }
    void set_output_delaytime_min_rise(double delaytime) { this->output_delay_min_rise = delaytime; }

    // double get_input_delaytime_max_fall() { return this->input_delay_max_fall; }
    // double get_input_delaytime_min_fall() { return this->input_delay_min_fall; }
    // double get_input_delaytime_max_rise() { return this->input_delay_max_rise; }
    // double get_input_delaytime_min_rise() { return this->input_delay_min_rise; }

    double get_output_delaytime_max_fall() { return this->output_delay_max_fall; }
    double get_output_delaytime_min_fall() { return this->output_delay_min_fall; }
    double get_output_delaytime_max_rise() { return this->output_delay_max_rise; }
    double get_output_delaytime_min_rise() { return this->output_delay_min_rise; }

    void set_wire_budget(double clock_time);

    string get_netname();

    Signal *create_signal(string input_chipletname);
    Signal *is_signal_exist(string input_chipletname);
};

class Signal
{
private:
    double input_delay_max_fall;
    double input_delay_min_fall;
    double input_delay_max_rise;
    double input_delay_min_rise;

    // double output_delay_max_fall;
    // double output_delay_min_fall;
    // double output_delay_max_rise;
    // double output_delay_min_rise;

public:
    Net *parent_net;

    string input_chipletname;
    //string output_chipletname;

    Signal(string chiplet_in, Net *_parent_net) : input_delay_max_fall(0), input_delay_min_fall(0), input_delay_max_rise(0), input_delay_min_rise(0),
                                                  parent_net(_parent_net), input_chipletname(chiplet_in) {}

    //void set_input_chip(string inout_chipname) {this->input_chipletname = }

    void set_input_delaytime_max_fall(double delaytime) { this->input_delay_max_fall = delaytime; }
    void set_input_delaytime_min_fall(double delaytime) { this->input_delay_min_fall = delaytime; }
    void set_input_delaytime_max_rise(double delaytime) { this->input_delay_max_rise = delaytime; }
    void set_input_delaytime_min_rise(double delaytime) { this->input_delay_min_rise = delaytime; }

    // void set_output_delaytime_max_fall(double delaytime) { this->output_delay_max_fall = delaytime; }
    // void set_output_delaytime_min_fall(double delaytime) { this->output_delay_min_fall = delaytime; }
    // void set_output_delaytime_max_rise(double delaytime) { this->output_delay_max_rise = delaytime; }
    // void set_output_delaytime_min_rise(double delaytime) { this->output_delay_min_rise = delaytime; }

    double get_input_delaytime_max_fall() { return this->input_delay_max_fall; }
    double get_input_delaytime_min_fall() { return this->input_delay_min_fall; }
    double get_input_delaytime_max_rise() { return this->input_delay_max_rise; }
    double get_input_delaytime_min_rise() { return this->input_delay_min_rise; }

    double get_output_delaytime_max_fall() { return this->parent_net->get_output_delaytime_max_fall(); }
    double get_output_delaytime_min_fall() { return this->parent_net->get_output_delaytime_min_fall(); }
    double get_output_delaytime_max_rise() { return this->parent_net->get_output_delaytime_max_rise(); }
    double get_output_delaytime_min_rise() { return this->parent_net->get_output_delaytime_min_rise(); }
    // double get_output_delaytime_max_fall() { return this->output_delay_max_fall; }
    // double get_output_delaytime_min_fall() { return this->output_delay_min_fall; }
    // double get_output_delaytime_max_rise() { return this->output_delay_max_rise; }
    // double get_output_delaytime_min_rise() { return this->output_delay_min_rise; }

    double get_input_delay_avgOf4() { return (this->input_delay_max_fall + this->input_delay_min_fall + this->input_delay_max_rise + this->input_delay_min_rise) / 4.0; }
    double get_output_delay_avgOf4() { return (this->parent_net->get_output_delaytime_max_fall() + this->parent_net->get_output_delaytime_min_fall() +
                                               this->parent_net->get_output_delaytime_max_rise() + this->parent_net->get_output_delaytime_min_rise()) /
                                              4.0; }
};

class Chiplet
{
public:
    string chiplet_name;
    string filename;
    vector<Net *> nets;
    Chiplet(string _chiplet_name, string _filename) : chiplet_name(_chiplet_name), filename(_filename){};
};

void parse(string filename, vector<Net *> &vec_net, double &clock_period);
Net *is_netname_exists(vector<Net *> vec_net, string new_netname);

bool is_contains_clock(vector<string> vec_str);
bool is_contains_fall(vector<string> vec_str);
bool is_contains_rise(vector<string> vec_str);
bool is_contains_max(vector<string> vec_str);
bool is_contains_min(vector<string> vec_str);

double get_delay_time(vector<string> vec_str);
double get_clock_time(vector<string> vec_str);

string get_netname_in_strs(vector<string> vec_str);

//collect the nets in each chiplets and union/delete those duplicate nets
void output_csv(vector<string> sdc_filenames, vector<Net *> vec_net);
void sort_netlist_by_delay(vector<Net *> &vec_net);
void sort_netlist_by_ascending_wire_budget(vector<Net *> &vec_net, double clock_period);
void output_netlist_with_delay(vector<string> sdc_filenames, vector<Net *> vec_net, double clock_period);

void set_all_wire_budget(vector<Net *> &vec_net, double clock_period);
}