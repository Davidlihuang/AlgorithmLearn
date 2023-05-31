#pragma once
#include <iostream>

class DisjointSet
{
private:
    /* data */
    int m_size;
    int *m_set;
    int *m_rank;
    DisjointSet(){};

public:
    explicit DisjointSet(int size);
    int  findRoot(int v);
    bool unionTree(int s, int e);
    ~DisjointSet();

};

DisjointSet::DisjointSet(int size)
{
    m_size = size;
    m_set = nullptr;
    m_rank = nullptr;
    m_set = new int[m_size];
    m_rank = new int[m_size];
    
    //初始化每个节点的父亲是自己
    for(int i=0; i< m_size; i++)
    {
        m_set[i] = i;
        m_rank[i] = 1;
    }
}

DisjointSet::~DisjointSet()
{
    if(m_set != nullptr)
    {
        delete[]m_set;

        m_set = nullptr;
    }

    if(m_rank != nullptr)
    {
        delete[]m_rank;
        m_rank = nullptr;
    }
}
/**
 * @brief 寻找结点v的根节点，并压缩v结点的中间父亲节点，v与root直连，减少查找次数
 *        两种实现方式： 1. 递归 2. 迭代 
 * @param v     待查找结点
 * @return int  结点v的root 
 */
int  DisjointSet::findRoot(int v)
{
    if(m_set == nullptr ) return -1;

    //递归解法；
    if(v != m_set[v])
    {
        m_set[v] = this->findRoot(m_set[v]); // 寻找祖先，并压缩路径
    }
    return m_set[v];

    // //迭代解法；
    // int root = v;
    // while(root != m_set[root])
    // {
    //     root = m_set[root];
    // }

    // //沿途非祖先节点的父亲们直接变为祖先节点
    // int tmp;
    // while(v!= root)
    // {
    //     tmp = m_set[v];
    //     m_set[v] = root;
    //     v = tmp;
    // }
    // return v;
}

/**
 * @brief 根据两颗树是否要合并，确定两个定点是否在同一个集合中
 * 将深度较小的树合并到深度较大的树，并增加作为祖先节点树的深度。
 *         
 * @param s 第一结点
 * @param e 第二结点
 * @return true  两个节点不在同一个集合中，合并
 * @return false 两个节点在同一个集合中，需要合并
 */
bool DisjointSet::unionTree(int s, int e)
{
    int sRoot = this->findRoot(s);
    int eRoot = this->findRoot(e);
    if(sRoot != eRoot)
    {
        if(m_rank[sRoot] < m_rank[eRoot])
        {
            m_set[sRoot] = eRoot;
            m_rank[eRoot] += m_rank[sRoot];
        }   
        else
        {
            m_set[eRoot] = sRoot;
            m_rank[sRoot] += m_rank[eRoot];
        } 
        return true;
    }
    return false;
}
