#include "Parser.h"

Parser::Parser(const char *sql) : sql(sql) {
    this->root = new ParserNode(0, "QUERY PLAN");
}


int Parser::callback(void *data, int argc, char **argv, char **azColName) {
    // int i;
    // for(i = 0; i<argc; i++) {
    //     printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    // }

    assert(strcmp(azColName[0], "id") == 0);
    assert(strcmp(azColName[1], "parent") == 0);
    assert(strcmp(azColName[2], "notused") == 0);
    assert(strcmp(azColName[3], "detail") == 0);
    assert(argv[0]);
    assert(argv[1]);
    assert(argv[2]);
    assert(argv[3]);


    int id = std::stoi(argv[0]);
    int parentId = std::stoi(argv[1]);
    std::string detail(argv[3]);

    // std::cout << id << std::endl;
    // std::cout << parentId << std::endl;
    // std::cout << detail << std::endl;

    auto root = (ParserNode *)data;
    auto curr = new ParserNode(id, detail);
    auto parent = root->findNode(parentId);

    parent->addChild(curr);

    return 0;
}

void Parser::parseQuery(const char* filename) {
    sqlite3 *db;
    char *zErrMsg = nullptr;
    int rc;

    rc = sqlite3_open(filename, &db);
    if (rc != 0) {
        std::cerr << "file not found!" << std::endl;
        exit(1);
    }

    rc = sqlite3_exec(db, this->sql, Parser::callback, this->root, &zErrMsg);

    sqlite3_close(db);
}

std::string Parser::toString() {
    std::string str = "\n---------- print plan ----------\n";
    return str + this->root->toString(0);
}

