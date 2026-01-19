#ifndef FUNCTION_H
#define FUNCTION_H

#include<iostream>
#include<fstream>
#include<sstream>
#include<nlohmann/json.hpp>
#include "crow_all.h"
#include<course.h>

template<typename T, typename Comparator, typename Extractor>
nlohmann::json SortEntities(
    const nlohmann::json& input_ids, 
    std::map<std::string, T>& data_map, 
    Comparator comp, 
    Extractor extract
) {
    std::vector<std::pair<std::string, T>> vec;
    for(auto& i : input_ids) {
        if(data_map.count(i["id"])) {
            vec.push_back({i["id"], data_map[i["id"]]});
        }
    }
    
    std::sort(vec.begin(), vec.end(), comp);
    
    nlohmann::json nj;
    for(auto& v : vec) {
        nlohmann::json item = extract(v.second);
        item["id"] = v.first;
        nj.push_back(item);
    }
    return nj;
}

template<typename T, typename Matcher, typename Extractor>
nlohmann::json SearchEntities(
    std::map<std::string, T>& data_map,
    Matcher matcher,
    Extractor extract
) {
    nlohmann::json nj;
    for(auto& [id, entity] : data_map) {
        if(matcher(id, entity)) {
            nlohmann::json item = extract(entity);
            item["id"] = id;
            nj.push_back(item);
        }
    }
    return nj;
}

nlohmann::json read_json_from_path(const std::string& path);
crow::json::wvalue json_to_wvalue(const nlohmann::json& j);

#endif



