#include <bits/stdc++.h>
using namespace std;

class Compare{
public:
    bool operator() (pair<double, vector<int>>& A, pair<double, vector<int>>& B){
        if(A.first < B.first){
            return true;
        } 
        else if(A.first == B.first && A.second[0] > B.second[0]){
            return true;
        }
        else if(A.first == B.first && A.second[0] == B.second[0] && A.second[1] > B.second[1]){
            return true;
        }
        return false;
    }
};

vector<string> ganttChart;

void calculateCompletionTime(vector<vector<int>> &processes, vector<int> &CT, int n){
    int currentTime = 0, i = 0; 
    unordered_map<int, vector<int>> readyQueue; // it is the ready queue

    while(i < n){
        while(i < n && processes[i][1] <= currentTime){
            readyQueue[processes[i][0]] = {processes[i][1], processes[i][2]};
            i++;        
        }
        if(readyQueue.size() == 1){ // if we have only single process then we have to schedule it
            currentTime += readyQueue.begin() -> second[1];
            ganttChart.push_back("P" + to_string(readyQueue.begin() -> first + 1));
            CT[readyQueue.begin() -> first] = currentTime;
            readyQueue.erase(readyQueue.begin() -> first);
        }
        else if(readyQueue.size() > 1){
            priority_queue<pair<double, vector<int>>, vector<pair<double, vector<int>>>, Compare> pq; // it is used for finding the maximum response ratio
            for(auto i: readyQueue){
                int waitingTime = currentTime - i.second[0];
                int burstTime = i.second[1];
                double responseRatio = (double) (waitingTime + burstTime) / (double) burstTime;

                // {response ratio, arrival time, pid, burst time}
                pq.push({responseRatio, {i.second[0], i.first, i.second[1]}});
            }
            auto feasibleProcess = pq.top(); // the process with the highest respone ratio
            pq.pop();

            ganttChart.push_back("P" + to_string(feasibleProcess.second[1] + 1));
            currentTime += feasibleProcess.second[2];
            CT[feasibleProcess.second[1]] = currentTime;
            readyQueue.erase(feasibleProcess.second[1]);
        }
        else {
            ganttChart.push_back("idle");
            currentTime++; // if no process is waiting in the ready queue then increase the current time.
        }
    }

    // Compute for the remaining process waiting in the ready queue
    while(readyQueue.size() > 0){
        priority_queue<pair<double, vector<int>>, vector<pair<double, vector<int>>>, Compare> pq;
        for(auto i: readyQueue){
            int waitingTime = currentTime - i.second[0];
            int burstTime = i.second[1];
            double responseRatio = (double) (waitingTime + burstTime) / (double) burstTime;
            // cout << "Response Ratio of " << i.first << " " << responseRatio << endl;
            pq.push({responseRatio, {i.second[0], i.first, i.second[1]}});
        }
        auto feasibleProcess = pq.top();
        pq.pop();

        ganttChart.push_back("P" + to_string(feasibleProcess.second[1] + 1));
        currentTime += feasibleProcess.second[2];
        CT[feasibleProcess.second[1]] = currentTime;
        readyQueue.erase(feasibleProcess.second[1]);
    }
}

static bool cmp(vector<int> &A, vector<int> &B){
    return A[1] < B[1];
}

int main(){

    int n;
    cout << "\nEnter the number of processes: ";
    cin >> n;

    vector<int> AT(n), BT(n);

    cout << "\nEnter the Arrival Time of " << n << " processes\n";
    for(int i=0; i<n; i++) cin >> AT[i];

    cout << "\nEnter the Burst Time of " << n << " processes" << endl;
    for(int i=0; i<n; i++) cin >> BT[i];

    vector<vector<int>> processes;
    for(int i=0; i<n; i++){
        processes.push_back({i, AT[i], BT[i]});
    }

    sort(processes.begin(), processes.end(), cmp); // sort according to the arrival time

    vector<int> CT(n);
    calculateCompletionTime(processes, CT, n);

    cout << "\nGantt Chart:\n";
    cout << "|";
    for(auto& str: ganttChart){
        cout << str << "|";
    }
    cout << endl;

    sort(processes.begin(), processes.end());

    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
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

        cout << pid << "\t" << at << "\t" << bt << "\t" << ct << "\t" << tat << "\t" << wt << endl;
    }
    cout << endl;
    double avgTAT = sum1 / (double) n;
    double avgWT = sum2 / (double) n;
    cout << "The Average Turn Around Time is: " << avgTAT << endl;
    cout << "\nThe Average Waiting Time is: " << avgWT << endl;
    return 0;
}



/* ---------------- OUTPUTS ---------------- */

// OUTPUT-1

/*

Enter the number of processes: 5

Enter the Arrival Time of 5 processes
0 2 4 6 8

Enter the Burst Time of 5 processes
3 6 4 5 2

PID     AT      BT      CT      TAT     WT
1       0       3       3       3       0
2       2       6       9       7       1
3       4       4       13      9       5
4       6       5       20      14      9
5       8       2       15      7       5

The Average Turn Around Time is: 8

The Average Waiting Time is: 4

*/

// OUTPUT-2

/*

Enter the number of processes: 6

Enter the Arrival Time of 6 processes
0 0 3 5 6 8

Enter the Burst Time of 6 processes
4 2 1 5 3 1

PID     AT      BT      CT      TAT     WT
1       0       4       4       4       0
2       0       2       6       6       4
3       3       1       7       4       3
4       5       5       12      7       2
5       6       3       16      10      7
6       8       1       13      5       4

The Average Turn Around Time is: 6

The Average Waiting Time is: 3.33333

*/