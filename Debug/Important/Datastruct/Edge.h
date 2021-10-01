#ifndef EDGE_H_
#define EDGE_H_

enum Estatus {
    UNDETERMINED,
    TREE,
    CROSS,
    FORWARD,
    BACKWARD,
};

template <typename T>
class Edge {
public:
    T data;         //数据
    Estatus status; //状态
    int weight;     //权重
    Edge() = default;
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
    ~Edge() = default;
};



#endif