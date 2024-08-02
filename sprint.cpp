#include <iostream>
#include <fstream>
#include<sstream>
#include <iomanip>
#include <algorithm>
#include <climits>
using namespace std;
#include "sprint.h"


sprint::sprint(int vel){
    sprintVel = vel;
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

//Returns -1 if no capacity
int sprint::checkCapacity(int start, int col, float workingDays) {
    float WD = start + workingDays; // Calculate the target cell after adding workingDays
    // Check capacity within the column

    for (int j = start; j < totalDays; j++) {
        float remainingDays = workingDays; // Reset the remaining working days for each iteration
        int k = j;

        while (remainingDays > 0) {
            // If the current cell is empty
            if (sprintTable[k][col].empty()) {
                k++; // Move to the next row
                if(remainingDays == 0.5)
                    remainingDays -= 0.5;
                else
                    remainingDays--;
            }
                // If the current cell is half-empty
            else if (sprintTable[k][col].back() == '/') {
                remainingDays -= 0.5; // Decrease remaining working days by 0.5
                k++; // Move to the next row
            }
                // If the current cell is not empty
            else {
                break; // Break the loop and move to the next starting row
            }
        }

        // If we have successfully allocated the required working days
        if (remainingDays == 0) {
            return j; // Return the starting row where the allocation is successful
        }
    }

    return -1; // Return -1 if no suitable starting row is found
}


void sprint::assignCells(int startIndex, float workingDays, int col, string featName){
    int i = startIndex;
    float rowRange = startIndex + workingDays;

    while(i < rowRange && workingDays > 0){
        //If cell is empty and working days greater than half
        if(sprintTable[i][col].empty() && workingDays > 0.5) {
            sprintTable[i][col] = featName;
            workingDays--;
            i++;
        }
        //If cell is empty and the remaining working days is half
        else if(sprintTable[i][col].empty() && workingDays == 0.5) {
            sprintTable[i][col] = featName + '/';
            workingDays -= 0.5;
        }
        //If cell is half empty
        else if(sprintTable[i][col].back() == '/') {
            sprintTable[i][col] += featName;
            workingDays -= 0.5;
            i++;
        }
    }
}

void sprint::makeSprint() {
    int beIndex, mobIndex, mobIndex1, mobIndex2, qccIndex, qcIndex, beStart, mobStart, qcStart, col;
    int qcCounter = 5;
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

        if(qcCounter >=8)
            qcCounter = 5;

        //If the feature requires both backend and mobile development;
        if ((features[i].BE_WD) > 0 && (features[i].Mob_WD > 0)) {
            //Check capacity within BE column
            beIndex = checkCapacity(beStart, 0, features[i].BE_WD); //Add exception if -1 returned
            assignCells(beIndex, features[i].BE_WD, 0, features[i].name);

            //Check capacity within Mob column
            mobStart = beIndex + features[i].BE_WD; //WITHOUT ADDING ONE
            mobIndex1 = checkCapacity(mobStart, 1, features[i].Mob_WD);
            mobIndex2 = checkCapacity(mobStart, 2, features[i].Mob_WD); //Add exceptions for all cases
            if(mobIndex1 < mobIndex2){
                mobIndex = mobIndex1;
                col = 1;
            }
            else{
                mobIndex = mobIndex2;
                col = 2;
            }
            assignCells(mobIndex, features[i].Mob_WD, col, features[i].name);

            //Check capacity within QC
            qcStart = mobIndex + features[i].Mob_WD;
            qcIndex = checkCapacity(qcStart, qcCounter, features[i].QC_E);
            qccIndex = checkCapacity(0, qcCounter, features[i].QC_C);
            qcCounter++;
            //while the test cases only finds capacity after execution, assign next col
            while((qcIndex < qccIndex) && qcCounter < 9){
                qcIndex = checkCapacity(qcStart, qcCounter, features[i].QC_E);
                qccIndex = checkCapacity(0, qcCounter, features[i].QC_C);
                qcCounter++;
            }
            if(qcIndex >= qccIndex){
                assignCells(qccIndex, features[i].QC_C, qcCounter - 1, features[i].name + " TC");
                assignCells(qcIndex, features[i].QC_E, qcCounter - 1, features[i].name);
            }
            else{
                cout << "No sufficient capacity" <<endl;
            }

        }
            //If the feature requires only mobile development
        else if ((features[i].BE_WD <= 0) && (features[i].Mob_WD > 0)) {
            //Check capacity within Mob column
            mobIndex1 = checkCapacity(mobStart, 1, features[i].Mob_WD);
            mobIndex2 = checkCapacity(mobStart, 2, features[i].Mob_WD); //Add exceptions for all cases
            if(mobIndex1 < mobIndex2){
                mobIndex = mobIndex1;
                col = 1;
            }
            else{
                mobIndex = mobIndex2;
                col = 2;
            }
            assignCells(mobIndex, features[i].Mob_WD, col, features[i].name);

            //Check capacity within QC
            qcStart = mobIndex + features[i].Mob_WD;
            qcIndex = checkCapacity(qcStart, qcCounter, features[i].QC_E);
            qccIndex = checkCapacity(0, qcCounter, features[i].QC_C);
            qcCounter++;
            //while the test cases only finds capacity after execution, assign next col
            while((qcIndex < qccIndex) && qcCounter < 9){
                qcIndex = checkCapacity(qcStart, qcCounter, features[i].QC_E);
                qccIndex = checkCapacity(0, qcCounter, features[i].QC_C);
                qcCounter++;
            }

            if(qcIndex >= qccIndex){
                assignCells(qccIndex, features[i].QC_C, qcCounter - 1, features[i].name + " TC");
                assignCells(qcIndex, features[i].QC_E, qcCounter - 1, features[i].name);
            }
            else{
                cout << "No sufficient capacity" <<endl;
            }
        }
            //If the feature requires only backend development
        else {
            //Check capacity within BE column
            beIndex = checkCapacity(beStart, 0, features[i].BE_WD);
            assignCells(beIndex, features[i].BE_WD, 0, features[i].name);

            //Check capacity within QC
            qcStart = beIndex + features[i].BE_WD;
            qcIndex = checkCapacity(qcStart, qcCounter, features[i].QC_E);
            qccIndex = checkCapacity(0, qcCounter, features[i].QC_C);
            qcCounter++;
            //while the test cases only finds capacity after execution, assign next col
            while((qcIndex < qccIndex) && qcCounter < 9){
                qcIndex = checkCapacity(qcStart, qcCounter, features[i].QC_E);
                qccIndex = checkCapacity(0, qcCounter, features[i].QC_C);
                qcCounter++;
            }

            if(qcIndex >= qccIndex){
                assignCells(qccIndex, features[i].QC_C, qcCounter - 1, features[i].name + " TC");
                assignCells(qcIndex, features[i].QC_E, qcCounter - 1, features[i].name);
            }
            else {
                cout << "No sufficient capacity" << endl;
            }
        }
    }

    //Copying Android to iOS
    for(int i = 0; i < totalDays; i++){
        //Android A to iOS A
        sprintTable[i][3] = sprintTable[i][1];

        //Android B to iOS B
        sprintTable[i][4] = sprintTable[i][2];
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
        outFile << setw(colWidth) << i + 1 << "|"; // Print the row index
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
