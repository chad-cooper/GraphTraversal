// Chad Cooper
// Ryan Gray
// Anna McMaster

#include <iostream>
#include <fstream>
#include <map>
#include "Node.cpp"
#include "mapUtility.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    //check number of parameters
    if (argc != 3)
    {
        cout << "Usage: ./executable_name inputFile.txt outputFile.txt" << endl;
        return 1;
    }
    
    ifstream inputFile;
    inputFile.open(argv[1]);
    int numRows = 0, numColumns = 0;
    
    ArrowMap arrowGraph;
    
    if (inputFile.is_open()) {
        
        inputFile >> numRows;
        inputFile >> numColumns;
        
        string line, dir;
        char color;
        int dashPos; // arrow = 1;
        
        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numColumns; j++){
                
                getline(inputFile, line, ' ');// gets 'Color-Direction' chunk
                
                dashPos = line.find("-");
                if (dashPos != -1){
                    
                    
                    color = (line.substr(dashPos-1, 1))[0]; // converts color substr to char
                    dir = line.substr(dashPos+1);
                    
                } else {
                    color = 'O';
                    dir = 'O';
                    //cout << "Bullseye! Position:" << " [" << i << "," << j << "]" << endl;
                } // end_if
                
                Arrow newArrow(i, j, color, dir);
                arrowGraph.insert(make_pair(make_pair(i, j), newArrow));
                
            } // end for-numColumns
        } // end for-numRows
        
        inputFile.close();
    } else {
        return 1;
    }
    
    //printMap(arrowGraph);
    
    buildDirectedEdges(arrowGraph, numRows, numColumns);
    
    vector<pair<int, int>> path; // Holds the path
    
    path.push_back(arrowGraph.begin()->first); // Adds [0,0] to path.
    arrowGraph.begin()->second.setAnchor(true);

    buildPath(arrowGraph, path);
    
    //cout << "This path is " << (pathIsValid(arrowGraph, path)?"":"NOT ") << "valid." << endl;
    
    string output = translateDirections(path);
    
    //cout << output << endl;
    
    ofstream outputFile;
    outputFile.open(argv[2]);
    
    outputFile << output;

    outputFile.close();
    
    return 0;
}





