#ifndef GRAPH_H_
#define GRAPH_H_

#include "Vertex.h"
#include "Edge.h"
#include <vector>
#include <queue>
#include <iostream>

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
    void BreadthFirstSearch(int v, int& clock);
    void BFS(int start_vertex);
    void DepthFirstSearch(int v, int& clock);
    void DFS(int start_vertex);
    MatrixGraph();
    ~MatrixGraph() {}
};

#endif  