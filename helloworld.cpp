#include <crow.h>
#include <nlohmann/json.hpp>
#include <college.h>
#include <course.h>
#include <functions.h>

int main() {
    crow::SimpleApp app;

    // 将工作目录切换到编译时确定的项目根目录
    std::filesystem::current_path(PROJECT_ROOT_DIR);

    College c1;

    //通用css路由
    CROW_ROUTE(app, "/static/<string>")([](crow::response& res,const std::string filename){
        std::string path = ("./frontend/" + filename);
        res.set_static_file_info(path);
        res.end();
    });

    //通用css图片搜索
    CROW_ROUTE(app,"/static/IMAGES/<string>")([](crow::response& res,const std::string imagename){
        std::string path = "./frontend/IMAGES/" + imagename;
        res.set_static_file_info(path);
        res.end();
    });

    //reload init
    CROW_ROUTE(app, "/")([](const crow::request& req, crow::response& res){
        res.set_static_file_info("./frontend/index.html");
        res.end();
    });

    //reload init
    CROW_ROUTE(app, "/script.js")([](const crow::request& req, crow::response& res){
        res.set_static_file_info("./frontend/script.js");
        res.end();
    });

    //fetch students and courses
    CROW_ROUTE(app,"/api/get_courses")([&c1](crow::response& res){
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.write(c1.ExportCourses_jsonsimple().dump());
        res.end();
    });

    CROW_ROUTE(app,"/api/get_students")([&c1](crow::response& res){
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.write(c1.ExportStudents_jsonsimple().dump());
        res.end();
    });

    //sort
    CROW_ROUTE(app, "/api/Sort/Course").methods(crow::HTTPMethod::OPTIONS,crow::HTTPMethod::POST)([&c1](const crow::request& req){
        if(req.method == crow::HTTPMethod::OPTIONS) {
            crow::response res(200);
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type");
            return res;
        }
        crow::response res(200);
        return res;
    });

    //search
    CROW_ROUTE(app, "/api/Search/Course").methods(crow::HTTPMethod::GET)([&c1](const crow::request& req){
        std::string type = req.url_params.get("type");
        std::string query = req.url_params.get("q");
        if (type.empty() || query.empty()) {
            return crow::response(400,"Missing 'type' or 'q'.");
        }
        nlohmann::json js = c1.SearchQuery(query,type);
        crow::response res(200);
        res.set_header("Content-Type", "application/json");
        res.write(js.dump());
        return res;
    });

    //设置监听的端口号，并run（）启动事件循环
    app.port(18080).multithreaded().run();
}