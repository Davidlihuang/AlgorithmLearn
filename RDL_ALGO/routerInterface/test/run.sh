rm -rf input
cp -r input_copy input
rm -rf output
mkdir output
cd release
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/huangli/BTDWork/RDL_ALGO/solver20211008_3/test/lib
./Test
cd ..
#python python.py
