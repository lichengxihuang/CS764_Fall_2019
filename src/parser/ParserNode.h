#ifndef SCHEDULER_PARSERNODE_H
#define SCHEDULER_PARSERNODE_H


#include <iostream>

class ParserNode {
private:
    int id;
    std::string detail;
    ParserNode *next;
    ParserNode *child;

public:
    ParserNode(int id, std::string detail);

    ParserNode *findNode(int nodeId);

    void addChild(ParserNode *node);

    bool hasNext();

    bool hasChild();

    std::string toString(int indent);
};


#endif //SCHEDULER_PARSERNODE_H
