#include<college.h>

College::College() {
    std::cout << "Init the college,,," <<std::endl;
    std::string dict = "./Example/";
    ImportCourses((dict + "courses.json"));
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
        Course newCourse(value);
        Courses[value["id"]] = newCourse;
        std::cout << "Import the Courses success" <<std::endl;
    }
}

nlohmann::json College::ExportCourses_jsonsimple() { 
    nlohmann::json nj;
    for (auto& c:Courses) {
        nj[c.first] = c.second.ExportCourse_to_simplejson();
    }
    return nj;
}

nlohmann::json College::SortCoursesByID() { // it not work
    std::vector<std::pair<std::string,Course>> vec(Courses.begin(),Courses.end());
    std::sort(vec.begin(),vec.end(),[](const auto& a, const auto& b){
        return a.first < b.first;
    });
    nlohmann::json nj;
    for(auto& v:vec) {
        nj[v.first] = v.second.ExportCourse_to_simplejson();
    }
    return nj;
}

nlohmann::json College::SortCoursesBySemester() { // it not work
    std::vector<std::pair<std::string,Course>> vec(Courses.begin(),Courses.end());
    std::sort(vec.begin(),vec.end(),[](auto& a, auto& b){
        std::pair<int,int> as = a.second.Get_Semester();
        std::pair<int,int> bs = b.second.Get_Semester();
        if (as.second == bs.second){
            return as.first < bs.first;
        }
        else {
            return as.second < bs.second;
        }
    });
    nlohmann::json nj;
    for(auto& v:vec) {
        nj[v.first] = v.second.ExportCourse_to_simplejson();
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
        nj[c.first] = c.second.ExportCourse_to_simplejson();
    }
    return nj;
}





