#include "../header/Network.h"
#include "../header/Exception.h"

using namespace std;

Network::Network()
{
    vector<vector<int>> lengths(NODE_NUM, vector<int>(NODE_NUM));
    sizes = lengths;
}

void Network::implementCN(std::vector <std::vector<int>> network_)
{
    network = network_;
}

void Network::updateCNSize()
{
    int maxi = 3;
    int nodeDigitCount = to_string(NODE_NUM).size();
    maxi = max(maxi, nodeDigitCount);

    for (int i = 0; i < NODE_NUM; i++)
        for (int j = 0; j < NODE_NUM; j++)
        {
            int digit = to_string(network[i][j]).size();
            sizes[i][j] = digit;
            maxi = max(maxi, digit);
        }
        
    showLength = maxi+2;    
}

void Network::showTopology()
{
    updateCNSize();

    printf("%*su|v%*s|", (showLength-3)/2, "", (showLength - 3 - (showLength-3)/2), "");
    for (int i=0 ; i < sizes.size() ; i++)
    {
        int digit = int(to_string(i+1).size());
        int first = (showLength-digit)/2;
        int sec = showLength-digit-first;
        printf("%*s%s%*s", (first), "", to_string(i+1).c_str(), (sec), "");
    }
    printf("\n");
    
    for(int i=0 ; i < (showLength * (NODE_NUM+1)) ; i++)
        printf("-");
    
    printf("\n");
    
    for (int j = 0; j < NODE_NUM; j++)
    {
        int digit = int(to_string(j+1).size());
        int first = (showLength-digit)/2;
        int sec = showLength-digit-first;
        printf("%*s%s%*s|", (first), "", to_string(j+1).c_str(), (sec), "");
        for (int i=0 ; i < NODE_NUM ; i++)
        {
            int first = (showLength-sizes[j][i])/2;
            int sec = showLength-sizes[j][i]-first;
            printf("%*s%s%*s", (first), "", to_string(network[j][i]).c_str(), (sec), "");
        }
        printf("\n");
    }

}

void Network::modifyCost(int src, int dst, int cost)
{
    network[src][dst] = cost;
    network[dst][src] = cost;
}

int Network::minDistance(int distance[], bool visited[])
{
    int min = MAX_VAL, min_index;
    for (int v = 0; v < NODE_NUM; v++)
        if (visited[v] == false && distance[v] <= min)
            min = distance[v], min_index = v;
    return min_index;
}

void Network::specialPrinter(int limit, string context)
{
    int digit = int(context.size());
    int first = (limit-digit)/2;
    int sec = limit-digit-first;
    printf("%*s%s%*s", (first), "", context.c_str(), (sec), "");
}

int GetNumberOfDigits (unsigned i)
{
    return i > 0 ? (int) log10 ((double) i) + 1 : 1;
}

void Network::printSolutionLSRP(int distance[])
{
    int maxi =0;
    for (int i = 0; i < NODE_NUM; i++)
    {
        int len = int(to_string(distance[i]).size());
        maxi = max(maxi, distance[i] == MAX_VAL ? 2 : len);
    }

    cout << "Dest  |";
    for (int i = 0; i < NODE_NUM; i++)
        specialPrinter(maxi+2, to_string(i+1)), printf("|");

    cout << endl << "Cost  |";
    for (int i = 0; i < NODE_NUM; i++)
        if(distance[i] != MAX_VAL)
            specialPrinter(maxi+2, to_string(distance[i])), printf("|");
        else
            specialPrinter(maxi+2, "-1"), printf("|");
    cout << endl;

    for (int i = 0; i < ((NODE_NUM)*(maxi+3))+7; i++)
        cout << "-";
    cout << endl;
    
}

string Network::printPath(int parent[], int j)
{
    if (parent[j] == -1)
        return to_string(j+1)+ "->";
    string prev = printPath(parent, parent[j]);
    string curr = prev + to_string(j+1) + "->";
    return curr;
}

