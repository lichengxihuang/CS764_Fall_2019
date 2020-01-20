#ifndef SCHEDULER_PARSER_H
#define SCHEDULER_PARSER_H

#include <iostream>
#include "sqlite3.h"
#include "ParserNode.h"

class Parser {
private:
    ParserNode* root;
    const char* sql;

    static int callback(void *data, int argc, char** argv, char** azColName);

public:
    explicit Parser(const char* sql);

    void parseQuery(const char* filename);

    std::string toString();

};




#endif //SCHEDULER_PARSER_H
