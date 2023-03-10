

# Default target, set to listing all receipes.
.default: list


# Ignores targets starting with anything but alphanum characters.
# Ignores the 'list' target itself
list:
	@LC_ALL=C $(MAKE) -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$'



run: build
	chmod +x run.sh && ./run.sh


rebuild: clean build


build: 
	@echo "---- Building the workspace ----"
	cd sdc_parser/src && make
	cd main/src/ && make
	cd Min_cost_solver/Min-Cost-Network-Flow-Lemon/lemon/ && cmake . && cd .. && make CXXFLAGS='-O3 -Wall -std=c++11 -I.'


clean:
	@echo "---- Cleaning the workspace ----"
	cd sdc_parser/src && make clean -i
	cd main/src/ && make clean -i
	cd Min_cost_solver/Min-Cost-Network-Flow-Lemon/ && make clean -i

veryclean:
	@echo "---- Cleaning the workspace & all output files ----"
	cd sdc_parser/src && make veryclean -i
	cd main/src/ && make veryclean -i
	cd Min_cost_solver/Min-Cost-Network-Flow-Lemon/ && make veryclean -i
