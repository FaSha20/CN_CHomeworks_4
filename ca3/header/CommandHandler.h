#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_  "COMMAND_HANDLER_H"

#include "Exception.h"
#include "Network.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>

#define TOPOLOGY "topology"
#define SHOW "show"
#define LSRP "lsrp"
#define DVRP "dvrp"
#define MODIFY "modify"
#define REMOVE "remove"

#define D_SPLITOR '-' // dash(hyphen)
#define S_SPLITOR ' ' // space
#define QUERY_TYPE 0
#define FIRST_NODE 0
#define SECOND_NODE 1
#define COST 2
#define NO_TOPOLOGY -1
#define CN_NODE_COUNT 13
#define MODIFY_IDX 1
#define REMOVE_IDX 1
#define NO_ARG 1

class CommandHandler
{
public:
    CommandHandler(Network* nw) : network(nw) {}
    void getQuery(std::string query);
    
private:
    std::vector<std::string> splitLine(std::string &choosenService, char splitor);

    void diagnoseQuery();
    void lsrp();
    void dvrp();
    void remove();
    void checkTopology();
    void checkModify();

    std::vector<std::string> wordsOfLine;
    std::vector<std::vector<int>> route;
    std::vector<std::string> modifyInfo;
    Network* network;
};

#endif