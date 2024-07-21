#ifndef SPRINTER_SPRINT_H
#define SPRINTER_SPRINT_H

#include <vector>
#include <string>
using namespace std;

struct feat{
    string name;
    string sp;
    string BE_WD;
    string Mob_WD;
    string QC_C;
    string QC_E;
    string QC_T;
    string QC_A;

    feat(const string &name, const string &sp, const string &beWd, const string &mobWd, const string &qcC,
         const string &qcE, const string &qcT, const string &qcA);
    //QC is always true
};

class sprint {
    vector<feat> features;
    int sprintVel;

public:
    //Constructor which takes sprint velocity
    sprint(int vel);
    //Reads in work flow document and creates sprint according to sprint velocity
    //Outputs features chosen for sprint
    void getFeat();
    void displayFeat();
    string getFirstWord(const string& str);
};


#endif //SPRINTER_SPRINT_H
