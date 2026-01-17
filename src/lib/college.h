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
    std::map<std::string,Course> Courses;
    std::map<std::string,Student> StudentRoll;
    std::map<std::string,Teacher> TeacherRoll;
    College();
    ~College();

    std::map<std::string,Course> Get_Courses();
    std::map<std::string,Student> Get_StudentRoll();
    std::map<std::string,Teacher> Get_TeacherRoll();

    void Set_Courses(const std::string& Cname, const Course& newCourse);
    void Set_Student(const Student& newStudent);
    void Set_Teacher(const Teacher& newTeacher);

    void ImportCourses(const std::string& path);
    void ImportStudents(const std::string& path);
    void ImportTeachers();

    nlohmann::json ExportCourses_jsonsimple();
    nlohmann::json ExportStudents_jsonsimple();
    void ExportStudents();
    void ExportTeachers();

    nlohmann::json SortCoursesByID();
    nlohmann::json SortCoursesBySemester();

    nlohmann::json SearchQuery(const std::string& str);
    nlohmann::json SearchQuery(const std::string& str,const std::string& type);

};

#endif