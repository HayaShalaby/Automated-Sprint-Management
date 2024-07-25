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
    inFile.open("C:\\Users\\Haya\\Desktop\\Orange 2024\\Sprinter\\pList.csv");
    string row, fName, SP, be, mob, qcc, qce, qct, qca;
    float SP_f, be_f, mob_f, qcc_f, qce_f, qct_f, qca_f;
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


        SP_f = stof(SP);
        be_f = stof(be);
        mob_f = stof(mob);
        qcc_f = stof(qcc);
        qce_f = stof(qce);
        qct_f = stof(qct);
        qca_f = stof(qca);

        feat featX(fName, SP_f, be_f, mob_f, qcc_f, qce_f, qct_f, qca_f);

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
    outFile.open("C:\\Users\\Haya\\Desktop\\Orange 2024\\Sprinter\\displayFile.txt", ios::app);

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
        //Converting to integer values for easier output
        int be = static_cast<int>(feature.BE_WD);
        int mob = static_cast<int>(feature.Mob_WD);
        int qcc = static_cast<int>(feature.QC_C);
        int qce = static_cast<int>(feature.QC_E);
        int qct = static_cast<int>(feature.QC_T);
        int qca = static_cast<int>(feature.QC_A);

        outFile << left << setw(columnWidths[0] + 2) << feature.name
             << setw(columnWidths[1] + 2) << to_string(feature.sp)
             << setw(columnWidths[2] + 2) << to_string(be)
             << setw(columnWidths[3] + 2) << to_string(mob)
             << setw(columnWidths[4] + 2) << to_string(qcc)
             << setw(columnWidths[5] + 2) << to_string(qce)
             << setw(columnWidths[6] + 2) << to_string(qct)
             << setw(columnWidths[7] + 2) << to_string(qca) << endl;
    }

    outFile << endl;

    outFile.close();
}

//void sprint::calcSprintDays(){
//    WDs = 0;
//
//    for(int i = 0; i < features.size(); i++){
//        WDs += (features[i].BE_WD + features[i].Mob_WD + features[i].QC_T / 4);
//    }
//
//    //Adding 3 days for bug fixing and regression
//    WDs += 3;
//}

int sprint::checkCapacity(int start, int col, float workingDays){
    bool available = true;

    //Check capacity within column
    for(int j = start; j < totalDays; j++){
        //If there's an empty cell, check consecutive cells
        if(sprintTable[j][col].empty()) {
            for (int k = j; k < workingDays - 1; k++) {
                if (!sprintTable[k][col].empty())
                    available = false;
                break;
            }

            return j;
        }
//        //If there's an empty half
//        else if(sprintTable[j][col].back() == '/'){
//
//        }
    }

    return -1;
}

void sprint::assignCells(int startIndex, float workingDays, int col, string featName){
    for(int i = startIndex; i < startIndex + workingDays; i++){
        sprintTable[i][col] = featName;
    }
}

