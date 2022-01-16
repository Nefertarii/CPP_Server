#ifndef EDGE_H_
#define EDGE_H_

enum Estatus {
    UNDETERMINED,   //待定类型
    TREE,           //树边类型
    CROSS,          //跨越边类型
    FORWARD,        //前向边类型
    BACKWARD,       //回向边类型
};

template <typename T>
struct Edge {
    T data;         //数据
    Estatus status; //状态
    int weight;     //权重
    Edge(T const& data_, int weight_) {
        data = data_;
        weight = weight_;
        status = UNDETERMINED;
    }
    Edge& operator=(const Edge& new_edge) {
        this->data = new_edge.data;
        this->status = new_edge.status;
        this->weight = new_edge.weight;
        return *this;
    }
    void Reset() {
        data = data();
        status = UNDETERMINED;
        weight = -1;
    }
};



#endif