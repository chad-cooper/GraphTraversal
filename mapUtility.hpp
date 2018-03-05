// Chad Cooper
// Ryan Gray
// Anna McMaster

#ifndef mapUtility_h
#define mapUtility_h

typedef map<pair<int, int>, Arrow> ArrowMap;
typedef ArrowMap::iterator ArrowPtr;

// Print an individual arrow
void printArrow(ArrowPtr& arrow){
    cout << arrow->second.getIndex() << "\t" << arrow->second.getDefinition() << endl;
}

// Print the entire graph.
void printMap(ArrowMap &myMap){
    for(ArrowPtr mapIt = myMap.begin(); mapIt != myMap.end(); mapIt++){
        printArrow(mapIt);
    }
}


/* This will traverse differently for all directions,
 as well as add the edges to the arrows adjacency lists. */
void travelDirection(ArrowMap &myMap, ArrowPtr &arrow, int totalRows, int totalColumns){
    totalRows = totalRows - 1; // Decremented for indexing purposes
    totalColumns = totalColumns -1;
    int row = arrow->second.getRow();
    int col = arrow->second.getColumn();
    
    string dir = arrow->second.getDirection();
    
    // Sadly there is no string switch statement in C++....
    if(dir.compare("N") == 0){
        for(int k = row; k >=0; k--){
            ArrowPtr checkArrow = myMap.find(make_pair(k, col));
            if(arrow->second.correctColor(checkArrow->second.getColor())){
                arrow->second.addOutgoingEdge(&(checkArrow->second));
                checkArrow->second.addIncomingEdge(&(arrow->second));
            }
        }
    } else if(dir.compare("NE") == 0){
        int remRows = row;
        int remCols = col;
        while(remRows >= 0 && remCols <= totalColumns){
            ArrowPtr checkArrow = myMap.find(make_pair(remRows, remCols));
            if(arrow->second.correctColor(checkArrow->second.getColor())){
                arrow->second.addOutgoingEdge(&(checkArrow->second));
                checkArrow->second.addIncomingEdge(&(arrow->second));
            }
            remRows--;
            remCols++;
        }
    } else if(dir.compare("E") == 0){
        for(int k = col; k <= totalColumns; k++){
            ArrowPtr checkArrow = myMap.find(make_pair(row, k));
            if(arrow->second.correctColor(checkArrow->second.getColor())){
                arrow->second.addOutgoingEdge(&(checkArrow->second));
                checkArrow->second.addIncomingEdge(&(arrow->second));
            }
        }
    } else if(dir.compare("SE") == 0){
        int remRows = row;
        int remCols = col;
        while(remRows <= totalRows && remCols <= totalColumns){
            ArrowPtr checkArrow = myMap.find(make_pair(remRows, remCols));
            if(arrow->second.correctColor(checkArrow->second.getColor())){
                arrow->second.addOutgoingEdge(&(checkArrow->second));
                checkArrow->second.addIncomingEdge(&(arrow->second));
            }
            remRows++;
            remCols++;
        }
    } else if(dir.compare("S") == 0){
        for(int k = row; k <= totalRows; k++){
            ArrowPtr checkArrow = myMap.find(make_pair(k, col));
            if(arrow->second.correctColor(checkArrow->second.getColor())){
                arrow->second.addOutgoingEdge(&(checkArrow->second));
                checkArrow->second.addIncomingEdge(&(arrow->second));
            }
        }
    } else if(dir.compare("SW") == 0){
        int remRows = row; // Starting position
        int remCols = col;
        while(remRows <= totalRows && remCols >= 0){
            ArrowPtr checkArrow = myMap.find(make_pair(remRows, remCols));
            if(arrow->second.correctColor(checkArrow->second.getColor())){
                arrow->second.addOutgoingEdge(&(checkArrow->second));
                checkArrow->second.addIncomingEdge(&(arrow->second));
            }
            remRows++;
            remCols--;
        }
    } else if(dir.compare("W") == 0){
        for(int k = col; k >= 0; k--){
            ArrowPtr checkArrow = myMap.find(make_pair(row, k));
            if(arrow->second.correctColor(checkArrow->second.getColor())){
                arrow->second.addOutgoingEdge(&(checkArrow->second));
                checkArrow->second.addIncomingEdge(&(arrow->second));
            }
        }
    } else if(dir.compare("NW") == 0){
        int remRows = row; // Starting position
        int remCols = col;
        while(remRows >= 0 && remCols >= 0){
            ArrowPtr checkArrow = myMap.find(make_pair(remRows, remCols));
            if(arrow->second.correctColor(checkArrow->second.getColor())){
                arrow->second.addOutgoingEdge(&(checkArrow->second));
                checkArrow->second.addIncomingEdge(&(arrow->second));
            }
            remRows--;
            remCols--;
        }
    }
    
} // End travelDirection

/* Build directed edges. For each arrow, travel in the direction it points
 and add valid arrows.*/
void buildDirectedEdges(ArrowMap &myMap, int totalRows, int totalColumns){
    for(ArrowPtr mapIt = myMap.begin(); mapIt != myMap.end(); mapIt++){
        travelDirection(myMap, mapIt, totalRows, totalColumns);
    }
    
}


/* Path functions */

// Find the last arrow that is an anchor.
pair<int, int> rewindToLastAnchor(ArrowMap& arrGraph, vector<pair<int, int>> &path){
    vector<pair<int, int>>::reverse_iterator rIt = path.rbegin();
    for(; rIt < path.rend(); rIt++){
        ArrowPtr curArr = arrGraph.find(*rIt);
        if(curArr->second.isAnchor()){
            return curArr->first;
        }
    }
    return make_pair(0, 0); // This will never happen.
}

