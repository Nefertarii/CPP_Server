#ifndef GRAPH_H_
#define GRAPH_H

#include <vector>

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
 * 邻接矩阵n*n n个顶点 (i,j)表示顶点之间的关系 可以表示带权图
 * 关联矩阵n*e n行e边 (i,j)表示顶点之间的关联关系
 */

enum Vstatus {
    UNDISCOVERED,
    DISCOVERED,
    VISITED,
};
enum Estatus {
    UNDETERMINED,
    TREE,
    CROSS,
    FORWARD,
    BACKWARD,
};

template <typename V>
struct Vertex {
    V data;
    int indegree;//定点入度数
    int outdegree;//顶点出度数
    Vstatus status;;//顶点状态信息
    int dtime;//顶点时间标签 被发现
    int ftime;//顶点时间标签 被访问完毕
    int parent;//顶点父节点信息
    int priority;//顶点优先级数
    Vertex(V const& d) {
        data(d);
        indegree = 0;
        outdegree = 0;
        status = UNDISCOVERED;
        dtime = -1;
        ftime = -1;
        parent = -1;
        priority = __INT_MAX__;
    }
};

template <typename E>
struct Edge {
    E data;
    int weight;//边权重
    Estatus status;
    Edge(E const& d, int w) {
        data = d;
        weight = w;
        status = UNDETERMINED;
    }
};

template <typename V, typename E>
struct Graph {
private:
    virtual int status(int i) = 0;
    virtual void status(int i, int j) = 0;//设置/查询边状态
    virtual int dTime(int i) = 0;
    virtual int fTime(int i) = 0;
    virtual int parent(int i) = 0;
    virtual int priority(int i) = 0;
    virtual int indegree(int i) = 0;
    virtual int outdegree(int i) = 0;
    virtual void exists(int i, int j) = 0;//两个顶点之间是否存在一条连边
    virtual void reset() {
        for (int i = 0;i != n;i++) {
            status(i) = UNDISCOVERED;
            dTime(i) = fTime(i) = -1;
            parent(i) = -1;
            priority(i) = __INT_MAX__;
            for (int j = 0;j != n;i++) {
                if (exists(i, j))
                    status(i, j) = UNDISCOVERED;
            }
        }
    }
public:
    ;
};

template <typename TV, typename TE>
class MatrixGraph : public Graph<TV, TE> {
private:
    std::vector<Vertex<TV>> V;
    std::vector<std::vector<Edge<TE>>> E;
public:
    //void status(int i, int j) = 0;//设置/查询边状态
    TV& Vertex(int i) { return V[i].data; }
    int Status(int i) { return V[i].status; }
    int Dtime(int i) { return V[i].dtime; }
    int Ftime(int i) { return V[i].ftime; }
    int Parent(int i) { return V[i].parent; }
    int Priority(int i) { return V[i].priority; }
    int Indegree(int i) { return V[i].indegree; }
    int Outdegree(int i) { return V[i].outdegree; }
    int Next_Neighbor(int i, int j) {
        while ((-1 < j) && !exists(i, --j))
            ;
        return j;
    }
    int First_Neighbor(int i) {
        int n = 999;//sentinal;
        return Next_Neighbor(i, n);
    }
    MatrixGraph() { n = e = 0; }
    ~MatrixGraph() {
        /*for (int i = 0;i != n;i++) {
            for (int j = 0;j != n;j++) {
                delete E[i][j];
            }
        }*/
    }
};


#endif