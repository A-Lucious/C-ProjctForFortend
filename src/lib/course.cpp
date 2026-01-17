#include<course.h>
#include<mankind.h>

Course::Course(const nlohmann::json& data) {
    ImportCourse(data);
}; 
Course::Course() {};
Course::~Course() {};

std::string Course::Get_ID() {
    return id;
}
std::string Course::Get_Teacher() {
    return teacher;
}
int Course::Get_Credit() {
    return credit;
}
std::string Course::Get_ClassName() {
    return class_name;
}
float Course::Get_GPA() {
    return GPA;
}
std::pair<int,int> Course::Get_Semester() {
    return semester;
}
std::vector<std::pair<std::string,std::string>> Course::Get_Tls() {
    return tls;
}
void Course::Set_ID(const int& newID)  {
    id = newID;
}
void Course::Set_Teahcer(const std::string& newTeacher) {
    teacher = newTeacher;
}
void Course::Set_Credit(const int& newCredit) {
    credit = newCredit;
}
void Course::Set_Semester(std::pair<int,int> newSemester) {
    semester = newSemester;
}
void Course::Set_Tls(std::pair<std::string,std::string> newTls,int indx) {
    tls[indx] = newTls;
}

void Course::Add_Tls(std::pair<std::string,std::string> newtls) {
    tls.push_back(newtls);
}

void Course::ImportCourse(const nlohmann::json& data) { // import the info from json
    id = data["id"];
    teacher = data["teacherid"];
    credit = data["credit"];
    class_name = data["classname"];
    semester.first = data["semester"][0];
    semester.second = data["semester"][1];
    GPA = data["GPA"];
    auto tlses = data["tls"];
    tls.clear();
    for(const auto& t:tlses) {
        std::pair<std::string,std::string> tl = {t[0],t[1]}; 
        tls.push_back(tl);
    }
}

nlohmann::json Course::ExportCourse_to_simplejson() { // export the simple info 
    nlohmann::json nj;
    nj["id"] = id;
    nj["teacher"] = teacher;
    nj["classname"] = class_name;
    nj["semester"] = semester;
    return nj;
}

Course& Course::operator=(const Course& other) {
    if (this != &other) {
        id = other.id;
        teacher = other.teacher;
        credit = other.credit;
        class_name = other.class_name;
        GPA = other.GPA;
        semester = other.semester;
        tls = other.tls;
    }
    return *this;
}