void sprint::makeSprint() {
    int beIndex, mobIndex, qcIndex, beStart, mobStart, qcStart;
    //Unless there is a dependency, we start checking column from 0
    beStart = 0; //never something other than 0
    mobStart = 0;
    qcStart = 0;

    //Initialize table with empty strings
    for(int i = 0; i < totalDays; i++){
        for(int j = 0; j <9; j++){
            sprintTable[i][j] = "";
        }
    }

    //Loop over all features
    for (int i = 0; i < features.size(); i++) {
        //Unless there is a dependency, we start checking column from 0
        beStart = 0; //never something other than 0
        mobStart = 0;
        qcStart = 0;

        //Check capacity within QC for test cases
        qcIndex = checkCapacity(qcStart, 5, features[i].QC_C);
        if (qcIndex == -1) {
            qcIndex = checkCapacity(qcStart, 6, features[i].QC_C);
            if (qcIndex == -1) {
                qcIndex = checkCapacity(qcStart, 7, features[i].QC_C);
                if (qcIndex == -1) {
                    qcIndex = checkCapacity(qcStart, 8, features[i].QC_C);
                    assignCells(qcIndex, features[i].QC_C, 8, features[i].name);
                    //if none of the QC columns have capacity - exception - another if condition
                } else
                    assignCells(qcIndex, features[i].QC_C, 7, features[i].name);
            } else
                assignCells(qcIndex, features[i].QC_C, 6, features[i].name);
        } else {
            assignCells(qcIndex, features[i].QC_C, 5, features[i].name);
        }


        //If the feature requires both backend and mobile development;
        if ((features[i].BE_WD) > 0 && (features[i].Mob_WD > 0)) {

            //Check capacity within BE column
            beIndex = checkCapacity(beStart, 0, features[i].BE_WD);
            assignCells(beIndex, features[i].BE_WD, 0, features[i].name);

            //Check capacity within Mob column
            mobStart = beIndex + features[i].BE_WD; //WITHOUT ADDING ONE
            mobIndex = checkCapacity(mobStart, 1, features[i].Mob_WD);
            if(mobIndex == -1){
                mobIndex = checkCapacity(mobStart, 2, features[i].Mob_WD);
                assignCells(mobIndex, features[i].Mob_WD, 2, features[i].name);
            }
            else
                assignCells(mobIndex, features[i].Mob_WD, 1, features[i].name);


            //Check capacity within QC
            qcStart = mobIndex + features[i].Mob_WD;
            qcIndex = checkCapacity(qcStart, 5, features[i].QC_E);
            if (qcIndex == -1) {
                qcIndex = checkCapacity(qcStart, 6, features[i].QC_E);
                if (qcIndex == -1) {
                    qcIndex = checkCapacity(qcStart, 7, features[i].QC_E);
                    if (qcIndex == -1) {
                        qcIndex = checkCapacity(qcStart, 8, features[i].QC_E);
                        assignCells(qcIndex, features[i].QC_E, 8, features[i].name);
                        //if none of the QC columns have capacity - exception - another if condition
                    } else
                        assignCells(qcIndex, features[i].QC_E, 7, features[i].name);
                } else
                    assignCells(qcIndex, features[i].QC_E, 6, features[i].name);
            } else
                assignCells(qcIndex, features[i].QC_E, 5, features[i].name);
        }
            //If the feature requires only mobile development
        else if ((features[i].BE_WD <= 0) && (features[i].Mob_WD > 0)) {
            //Check capacity within Mob column
            mobIndex = checkCapacity(mobStart, 1, features[i].Mob_WD);
            if(mobIndex == -1){
                mobIndex = checkCapacity(mobStart, 2, features[i].Mob_WD);
                assignCells(mobIndex, features[i].Mob_WD, 2, features[i].name);
            }
            else
                assignCells(mobIndex, features[i].Mob_WD, 1, features[i].name);

            //Check capacity within QC
            qcStart = mobIndex + features[i].Mob_WD;
            qcIndex = checkCapacity(qcStart, 5, features[i].QC_E);
            if (qcIndex == -1) {
                qcIndex = checkCapacity(qcStart, 6, features[i].QC_E);
                if (qcIndex == -1) {
                    qcIndex = checkCapacity(qcStart, 7, features[i].QC_E);
                    if (qcIndex == -1) {
                        qcIndex = checkCapacity(qcStart, 8, features[i].QC_E);
                        assignCells(qcIndex, features[i].QC_E, 8, features[i].name);
                        //if none of the QC columns have capacity - exception - another if condition
                    } else
                        assignCells(qcIndex, features[i].QC_E, 7, features[i].name);
                } else
                    assignCells(qcIndex, features[i].QC_E, 6, features[i].name);
            } else
                assignCells(qcIndex, features[i].QC_E, 5, features[i].name);
        }
            //If the feature requires only backend development
        else {
            //Check capacity within BE column
            beIndex = checkCapacity(beStart, 0, features[i].BE_WD);
            assignCells(beIndex, features[i].BE_WD, 0, features[i].name);

            //Check capacity within QC
            qcStart = beIndex + features[i].BE_WD;
            qcIndex = checkCapacity(qcStart, 5, features[i].QC_E);
            if (qcIndex == -1) {
                qcIndex = checkCapacity(qcStart, 6, features[i].QC_E);
                if (qcIndex == -1) {
                    qcIndex = checkCapacity(qcStart, 7, features[i].QC_E);
                    if (qcIndex == -1) {
                        qcIndex = checkCapacity(qcStart, 8, features[i].QC_E);
                        assignCells(qcIndex, features[i].QC_E, 8, features[i].name);
                        //if none of the QC columns have capacity - exception - another if condition
                    } else
                        assignCells(qcIndex, features[i].QC_E, 7, features[i].name);
                } else
                    assignCells(qcIndex, features[i].QC_E, 6, features[i].name);
            } else
                assignCells(qcIndex, features[i].QC_E, 5, features[i].name);
        }
    }

    //Copying Android to iOS
    for(int i = 0; i < totalDays; i++){
        //Android A to iOS A
        sprintTable[i][3] = sprintTable[i][1];

        //Android B to iOS B
        sprintTable[i][3] = sprintTable[i][1];
    }
}

void sprint::displaySprintFeat(){
    ofstream outFile;
    outFile.open("C:\\Users\\Haya\\Desktop\\Orange 2024\\Sprinter\\displayFile.txt", ios::app);

    // Define the width for each column
    const int colWidth = 12;

    // Print the header row (column indices)
    outFile << setw(colWidth) << "Day" << "|"
            << setw(colWidth) << "BE" << "|"
            << setw(colWidth) << "Android A" << "|"
            << setw(colWidth) << "Android B" << "|"
            << setw(colWidth) << "iOS A" << "|"
            << setw(colWidth) << "iOS B" << "|"
            << setw(colWidth) << "QC 1" << "|"
            << setw(colWidth) << "QC 2" << "|"
            << setw(colWidth) << "QC 3" << "|"
            << setw(colWidth) << "QC 4"
            << endl;

    // Separator line
    outFile << string(colWidth, '-') << "+";
    for (int j = 0; j < 8; ++j) {
        outFile << string(colWidth, '-') << "+";
    }
    outFile << string(colWidth, '-') << endl;

    // Print the 2D array with row indices and formatted as a table
    for (int i = 0; i < totalDays; ++i) {
        outFile << setw(colWidth) << i << "|"; // Print the row index
        for (int j = 0; j < 9; ++j) {
            outFile << setw(colWidth) << sprintTable[i][j] << "|";
        }
        outFile << endl;
    }

    outFile << endl;

    outFile.close();
}


feat::feat(const string &name, int sp, float beWd, float mobWd, float qcC, float qcE, float qcT, float qcA) : name(
        name), sp(sp), BE_WD(beWd), Mob_WD(mobWd), QC_C(qcC), QC_E(qcE), QC_T(qcT), QC_A(qcA) {}
