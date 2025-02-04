#include "../header/CommandHandler.h"
#include "../header/Network.h"

using namespace std;

int main()
{
    Network* network = new Network();
    CommandHandler commandHandler = CommandHandler(network);
    string command;
    while(getline(cin, command))
    {
        try
        {
            commandHandler.getQuery(command);
        }
        catch(...){}
    }
    return 0;
}