void Network::printSolution(int src, int distance[],  int parent[])
{
    vector<string> solution(NODE_NUM);
    int spMaxCol = 0;
    int minMaxCol = 0;
    int pMaxCol = max(18, int(to_string(NODE_NUM).size()));

    parent[src] = -1;
    for (int i = 0; i < NODE_NUM; i++)
    {
        if(i == src)
            continue;
        solution[i] = printPath(parent, parent[i]); 
        solution[i] += to_string(i+1);
        spMaxCol = max(spMaxCol, int(solution[i].size()));
        minMaxCol = max(minMaxCol, int(to_string(distance[i]).size()));
    }

    spMaxCol = max(spMaxCol, 15);
    minMaxCol = max(minMaxCol, 10);    

    specialPrinter(pMaxCol, "Path: [s] -> [d]");
    specialPrinter(minMaxCol, "Min-Cost");
    specialPrinter(spMaxCol, "Shortest Path");
    printf("\n");

    specialPrinter(pMaxCol, "----------------");
    specialPrinter(minMaxCol, "--------");
    specialPrinter(spMaxCol, "-------------");
    printf("\n");

    for (int i = 0; i < NODE_NUM; i++) 
    {
        if(src == i)
            continue;
        string path_ = "[" + to_string(src+1) + "] -> [" + to_string(i+1) + "]";
        string cost_ = to_string(distance[i]);

        specialPrinter(pMaxCol, path_);
        specialPrinter(minMaxCol, cost_);
        specialPrinter(spMaxCol, solution[i]);
        printf("\n");
    }
    
}
 
void Network::runLSRP(int src)
{
    int distance[NODE_NUM]; 
    bool visited[NODE_NUM];
    int parent[NODE_NUM] = {-1};
    for (int i = 0 ; i < NODE_NUM ; i++)
        distance[i] = MAX_VAL, visited[i] = 0;
    distance[src] = 0;
 
    for (int count = 0 ; count < NODE_NUM - 1 ; count++)
    {
        //find index of node with min distance form src node
        int u = minDistance(distance, visited);
        visited[u] = true;
        for (int v = 0; v < NODE_NUM; v++)
            if (!visited[v] && network[u][v] != INVALID && distance[u] != MAX_VAL
                && distance[u] + network[u][v] < distance[v])
                {
                    distance[v] = distance[u] + network[u][v];
                    parent[v] = u;
                }
        cout << "Iter " << count << ":" << endl;
        printSolutionLSRP(distance);
    }
    printSolution(src, distance, parent);
}

void Network::runDVRP(int source)
{
    vector<pair<pair<int,int>,int>> edges;//<<source,dest>,weight>
    int dis[NODE_NUM];
    for (int i = 0 ; i < NODE_NUM ; i++)
        dis[i] = MAX_VAL; //distance from start/final vertex 

    //make collection of edges for Bellman Ford algurithm
    for(int i = 0; i<NODE_NUM ; i++)
        for (int j = 0; j < NODE_NUM; j++)
            if (i!=j and network[i][j] !=-1)
                edges.push_back(make_pair(make_pair(i,j), network[i][j]));

    dis[source] = 0;

    int edgCount = edges.size();
    int parent[NODE_NUM] = {-1};

    //Relaxtion code
    for(int i=1;i<NODE_NUM;i++)
    {
        for(int j=0;j<edgCount;j++)
        {
            int src (edges[j].first.first);
            int dest (edges[j].first.second);
            int wt (edges[j].second);
            //relaxtion check
            if(dis[src] != inf and  dis[src] + wt < dis[dest])
            {
                dis[dest] = dis[src] + wt;
                parent[dest] = src;
            }
        }
    } 

    printSolution(source, dis, parent);
}

void Network::removeConnection(int src, int dst)
{ 
    if (network[src][dst] == -1){
        throw NotFoundError();
    }  
    network[src][dst] = -1;
    network[dst][src] = -1;
}


    