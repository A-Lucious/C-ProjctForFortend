#include<college.h>
#include<functions.h>

College::College() {
    std::cout << "Init the college,,," <<std::endl;
    std::string dict = "./Example/";
    ImportCourses((dict + "courses.json"));
    ImportStudents((dict + "students.json"));
    ImportTeachers((dict + "teachers.json"));
}
College::~College() {
    delete Courses;
    delete StudentRoll;
    delete TeacherRoll;
};

std::map<std::string,Course> College::Get_Courses() {
    return (*Courses);
}

std::map<std::string,Student> College::Get_StudentRoll() {
    return (*StudentRoll);
}

std::map<std::string,Teacher> College::Get_TeacherRoll() {
    return (*TeacherRoll);
}

void College::Set_Courses(const std::string& Cname, const Course& newCourse) {
    (*Courses)[Cname] = newCourse;
}

void College::Set_Student(const Student& newStudent) {
    (*StudentRoll)[newStudent.net_id] = newStudent;
}

void College::Set_Teacher(const Teacher& newTeacher) {
    (*TeacherRoll)[newTeacher.net_id] = newTeacher;
}

void College::SetCourse(const nlohmann::json& item) {
    std::string id = item["id"];
    Course& c = (*Courses)[id];
    c.id = item["id"];
    c.class_name = item["classname"];
    c.teacher = item["teacher"];
    c.credit = item["credit"];
    c.GPA = item["GPA"];
    c.tls = item["tls"];
}
void College::SetStudent(const nlohmann::json& item) {
    Student& stu = (*StudentRoll)[item["stunum"]];
    stu.net_id = item["stunum"];
    stu.name = item["name"];
    stu.been.first = item["erldt"];
    stu.been.second = item["gdtdt"];
    stu.college_name = item["college"];
    stu.needcredit = item["needcredit"];
    stu.maxGPA = item["maxGPA"];
    stu.been = item["been"];
    stu.course_ids = item["course_ids"];
    stu.scores_credit_GPA = item["course_scores"];
}
void College::ReFresh(const nlohmann::json& item) {
    Student& stu = (*StudentRoll)[item["stunum"]];
    stu.ReFresh(item,(*Courses));
}

void College::ImportCourses(const std::string& path) { //import courses json from path
    nlohmann::json courses = read_json_from_path(path);
    if (courses.empty()) {
        std::cout << "The Courses Json is empty" << std::endl;
    }
    Courses = new std::map<std::string,Course>;
    for(auto& [key,value] : courses.items()) {
        std::cout << value["id"] << std::endl;
        Course newCourse(value);
        (*Courses)[value["id"]] = newCourse;
        std::cout << "Import the Courses success" <<std::endl;
    }
}

void College::ImportStudents(const std::string& path) {
    nlohmann::json students = read_json_from_path(path);
    if (students.empty()) {
        std::cout << "The Students Json is empty" << std::endl;
    }
    StudentRoll = new std::map<std::string,Student>;
    for(auto& [key,value] : students.items()) {
        Student newStudent(value,(*Courses));
        (*StudentRoll)[value["stunum"]] = newStudent;
        std::cout << "Import the Students success" << std::endl;
    }
}

void College::ImportTeachers(const std::string& path) {
    nlohmann::json teachers = read_json_from_path(path);
    if (teachers.empty()) {
        std::cout << "The Teachers Json is empty" << std::endl;
    }
    TeacherRoll = new std::map<std::string,Teacher>;
}

nlohmann::json College::ExportCourses_jsonsimple() { 
    nlohmann::json nj;
    for (auto& c:(*Courses)) {
        nlohmann::json item = c.second.ExportCourse_to_simplejson();
        item["id"] = c.first;
        nj.push_back(item);
    }
    return nj;
}

nlohmann::json College::ExportStudents_jsonsimple() {
    nlohmann::json nj;
    for(auto& stu : (*StudentRoll)) {
        nlohmann::json item = stu.second.ExportStudent_to_simplejson();
        item["id"] = stu.first;
        nj.push_back(item);
    }
    return nj;
}

nlohmann::json College::ExportCourse_josn(const std::string& id) {
    nlohmann::json nj;
    Course c = (*Courses)[id];
    nj["id"] = id;
    nj["classname"] = c.Get_ClassName();
    nj["credit"] = c.Get_Credit();
    nj["teacher"] = c.Get_Teacher();
    nj["semester"] = c.Get_Semester();
    nj["GPA"] = c.Get_GPA();
    nj["tls"] = c.Get_Tls();
    return nj;
}

