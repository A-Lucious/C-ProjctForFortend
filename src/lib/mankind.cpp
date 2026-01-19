#include<mankind.h>

Mankind::Mankind() {};
Mankind::~Mankind() {};

std::string Mankind::Get_Name() const {
    return name;
}
int Mankind::Get_Age() const {
    return age;
}
std::string Mankind::Get_Sex() const {
    return sex;
}
void Mankind::Set_Name(const std::string& newName) {
    name = newName;
}
void Mankind::Set_Age(const int& newAge) {
    age = newAge;
}
void Mankind::Set_Sex(const std::string& newSex) {
    sex = newSex;
}

Mankind& Mankind::operator=(const Mankind& other) {
    if (this != &other) {
        this->name = other.name;
        this->sex = other.sex;
        this->age = other.age;
    }
    return *this;
}

Teacher::Teacher() {};
Teacher::~Teacher() {};

Teacher& Teacher::operator=(const Teacher& other) {
    if (this != &other) {
        this->name = other.name;
        this->sex = other.sex;
        this->age = other.age;
    }
    return *this;
}

Student::Student() {};
Student::Student(const nlohmann::json& data, std::map<std::string,Course>& Courses) {
    ImportStudent(data,Courses);
};
Student::~Student() {};

std::string Student::Get_StuNum() const {
    return stunum;
}
std::string Student::Get_NetId() const {
    return net_id;
}
int Student::Get_GradeClass() const {
    return grade_class;
}
std::string Student::Get_CollegeName() const {
    return college_name;
}
std::string Student::Get_Password() const {
    return password;
}
int Student::Get_Ass() const {
    return ass;
}
int Student::Get_NeedCredit() const {
    return needcredit;
}
int Student::Get_MaxGPA() const {
    return maxGPA;
}
int Student::Get_CurrentCredit() const {
    return current_credit;
}
std::map<std::pair<int,int> ,float> Student::Get_CurrentGPA() const {
    return current_GPA;
}
float Student::Get_MeanGPA() const {
    float meanGPA = 0.0;
    int n = 0;
    for(const auto&g:current_GPA) {
        n++;
        meanGPA += g.second; 
    }
    return meanGPA/n;
}
std::pair<std::string,std::string> Student::Get_Been() const {
    return been;
}
std::pair<int,int> Student::Get_CurrentSemester() const {
    return current_semester;
}
std::list<std::pair<std::pair<int,int>,std::string>> Student::Get_CourseIds() const {
    return course_ids;
}
std::map<std::string,std::array<float,3>> Student::Get_ScoresCredit() const {
    return scores_credit_GPA;
}
std::array<float,3> Student::Get_OneScoresCredit(std::string course_id) const {
    return scores_credit_GPA.at(course_id);
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
void Student::Set_ScoresCredit(const nlohmann::json& item) {
    scores_credit_GPA = item;
    Set_CurrentCredit(false,0);
    Set_CurrentGPA();
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

void Student::ImportStudent(const nlohmann::json& data, std::map<std::string,Course>& Courses) {
    name = data["name"];
    age = data["age"];
    sex = data["sex"];
    stunum = data["stunum"];
    net_id = data["net_id"];
    grade_class = data["grade_class"];
    college_name = data["college_name"];
    password = data["password"];
    ass = 0; // maybe cant modify
    needcredit = data["needcredit"];
    maxGPA = data["maxGPA"];
    been = {data["been"][0],data["been"][1]};
    auto courses = data["course_ids"];
    course_ids.clear();
    for(auto& c:courses) {
        std::pair<std::pair<int,int>,std::string> cid = {{c[0],c[1]},c[2]};
        course_ids.push_back(cid);
    }
    auto courses_scores = data["course_scores"];
    current_credit = 0;
    scores_credit_GPA.clear();
    std::map<std::pair<int,int>,std::vector<float>> sgpas;
    std::map<std::pair<int,int>,float> totalgpa;
    for(auto& i:courses_scores) {
        auto it = Courses.find(i[0]);
        Course c;
        if (it != Courses.end()) {
            c = Courses[i[0]];
        }else {
            continue;
        }
        float gpa = c.Get_GPA()*((float)i[1]/100.0);
        scores_credit_GPA[i[0]] = {i[1],(float)c.Get_Credit(),gpa};
        current_credit += c.Get_Credit();
        sgpas[c.Get_Semester()].push_back(gpa);
        totalgpa[c.Get_Semester()] += gpa;
    }
    current_semester = data["current_semester"];
    current_GPA[current_semester] = 0.0;
    for(auto& i:sgpas) {
        float len = (float)i.second.size();
        current_GPA[i.first] = totalgpa[i.first]/len;
    }
}

void Student::ReFresh(const nlohmann::json& data,std::map<std::string,Course>& Courses) {
    auto courses_scores = data["course_scores"];
    current_credit = 0;
    scores_credit_GPA.clear();
    std::map<std::pair<int,int>,std::vector<float>> sgpas;
    std::map<std::pair<int,int>,float> totalgpa;
    for(auto& [key, value]:courses_scores.items()) {
        auto it = Courses.find(key);
        Course c;
        if (it != Courses.end()) {
            c = Courses[key];
        }else {
            continue;
        }
        float gpa = c.Get_GPA()*((float)value[0]/100.0);
        scores_credit_GPA[key] = {value[0],(float)c.Get_Credit(),gpa};
        current_credit += c.Get_Credit();
        sgpas[c.Get_Semester()].push_back(gpa);
        totalgpa[c.Get_Semester()] += gpa;
    }
    current_semester = data["current_semester"];
    current_GPA[current_semester] = 0.0;
    for(auto& i:sgpas) {
        float len = (float)i.second.size();
        current_GPA[i.first] = totalgpa[i.first]/len;
    }
}

nlohmann::json Student::ExportStudent_to_simplejson() {
    nlohmann::json nj;
    nj["stunum"] = stunum;
    nj["name"] = name;
    nj["erldt"] = been.first;
    nj["gdtdt"] = been.second;
    return nj;
}

void to_json(nlohmann::json& j, const Student& s) {
    j["name"] = s.Get_Name();
    j["age"] = s.Get_Age();
    j["sex"] = s.Get_Sex();
    j["stunum"] = s.Get_StuNum();
    j["net_id"] = s.Get_NetId();
    j["grade_class"] = s.Get_GradeClass();
    j["college_name"] = s.Get_CollegeName();
    j["password"] = s.Get_Password();
    j["needcredit"] = s.Get_NeedCredit();
    j["maxGPA"] = s.Get_MaxGPA();
    j["been"] = {s.Get_Been().first,s.Get_Been().second};
    j["course_ids"] = s.Get_CourseIds();
    for (auto& [key, value] : s.Get_ScoresCredit()) {
        j["course_scores"].push_back({key,value[0]});
    }
    j["current_semester"] = s.Get_CurrentSemester();

};

Student& Student::operator=(const Student& other) {
    if (this != &other) {
        this->name = other.name;
        this->stunum = other.stunum;
        this->net_id = other.net_id;
        this->password = other.password;
        this->needcredit = other.needcredit;
        this->maxGPA = other.maxGPA;
        this->been = other.been;
        this->course_ids = other.course_ids;
        this->scores_credit_GPA = other.scores_credit_GPA;
        this->current_semester = other.current_semester;
        this->current_credit = other.current_credit;
        this->current_GPA = other.current_GPA;
        this->sex = other.sex;
        this->age = other.age;
        this->college_name = other.college_name;
        this->grade_class = other.grade_class;
        this->ass = other.ass;
    }
    return *this;
}