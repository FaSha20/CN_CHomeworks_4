#include "../headers/config.h"
#include "../headers/parser.h"

using namespace std;


Configuration::Configuration(string path)
{
    map<string, vector<string>> jsonMap = parseJson(path);


}


int Configuration::getPort() 
{
    return port;
}

string Configuration::getHost() 
{
    return host;
}

int main(){
    Configuration config("../config.json");
}