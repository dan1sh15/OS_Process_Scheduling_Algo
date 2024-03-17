#include <bits/stdc++.h>
using namespace std;

int cpuIdleTime = 0;

static bool cmp(vector<int> &A, vector<int> &B){ // CUSTOM COMPARARTOR FOR SORTING ACCORDING TO THE ARRIVAL TIME
    return A[1] < B[1];
}

// CUSTOM CLASS COMPARATOR TO MAXIMISE THE BURST TIME AND MINIMISE THE ARRIVAL TIME AND PID
class Compare{
public:
    bool operator()(pair<int, pair<int,int>>& A, pair<int, pair<int,int>>& B){
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

void computeCompletionTime(vector<vector<int>> &processes, int* CT, int n, vector<pair<int, bool>> &RT, vector<string> &ganttChart){
    priority_queue<pair<int, pair<int,int>>, vector<pair<int, pair<int,int>>>, Compare> rq; // create a maxheap {BT} but minimise the AT and ProcessID

    int currTime = 0, i = 0;
    while(i < n){
        // push into the ready queue
        while(i < n && processes[i][1] <= currTime){
            rq.push(make_pair(processes[i][2], make_pair(processes[i][1], processes[i][0]))); // {BT, AT, ID} - It sorts according to burst time then arival time and then process id
            i++;
        }
        if(!rq.empty()){ // IF READY QUEUE IS NOT EMPTY
            auto top = rq.top();
            rq.pop();

            ganttChart.push_back("P" + to_string(top.second.second + 1));

            if(!RT[top.second.second].second){ // TO CHECK WHEN THE PROCESS ARRIVES FIRST
                RT[top.second.second].first = currTime - top.second.first; // calculate the response time since,  RESPONSE_TIME = CURRENT_CPUT_TIME - ARRIVAL_TIME
                RT[top.second.second].second = true;
            }
            
            currTime++; // SINCE PREEMPTIVE THEN INCREASE THE CURRENT TIME BY ONE UNIT AND CHECK FOR OTHER PROCESS
            top.first--;

            if(top.first == 0){ // IF THE BURST TIME BECOMES ZERO THEN UPDATE IT INTO THE COMPLETION TIME ARRAY
                CT[top.second.second] = currTime; // ELSE IF BURST TIME IS REMAINING THEN PUSH IT INTO THE READY QUEUE AGAIN
            }
            else{
                rq.push({top.first, {top.second.first, top.second.second}});
            }
        }
        else {
            currTime++;
            cpuIdleTime++;
            ganttChart.push_back("Idle");
        }
    }

    while(!rq.empty()){ // THIS LOOP PERFORMS THE SAME LOGIC WHEN ALL THE PROCESSES ARRIVE IN THE READY QUEUE
        auto top = rq.top();
        rq.pop();

        ganttChart.push_back("P" + to_string(top.second.second + 1));

        if(!RT[top.second.second].second){ // TO CHECK WHEN THE PROCESS ARRIVES FIRST
            RT[top.second.second].first = currTime - top.second.first; // calculate the response time since,  RESPONSE_TIME = CURRENT_CPUT_TIME - ARRIVAL_TIME
            RT[top.second.second].second = true;
        }
        
        currTime++; // SINCE PREEMPTIVE THEN INCREASE THE CURRENT TIME BY ONE UNIT AND CHECK FOR OTHER PROCESS
        top.first--;

        if(top.first == 0){ // IF THE BURST TIME BECOMES ZERO THEN UPDATE IT INTO THE COMPLETION TIME ARRAY
            CT[top.second.second] = currTime; // ELSE IF BURST TIME IS REMAINING THEN PUSH IT INTO THE READY QUEUE AGAIN
        }
        else{
            rq.push({top.first, {top.second.first, top.second.second}});
        }
    }
    return ;
}

int main(){

    vector<vector<int>> processes;
    int n;
    cout << "\nEnter the number of processes: ";
    cin >> n;

    int* AT = new int[n];
    cout << "\nEnter the Arrival Time of the processes: \n";
    for(int i=0; i<n; i++){
        cin >> AT[i];
    }

    int* BT = new int[n];
    cout << "\nEnter the Burst Time of the processes: \n";
    for(int i=0; i<n; i++){
        cin >> BT[i];
    }

    for(int i=0; i<n; i++){
        // {PID, AT, BT}
        processes.push_back({i, AT[i], BT[i]});
    }

    int* CT = new int[n];
    vector<string> ganttChart;
    vector<pair<int, bool>> RT(n); // pair is used to check when the process arrive for the first time
    sort(processes.begin(), processes.end(), cmp); // sort according to the arrival time
    computeCompletionTime(processes, CT, n, RT, ganttChart);

    sort(processes.begin(), processes.end()); // sort again according to PID

    cout << "\nThe Gantt Chart for the above process is: \n";
    cout << "|";
    for(auto& s: ganttChart){
        cout << s << "|";
    }
    cout << endl << endl;


    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n"; 
    double sum1 = 0, sum2 = 0;
    for(int i=0; i<n; i++){
        int pid = processes[i][0] + 1;
        int at = processes[i][1];
        int bt = processes[i][2];
        int ct = CT[i];
        int tat = ct - at;
        int rt = RT[i].first;
        sum1 += tat;
        int wt = tat - bt;
        sum2 += wt;

        cout << pid << "\t" << at << "\t" << bt << "\t" << ct << "\t" << tat << "\t" << wt << "\t" << rt << "\n";
    }

    cout << "\nThe CPU Idle time is: " << cpuIdleTime << endl;

    double avgTatTime = (double) sum1 / (double) n;
    cout << "\nThe Average Turn Around Time is: " << avgTatTime << endl;

    double avgWaitingTime = (double) sum2 / (double) n;
    cout << "\nThe Average Waiting Time is: " << avgWaitingTime << endl << endl;

    delete [] AT;
    delete [] BT;
    delete [] CT;

    return 0;
}

/* ---------------------- OUTPUTS ---------------------- */

// OUTPUT-1

/*

Enter the number of processes: 3

Enter the Arrival Time of the processes:
0 0 0

Enter the Burst Time of the processes:
2 4 8

The Gantt Chart for the above process is:
|P3|P3|P3|P3|P2|P3|P2|P3|P1|P2|P3|P1|P2|P3|


PID     AT      BT      CT      TAT     WT      RT
1       0       2       12      12      10      8
2       0       4       13      13      9       4
3       0       8       14      14      6       0

The CPU Idle time is: 0

The Average Turn Around Time is: 13

The Average Waiting Time is: 8.33333

*/

// OUTPUT-2

/*

Enter the number of processes: 4

Enter the Arrival Time of the processes:
1 2 3 4

Enter the Burst Time of the processes:
2 4 6 8

The Gantt Chart for the above process is:
|Idle|P1|P2|P3|P4|P4|P4|P3|P4|P3|P4|P2|P3|P4|P2|P3|P4|P1|P2|P3|P4|


PID     AT      BT      CT      TAT     WT      RT
1       1       2       18      17      15      0
2       2       4       19      17      13      0
3       3       6       20      17      11      0
4       4       8       21      17      9       0

The CPU Idle time is: 1

The Average Turn Around Time is: 17

The Average Waiting Time is: 12

*/

// OUTPUT-3

/*

Enter the number of processes: 6

Enter the Arrival Time of the processes:
0 1 2 3 4 5

Enter the Burst Time of the processes:
7 5 3 1 2 1

The Gantt Chart for the above process is:
|P1|P1|P1|P2|P1|P2|P1|P2|P3|P1|P2|P3|P5|P1|P2|P3|P4|P5|P6|


PID     AT      BT      CT      TAT     WT      RT
1       0       7       14      14      7       0
2       1       5       15      14      9       2
3       2       3       16      14      11      6
4       3       1       17      14      13      13
5       4       2       18      14      12      8
6       5       1       19      14      13      13

The CPU Idle time is: 0

The Average Turn Around Time is: 14

The Average Waiting Time is: 10.8333

*/