nlohmann::json College::ExportStudent_json(const std::string& id) {
    nlohmann::json nj;
    Student stu = (*StudentRoll)[id];
    nj["stunum"] = stu.Get_StuNum();
    nj["name"] = stu.Get_Name();
    nj["erldt"] = stu.Get_Been().first;
    nj["gdtdt"] = stu.Get_Been().second;
    nj["college"] = stu.Get_CollegeName();
    nj["needcredit"] = stu.Get_NeedCredit();
    nj["maxGPA"] = stu.Get_MaxGPA();
    nj["been"] = stu.Get_Been();
    nj["course_ids"] = stu.Get_CourseIds();
    nj["course_scores"] = stu.Get_ScoresCredit();
    nj["current_semester"] = stu.Get_CurrentSemester();
    return nj;
}

void College::ExportCourses(const std::string& path) {
    nlohmann::json j;
    for(auto& [key, course] : *Courses) {
        j[key] = course;
    }
    std::ofstream file(("./Example/" + path).c_str());
    file << j.dump(4);
}

void College::ExportStudents(const std::string& path) {
    nlohmann::json j;
    for(auto& [key, student] : *StudentRoll) {
        j[key] = student;
    }

    std::ofstream file(("./Example/" + path).c_str());
    file << j.dump(4);
}

nlohmann::json College::SortCourses(const nlohmann::json& js, const std::string& type) {
    return SortEntities(
        js, 
        (*Courses),
        [&](const std::pair<std::string, Course>& a, const std::pair<std::string, Course>& b) {
            if(type == "course_number") return a.second.Get_ID() < b.second.Get_ID();
            if(type == "course_name") return a.second.Get_ClassName() < b.second.Get_ClassName();
            if(type == "course_teacher") return a.second.Get_Teacher() < b.second.Get_Teacher();
            return false;
        },
        [](Course& c) {
            return c.ExportCourse_to_simplejson();
        }
    );
}

nlohmann::json College::SortStudent(const nlohmann::json& js, const std::string& type) {
    return SortEntities(
        js, 
        (*StudentRoll),
        [&](const std::pair<std::string, Student>& a,const std::pair<std::string, Student>& b) {
            if(type == "netid") return a.second.Get_NetId() < b.second.Get_NetId();
            if(type == "student_name") return a.second.Get_Name() < b.second.Get_Name();
            if(type == "student_number") return a.second.Get_StuNum() < b.second.Get_StuNum();
            return false;
        },
        [](Student& s) {
            return s.ExportStudent_to_simplejson();
        }
    );
}

nlohmann::json College::SearchQuery(const std::string& str) { // it not work 
    std::map<std::string,Course> cs; 
    for (auto& pair : (*Courses)) {
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
    return SearchEntities(
        (*Courses),
        [&](const std::string& id, Course& c) {
            if (type == "fuzzy") {
                std::string p = c.Get_ClassName() + " " + c.Get_Teacher() + " " + c.Get_Teacher();
                auto it = std::search(p.begin(), p.end(), std::boyer_moore_searcher(str.begin(), str.end()));
                return it != p.end();
            }
            if (type == "class_name") return c.Get_ClassName() == str;
            if (type == "teacher_name") return c.Get_Teacher() == str;
            if (type == "id") return id == str;
            return false;
        },
        [](Course& c) {
            return c.ExportCourse_to_simplejson();
        }
    );
}

nlohmann::json College::SearchStuQuery(const std::string& str, const std::string& type) {
    return SearchEntities(
        (*StudentRoll),
        [&](const std::string& id, Student& s) {
            if (type == "fuzzy") {
                std::string p = s.Get_NetId() + " " + s.Get_Name() + " " + s.Get_Name();
                auto it = std::search(p.begin(), p.end(), std::boyer_moore_searcher(str.begin(), str.end()));
                return it != p.end();
            }
            if (type == "netid") return s.Get_NetId() == str;
            if (type == "student_name") return s.Get_Name() == str;
            if (type == "student_number") return s.Get_Name() == str;
            return false;
        },
        [](Student& s) {
            return s.ExportStudent_to_simplejson();
        }
    );
}

nlohmann::json College::SearchSelectStudent(const std::string& stunum) {
    Student stu;
    for(auto& i : (*StudentRoll)) {
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
        std::string cname = (*Courses)[i.first].Get_ClassName();
        nj["chart_data"]["grades_history"].push_back({{"course_name",cname},{"score",i.second[0]}});
        nj["chart_data"]["current_semester_gpa"].push_back({{"course_name",cname},{"credit",i.second[1]},{"gpa",i.second[2]}});
    }
    return nj;
}








