#ifndef SPRINTER_SPRINT_H
#define SPRINTER_SPRINT_H

#include <vector>
#include <string>
using namespace std;

struct feat{
    string name;
    int sp;
    int BE_WD;
    int Mob_WD;
    int QC_C;
    int QC_E;
    int QC_T;
    int QC_A;

    feat(const string &name, int sp, int beWd, int mobWd, int qcC, int qcE, int qcT, int qcA);
};

class sprint {
    vector<feat> features;
    vector<feat> validFeatures;
    int sprintVel;
    int WDs;
    vector<vector<string>> sprintTable;

public:
    //Constructor which takes sprint velocity
    sprint(int vel);
    //Reads in work flow document and creates sprint according to sprint velocity
    //Outputs features chosen for sprint
    void getAllFeat();
    void getValidFeat();
    void displayFeat();
    string getFirstWord(const string& str);
    void calcSprintDays();
    int checkCapacity(int start, int col, int workingDays);
    void makeSprint();
};


#endif //SPRINTER_SPRINT_H
