#ifndef COLLEGE_H
#define COLLEGE_H

#include<iostream>
#include<algorithm>
#include<vector>
#include<list>
#include<string>
#include<map>
#include<set>
#include<fstream>
#include<sstream>

#include<nlohmann/json.hpp>
#include<mankind.h>
#include<course.h>
#include<functions.h>


class College {
private:
public:
    std::map<std::string,Course>* Courses;
    std::map<std::string,Student>* StudentRoll;
    std::map<std::string,Teacher>* TeacherRoll;
    College();
    ~College();

    std::map<std::string,Course> Get_Courses();
    std::map<std::string,Student> Get_StudentRoll();
    std::map<std::string,Teacher> Get_TeacherRoll();

    void Set_Courses(const std::string& Cname, const Course& newCourse);
    void Set_Student(const Student& newStudent);
    void Set_Teacher(const Teacher& newTeacher);
    void SetCourse(const nlohmann::json& item);
    void SetStudent(const nlohmann::json& item); 
    void ReFresh(const nlohmann::json& item);

    void ImportCourses(const std::string& path);
    void ImportStudents(const std::string& path);
    void ImportTeachers(const std::string& path);

    nlohmann::json ExportCourses_jsonsimple(); // export the info json
    nlohmann::json ExportStudents_jsonsimple();
    nlohmann::json ExportCourse_josn(const std::string& id);
    nlohmann::json ExportStudent_json(const std::string& id);
    void ExportCourses(const std::string& path);
    void ExportStudents(const std::string& path);
    void ExportTeachers(const std::string& path);

    nlohmann::json SortCourses(const nlohmann::json& js, const std::string& type);
    nlohmann::json SortStudent(const nlohmann::json& js, const std::string& type);

    nlohmann::json SearchQuery(const std::string& str); //search
    nlohmann::json SearchQuery(const std::string& str,const std::string& type);
    nlohmann::json SearchStuQuery(const std::string& str,const std::string& type);

    nlohmann::json SearchSelectStudent(const std::string& stunum);

};

#endif