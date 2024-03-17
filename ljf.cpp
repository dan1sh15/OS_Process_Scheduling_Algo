#include <bits/stdc++.h>
using namespace std;

static bool cmp(vector<int> &A, vector<int> &B){
    return A[1] < B[1];
}

vector<string> ganttChart;

class Compare{
public:
    bool operator()(pair<int, pair<int,int>> &A, pair<int, pair<int,int>> &B){
        if(A.first < B.first){
            return true;
        }
        else if(A.first == B.first && A.second.first > B.second.first){
            return true;
        }
        else if(A.first == B.first && A.second.first == B.second.first && A.second.second > B.second.second){
            return true;
        }
        return false;
    }
};

void computeCompletionTime(vector<vector<int>> &processes, int* CT, int n){
    priority_queue<pair<int, pair<int,int>>, vector<pair<int, pair<int,int>>>, Compare> rq; // create a maxheap {BT} but minimise the AT and ProcessID. IT WILL ACT AS THE READY QUEUE

    int currTime = 0, i = 0;
    while(i < n){
        // push into the ready queue
        while(i < n && currTime >= processes[i][1]){
            rq.push({processes[i][2], {processes[i][1], processes[i][0]}}); // {BT, AT, ID} - It sorts according to burst time then arival time and then process id
            i++;
        }
        if(!rq.empty()){
            auto top = rq.top();
            ganttChart.push_back("P" + to_string(top.second.second + 1));
            CT[top.second.second] = currTime + top.first;
            rq.pop();
            currTime += top.first;
        }
        else {
            ganttChart.push_back("idle");
            currTime++;
        }
    }

    while(!rq.empty()){
        auto top = rq.top();
        ganttChart.push_back("P" + to_string(top.second.second + 1));
        rq.pop();
        CT[top.second.second] = currTime + top.first;
        currTime += top.first;
    }
    return ;
}

int main(){

    vector<vector<int>> processes;
    int n;
    std::cout << "\nEnter the number of processes: ";
    cin >> n;

    int* AT = new int[n];
    std::cout << "\nEnter the Arrival Time of the processes: \n";
    for(int i=0; i<n; i++){
        cin >> AT[i];
    }

    int* BT = new int[n];
    std::cout << "\nEnter the Burst Time of the processes: \n";
    for(int i=0; i<n; i++){
        cin >> BT[i];
    }

    for(int i=0; i<n; i++){
        // {PID, AT, BT}
        processes.push_back({i, AT[i], BT[i]});
    }

    int* CT = new int[n];
    std::sort(processes.begin(), processes.end(), cmp); // sort according to arrival time
    computeCompletionTime(processes, CT, n);

    cout << "\nGantt Chart:\n";
    cout << "|";
    for(auto& str: ganttChart){
        cout << str << "|";
    }
    cout << endl;

    std::sort(processes.begin(), processes.end());
    std::cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n"; 
    double sum1 = 0, sum2 = 0;
    for(int i=0; i<n; i++){
        int pid = processes[i][0] + 1;
        int at = processes[i][1];
        int bt = processes[i][2];
        int ct = CT[i];
        int tat = ct - at;
        sum1 += tat;
        int wt = tat - bt;
        sum2 += wt;

        std::cout << pid << "\t" << at << "\t" << bt << "\t" << ct << "\t" << tat << "\t" << wt << "\n";
    }

    double avgTatTime = (double) sum1 / (double) n;
    std::cout << "\nThe Average Turn Around Time is: " << avgTatTime << endl;

    double avgWaitingTime = (double) sum2 / (double) n;
    std::cout << "\nThe Average Waiting Time is: " << avgWaitingTime << endl << endl;

    delete[] AT;
    delete[] BT;
    delete[] CT;

    return 0;
}


//          OUTPUT 1
/*
Enter the number of processes: 5

Enter the Arrival Time of the processes:
1 2 3 4 5

Enter the Burst Time of the processes:
3 5 1 2 8

PID     AT      BT      CT      TAT     WT
1       1       3       4       3       0
2       2       5       9       7       2
3       3       1       20      17      16
4       4       2       19      15      13
5       5       8       17      12      4

The Average Turn Around Time is: 10.8

The Average Waiting Time is: 7
*/

//          OUTPUT 2
/*
Enter the number of processes: 6

Enter the Arrival Time of the processes:
0 4 2 2 7 8

Enter the Burst Time of the processes:
1 4 2 4 2 1

PID     AT      BT      CT      TAT     WT
1       0       1       1       1       0
2       4       4       10      6       2
3       2       2       12      10      8
4       2       4       6       4       0
5       7       2       14      7       5
6       8       1       15      7       6

The Average Turn Around Time is: 5.83333

The Average Waiting Time is: 3.5
*/