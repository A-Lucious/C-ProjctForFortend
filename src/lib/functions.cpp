#include<functions.h>

nlohmann::json read_json_from_path(const std::string& path) { // convert the filestream to json
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "failed to open " << path << std::endl;
        return nlohmann::json::object();
    }
    nlohmann::json j;
    f >> j;
    return j;
}