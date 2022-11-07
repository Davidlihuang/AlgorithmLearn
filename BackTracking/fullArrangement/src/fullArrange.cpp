/**
 * @file fullArrange.cpp
 * @author your name (you@domain.com)
 * @brief  n[1,2,3] full arrangement. O(N!)
 * @version 0.1
 * @date 2022-11-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> result;


void backtrace(const vector<int>& numList, vector<int>& path)
{
    ///< 当前解path填满，结速
    if(path.size() == numList.size())
    {
       cout << "find result" << endl;
       result.push_back(path);
       return;
    }

    
    for(int i=0; i< numList.size(); i++)
    {
        ///< 做选择
        //剪枝
        if(std::find(path.begin(), path.end(), numList[i]) != path.end())
        {
            continue;
        }

        ///< 添加路径
        path.push_back(numList[i]);
        
        ///< 进入下一层决策树
        backtrace(numList, path);

        ///< 恢复现场
        path.pop_back();
    }

}

vector<vector<int>> fullArrangement(vector<int>& numList)
{
    vector<int> path;
    backtrace(numList, path);
    return result;

}

int main()
{
    vector<int> nums{1,2,3};
    vector<vector<int>> res;
    res = fullArrangement(nums);
    cout << "result = "<< endl;
    for(int i =0; i< res.size(); i++)
    {
        cout << "[ ";
        for(int j  = 0; j< res[i].size(); j++)
        {
            cout << res[i][j] << ", ";
        }
        cout << " ]" << endl;
    }
    return 0;
}
