#include <iostream>
#include <fstream>
#include<sstream>
using namespace std;
#include "sprint.h"


sprint::sprint(int vel){
    sprintVel = vel;
}

string sprint::getFirstWord(const string& str) {
    istringstream iss(str);
    string firstWord;
    iss >> firstWord; // Extract the first word
    return firstWord;
}

void sprint::getFeat(){
    //user should be able to input the file through frontend
    ifstream inFile;
    inFile.open("C:\\Users\\Haya\\Desktop\\Orange 2024\\sprinter-\\pList.csv");
    string row, fName, SP, be, mob, qcc, qce, qct, qca;
//    int SP_int, be_int, mob_int, qcc_int, qce_int, qct_int, qca_int;
    //Skipping first line (headers)
    getline(inFile, row);

    getline(inFile, row);

//    while (getline(inFile, row))
//    {
        stringstream linestream(row);

        getline(linestream, SP, ',');
        getline(linestream, fName, ',');
        getline(linestream, be, ',');
        getline(linestream, mob, ',');
        getline(linestream, qcc, ',');
        getline(linestream, qce, ',');
        getline(linestream, qct, ',');
        getline(linestream, qca, ',');

        if(SP == "?")
            SP = "-1";

        if(SP.empty())
            SP = "0";

        if(be.empty())
            be = "0";
        else
            be = getFirstWord(be);

        if(mob.empty())
            mob = "0";
        else
            mob = getFirstWord(mob);

        if(qcc.empty())
            qcc = "0";
        else
            qcc = getFirstWord(qcc);

        if(qce.empty())
            qce = "0";
        else
            qce = getFirstWord(qce);

        if(qct.empty())
            qct = "0";

        if(qca.empty())
            qca = "0";


//        SP_int = stoi(SP);
//        be_int = stoi(be);
//        mob_int = stoi(mob);
//        qcc_int = stoi(qcc);
//        qce_int = stoi(qce);
//        qct_int = stoi(qct);
//        qca_int = stoi(qca);

        feat featX(fName, SP, be, mob, qcc, qce, qct, qca);

        features.push_back(featX);
//    }
}

void sprint::displayFeat() {
    ofstream outFile;
    outFile.open("C:\\Users\\Haya\\Desktop\\Orange 2024\\sprinter-\\displayFile.txt");
    outFile << "SP   Feature   Mobile  QC Creation  QC Execution  QC Total  QC Automation" <<endl;
    for(int i = 0; i < features.size(); i++){
        outFile << features[i].sp << "\t" << features[i].name << "\t" << features[i].BE_WD << "\t" << features[i].Mob_WD << "\t" << features[i].QC_C << "\t" << features[i].QC_E << "\t" << features[i].QC_T << "\t" << features[i].QC_A <<endl;
    }
}


feat::feat(const string &name, const string &sp, const string &beWd, const string &mobWd, const string &qcC,
           const string &qcE, const string &qcT, const string &qcA) : name(name), sp(sp), BE_WD(beWd), Mob_WD(mobWd),
                                                                      QC_C(qcC), QC_E(qcE), QC_T(qcT), QC_A(qcA) {}