// Scans vector from end to beginning, looking for matching index.
// If the coordinate is already in the vector, return true.
bool checkVec(vector<pair<int, int>> &path, pair<int, int> coord){
    for(vector<pair<int, int>>::reverse_iterator rIt = path.rbegin();
        rIt < path.rend(); rIt++){
        if(*(rIt) == coord)
            return true;
    }
    return false;
}

// Checks if the search is finished.
bool finished(ArrowMap& arrGraph, vector<pair<int, int>>& path){
    if(arrGraph.find(path.back())->second.getColor() == 'O'){
        return true;
    }
    return false;
}

/* Build path uses a DFS to find the first path (without any cycles) from [0,0] to the bullseye.
    If an arrow has multiple paths, it becomes an anchor. The first available
    path from an anchor is tried, and if this route yields nothing (e.g., it
    results in a cycle or leads out of the graph), then the next available path
    from the last anchor is tried. If all paths from an anchor are exhausted,
    then that arrows anchor status is removed, and the path returns to next previous anchor.
    This continues until the bullseye is reached.
 */
void buildPath(ArrowMap& arrGraph, vector<pair<int, int>>& path){
    // If a path has already been found, do nothing and return.
    if(finished(arrGraph, path)){
        return;
    }
    
    // Progress from the last arrow in the path.
    ArrowPtr curArrow = arrGraph.find(path.back());
    
    // The number of potential paths from the current arrow
    int potentialPaths;
 
    // If There are multiple paths from this arrow, anchor it.
    if(curArrow->second.getLeads_To()->size() > 1){
        curArrow->second.setAnchor(true);
        potentialPaths = curArrow->second.getLeads_To()->size();
    } else if(curArrow->second.getLeads_To()->size() == 1) {
        potentialPaths = 1;
    } else {
        potentialPaths = 0; // This arrow leads nowhere.
    }
    
    /* If the current arrow leads nowhere, do not add,
     and rewind to the last anchor.*/
    if(potentialPaths == 0){
        if(finished(arrGraph, path)){
            return; // Exits other recursive calls still running.
        }
        
        pair<int, int> lastAnchor = rewindToLastAnchor(arrGraph, path);
        while(path.back() != lastAnchor){
            path.pop_back(); // Remove coordinates until last anchor is reached.
        }
        
        // Continue from last anchor.
        curArrow = arrGraph.find(path.back());
        curArrow->second.setNextLead(curArrow->second.getNextLead()+1);
        potentialPaths = curArrow->second.getLeads_To()->size();
    }
    
    // While there are still paths available
    while(curArrow->second.getNextLead() < potentialPaths){
        if(finished(arrGraph, path)){
            return;
        }
        
        // Find the next arrow's coordinates
        vector<Arrow*> currentLeadsTo = *(curArrow->second.getLeads_To());
        Arrow* nextArrow = currentLeadsTo[curArrow->second.getNextLead()];
        int row = nextArrow->getRow();
        int col = nextArrow->getColumn();
        
        // If the next arrow is already in the path, do not add.
        if(checkVec(path, make_pair(row, col))){
            // continue with next possible path next loop
            curArrow->second.setNextLead(curArrow->second.getNextLead()+1);
            
            // Don't remove arrows that still have potential paths.
            if(curArrow->second.getNextLead() >= potentialPaths){
                path.pop_back();
            }
            continue;
        }
        
        // Add the arrow to path
        path.push_back(make_pair(row, col));
        
        // Recursive call
        buildPath(arrGraph, path);
        curArrow->second.setNextLead(curArrow->second.getNextLead()+1);
    }
    
    // Remove current arrow if it does not belong.
    if(path.back() == curArrow->first){
        path.pop_back();
    }
    
    // If all possible routes exhausted, this arrow is no longer an anchor.
    curArrow->second.setAnchor(false);
}


/* This function checks if the path that has been found is valid.
    Starting at [0,0], check if this node leads to the next node,
    then continue until the bullseye is reached.*/
bool pathIsValid(ArrowMap &arrGraph, vector<pair<int, int>> &path){
    vector<pair<int, int>>::iterator vecIt = path.begin();
    for (; vecIt < path.end()-1; vecIt++){
        ArrowPtr currentArrow = arrGraph.find(*vecIt);
        ArrowPtr nextArrow = arrGraph.find(*(vecIt + 1));
        if (!currentArrow->second.leadsTo(&nextArrow->second)) {
            return false;
        }
    }
    return true;
}

// Translates indices from the path vector into directions for output.
string translateDirections(vector<pair<int, int>> &path){
    int i, j, i2, j2, I, J;
    
    string directions;
    
    vector<pair<int, int>>::iterator vecIt = path.begin();
    for (; vecIt < path.end()-1; vecIt++){
        i = vecIt->first;
        j = vecIt->second;
        i2 = (vecIt+1)->first;
        j2 = (vecIt+1)->second;
        
        I = i2 - i;
        J = j2 - j;
        
        if(I < 0 && J == 0){
            directions += to_string(abs(I)) + "N ";
        } else if (I < 0 && J > 0){
            directions += to_string(J) + "NE ";
        } else if (I == 0 && J > 0){
            directions += to_string(J) + "E ";
        } else if (I > 0 && J > 0){
            directions += to_string(I) + "SE ";
        } else if(I > 0 && J == 0){
            directions += to_string(I) + "S ";
        } else if (I > 0 && J < 0){
            directions += to_string(I) + "SW ";
        } else if (I == 0 && J < 0){
            directions += to_string(abs(J)) + "W ";
        } else if (I < 0 && J < 0){
            directions += to_string(abs(I)) + "NW ";
        }
    }
    return directions;
}




#endif /* mapUtility_h */
