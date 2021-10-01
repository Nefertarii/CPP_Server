#include "Graph.h"
#include <iostream>
using namespace std;

int main() {
    MatrixGraph<int, int> mtx_map;
    mtx_map.Reset();
    mtx_map.InsertVertex(10);
    mtx_map.InsertVertex(20);
    mtx_map.InsertVertex(30);
    mtx_map.InsertVertex(40);
    mtx_map.InsertEdge(1, 1, 1, 2);
    mtx_map.InsertEdge(1, 2, 2, 2);
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