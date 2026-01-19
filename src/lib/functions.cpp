#include<functions.h>

nlohmann::json read_json_from_path(const std::string& path) { // convert the filestream to json
    std::ifstream f(path);
    if (!f.is_open() || f.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "failed to open or it is none  " << path << std::endl;
        return nlohmann::json::object();
    }
    nlohmann::json j;
    f >> j;
    return j;
}

template<typename T>
void swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}