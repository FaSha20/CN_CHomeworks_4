#include "../include/Server.hpp"
using namespace std;

char SERVER_ABSOLUTE_PATH[BUFFER_SIZE];

Server::Server(map<string, vector<string>> inputs) {
        convertConfig(inputs);
        serverCmdFd = setupServer(cmdChannelPort);
        serverDataFd = setupServer(dataChannelPort);
        curr_log = "";
        getcwd(SERVER_ABSOLUTE_PATH, BUFFER_SIZE); 
        log_path = string(SERVER_ABSOLUTE_PATH) + LOG_FILE_NAME;
};  

int Server::getFileSize(string fileName) {
    ifstream in_file(fileName, ios::binary);
    in_file.seekg(0, ios::end);
    int file_size = in_file.tellg();
    return file_size;
}

string Server::handleQuit(User* currUser, int userFd) {
    fdLoggedInUser.erase(userFd);
    currUser->logout();
    return SUCCESSFUL_QUIT;
}

string Server::handleHelp() {
    string help;
    help += "214\n";
    help += "user [username]: This command is used to submit a username for authentication.\n\n";
    help += "pass [password]: This command is used to submit a password for the previously submitted username for authentication.";
    help += "This command must be executed directly after the \"user\" command.\n\n";
    help += "pwd: Show the path of the current working directoty.\n\n";
    help += "mkd [path]: Make a new directory in the specified path.\n\n";
    help += "dele [-f|-d] [file name| directory path]: Delete the file with the name or the directory in the path specified in its argument\n\n";
    help += "ls: Show all the files and folders in the current directory.\n\n";
    help += "cwd [path]: Change the current working directory and move to the specified path.\n\n";
    help += "rename [from] [to]: Change the name of the file given in the first argument to the name specified in the second argument.\n\n";
    help += "retr [file name]: Download the file with the specified name.\n\n";
    help += "help: Show the system commands and their descriptions.\n\n";
    help += "quit: Logout from the server.\n\n";
    return help;
}

string Server::handleRetr(User* currUser, string argument1, int dataFd)  {
    if(!hasFileAccess(currUser, argument1))
        return FILE_UNAVAILABLE;
    string fileString;
    string path = currUser->getRelativeDir(argument1);
    ifstream ifs(path);
    int fileSize = getFileSize(argument1);
    if(!currUser->canDownload(fileSize)) 
        return CANT_OPEN_DATA_CONNECTION;

    currUser->updateDataLimit(fileSize);
    stringstream buffer;
    buffer << ifs.rdbuf();
    if(!buffer.str().size())
        return SYNTAX_ERROR;
        
    fileString = buffer.str();
    string res = argument1 + ":\n" + fileString; 
    if(fileString == "")
        res = "";
    sendData(res, dataFd);
    return SUCCESSFUL_DOWNLOAD;
}

string Server::handleRename(User* currUser, string argument1, string argument2) {
    if(!hasFileAccess(currUser, argument1))
        return FILE_UNAVAILABLE;
    if(!rename(argument1.c_str(), argument2.c_str())) 
        return SUCCESSFUL_CHANGE;
    return SYNTAX_ERROR;
}

void Server::sendData(string file, int dataFd){
    send(dataFd, file.c_str(), strlen(file.c_str()), 0);
}

string Server::handleLs(User* currUser, int dataFd) {
    string filesString = "";
    DIR *dir; struct dirent *diread;
    vector<char *> files;
    if((dir = opendir(currUser->getCurrDir())) != nullptr) {
        while((diread = readdir(dir)) != nullptr) {
            files.push_back(diread->d_name);
        }
        closedir(dir);
    } else {
        perror("opendir");
        return ERROR;
    }
    for(auto file : files)
        filesString += "|" + string(file) + " ";
    sendData(filesString, dataFd);
    return LIST_TRANSFER_DONE;
}

string Server::handleCwd(User* currUser, string argument1) {
    if(argument1 == "") {
        if(currUser->resetDir())
            return SUCCESSFUL_CHANGE; 
        else
            return ERROR;
    }
    else if(currUser->updateDir(argument1)) 
        return SUCCESSFUL_CHANGE;

    return SYNTAX_ERROR;
}

string Server::handleFileDel(User* currUser, string argument2) {
    if (!hasFileAccess(currUser, argument2))
        return FILE_UNAVAILABLE;
    char path[BUFFER_SIZE];
    strcpy(path, currUser->getRelativeDir(argument2).c_str());
    return !remove(path) ? "250: " + argument2 + " deleted." : SYNTAX_ERROR;
}

string Server::handleDirDel(User* currUser, string argument2) {
    if (!hasDirectoryAccess(currUser, argument2))
        return FILE_UNAVAILABLE;
    string bashCommand = "rm -r " + currUser->getRelativeDir(argument2); 
    return !system(bashCommand.c_str()) ? "257: " + argument2 + " deleted." : SYNTAX_ERROR;
}

