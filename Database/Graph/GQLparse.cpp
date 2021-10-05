#include "GQLparse.h"

bool GQLparser::GQLvalidator(std::vector<std::string>* query_words,
                             std::multimap<size_t, std::string>* query_map) {
    size_t tmpvalue = 0;
    for (size_t i = 0; i != query_words->size(); i++) {
        if (i % 2 == 0) {
            tmpvalue = 0;
            auto it = dictionary.find((*query_words)[i]);
            if (it != dictionary.end()) {
                tmpvalue = it->second;
            } else {
                query_map->clear();
                return false;
            }
        } else {
            query_map->insert(std::make_pair(tmpvalue, (*query_words)[i]));
        }
    }
    return true;
}

GQLparser::GQLparser() {
    std::string dictionary_dir = "/home/cs18/vscode/Webserver/Config/Dictionary.txt";
    Gfile::ReadConfig(dictionary_dir, &dictionary);
}

GQLparser::GQLparser(std::string dictionary_dir) {
    Gfile::ReadConfig(dictionary_dir, &dictionary);
}

std::multimap<size_t, std::string> GQLparser::GQLparse(std::string query) {
    query += ' ';
    size_t index = 0;
    size_t i = 0;
    std::vector<std::string> query_words;
    while (index != query.size()) {
        query_words.push_back(Substr(query, index, 20, ' '));
        index += query_words[i].size() + 1;
        i += 1;
    }
    std::multimap<size_t, std::string> query_map;
    if (GQLvalidator(&query_words, &query_map)) {
        return query_map;
    } else {
        return;
    }
}