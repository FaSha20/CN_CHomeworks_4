#include <iostream>
#include <thread>
#include <ctime>
#include <cmath>

using namespace std;
using namespace std::chrono;

#define SLOW_START 0
#define CONG_AVOIDANCE 1
#define FAST_RECOVERY 2

#define INF 10000

#define RENO 0
#define NEW_RENO 1
#define BBR 2


class TCPConnection{
private:
    int cwnd; 
    int initial_cwnd;
    int windStart = 0;
    int windEnd = 0;
    int ssthresh;
    int rtt;
    int be_sleep = 1;
    int phase = SLOW_START;
    int last_drop;
    int model;
public:
TCPConnection(int _cwnd, int _ssthresh, int _model){
    cwnd = _cwnd;
    initial_cwnd = _cwnd;
    ssthresh = _ssthresh;
    rtt = 1;
    model = _model;
}

int sendData(){
    if(windEnd - windStart < cwnd){
        cout << "Trasmitting packet...\n";
        windEnd ++;
    }
    return windEnd;
}
void handleAck(int ackNum){
    if(ackNum > windStart){
        cout << "Ack #" << ackNum << " recieved.\n";
        windStart = ackNum;

        if(phase == CONG_AVOIDANCE)
            cwnd ++;
        else if (phase == SLOW_START)
        {
            cwnd *= 2;
            if(cwnd >= ssthresh){
                cout << "!!Congestion avoidanve region!!\n";
                cwnd = ssthresh;
                phase = CONG_AVOIDANCE;
            }
        }
    }else{
        cout << "----Timeout occurred...\n";
        cout << "----Packet number #" << ackNum+1 << " did not recieved.\n";
        windEnd --;
        ssthresh = cwnd / 2;
        cwnd = initial_cwnd;
        phase = SLOW_START;
    }
}
int checkTimeout(){
    int a = rand()%10;
    if( a > rtt ){
    return 1;
    }   
    else return 0;
}
void onPacketLoss(int packnum){
    phase = FAST_RECOVERY;
    cout << "Packet loss occurred...\n";
    //New Reno
    if(model == NEW_RENO)
        if(int(log2(packnum)) == last_drop){
            return;
        }
    //Reno
    last_drop = log2(packnum);
    //cout << "last+drop:: " << last_drop <<endl;
    ssthresh = cwnd / 2;
    cwnd = ssthresh;
    phase = CONG_AVOIDANCE; 
}
void onRTTUpdate(int newRtt){
    rtt = newRtt;
}
void PrintReport(){
    cout << "\n-----------------------------\n";
    cout << "congestion window: " << cwnd << endl;
    cout << "ssthresh: " << ssthresh << endl;
    string ph = (phase == SLOW_START) ? "Slow Start" : 
                (phase == CONG_AVOIDANCE) ? "Congestion avoidance (AIMD)":"Fast Recavery";
    cout << "Phase: " << ph << endl;
    cout << "-----------------------------\n\n";
}
};

int main(){
    srand(time(0));
    int packnum;
    int rtt = 1;
   
    TCPConnection tcp1(1, INF, RENO);
    
    tcp1.onRTTUpdate(8);
    
    for(int i = 0; i < 20 ; i++){
        packnum = tcp1.sendData();
        
        if(i == 5 || i == 6){
            tcp1.onPacketLoss(packnum);
        }
        else{
            if(i > 6 && tcp1.checkTimeout()) {
                packnum --;
            }
            tcp1.handleAck(packnum);
        }
        tcp1.PrintReport();

        
    }
   


}
