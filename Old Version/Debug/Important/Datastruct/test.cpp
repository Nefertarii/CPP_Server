#include "Graph.h"
#include <iostream>
using namespace std;

int main() {
    MatrixGraph<int, int> mtx_map;
    mtx_map.Reset();
    for (int i = 0;i != 10;i++) {
        mtx_map.InsertVertex(i*10);
    }
    mtx_map.InsertEdge(0, 1, 1, 1);
    mtx_map.InsertEdge(1, 2, 1, 1);
    mtx_map.InsertEdge(1, 3, 1, 1);
    mtx_map.InsertEdge(2, 4, 1, 1);
    mtx_map.InsertEdge(4, 5, 1, 1);
    mtx_map.InsertEdge(5, 6, 1, 1);
    mtx_map.InsertEdge(6, 7, 1, 1);
    mtx_map.InsertEdge(6, 8, 1, 1);
    mtx_map.InsertEdge(3, 9, 1, 1);
    mtx_map.InsertEdge(1, 0, 1, 1);
    mtx_map.InsertEdge(2, 1, 1, 1);
    mtx_map.InsertEdge(3, 1, 1, 1);
    mtx_map.InsertEdge(4, 2, 1, 1);
    mtx_map.InsertEdge(5, 4, 1, 1);
    mtx_map.InsertEdge(6, 5, 1, 1);
    mtx_map.InsertEdge(7, 6, 1, 1);
    mtx_map.InsertEdge(8, 6, 1, 1);
    mtx_map.InsertEdge(9, 3, 1, 1);
    mtx_map.InsertEdge(7, 9, 1, 1);
    mtx_map.InsertEdge(9, 7, 1, 1);
    int i = 0;
    mtx_map.DepthFirstSearch(0, i);
        
    /*
    mtx_map.Reset();
    mtx_map.Exist(int i, int j);

    mtx_map.NextNeighbor(int i, int j);
    mtx_map.FirstNeighbor(int i);
    mtx_map.Insert(EDGE const& edge, int w, int i, int j);
    mtx_map.Remove(int i, int j);
    mtx_map.Insert(VERTEX const& vertex);
    mtx_map.Remove(int i);
    */
    return 0;
}