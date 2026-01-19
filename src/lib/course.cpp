#include<course.h>
#include<mankind.h>

Course::Course(const nlohmann::json& data) {
    ImportCourse(data);
}; 
Course::Course() {};
Course::~Course() {};

std::string Course::Get_ID() const{
    return id;
}
std::string Course::Get_Teacher() const{
    return teacher;
}
int Course::Get_Credit() const{
    return credit;
}
std::string Course::Get_ClassName() const{
    return class_name;
}
float Course::Get_GPA() const{
    return GPA;
}
std::pair<int,int> Course::Get_Semester() const{
    return semester;
}
std::vector<std::pair<std::string,std::string>> Course::Get_Tls() const{
    return tls;
}

void Course::Set_ClassName(const std::string& str) {
    class_name = str;
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

void to_json(nlohmann::json& j, const Course& c) {
    j["id"] = c.Get_ID();
    j["teacherid"] = c.Get_Teacher();
    j["credit"] = c.Get_Credit();
    j["classname"] = c.Get_ClassName();
    j["semester"] = c.Get_Semester();
    j["GPA"] = c.Get_GPA();
    j["tls"] = c.Get_Tls();
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