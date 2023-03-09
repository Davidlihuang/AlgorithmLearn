#Input Files
Input_JSON=../input/design_data_cLoc.json
Input_SDC_0=../input/part_main.sdc 
Input_SDC_1=../input/part_mem1.sdc 
Input_SDC_2=../input/part_mem2.sdc

# Input_JSON=../input/design_data_cLoc.json
# Input_SDC_0=../input/u_core_context.sdc 
# Input_SDC_1=../input/u_mem_context1.sdc 
# Input_SDC_2=../input/u_mem_context2.sdc

#Directories
Dir_sdc_parser=sdc_parser/src
Dir_main=main/src
Dir_MCMF_solver=Min_cost_solver/Min-Cost-Network-Flow-Lemon
Dir_python=python

echo "----------[Parse SDC files]----------"
echo "directory: /$Dir_sdc_parser/"
cd $Dir_sdc_parser
../bin/main $Input_SDC_0 $Input_SDC_1 $Input_SDC_2
echo "----------[Node/Edge Generator for Network Flow Solver]----------"
echo "directory: /$Dir_main/" 
cd ../../$Dir_main
../bin/main $Input_JSON -vc 2
echo "----------[MCMF Network Flow Solver]----------"
echo "directory: /$Dir_MCMF_solver/"
cd ../../$Dir_MCMF_solver
./MCMF
echo "----------[Read Solution and Compact Solution]----------"
echo "directory: /$Dir_main/" 
cd ../../$Dir_main
../bin/main $Input_JSON -rs -it 10 -vc 2
echo "----------[Draw Layout]----------"
echo "directory: /$Dir_python/"
cd ../../$Dir_python
python3 python.py
echo "----------[END OF EXECUTION]----------"