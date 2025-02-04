#ifndef NETWORK_H_
#define NETWORK_H_

#include <string>
#include <vector>
#include <math.h>
#include <iostream>

#define inf 1e9

#define MAX_VAL 9999
#define NODE_NUM 13
#define INVALID -1

class Network
{
public:
    Network();
    void implementCN(std::vector <std::vector<int>> network);
    void showTopology();
    void modifyCost(int src, int dst, int cost);
    void runLSRP(int src);
    void runDVRP(int src);
    void runBGP();
    void removeConnection(int src, int dst);
    int  minDistance(int distance[], bool visited[]);
    
private:
    void updateCNSize();

    void printSolutionLSRP(int distance[]);
    void printSolution(int src, int distance[], int parent[]);
    std::string printPath(int parent[], int j);
    void specialPrinter(int limit, std::string context);
    
    std::vector<std::vector<int>> sizes;
    std::vector <std::vector<int>> network; 

    int showLength=-1; // for show table, max digit count of numbers for shaping table 
};


#endif