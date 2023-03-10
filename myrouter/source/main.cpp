#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "dsnparser.h"
using namespace DSN;
void ss_reset(std::stringstream &ss, const std::string &s)
{
	ss.str(s);
	ss.clear();
}

int main(int argc, char *argv[])
{
	//process comand args
	auto use_file = false;
	std::ifstream arg_infile;
	auto arg_b = 1;
	std::string filename;
	std::stringstream ss;
	for (auto i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			//option
			std::string opt = argv[i];
			while (!opt.empty() && opt[0] == '-') opt.erase(0, 1);
			if (++i >= argc) goto help;
			ss_reset(ss, argv[i]);
			if (opt == "b") ss >> arg_b;
			else
			{
			help:
				std::cout << "c_pcb_dsn [switches] [filename]\neg. c_pcb_dsn -b 6 test1.dsn\n";
				std::cout << "reads from stdin if no filename.\n";
				std::cout << "-b: border gap, default 1\n";
				exit(0);
			}
		}
		else
		{
			//filename
			filename = argv[i];
			arg_infile.open(argv[i], std::ifstream::in);
			use_file = true;
		}
	}

	//reading from stdin or file
	std::istream &in = use_file ? arg_infile : std::cin;

	//create tree from input
	//auto tree = read_tree(in);
   
    DsnParser& dsn_parser = DsnParser::getInstance(filename);
	auto tree = dsn_parser.get_tree();
	dsn_parser.print_tree(tree, 4);
}