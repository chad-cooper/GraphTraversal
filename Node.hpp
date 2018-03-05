// Chad Cooper
// Ryan Gray
// Anna McMaster

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

class Arrow {
    int row, column, nextLead;
    string index, definition;
    char color;
    string direction;
    vector<Arrow*> leads_to;
    vector<Arrow*>  lead_from;
    bool anchor;
    
public:
    
    // Constructor
    Arrow(int r, int c, char colour, string dir);
    
    // Accessors
    const string getIndex();
    const string getDefinition();
    const string getDirection();
    const char getColor();
    const int getRow();
    const int getColumn();
    const bool isAnchor();
    const int getNextLead();
    
    vector<Arrow*>* getLeads_To(); // Returns pointer to leads_to list.
    const vector<Arrow*>* getLead_From(); // Returns pointer lead_from list.
    
    const bool correctColor(char currentColor);
    
    const bool leadsTo(Arrow* arrow);
    
    // Mutators
    
    void addOutgoingEdge(Arrow* outEdge);
    void addIncomingEdge(Arrow* inEdge);
    void setNextLead(int val);
    void setAnchor(bool val);
 
};

#endif /* Node_hpp */
