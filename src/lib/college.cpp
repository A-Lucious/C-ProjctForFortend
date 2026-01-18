#include<college.h>

College::College() {
    std::cout << "Init the college,,," <<std::endl;
    std::string dict = "./Example/";
    ImportCourses((dict + "courses.json"));
    ImportStudents((dict + "students.json"));
}
College::~College() {};

std::map<std::string,Course> College::Get_Courses() {
    return Courses;
}

std::map<std::string,Student> College::Get_StudentRoll() {
    return StudentRoll;
}

std::map<std::string,Teacher> College::Get_TeacherRoll() {
    return TeacherRoll;
}

void College::Set_Courses(const std::string& Cname, const Course& newCourse) {
    Courses[Cname] = newCourse;
}

void College::Set_Student(const Student& newStudent) {
    StudentRoll[newStudent.net_id] = newStudent;
}

void College::Set_Teacher(const Teacher& newTeacher) {
    TeacherRoll[newTeacher.net_id] = newTeacher;
}

void College::ImportCourses(const std::string& path) { //import courses json from path
    nlohmann::json courses = read_json_from_path(path);
    if (courses.empty()) {
        std::cout << "The Courses Json is empty" << std::endl;
    }
    for(auto& [key,value] : courses.items()) {
        std::cout << value["id"] << std::endl;
        Course newCourse(value);
        Courses[value["id"]] = newCourse;
        std::cout << "Import the Courses success" <<std::endl;
    }
}

void College::ImportStudents(const std::string& path) {
    nlohmann::json students = read_json_from_path(path);
    if (students.empty()) {
        std::cout << "The Students Json is empty" << std::endl;
    }
    for(auto& [key,value] : students.items()) {
        Student newStudent(value,Courses);
        StudentRoll[value["stunum"]] = newStudent;
        std::cout << "Import the Students success" << std::endl;
    }
}

nlohmann::json College::ExportCourses_jsonsimple() { 
    nlohmann::json nj;
    for (auto& c:Courses) {
        nlohmann::json item = c.second.ExportCourse_to_simplejson();
        item["id"] = c.first;
        nj.push_back(item);
    }
    return nj;
}

nlohmann::json College::ExportStudents_jsonsimple() {
    nlohmann::json nj;
    for(auto& stu : StudentRoll) {
        nlohmann::json item = stu.second.ExportStudent_to_simplejson();
        item["id"] = stu.first;
        nj.push_back(item);
    }
    return nj;
}

nlohmann::json College::SortCourses(const nlohmann::json& js, const std::string& type) { // it not work
    std::vector<std::pair<std::string,Course>> cs;
    for(auto& i : js) {
        cs.push_back({i["id"], Courses[i["id"]]});
    }

    if(type == "course_number") {
        std::sort(cs.begin(), cs.end(), [](auto& a, auto& b) {
            return a.second.Get_ID() < b.second.Get_ID();
        });
    }else if(type == "course_name") {
        std::sort(cs.begin(), cs.end(), [](auto& a, auto& b) {
            return a.second.Get_ClassName() < b.second.Get_ClassName();
        });
    }else if(type == "course_teacher") {
        std::sort(cs.begin(), cs.end(), [](auto& a, auto& b) {
            return a.second.Get_Teacher() < b.second.Get_Teacher();
        });
    }

    nlohmann::json nj; // prevent the json is unorder when transit
    for (auto& c : cs) {
        nlohmann::json item = c.second.ExportCourse_to_simplejson();
        item["id"] = c.first;
        nj.push_back(item);
    }
    return nj;
}

nlohmann::json College::SortStudent(const nlohmann::json& js, const std::string& type) {
    std::vector<std::pair<std::string,Student>> stus;
    for(auto& i : js) {
        stus.push_back({i["id"], StudentRoll[i["id"]]});
    }

    if(type == "netid") {
        std::sort(stus.begin(), stus.end(), [](auto& a, auto& b) {
            return a.second.Get_NetId() < b.second.Get_NetId();
        });
    }else if(type == "student_name") {
        std::sort(stus.begin(), stus.end(), [](auto& a, auto& b) {
            return a.second.Get_Name() < b.second.Get_Name();
        });
    }else if(type == "student_number") {
        std::sort(stus.begin(), stus.end(), [](auto& a, auto& b) {
            return a.second.Get_StuNum() < b.second.Get_StuNum();
        });
    }

    nlohmann::json nj; // prevent the json is unorder when transit
    for (auto& stu : stus) {
        nlohmann::json item = stu.second.ExportStudent_to_simplejson();
        item["id"] = stu.first;
        nj.push_back(item);
    }
    return nj;
}

