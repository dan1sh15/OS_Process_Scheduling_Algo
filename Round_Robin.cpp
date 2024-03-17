#include <bits/stdc++.h>
using namespace std;

static bool cmp(vector<int> &A, vector<int> &B){
    return A[1] < B[1];
}

int cpuIdleTime = 0;

void calculateCompletionTime(vector<int> &CT, vector<vector<int>> &processes, int& timeQuantum, vector<pair<int, bool>> &RT, vector<string> &ganttChart){
    queue<vector<int>> rq; // READY QUEUE

    int i = 0, currTime = 0, n = processes.size();
    while(i < n){
        while(i < n && currTime >= processes[i][0]) { // while current time is greater than arrival time then push it into the ready queue {AT, pid, BT}
            rq.push(processes[i]);
            i++;
        }

        if(!rq.empty()){
            auto feasibleProcess = rq.front();
            ganttChart.push_back("P" + to_string(feasibleProcess[1] + 1));
            // cout << feasibleProcess[1] << " " << feasibleProcess[0] << " " << feasibleProcess[2] << endl;
            rq.pop();

            if(!RT[feasibleProcess[1]].second){ // first time when the process gets CPU 
                RT[feasibleProcess[1]].first = currTime - feasibleProcess[0];
                RT[feasibleProcess[1]].second = true;
            }

            int timeToUse = min(timeQuantum, feasibleProcess[2]); // time to utilise will be minimum of time quantum and remaining burst time of the process
            currTime += timeToUse; // add the time to utilise to current time of the CPU


            // after the time updation we have to push all the processes into the ready queue whose AT <= Current time
            while(i < n && currTime >= processes[i][0]){
                rq.push(processes[i]);
                i++;
            }

            int newBurstTime = feasibleProcess[2] - timeToUse; // the new burst time of the feasible process 

            if(newBurstTime == 0){ // if new burst time becomes zero then update the current time of the feasible process
                CT[feasibleProcess[1]] = currTime;
            }
            else{
                // if burst time is not zero then push it into the ready queue again
                rq.push({feasibleProcess[0], feasibleProcess[1], newBurstTime});
            }
        }
        else {
            currTime++; // if there is no process in the ready queue the update the current time
            cpuIdleTime++; // there is no process so the CPU is idle hence increment the CPU idle time
            ganttChart.push_back("idle");
        }
    }

    while(!rq.empty()){
        auto feasibleProcess = rq.front();
        // cout << feasibleProcess[1] << " " << feasibleProcess[0] << " " << feasibleProcess[2] << endl;
        rq.pop();
        ganttChart.push_back("P" + to_string(feasibleProcess[1]+1));

        if(!RT[feasibleProcess[1]].second){
            RT[feasibleProcess[1]].first = currTime - feasibleProcess[0];
            RT[feasibleProcess[1]].second = true;
        }

        int timeToUse = min(timeQuantum, feasibleProcess[2]);
        currTime += timeToUse;
        int newBurstTime = feasibleProcess[2] - timeToUse;

        if(newBurstTime == 0) {
            CT[feasibleProcess[1]] = currTime;
        }
        else {
            rq.push({feasibleProcess[0], feasibleProcess[1], newBurstTime});
        }
    }
    return;
}

