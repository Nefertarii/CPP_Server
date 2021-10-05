#include "Vertex.h"

template <typename T>
Vertex<T>::Vertex(T const& data_) {
    data = data_;
    status = UNDISCOVERED;
    indegree = 0;
    outdegree = 0;
    dtime = -1;
    ftime = -1;
    priority = -1;
    parent = nullptr;
}

template <typename T>
Vertex<T>& Vertex<T>::operator= (const Vertex& new_vertex) {
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

template <typename T>
void Vertex<T>::Reset() {
    data = data();
    status = UNDISCOVERED;
    indegree = -1;
    outdegree = -1;
    dtime = -1;
    ftime = -1;
    priority = -1;
    parent = nullptr;
}
