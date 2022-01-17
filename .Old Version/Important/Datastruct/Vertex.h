#ifndef VERTEX_H_
#define VERTEX_H_

enum Vstatus {
    UNDISCOVERED,   //未发现
    DISCOVERED,     //发现
    VISITED,        //访问完毕
};

template <typename T>
struct Vertex {
    T data;         //数据
    Vstatus status; //状态
    int indegree;   //入度
    int outdegree;  //出度
    int dtime;      //被发现时间
    int ftime;      //被访问完成时间
    int priority;   //优先级数
    Vertex* parent; //父节点
    Vertex(T const& data_);
    Vertex& operator= (const Vertex& new_vertex);
    void Reset();
};



#endif