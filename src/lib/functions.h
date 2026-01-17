#ifndef FUNCTION_H
#define FUNCTION_H

#include<iostream>
#include<fstream>
#include<sstream>
#include<nlohmann/json.hpp>
#include<crow.h>
#include<course.h>

nlohmann::json read_json_from_path(const std::string& path);
crow::json::wvalue json_to_wvalue(const nlohmann::json& j);

#endif