string Server::handleDele(User* currUser, string argument1, string argument2) {
    if(argument1 == "-f")
        return handleFileDel(currUser, argument2);
    else if(argument1 == "-d")
        return handleDirDel(currUser, argument2);
    else 
        return SYNTAX_ERROR;
}

User* Server::findUserByFd(int userFd) {
    for(auto& user : users) 
        if(user.fdMatches(userFd)) 
            return &user;  
    return NULL;
}

string Server::handleUser(int userFd, string argument1){
    if (fdLoggedInUser.find(userFd) != fdLoggedInUser.end())
        return NEED_TO_LOGOUT; 
    User* curr_user = findUserByName(argument1);
    if(curr_user) {
        if (!curr_user->isLoggedIn()){
            curr_user->updateFd(userFd);
            fdLastRequest[userFd] = argument1;
            return USERNAME_OK;
        }
        else
            return ALREADY_LOGGED_IN;
    }
    else
        return INVALID_LOGIN;
}

string Server::handlePass(int userFd, string argument1){
    if (fdLastRequest.find(userFd) != fdLastRequest.end()){
        if (loginUser(argument1, userFd, fdLastRequest[userFd]))
            return SUCCESSFUL_LOGIN;
        else
            return INVALID_LOGIN;
    }
    else
        return BAD_SEQUENCE;
}

string Server::handlePwd(User* currUser) {
    return "257: " + string(currUser->getCurrDir());
}

string Server::handleMkd(User* currUser, string argument) {
    string bashCommand = "mkdir " + currUser->getRelativeDir(argument);
    return !system(bashCommand.c_str()) ? "257: " + argument + " created." : SYNTAX_ERROR;
}

void Server::printServer() {
    cout << "Server info:\n";
    cout << "Command Channel Port: " << cmdChannelPort << endl;
    cout << "Data Channel Port: " << dataChannelPort << endl;
    cout << "files: " << endl;
    for (auto file: adminFiles){
        cout << file << " ";
    }
    cout << endl;
    cout << "User List: \n";
    for(auto user : users) 
        user.printUser();
}

User* Server::findUserByName(string username_) {
    for(auto& user : users) 
        if(user.isValid(username_)) 
            return &user;  
    return NULL;
}

bool Server::loginUser(string password, int fd, string lastUser) {
    for(auto& user : users) {
        if(user.fdMatches(fd) && user.isValid(lastUser)){
            if(user.login(password)){
                fdLoggedInUser[fd] = lastUser;
                return true;
            }
            else
                return false;
        }
    }
    return false;
}

bool Server::hasFileAccess(User* currUser, string file){
    return (!count(adminFiles.begin(), adminFiles.end(), file) || currUser->isAdmin());
}

bool Server::hasDirectoryAccess(User* currUser, string file){
    DIR *dir; struct dirent *diread;
    if((dir = opendir(currUser->getRelativeDir(file).c_str())) != nullptr) {
        while((diread = readdir(dir)) != nullptr) {
            if (diread->d_name == ".." || diread->d_name == ".")
                continue;
            if (!hasFileAccess(currUser, diread->d_name))
                return false;
        }
        closedir(dir);
    } else {
        perror("opendir");
        return false;
    }
    return true;
}

string Server::handleCommand(string command, string argument, int userFd, int userDataFd) {
    curr_log += "Client (fd = " + to_string(userFd) + ") requested: " + command;
    writeLog();

    string argument1, argument2;

    if(command == HELP_COMMAND) 
        return handleHelp();

    if (command == DELE_COMMAND || command == RENAME_COMMAND) {
        stringstream ss(argument);
        getline(ss, argument1, ' ');
        getline(ss, argument2, '\n');
    }
    else
        argument1 = argument;

    if(command == USER_COMMAND) 
        return handleUser(userFd, argument1);
    else if (command != PASS_COMMAND)
        fdLastRequest.erase(userFd);

    if(command == PASS_COMMAND)
        return handlePass(userFd, argument1);

    if(fdLoggedInUser.find(userFd) == fdLoggedInUser.end())
        return SHOULD_LOGIN;
    User* currUser = findUserByFd(userFd);

    if(command == PWD_COMMAND) 
        return handlePwd(currUser);
    if(command == MKD_COMMAND)
        return handleMkd(currUser, argument1);
    if(command == DELE_COMMAND)
        return handleDele(currUser, argument1, argument2);
    if(command == LS_COMMAND)
        return handleLs(currUser, userDataFd);
    if(command == CWD_COMMAND) 
        return handleCwd(currUser, argument1);
    if(command == RENAME_COMMAND)
        return handleRename(currUser, argument1, argument2);
    if(command == RETR_COMMAND)
        return handleRetr(currUser, argument1, userDataFd);
    if(command == QUIT_COMMAND)
        return handleQuit(currUser, userFd);
        
    return SYNTAX_ERROR;  
}

