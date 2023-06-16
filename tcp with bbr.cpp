#include <iostream>
#include <thread>
#include <ctime>
#include <cmath>

using namespace std;
using namespace std::chrono;

#define SLOW_START 0
#define CONG_AVOIDANCE 1
#define FAST_RECOVERY 2
#define BBR_STABILITY 3

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
    int BtlBW = 1;
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
        if(phase == BBR_STABILITY){
            cwnd = rtt * BtlBW;
        }
        if(phase == CONG_AVOIDANCE)
            cwnd ++;
        else if (phase == SLOW_START)
        {
            cwnd *= 2;
            if(model == BBR){
                if(cwnd > rtt * BtlBW){
                    cwnd = rtt * BtlBW;
                    phase = BBR_STABILITY;
                }
            }
            else if(cwnd >= ssthresh){
                cout << "!!Congestion avoidanve region!!\n";
                cwnd = ssthresh;
                phase = CONG_AVOIDANCE;
            }
        }
    }else{
        cout << "----Timeout occurred...\n";
        cout << "----Packet number #" << ackNum+1 << " not recieved.\n";
        windEnd --;
        ssthresh = cwnd / 2;
        cwnd = initial_cwnd;
        phase = SLOW_START;
    }
}
int checkTimeout(){
    srand(time(0));
    int a = rand()%10;
    //cout <<"a: "<<a << endl;
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
            phase = CONG_AVOIDANCE; 
            return;
        }
    //Reno
    last_drop = log2(packnum);
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
    string ph = (phase == SLOW_START) ? "Slow Start" : 
                (phase == CONG_AVOIDANCE) ? "Congestion avoidance (AIMD)":
                (phase == FAST_RECOVERY) ? "Fast Recavery": "BBR stability";
    cout << "Phase: " << ph << endl;
    if(model == BBR){
        cout << "rtt: " << rtt << endl;
        cout << "Buttleneck bw: " << BtlBW << endl;
    }else{
        cout << "ssthresh: " << ssthresh << endl;
    }    
    cout << "-----------------------------\n\n";
}
};

int main(){
    srand(time(0));
    int packnum;
    int rtt = 1;
    int type = BBR;
   
    auto start = high_resolution_clock::now();
    
    TCPConnection tcp1(1, INF, type);

    tcp1.onRTTUpdate(8);
    
    for(int i = 0; i < 20 ; i++){
        packnum = tcp1.sendData();
        
        if(i == 5 || i == 6){
            if(type == BBR){
                tcp1.onRTTUpdate(4);
                tcp1.handleAck(packnum);
            }
            else tcp1.onPacketLoss(packnum);
        }
        else{
            if(i == 10 || (i > 15 && tcp1.checkTimeout())) {
                //Enable Timeout
                packnum --;
            }
            tcp1.handleAck(packnum);
        }     
        
        tcp1.PrintReport();

        
    }
   
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    string t = (type == RENO) ? "Reno":
               (type == NEW_RENO) ? "New Reno": "BBR";
    cout << "Time taken by " << t  << " : " <<  duration.count() << " microseconds" << endl;

}