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

    std::string Get_ID() const;
    std::string Get_Teacher() const;
    int Get_Credit() const;
    std::string Get_ClassName() const;
    float Get_GPA() const;
    std::pair<int,int> Get_Semester() const;
    std::vector<std::pair<std::string,std::string>> Get_Tls() const;

    void Set_ClassName(const std::string& str);
    void Set_ID(const int& newID);
    void Set_Teahcer(const std::string& newTeacher);
    void Set_Credit(const int& newCredit);
    void Set_Semester(std::pair<int,int> newSemester);
    void Set_Tls(std::pair<std::string,std::string> newTls,int indx);

    void ImportCourse(const nlohmann::json& data);
    nlohmann::json ExportCourse_to_simplejson();
    friend void to_json(nlohmann::json& j, const Course& c);
    void ImportTls();
    void ExportTls();
    
    void Add_Tls(std::pair<std::string,std::string> newtls);

    Course& operator=(const Course& other);

};

#endif