void Server::convertConfig(map<string, vector<string>> inputs) {
    cmdChannelPort = stoi(inputs[COMMAND_CHANNEL_PORT][0]);
    dataChannelPort = stoi(inputs[DATA_CHANNEL_PORT][0]);

    for (auto x: inputs[CONFIG_ADMIN_FILES])
        adminFiles.push_back(x);

    for(int i = 0; i < inputs[CONFIG_USER].size(); i++) {
        User newUser(inputs[CONFIG_USER][i], inputs[CONFIG_PASSWORD][i],
                        inputs[CONFIG_IS_ADMIN][i], inputs[CONFIG_SIZE][i]); 
        users.push_back(newUser);
    }
}

void Server::writeLog(){
    if (curr_log == "")
        return;

	time_t now = time(0);
	char* dt = strtok(ctime(&now), "\n");
	string newLog = "[";
    newLog += dt;
	newLog += "] ";
	newLog += curr_log;
	newLog += "\n";
    
	ofstream logFile;
	logFile.open(log_path, ios_base::app);
	logFile << newLog;
    logFile.close();
    curr_log = "";
}

int Server::setupServer(int port) {
    struct sockaddr_in address;
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    // only for command 
    listen(server_fd, 4);

    return server_fd;
}

int Server::acceptClient(int port) {
    int client_fd;
    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);
    client_fd = accept(port, (struct sockaddr *)&client_address, (socklen_t*) &address_len);
    return client_fd;
}

void Server::run(){
    int new_data_socket, new_cmd_socket, data_max_sd, cmd_max_sd;
    char buffer[BUFFER_SIZE] = {0};
    fd_set cmd_master_set, cmd_working_set;
    map<int, int> cmdDataFd;
    FD_ZERO(&cmd_master_set);
    cmd_max_sd = serverCmdFd;
    FD_SET(serverCmdFd, &cmd_master_set);

    printf("Server is up!\n");
    curr_log += "Server is now online.";
    writeLog();

    while (1) {
        cmd_working_set = cmd_master_set;
        select(cmd_max_sd + 1, &cmd_working_set, NULL, NULL, NULL);
        for(int i = 0; i <= cmd_max_sd; i++) {
            memset(buffer, 0, BUFFER_SIZE);
            if (FD_ISSET(i, &cmd_working_set)) {
                if (i == serverCmdFd) {  // new clinet
                    new_cmd_socket = acceptClient(serverCmdFd);
                    new_data_socket = acceptClient(serverDataFd);
                    FD_SET(new_cmd_socket, &cmd_master_set);
                    cmdDataFd[new_cmd_socket] = new_data_socket;
                    if (new_cmd_socket > cmd_max_sd)
                        cmd_max_sd = new_cmd_socket;
                    
                    curr_log += "new client connected with fd " + to_string(new_cmd_socket);
                    writeLog();
                }
                else { // client sending msg
                    int bytes_received;
                    bytes_received = recv(i , buffer, BUFFER_SIZE, 0);
                    if (bytes_received == 0) { // EOF
                        if (fdLoggedInUser.find(i) != fdLoggedInUser.end()){
                            findUserByName(fdLoggedInUser[i])->logout();
                            fdLoggedInUser.erase(i);
                        }
                        fdLastRequest.erase(i);

                        curr_log += "Client on fd " + to_string(i) + " disconnected";
                        writeLog();

                        write(1, "Client disconnected!\n", 22);
                        close(i);
                        close(cmdDataFd[i]);
                        FD_CLR(i, &cmd_master_set);
                        auto it = cmdDataFd.find (i);
                        cmdDataFd.erase(it);
                        continue;
                    }
                    stringstream ss(buffer);
                    string command, argument;
                    getline(ss, command, ' ');
                    getline(ss, argument, '\n');
                    if (command.back() == '\n')
                        command.pop_back();

                    strcpy(buffer, handleCommand(command, argument, i, cmdDataFd[i]).c_str());
                    curr_log += "Server response (fd = " + to_string(i) + ") was: "+ buffer;
                    writeLog();

                    send(i, buffer, strlen(buffer), 0);
                }
            }
        }
    }
}

void exitLog(string log){
	time_t now = time(0);
	char* dt = strtok(ctime(&now), "\n");
	string newLog = "[";
    newLog += dt;
	newLog += "] ";
	newLog += log;
	newLog += "\n";

	ofstream logFile;
	logFile.open(string(SERVER_ABSOLUTE_PATH) + LOG_FILE_NAME, ios_base::app);
	logFile << newLog;	
    logFile.close();
}

void signal_callback_handler(int signum) {
   exitLog("Server is offline.");
   printf("\n");
   exit(signum);
}







int main(int argc, char const *argv[]) {
    auto inputs = parseJson(CONFIG_FILE);
    Server server(inputs);
    signal(SIGINT, signal_callback_handler);

    //uncomment to see server info ("config.json")
    //server.printServer();

    server.run();
}
