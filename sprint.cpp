#include <iostream>
#include <fstream>
#include<sstream>
#include <iomanip>
#include <algorithm>
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

void sprint::getAllFeat(){
    //user should be able to input the file through frontend
    ifstream inFile;
    inFile.open("C:\\Users\\Haya\\Desktop\\Orange 2024\\sprinter-\\pList.csv");
    string row, fName, SP, be, mob, qcc, qce, qct, qca;
    int SP_int, be_int, mob_int, qcc_int, qce_int, qct_int, qca_int;
    //Skipping first line (headers)
    getline(inFile, row);

    while (getline(inFile, row))
    {
        stringstream linestream(row);

        getline(linestream, SP, ',');
        if(SP == "?")
            continue;

        getline(linestream, fName, ',');
        getline(linestream, be, ',');
        getline(linestream, mob, ',');
        getline(linestream, qcc, ',');
        getline(linestream, qce, ',');
        getline(linestream, qct, ',');
        getline(linestream, qca, ',');


        SP_int = stoi(SP);
        be_int = stoi(be);
        mob_int = stoi(mob);
        qcc_int = stoi(qcc);
        qce_int = stoi(qce);
        qct_int = stoi(qct);
        qca_int = stoi(qca);

        feat featX(fName, SP_int, be_int, mob_int, qcc_int, qce_int, qct_int, qca_int);

        features.push_back(featX);
    }
}

void sprint::getValidFeat() {
    int size = features.size();
    for(int i = 0; i < size; i++){
        if((sprintVel - features[i].sp) >= 0){
            validFeatures.push_back(features[i]);
            sprintVel -= features[i].sp;
        }
    }

    features = validFeatures;
}


void sprint::displayFeat() {
    ofstream outFile;
    outFile.open("C:\\Users\\Haya\\Desktop\\Orange 2024\\sprinter-\\displayFile.txt", ios::app);

    vector<string> headers = {"Name", "SP", "BE_WD", "Mob_WD", "QC_C", "QC_E", "QC_T", "QC_A"};
    vector<int> columnWidths(headers.size(), 0);

    // Calculate width based on headers
    for (size_t i = 0; i < headers.size(); ++i) {
        columnWidths[i] = headers[i].size();
    }

    // Output table header
    for (size_t i = 0; i < headers.size(); ++i) {
        outFile << left << setw(columnWidths[i] + 2) << headers[i];
    }
    outFile << endl;

    // Separator line
    outFile << string(headers.size() * 12, '-') << endl;

    // Output table data
    for (const auto& feature : features) {
        outFile << left << setw(columnWidths[0] + 2) << feature.name
             << setw(columnWidths[1] + 2) << to_string(feature.sp)
             << setw(columnWidths[2] + 2) << to_string(feature.BE_WD)
             << setw(columnWidths[3] + 2) << to_string(feature.Mob_WD)
             << setw(columnWidths[4] + 2) << to_string(feature.QC_C)
             << setw(columnWidths[5] + 2) << to_string(feature.QC_E)
             << setw(columnWidths[6] + 2) << to_string(feature.QC_T)
             << setw(columnWidths[7] + 2) << to_string(feature.QC_A) << endl;
    }

    outFile << endl;

    outFile.close();
}


feat::feat(const string &name, int sp, int beWd, int mobWd, int qcC, int qcE, int qcT, int qcA) : name(name), sp(sp),
                                                                                                  BE_WD(beWd),
                                                                                                  Mob_WD(mobWd),
                                                                                                  QC_C(qcC), QC_E(qcE),
                                                                                                  QC_T(qcT),
                                                                                                  QC_A(qcA) {}
