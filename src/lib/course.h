#ifndef COURSE_H
#define COURSE_H

#include<iostream>
#include<algorithm>
#include<vector>
#include<list>
#include<map>
#include<string>
#include<set>
#include<fstream>
#include<sstream>
#include<unordered_map>

#include<nlohmann/json.hpp>

class Course {
private:
public:
    std::string id;
    std::string teacher;
    int credit;
    std::string class_name;
    float GPA;
    std::pair<int,int> semester;
    std::vector<std::pair<std::string,std::string>> tls;

    Course();
    Course(const nlohmann::json& data);
    ~Course();

    std::string Get_ID();
    std::string Get_Teacher();
    int Get_Credit();
    std::string Get_ClassName();
    float Get_GPA();
    std::pair<int,int> Get_Semester();
    std::vector<std::pair<std::string,std::string>> Get_Tls();

    void Set_ID(const int& newID);
    void Set_Teahcer(const std::string& newTeacher);
    void Set_Credit(const int& newCredit);
    void Set_Semester(std::pair<int,int> newSemester);
    void Set_Tls(std::pair<std::string,std::string> newTls,int indx);

    void ImportCourse(const nlohmann::json& data);
    nlohmann::json ExportCourse_to_simplejson();
    void ImportTls();
    void ExportTls();
    
    void Add_Tls(std::pair<std::string,std::string> newtls);

    Course& operator=(const Course& other);

};

#endif