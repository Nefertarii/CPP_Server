#ifndef GQLPARSER_H_
#define GQLPARSER_H_

#include "../Storage/Graphquery.h"
#include "../../Important/Gstring.h"
#include "../../Important/Gfilefunc.h"
#include <vector>
#include <map>

class GQLparser {
private:
    std::map<std::string, size_t> dictionary;
    bool GQLvalidator(std::vector<std::string>* query_words,
                      std::multimap<size_t, std::string>* query_map);    //校验
    void GQLplanner();      //计划执行
    void GQLexecutor();     //执行器
public:
    GQLparser();
    GQLparser(std::string dictionary_dir);
    std::multimap<size_t, std::string> GQLparse(std::string query);
    ~GQLparser() = default;
};

#endif