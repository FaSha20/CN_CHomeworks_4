#include "../include/Parser.hpp"
using namespace std;

bool hasCharacter(string line, char c) {
    return find(line.begin(), line.end(), c) != line.end() || 
        find(line.begin(), line.end(), c) != line.end();
}
void removeCharacter(string& str, char c){
    str.erase(remove(str.begin(), str.end(), c), str.end());
}

// !!!!!!works with customizable json file with any permutation!!!!!!!
map<string, vector<string>> parseJson(string fileName) {    
    ifstream file;
    file.open(fileName);
    string line, listString = "", field, value, dump;
    map <string, vector<string>> inputs;
    while(getline(file, line)) {
        stringstream ss(line);

        if(hasCharacter(line, '{') || hasCharacter(line, '}') || hasCharacter(line, ']')){
            listString = "";
            continue;
        }
        else if (hasCharacter(line, '[')){
            getline(ss, dump, '"');
            getline(ss, listString, '"');
        }
        else if (listString != ""){
            getline(ss, dump, '"');
            getline(ss, field, '"');
            inputs[listString].push_back(field);
        }
        else{
            getline(ss, dump, '"');
            getline(ss, field, '"');
            getline(ss, value, ',');
            removeCharacter(value, '"');
            removeCharacter(value, ' ');
            removeCharacter(value, ':');
            if(value != "")
                inputs[field].push_back(value);
        }
    }
    file.close();
    return inputs;
}