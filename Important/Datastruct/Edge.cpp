#include "Edge.h"

template <typename T>
Edge<T>::Edge(T const& data_, int weight_) {
    data = data_;
    weight = weight_;
    status = UNDETERMINED;
}
template <typename T>
Edge<T>& Edge<T>::operator=(const Edge& new_edge) {
    this->data = new_edge.data;
    this->status = new_edge.status;
    this->weight = new_edge.weight;
    return *this;
}
template <typename T>
void Edge<T>::Reset() {
    data = data();
    status = UNDETERMINED;
    weight = -1;
}