nlohmann::json College::SearchQuery(const std::string& str) { // it not work 
    std::map<std::string,Course> cs; 
    for (auto& pair : Courses) {
        if (pair.first == str ||
            pair.second.Get_Teacher() == str ||
            pair.second.Get_ClassName() == str) {
            cs[pair.first] = pair.second;
        } else {
            try {
                int num = std::stoi(str);
                if (pair.second.Get_Credit() == num ||
                    std::abs(pair.second.Get_GPA() - num) < 1e-6) {
                    cs[pair.first] = pair.second;
                }
            } catch (...) {}
        }
    }
    nlohmann::json nj;
    for (auto& c : cs) {
        nj[c.first] = c.second.ExportCourse_to_simplejson();
    }
    return nj;
    
}

nlohmann::json College::SearchQuery(const std::string& str,const std::string& type) { // for special search
    std::map<std::string,Course> cs;
    if (type == "fuzzy") {
        for(auto& i : Courses) {
            std::string p = (i.second.Get_ClassName() + " " + i.second.Get_Teacher() + " " + i.second.Get_Teacher()).c_str();
            auto it = std::search(p.begin(), p.end(),std::boyer_moore_searcher(str.begin(), str.end()));
            if(it != p.end()) {
                cs[i.first] = i.second;
            }
        }
    }
    if(type == "class_name") {
        for(auto& i : Courses) {
            if(i.second.Get_ClassName() == str) cs[i.first] = i.second;
        }
    }else if(type == "teacher_name") {
        for(auto& i : Courses) {
            if(i.second.Get_Teacher() == str) cs[i.first] = i.second;
        }
    }else if(type == "id") {
        for(auto& i : Courses) {
            if(i.first == str) cs[i.first] = i.second;
        }
    }
    nlohmann::json nj;
    for(auto& c : cs) {
        nlohmann::json item = c.second.ExportCourse_to_simplejson();
        item["id"] = c.first;
        nj.push_back(item);
    }
    return nj;
}

nlohmann::json College::SearchStuQuery(const std::string& str, const std::string& type) {
    std::map<std::string, Student> stus;
    if (type == "fuzzy") {
        for(auto& i : StudentRoll) {
            std::string p = (i.second.Get_NetId() + " " + i.second.Get_Name() + " " + i.second.Get_Name()).c_str();
            auto it = std::search(p.begin(),p.end(),std::boyer_moore_searcher(str.begin(), str.end()));
            if(it != p.end()) {
                stus[i.first] = i.second;
            }
        }
    }
    if(type == "netid") {
        for(auto& i : StudentRoll) {
            if(i.second.Get_NetId() == str) stus[i.first] = i.second;
        }
    }else if(type == "student_name") {
        for(auto& i : StudentRoll) {
            if(i.second.Get_Name() == str) stus[i.first] = i.second;
        }
    }else if(type == "student_number") {
        for(auto& i : StudentRoll) {
            if(i.second.Get_Name() == str) stus[i.first] = i.second;
        }

    }
    nlohmann::json nj;
    for(auto& stu : stus) {
        nlohmann::json item = stu.second.ExportStudent_to_simplejson();
        item["id"] = stu.first;
        nj.push_back(item);
    }
    return nj;
}

nlohmann::json College::SearchSelectStudent(const std::string& stunum) {
    Student stu;
    for(auto& i : StudentRoll) {
        if (i.second.Get_StuNum() == stunum) stu = i.second;
    }
    nlohmann::json nj;
    nj["basic_info"] = {
        {"name",stu.Get_Name()},
        {"stunum",stu.Get_StuNum()},
        {"college",stu.Get_CollegeName()}
    };
    nj["chart_data"]["credits"] = {
        {"total_required",stu.Get_NeedCredit()},
        {"earned",stu.Get_CurrentCredit()},
    };
    for(auto& i : stu.Get_ScoresCredit()) {
        std::string cname = Courses[i.first].Get_ClassName();
        nj["chart_data"]["grades_history"].push_back({{"course_name",cname},{"score",i.second[0]}});
        nj["chart_data"]["current_semester_gpa"].push_back({{"course_name",cname},{"credit",i.second[1]},{"gpa",i.second[2]}});
    }
    return nj;
}








