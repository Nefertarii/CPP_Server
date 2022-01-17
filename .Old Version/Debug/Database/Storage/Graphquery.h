#ifndef GRAPHQUERY_H_
#define GRAPHQUERY_H_

#include "../../Important/Datastruct/Graph.h"
#include "Graphstorage.h"
#include <string>
#include <vector>
#include <map>


enum GQLgraph_operator {
    SEARCH,
    INSERT,
    CHANGE,
    DELEDE,
    CREATE,
    DROP,
    
    WHERE,
    
    EQUAL,
    LIKE,
};

class GraphQuery {
private:
    MatrixGraph graph;
    void ReadHotGraph();
    void ReadGraph(std::string graph_name);
    void SaveGraph();
    void SearchGraph();
public:
    GraphQuery() = default;
    std::map<std::string, std::string> Query(std::multimap<size_t, std::string> query);
    ~GraphQuery() = default;
};

std::map<std::string, std::string> GraphQuery::Query(std::multimap<size_t, std::string> query) {
    for (auto it : query) {
        
    }
    return "0";
}






#endif