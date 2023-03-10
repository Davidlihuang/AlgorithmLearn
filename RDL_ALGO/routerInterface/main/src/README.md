HOW TO COMPILE
$ make clean
$ make


1. For 1st run with Maze Route and MCMF Network Flow Algorithm
$ [executable binary file] [input JSON file]

e.g.
```
$ ../bin/main ../input/design_data_cLoc.json 
```

2. For reading the soultion (-rs) from the Network flow solver and wire layout compaction (-it)

$ [executable binary file] [input JSON file] -rs -it [Number of iteration of compaction]

e.g.
```
$ ../bin/main ../input/design_data_cLoc.json -rs -it 6
```

-vc [cost of via]
(default is 10)

e.g.
```
$ ../bin/main ../input/design_data_cLoc.json -vc 5
```