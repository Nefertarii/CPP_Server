#ifndef MAP_H_
#define MAP_H_

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
 * 有向图的圈(cycle)是w1 = wn，长至少为1的一条路径
 * 该路径若是简单路径则该圈为简单圈
 * 无向图的边是互异的，不存在圈(假设路径(u,v,u),在无向图中uv = vu)
 * 如果一个有向图没有圈，称有向无圈图(DAG)
 *
 * 如果一个无向图从每一个顶点到其他每个顶点都存在一条路径，称该图为连通的(connected)
 * 如果具有该性质的是有向图，称强连通的(strongly connected)
 * 有向图的基础图(underlying graph)是其弧去掉方向形成的图
 * 如果有向图不是强连通但它的基础图是连通的称弱连通的(weakly connected)
 * 完全图(complete graph)是指每一对顶点间都存在一条边的图
 */



#endif