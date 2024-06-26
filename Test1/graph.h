#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include<bits/stdc++.h>
using namespace std;

// 定义用邻接表表示的无权有向图

struct Edge
{
    char weight; // 存储该边的吸收字符
    Edge *link; // 边
    int nextVertex; // 下一顶点编号
    Edge() {}
    Edge(char c, Edge *p, int num):
        weight(c), link(p),nextVertex(num) {}
};

struct Vertex1
{
    int data;
    int final = 0;
    Edge *adj;
    bool end = 0;
};

class Graph
{
private:
    int maxVertices; // 最大顶点
    int numEdges; // 边的编号
public:
        Vertex1 *NodeTable;
        int numVertices; // 当前节点数

        // Ctor
        Graph()
        {
            maxVertices = 100;
            numVertices = 0;
            numEdges = 0;
            NodeTable= new Vertex1[maxVertices];
            for(int i=0; i< maxVertices; i++)
            {
               NodeTable[i].adj = 0;
            }
        }

        bool insertVertex(); // 插入点
        bool insertEdge(int v1,int v2,char c); // 插入边

        int getFirstNeighbor(int v,char c);
        int getNextNeighbor(int v, int w,char c);

        int getValue(int i); // 根据结点序号获取结点的值
        int getVertexPos(int Vertex); // 根据结点的值获取结点的序号

        int NumberOfVertices();
        int NumberOfEdges();

        //深度优先搜索
        void DFS(int v,char c,bool visited[]);
        void DFS(int v,char c);

        //广度优先遍历

};


#endif // GRAPH_H
