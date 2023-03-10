1. How to run 
```
$ bash ./run.sh
```

2. SDC files:
All SDC files are stored in the directory "./sdc_parser/input". If you changed the file names of SDC files, you may open the "./run.sh" mentioned in 1. to edit the file path of SDC files.

3. Where the output JSON file with routing solution locates:
	./main/output/design_data_cLoc-output.json

4. The binary files are built via gcc version 7.5.0. 

5. You may edit the Makefile(s) to change the environment and rebuild the binary files.

Four Makefile(s) are located in the following directory:
	./main/src/Makefile
	./Min_cost_solver/Min-Cost-Network-Flow-Lemon/Makefile
	./pin-connect/src/Makefile
	./sdc_parser/src/Makefile
	
2021/09/16
6. Default via cost (-vc command) has been changed to 2

7. Fatal errors are fixed to prevent the program from generating wrong routing results and from encountering Segmentation fault. 
8. Delete unnecessary signals: XTAL2, $_Logic_$

2021/09/18
9. Bug fix for wrong IO pad locations in JSON
10. Shell script run.sh uses variable format for input files
11. "veryclean" command is adde in Makefiles and top-level makefiled. It removes all output files along with the executable binary files.
12. All significant output files are moved to the directory: output/

2021/09/19
13. Bug fix for the wrong equation of wire_budget

2021/09/20
14. Bug fix for the wrong equation of wire_budget

2021/09/22
15. Bug fix for wire-overlap violation
16. Bug fix for mismatching signal names between pins and wires (CSV vs. JSON)
17. Signal XTAL2 is added back to JSON with the lowest priority of signal assignment
18. Layout figures have separate images for RDL1, 2 and 3 respectively

2021/10/07
19. Bug fix for opposite timing budget sorting order of net in ./main
20. Bug fix for wrong timing budget for I/O signals in ./sdc_parser

2021/10/08_2
21. Default vc(via cost) is set to 2 in ./main/src/main.cpp
22. Filter out signal names which has *Logic* string in ./sdc_parser/src/net.cpp
23. wire budget filename changed to ./sdc_parser/output/sdc_netlist_with_ascending_delay_wirebudget.out
24. rm binary file function in Makefile in "./Min_cost_solver/Min-Cost-Network-Flow-Lemon" has been cancelled.

2021/10/08_3
25. Fix maze pin selection mechanism for 3-pin nets, which were using undesired pin locations and produces congestion for later 2-pin net routing by MCMF solver in ./main/src/main.cpp, parse.cpp, maze.cpp
26. 3-pin nets are given higher order for pin selection along with up to five 2-pin nets
27. Signal XTAL2 deleted from the input JSON file in ./main/input/
28. Fix the wrong ordering by ascending wire budget, which caused improper signal assignment.
