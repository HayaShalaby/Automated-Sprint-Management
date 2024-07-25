#ifndef SPRINTER_SPRINT_H
#define SPRINTER_SPRINT_H

#include <vector>
#include <string>
using namespace std;

struct feat{
    string name;
    int sp;
    float BE_WD;
    float Mob_WD;
    float QC_C;
    float QC_E;
    float QC_T;
    float QC_A;

    feat(const string &name, int sp, float beWd, float mobWd, float qcC, float qcE, float qcT, float qcA);
};

class sprint {
    vector<feat> features;
    vector<feat> validFeatures;
    int sprintVel;
//    int WDs;
    const int totalDays = 11;
    string sprintTable[11][9];

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
    int checkCapacity(int start, int col, float workingDays);
    void assignCells(int startIndex, float workingDays, int col, string featName);
    void makeSprint();
    void displaySprintFeat();
};


#endif //SPRINTER_SPRINT_H
