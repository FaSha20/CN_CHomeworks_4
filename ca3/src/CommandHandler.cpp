#include "../header/CommandHandler.h"

using namespace std;
using namespace std::chrono;


vector<string> CommandHandler::splitLine(std::string &choosenService, char splitor)
{
    vector<string> words;
    stringstream iss(choosenService);
    string currentWord;

    while(getline(iss,currentWord,splitor))
        words.push_back(currentWord);

    return words;
}

void CommandHandler::checkTopology()
{
    vector<string> connection;
    int count = wordsOfLine.size();
    vector<vector<int>> link(CN_NODE_COUNT+1, vector<int>(CN_NODE_COUNT+1, -1));

    for (int i = 1; i < count; i++)
    {
        connection = splitLine(wordsOfLine[i], D_SPLITOR);

        if (connection[FIRST_NODE] == connection[SECOND_NODE])
            throw IDsAlike();
        int src = stoi(connection[FIRST_NODE]) -1;
        int des = stoi(connection[SECOND_NODE]) -1;
        int cost = stoi(connection[COST]);
        link[src][des] = cost;
        link[des][src] = cost;
    }
    for (int i = 0; i < CN_NODE_COUNT; i++)
        link[i][i] = 0;

    route = link;
}

void CommandHandler::checkModify()
{
    modifyInfo = splitLine(wordsOfLine[MODIFY_IDX], D_SPLITOR);

    if (modifyInfo[FIRST_NODE] == modifyInfo[SECOND_NODE])
        throw ModifySameID();
}

void CommandHandler::lsrp()
{
    if (wordsOfLine.size() == NO_ARG)
    {
        auto start = high_resolution_clock::now();
        for (int i = 0; i < CN_NODE_COUNT; i++)
        {
            cout << "lsrp for node "<< i+1<< ":" << endl;
            network->runLSRP(i);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken by LSRP function: "<< duration.count() << " microseconds" << endl;
    }
    else
    {
        network->runLSRP(stoi(wordsOfLine[NO_ARG])-1);
    }
}

void CommandHandler::dvrp()
{
    if (wordsOfLine.size() == NO_ARG)
    {
        auto start = high_resolution_clock::now();
        for (int i = 0; i < CN_NODE_COUNT; i++)
        {
            cout << "dvrp for node "<< i+1<< ":" << endl;
            network->runDVRP(i);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken by DVRP function: "<< duration.count() << " microseconds" << endl;
    }
    else
    {
        network->runDVRP(stoi(wordsOfLine[NO_ARG])-1);
    }

}


void CommandHandler::remove()
{
    vector<string> removeID = splitLine(wordsOfLine[REMOVE_IDX], D_SPLITOR);
    network->removeConnection(stoi(removeID[FIRST_NODE])-1, stoi(removeID[SECOND_NODE])-1);

}

void CommandHandler::diagnoseQuery()
{
    if (wordsOfLine[QUERY_TYPE] == TOPOLOGY){
        checkTopology();
        network->implementCN(route);
        cout << "OK\n";
    }
    else if (wordsOfLine[QUERY_TYPE] == SHOW)
        network->showTopology();
    else if (wordsOfLine[QUERY_TYPE] == MODIFY){
        checkModify();
        network->modifyCost(stoi(modifyInfo[FIRST_NODE])-1, stoi(modifyInfo[SECOND_NODE])-1, stoi(modifyInfo[COST]));
        cout << "OK\n";
    }
    else if (wordsOfLine[QUERY_TYPE] == REMOVE){
        vector<string> removeID = splitLine(wordsOfLine[REMOVE_IDX], D_SPLITOR);
        network->removeConnection(stoi(removeID[FIRST_NODE])-1, stoi(removeID[SECOND_NODE])-1);
        cout << "OK\n";
    }
    else if (wordsOfLine[QUERY_TYPE] == LSRP)
        lsrp() ;
    else if (wordsOfLine[QUERY_TYPE] == DVRP)
        dvrp() ;
    else
        throw BadRequestError();
}

void CommandHandler::getQuery(string query)
{
    stringstream iss(query);
    string currentWord;
    wordsOfLine = {} ;

    while(getline(iss,currentWord,S_SPLITOR))
        wordsOfLine.push_back(currentWord);

    if (wordsOfLine.empty())       
        throw BadRequestError();

   
    diagnoseQuery();
}
