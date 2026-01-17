#ifndef MANKIND_H
#define MANKIND_H

#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include<list>
#include<set>
#include<string>
#include<fstream>
#include<sstream>

#include<nlohmann/json.hpp>
#include<course.h>

class Mankind {
private:
    std::string name;
    int age;
    int sex;
public:
    Mankind();
    virtual ~Mankind();

    std::string Get_Name();
    int Get_Age();
    int Get_Sex();

    void Set_Name(const std::string& newName);
    void Set_Age(const int& newAge);
    void Set_Sex(const int& newSex);
};

class Teacher : public Mankind {
private:
public:
    std::string net_id;
    std::string password;
    int ass;
    
    Teacher();
    ~Teacher();
};

class Student : public Mankind {
private:
public:
    Student();
    ~Student();
    std::string net_id; //学生id
    int grade_class; //班级
    std::string college_name; //学院
    std::string password; //密码
    int ass; //权限
    int needcredit; //毕业所需学分
    int maxGPA; //最大绩点

    std::list<std::pair<std::pair<int,int>,std::string>> course_ids; //所选课程
    std::map<std::string,std::vector<bool>> checks; //课程对应的考勤
    std::map<std::string,float[3]> scores_credit_GPA; //课程对应的成绩与学分与绩点

    std::pair<int,int> current_semester; //当前学期
    int current_credit; //当前学分
    std::map<std::pair<int,int> ,float> current_GPA;  //每学期绩点
    
    std::string Get_NetId();
    int Get_GradeClass();
    std::string Get_CollegeName();
    std::string Get_Password();
    int Get_Ass();
    int Get_NeedCredit();
    int Get_MaxGPA();
    int Get_CurrentCredit();
    std::map<std::pair<int,int> ,float> Get_CurrentGPA();
    float Get_MeanGPA();
    std::pair<int,int> Get_CurrentSemester();
    std::list<std::pair<std::pair<int,int>,std::string>> Get_CourseIds(); 
    std::map<std::string,std::vector<bool>> Get_Checks(); 
    std::map<std::string,float[3]> Get_ScoresCredit();
    std::vector<bool> Get_OneChecks(std::string course_id);
    float* Get_OneScoresCredit(std::string course_id);

    void Set_NetId(const std::string& newid);
    void Set_GradeClass(const int& newClass);
    void Set_CollegeName(const std::string& newCname);
    void Set_PassWord(const std::string& newPW);
    void Set_Ass(const int& newAss);
    void Set_NeedCredit(const int& newNC);
    void Set_MAXGPA(const int& newMG);
    void Set_Course(const std::pair<std::pair<int,int>,std::string>& newCourse);
    void Set_Checks(const std::string& course_id, const bool& check, const int& indx);
    void Set_ScoresCredit(const std::string& course_id, const float& scores, const float& credit, const std::map<std::string,Course>& Courses);
    void Set_Semester(const int& S1,const int& S2);
    void Set_CurrentCredit(const bool& set, const int& credit);
    void Set_CurrentGPA();

    void ExportThings();
};

#endif
