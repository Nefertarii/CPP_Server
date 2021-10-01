#ifndef GRAPH_H_
#define GRAPH_H_

#include "Vertex.h"
#include "Edge.h"
#include <vector>
#include <queue>

template <typename VERTEX, typename EDGE>
class MatrixGraph {
public:
    int nodes;//节点数
    int edges;//边数
    int matrix_size;
    std::vector<Vertex<VERTEX>> vertex_vec;
    std::vector<std::vector<Edge<EDGE>>> edge_vec;
    bool Exist(int i, int j);
    void Reset();
    void MatrixResize();
    int NextNeighbor(int i, int j);
    int FirstNeighbor(int i);
    void InsertEdge(int i, int j, EDGE const& data_, int weight_ = 1);
    void InsertVertex(VERTEX const& data_);
    Edge<EDGE> RemoveEdge(int i, int j);
    Vertex<VERTEX> RemoveVertex(int i);
    void BreadthFirstSearch(VERTEX v, int& clock);
    void BFS(int start_vertex);
    MatrixGraph();
    ~MatrixGraph(){}
};

template <typename VERTEX, typename EDGE>
bool MatrixGraph<VERTEX, EDGE>::Exist(int i, int j) {
    if (i <= 0 || j <= 0) {
        return false;
    }
    if (nodes < i || nodes < j) {
        return false;
    }
    if (edge_vec[i][j].weight == 0) {
        return false;
    }
    return true;
}

template <typename VERTEX, typename EDGE>
void MatrixGraph<VERTEX, EDGE>::Reset() {
    for (int i = 0;i != nodes;i++) {
        vertex_vec[i].status = UNDISCOVERED;
        vertex_vec[i].dtime = -1;
        vertex_vec[i].ftime = -1;
        vertex_vec[i].priority = -1;
        vertex_vec[i].parent = nullptr;
        for (int j = 0;j != nodes;j++) {
            if (Exist(i, j)) {
                edge_vec[i][j].status = UNDETERMINED;
            }
        }
    }
}

template <typename VERTEX, typename EDGE>
void MatrixGraph<VERTEX, EDGE>::MatrixResize() {
    vertex_vec.resize(matrix_size);
    edge_vec.resize(matrix_size);
    for (int i = 0;i != matrix_size;i++) {
        edge_vec[i].resize(matrix_size);
    }
    for (int i = 0;i != matrix_size;i++) {
        //vertex_vec[i].Reset();
        //edge_vec[i][nodes].Reset();
        //edge_vec[nodes][i].Reset();
    }
}

template <typename VERTEX, typename EDGE>
int MatrixGraph<VERTEX, EDGE>::NextNeighbor(int i, int j) {
    while ((-1 < j) && !Exist(i, --j)) {
        ;//do something
        return j;
    }
    return j;
}

template <typename VERTEX, typename EDGE>
int MatrixGraph<VERTEX, EDGE>::FirstNeighbor(int i) {
    return NextNeighbor(i, nodes);
}

template <typename VERTEX, typename EDGE>
void MatrixGraph<VERTEX, EDGE>::InsertEdge(int i, int j, EDGE const& data_, int weight_) {
    if (Exist(i, j))
        return;
    edges += 1;
    edge_vec[i][j].data = data_;
    edge_vec[i][j].weight = weight_;
    vertex_vec[i].outdegree += 1;
    vertex_vec[j].indegree += 1;
}

template <typename VERTEX, typename EDGE>
void MatrixGraph<VERTEX, EDGE>::InsertVertex(VERTEX const& data_) {
    vertex_vec[nodes].data = data_;
    nodes += 1;
    if (nodes >= matrix_size) {
        (int)(matrix_size *= 1.5);
        MatrixResize();
    }
} 

template <typename VERTEX, typename EDGE>
Edge<EDGE> MatrixGraph<VERTEX, EDGE>::RemoveEdge(int i, int j) {
    Edge<EDGE> edge;
    edge = edge_vec[i][j];
    edge_vec[i][j].Reset();
    return edge;
}

template <typename VERTEX, typename EDGE>
Vertex<VERTEX> MatrixGraph<VERTEX, EDGE>::RemoveVertex(int i) {
    Vertex<VERTEX> vertex;
    vertex = vertex_vec[i];
    vertex_vec[i].Reset();
    return vertex;
}

template <typename VERTEX, typename EDGE>
MatrixGraph<VERTEX, EDGE>::MatrixGraph() {
    nodes = 0;
    edges = 0;
    matrix_size = 2;
    MatrixResize();
}

template <typename VERTEX, typename EDGE>
void MatrixGraph<VERTEX, EDGE>::BreadthFirstSearch(VERTEX v, int& clock)  {//BFS
    std::queue<VERTEX> queue;
    vertex_vec[v].status = DISCOVERED;
    queue.push(v);
    while (!queue.empty()) {
        v = queue.top();
        vertex_vec[v].dtime = ++clock;
        for (int u = FirstNeighbor(u);u != -1;u = NextNeighbor(v, u)) { //v的邻居u
            if (vertex_vec[u].status == UNDISCOVERED) {
                vertex_vec[u].status = DISCOVERED;
                queue.push(u);
                edge_vec[v, u].status = TREE;
                vertex_vec[u].parent = vertex_vec[v];
            } else {
                edge_vec[v, u].status = CROSS;
            }
            vertex_vec[v].status = VISITED;
        }
        vertex_vec[v].status = VISITED;
    }
}

template <typename VERTEX, typename EDGE>
void MatrixGraph<VERTEX, EDGE>::BFS(int start_vertex) {
    int clock = 0;
    int vertex = start_vertex;
    do {
        if (vertex_vec[vertex].status == UNDISCOVERED) {
            BreadthFirstSearch(vertex, clock);
        }
    } while (start_vertex != (vertex = (++v % nodes)));
}

#endif  