int main(){

    int n;
    cout << "\nEnter the number of process: ";
    cin >> n;

    vector<int> AT(n), BT(n);
    cout << "\nEnter the arrival time of " << n << " processes" << endl;

    for(int i=0; i<n; i++) cin >> AT[i];

    cout << "\nEnter the Burst time of " << n << " processes " << endl;
    for(int i=0; i<n; i++) cin >> BT[i];

    int timeQuantum;
    cout << "\nEnter the Time Quantum for the process: ";
    cin >> timeQuantum;

    vector<vector<int>> processes;
    for(int i=0; i<n; i++){
        processes.push_back({AT[i], i, BT[i]});
    }

    sort(processes.begin(), processes.end());

    vector<int> CT(n);
    vector<string> ganttChart;
    vector<pair<int, bool>> RT(n);
    calculateCompletionTime(CT, processes, timeQuantum, RT, ganttChart);

    sort(processes.begin(), processes.end(), cmp);

    cout << "\nThe Gantt Chart for the process is: \n";
    cout << "|";
    for(auto& s: ganttChart){
        cout << s << "|";
    }
    cout << endl << endl;
    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n";

    int sum1 = 0, sum2 = 0;


    for(int i=0; i<n; i++){
        int pid = i+1;
        int at = processes[i][0];
        int bt = processes[i][2];
        int ct = CT[i];
        int tat = ct - at;
        sum1 += tat;
        int wt = tat - bt;
        sum2 += wt;
        int rt = RT[i].first;

        cout << pid << "\t" << at << "\t" << bt << "\t" << ct << "\t" << tat << "\t" << wt << "\t" << rt << "\n";
    }

    double avgTAT = sum1 / (double) n;
    double avgWT = sum2 / (double) n;

    cout << "\nThe Average Turn Around Time is: " << avgTAT << endl;
    cout << "\nThe Average Waiting Time is: " << avgWT << endl;
    cout << "\nThe CPU Idle Time is: " << cpuIdleTime << endl;

    return 0;
}


/************************** OUTPUTS **************************/

// OUTPUT-1
/*

Enter the number of process: 4

Enter the arrival time of 4 processes
0 0 0 0

Enter the Burst time of 4 processes 
5 4 2 3

Enter the Time Quantum for the process: 2

The Gantt Chart for the process is: 
|P1|P2|P3|P4|P1|P2|P4|P1|


PID     AT      BT      CT      TAT     WT      RT
1       0       5       14      14      9       0
2       0       4       12      12      8       2
3       0       2       6       6       4       4
4       0       3       13      13      10      6

The Average Turn Around Time is: 11.25

The Average Waiting Time is: 7.75

The CPU Idle Time is: 0

*/

// OUTPUT-2
/*

Enter the number of process: 4

Enter the arrival time of 4 processes
0 2 3 9

Enter the Burst time of 4 processes 
7 4 2 1

Enter the Time Quantum for the process: 2

The Gantt Chart for the process is: 
|P1|P2|P1|P3|P2|P1|P4|P1|


PID     AT      BT      CT      TAT     WT      RT
1       0       7       14      14      7       0
2       2       4       10      8       4       0
3       3       2       8       5       3       3
4       9       1       13      4       3       3

The Average Turn Around Time is: 7.75

The Average Waiting Time is: 4.25

The CPU Idle Time is: 0

*/

// OUTPUT-3
/*

Enter the number of process: 6

Enter the arrival time of 6 processes
0 1 2 3 4 6

Enter the Burst time of 6 processes 
4 5 2 1 6 3

Enter the Time Quantum for the process: 2

The Gantt Chart for the process is: 
|P1|P2|P3|P1|P4|P5|P2|P6|P5|P2|P6|P5|


PID     AT      BT      CT      TAT     WT      RT
1       0       4       8       8       4       0
2       1       5       18      17      12      1
3       2       2       6       4       2       2
4       3       1       9       6       5       5
5       4       6       21      17      11      5
6       6       3       19      13      10      7

The Average Turn Around Time is: 10.8333

The Average Waiting Time is: 7.33333

The CPU Idle Time is: 0

*/

// OUTPUT-4
/*

Enter the number of process: 6

Enter the arrival time of 6 processes
5 4 3 1 2 6

Enter the Burst time of 6 processes 
5 6 7 9 2 3

Enter the Time Quantum for the process: 3

The Gantt Chart for the process is: 
|idle|P4|P5|P3|P2|P4|P1|P6|P3|P2|P4|P1|P3|


PID     AT      BT      CT      TAT     WT      RT
1       5       5       32      27      22      10
2       4       6       27      23      17      5
3       3       7       33      30      23      3
4       1       9       30      29      20      0
5       2       2       6       4       2       2
6       6       3       21      15      12      12

The Average Turn Around Time is: 21.3333

The Average Waiting Time is: 16

The CPU Idle Time is: 1

*/