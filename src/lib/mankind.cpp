#include<mankind.h>

Mankind::Mankind() {};
Mankind::~Mankind() {};

std::string Mankind::Get_Name() {
    return name;
}
int Mankind::Get_Age() {
    return age;
}
int Mankind::Get_Sex() {
    return sex;
}
void Mankind::Set_Name(const std::string& newName) {
    name = newName;
}
void Mankind::Set_Age(const int& newAge) {
    age = newAge;
}
void Mankind::Set_Sex(const int& newSex) {
    sex = newSex;
}

Teacher::Teacher() {};
Teacher::~Teacher() {};

Student::Student() {};
Student::~Student() {};

std::string Student::Get_NetId() {
    return net_id;
}
int Student::Get_GradeClass() {
    return grade_class;
}
std::string Student::Get_CollegeName() {
    return college_name;
}
std::string Student::Get_Password() {
    return password;
}
int Student::Get_Ass() {
    return ass;
}
int Student::Get_NeedCredit() {
    return needcredit;
}
int Student::Get_MaxGPA() {
    return maxGPA;
}
int Student::Get_CurrentCredit() {
    return current_credit;
}
std::map<std::pair<int,int> ,float> Student::Get_CurrentGPA() {
    return current_GPA;
}
float Student::Get_MeanGPA() {
    float meanGPA = 0.0;
    int n = 0;
    for(const auto&g:current_GPA) {
        n++;
        meanGPA += g.second; 
    }
    return meanGPA/n;
}
std::pair<int,int> Student::Get_CurrentSemester() {
    return current_semester;
}
std::list<std::pair<std::pair<int,int>,std::string>> Student::Get_CourseIds() {
    return course_ids;
}
std::map<std::string,std::vector<bool>> Student::Get_Checks() {
    return checks;
} 
std::map<std::string,float[3]> Student::Get_ScoresCredit() {
    return scores_credit_GPA;
}
std::vector<bool> Student::Get_OneChecks(std::string course_id) {
    return checks[course_id];
}
float* Student::Get_OneScoresCredit(std::string course_id) {
    return scores_credit_GPA[course_id];
}
void Student::Set_NetId(const std::string& newid) {
    net_id = newid;
}
void Student::Set_GradeClass(const int& newClass) {
    grade_class = newClass;
}
void Student::Set_CollegeName(const std::string& newCname) {
    college_name = newCname;
}
void Student::Set_PassWord(const std::string& newPW) {
    password = newPW;
}
void Student::Set_Ass(const int& newAss) {
    ass = newAss;
}
void Student::Set_NeedCredit(const int& newNC) {
    needcredit = newNC;
}
void Student::Set_MAXGPA(const int& newMG) {
    maxGPA = newMG;
}
void Student::Set_Course(const std::pair<std::pair<int,int>,std::string>& newCourse) {
    course_ids.push_back(newCourse);
}
void Student::Set_Checks(const std::string& course_id, const bool& check, const int& indx) {
    checks[course_id][indx] = check;
}
void Student::Set_ScoresCredit(const std::string& course_id, const float& scores, const float& credit, const std::map<std::string,Course>& Courses) {
    scores_credit_GPA[course_id][0] = scores;
    scores_credit_GPA[course_id][1] = credit;
    auto it = Courses.find(course_id);
    if(it != Courses.end()) {
        Course currentCs = it->second;
        scores_credit_GPA[course_id][2] = currentCs.Get_GPA()*(scores/100.0);
    }
    else {
        std::cout << "The course not exist in the coursesRoll" << std::endl;
        scores_credit_GPA[course_id][2] = -1.0;
    }

}
void Student::Set_Semester(const int& S1,const int& S2) {
    current_semester.first = S1;
    current_semester.second = S2;
}
void Student::Set_CurrentCredit(const bool& set=false, const int& credit=0) {
    if(set) {
        current_credit = credit;
    }
    else {
        current_credit = 0;
        for(const auto& i:scores_credit_GPA) {
            current_credit += i.second[0];
        }
    }
}
void Student::Set_CurrentGPA() {
    std::map<std::pair<int,int>,std::pair<int,float>> tmp;
    for(const auto& i:course_ids) {
        tmp[i.first].first += 1;
        tmp[i.first].second += scores_credit_GPA[i.second][2];
    }
    for(const auto& t:tmp) {
        current_GPA[t.first] = t.second.second/t.second.first;
    }
}

void ExportThings() {};