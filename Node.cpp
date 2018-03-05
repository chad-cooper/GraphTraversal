// Chad Cooper
// Ryan Gray
// Anna McMaster

#include "Node.hpp"

Arrow::Arrow(int r, int c, char colour, string dir){
    row = r; // Stores the row
    column = c; // Stores the column
    color = colour; 
    direction = dir;
    
    anchor = false;
    nextLead = 0;
    
    // Because I was too lazy to overload << operator
    index = "[" + to_string(row) + "," + to_string(column) + "]";
    definition += color;
    definition += "-" + direction;
}

// Accessors

const string Arrow::getIndex(){return index;}

const string Arrow::getDefinition(){return definition;}

const string Arrow::getDirection(){return direction;}

const char Arrow::getColor(){return color;}

const int Arrow::getRow(){return row;}

const int Arrow::getColumn(){return column;}

vector<Arrow*>* Arrow::getLeads_To(){ // Retruns address of leads_to list.
    return &leads_to;
}


const vector<Arrow*>* Arrow::getLead_From(){// Returns address of lead_from list.
    return  &lead_from;
}

// Color is correct if it doesn't match this color.
const bool Arrow::correctColor(char currentColor){
    if(currentColor != this->color){
        return true;
    } else {
        return false;
    }
}

const int Arrow::getNextLead(){return nextLead;}

const bool Arrow::isAnchor(){return anchor;}

// Verifies if the current arrow leads to the argument arrow.
const bool Arrow::leadsTo(Arrow* arrow){
    vector<Arrow*>::iterator vecIt = this->leads_to.begin();
    for (; vecIt < this->leads_to.end() ; vecIt++) {
        if(*vecIt == arrow){
            return true;
        }
    }
    return false;
}

// Mutators

void Arrow::addOutgoingEdge(Arrow* outEdge){
    leads_to.push_back(outEdge);
}
void Arrow::addIncomingEdge(Arrow* inEdge){
    lead_from.push_back(inEdge);
}

void Arrow::setAnchor(bool val){
    anchor = val;
}

void Arrow::setNextLead(int val){
    nextLead = val;
}
