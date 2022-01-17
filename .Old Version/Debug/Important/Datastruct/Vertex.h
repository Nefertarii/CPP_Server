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
    Vertex(T const& data_) {
        data = data_;
        status = UNDISCOVERED;
        indegree = 0;
        outdegree = 0;
        dtime = -1;
        ftime = -1;
        priority = -1;
        parent = nullptr;
    }
    Vertex& operator= (const Vertex& new_vertex) {
        this->data = new_vertex.data;
        this->status = new_vertex.status;
        this->indegree = new_vertex.indegree;
        this->outdegree = new_vertex.outdegree;
        this->dtime = new_vertex.dtime;
        this->ftime = new_vertex.ftime;
        this->priority = new_vertex.priority;
        this->parent = new_vertex.parent;
        return *this;
    }
    void Reset() {
        data = data();
        status = UNDISCOVERED;
        indegree = -1;
        outdegree = -1;
        dtime = -1;
        ftime = -1;
        priority = -1;
        parent = nullptr;
    }
};



#endif