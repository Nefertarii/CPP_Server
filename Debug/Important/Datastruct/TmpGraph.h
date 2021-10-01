#ifndef GRAPH_H_
#define GRAPH_H

#include <vector>
#include <queue>
#include "Vertex.h"
#include "Edge.h"

/*
 * G=(V, E)
 * 图(Graph)=顶点集(Vertex)+边集(Edge)
 * 每条边为一个点对(v, w),(v, w)属于V,边有时还称弧(arc)
 * 点对有序该图为有向图(digraph),顶点w邻接到v,且(v, w)属于E
 * 无向图(undirected graph),w邻接到v,且v也邻接到w
 * 边还能拥有权(weight)或值(cost)
 *
 * 图的路径(path)是一个顶点序列(w1,w2,w3,...,wn),路径的长为路径上的边数N-1
 * 路径长可以为0,路径(v,v)表示从一个顶点到他自身，称为环(loop)
 * 简单路径是它的所有顶点都是不同的(除了起始和结束顶点可能会相同)
 * 有向图的环(cycle)是w1 = wn，长至少为1的一条路径
 * 该路径若是简单路径则该环为简单环路
 * 无向图的边是互异的，不存在环路(假设路径(u,v,u),在无向图中uv = vu)
 * 如果一个有向图没有环路，称有向无环图(DAG)
 * 一个环路经过图的所有顶点的环路为欧拉环路
 * 一个环路经过图的所有顶点且只有一次的环路为哈密尔顿环路
 *
 * 如果一个无向图从每一个顶点到其他每个顶点都存在一条路径，称该图为连通的(connected)
 * 如果具有该性质的是有向图，称强连通的(strongly connected)
 * 有向图的基础图(underlying graph)是其弧去掉方向形成的图
 * 如果有向图不是强连通但它的基础图是连通的称弱连通的(weakly connected)
 * 完全图(complete graph)是指每一对顶点间都存在一条边的图
 *
 * 邻接矩阵n*node_nums n个顶点 (i,j)表示顶点之间的关系 可以表示带权图
 * 关联矩阵n*edge_nums n行e边 (i,j)表示顶点之间的关联关系
 *
 * 权重初始化为0，默认为1
 */

template <typename VERTEX, typename EDGE>
class MatrixGraph {
public:
    std::vector<Vertex<VERTEX>> V;
    std::vector<std::vector<Edge<EDGE>*>> E;
    int node_nums;
    int edge_nums;
    void Reset() {
        for (int i = 0;i != node_nums;i++) {
            Status(i) = UNDISCOVERED;
            Dtime(i) = -1;
            Ftime(i) = -1;
            Parent(i) = -1;
            Priority(i) = __INT_MAX__;
            for (int j = 0;j != node_nums;i++) {
                if (Exist(i, j))
                    Status(i, j) = UNDETERMINED;
            }
        }
    }
    VERTEX& VertexData(int i) { return V[i].data; }
    EDGE& EdgeData(int i, int j) { return E[i][j]->data; }//边数据
    bool Exist(int i, int j) {
        if (i <= 0)
            return false;
        if (node_nums < i)
            return false;
        if (j <= 0)
            return false;
        if (node_nums < j)
            return false;
        if (E[i][j] == nullptr)
            return false;
        return true;
    }
    Vstatus& Status(int i) { return V[i].status; }
    int& Dtime(int i) { return V[i].dtime; }
    int& Ftime(int i) { return V[i].ftime; }
    int& Parent(int i) { return V[i].parent; }
    int& Priority(int i) { return V[i].priority; }
    int Indegree(int i) { return V[i].indegree; }
    int Outdegree(int i) { return V[i].outdegree; }
    Estatus& Status(int i, int j) { return E[i][j]->status; }//边状态
    int& Weight(int i, int j) { return E[i][j]->weight; }//边权重
    int NextNeighbor(int i, int j) {
        while ((-1 < j) && !Exist(i, --j))
            ;
        return j;
    }
    int FirstNeighbor(int i) {
        int node_nums = 999;//sentinal;
        return NextNeighbor(i, node_nums);
    }
    void InsertEdge(EDGE const& edge, int w, int i, int j) {
        if (Exist(i, j)) return;
        E[i][j] = new Edge<EDGE>(edge, w);
        edge_nums++;
        V[i].outdegree += 1;
        V[j].indegree += 1;
    }
    EDGE Remove(int i, int j) {
        EDGE tmp_edge = Edge(i, j);
        delete E[i][j];
        E[i][j] = nullptr;
        edge_nums -= 1;
        V[i].outdegree -= 1;
        V[j].indegree -= 1;
        return tmp_edge;
    }
    int InsertVertex(VERTEX const& vertex) {
        for (int j = 0;j != node_nums;j++) {
            E[j].insert(nullptr);
            node_nums += 1;
        }
        E.insert(std::vector<Edge<EDGE>*>(node_nums, node_nums, nullptr));
        return V.insert(Vertex<VERTEX>(vertex));
    }
    VERTEX Remove(int i) {
        for (int j = 0; j != node_nums; j++) {
            if (Exist(i, j)) {
                delete E[i][j];
                E[i][j] = nullptr;
                V[j].indegree -= 1;
            }
            if (Exist(j, i)) {
                delete E[j].Remove(i);
                E[j] = nullptr;
                V[j].outdegree -= 1;
            }
        }
        VERTEX tmp_vertex = V(i);
        V.remove(i);
        return tmp_vertex;
    }
public:
    MatrixGraph() { node_nums = edge_nums = 0; }
    template <typename T>
    /*BreadthFirstSearch(T v, int& clock) {//BFS
        std::queue<T> Q;
        Status(v) = DISCOVERED;
        Q.push(v);
        while (!Q.empty()) {
            v = Q.top();
            Dtime(v) = ++clock;
            for (int i = FirstNeighbor(i);i != -1;i = NextNeighbor(v, i)) {
                ;
            }
            Status(v) = VISITED;
        }
    }*/
    ~MatrixGraph() {
        for (int i = 0;i != node_nums;i++) {
            for (int j = 0;j != node_nums;j++) {
                delete E[i][j];
                E[i][j] = nullptr;
            }
        }
    }
};


#endif