#include <bits/stdc++.h>
using namespace std;
bool increasing = true;
 
class Compare{
public:
    bool operator()(pair<int, vector<int>> &A, pair<int, vector<int>> &B){
        if(increasing){
            if(A.first > B.first){
                return true;
            }
            else if(A.first == B.first && A.second[0] > B.second[0]){
                return true;
            }
            else if(A.first == B.first && A.second[0] == B.second[0] && A.second[1] > B.second[1]){
                return true;
            }
            else return false;
        }
        else{
            if(A.first < B.first){
                return true;
            }
            else if(A.first == B.first && A.second[0] < B.second[0]){
                return true;
            }
            else if(A.first == B.first && A.second[0] == B.second[0] && A.second[1] < B.second[1]){
                return true;
            }
            else return false;
        }
    }
};

vector<string> ganttChart;
 
void calculateCompletionTime(vector<vector<int>> &processes, vector<int> &CT, int n){
    int currentTime = 0, i = 0;
    priority_queue<pair<int, vector<int>>, vector<pair<int, vector<int>>>, Compare> rq;
    while(i < n){
        while(i < n && processes[i][1] <= currentTime){
            rq.push({processes[i][3], {processes[i][1], processes[i][0], processes[i][2]}}); // {priority, arrivaltime, id, burst time}
            i++;
        }
        if(!rq.empty()){
            auto feasibleProcess = rq.top();
            rq.pop();

            ganttChart.push_back("P" + to_string(feasibleProcess.second[1] + 1));
            currentTime += feasibleProcess.second[2];
            CT[feasibleProcess.second[1]] = currentTime;
        }
        else {
            ganttChart.push_back("idle");
            currentTime++;
        }
    }

    while(!rq.empty()){
        auto feasibleProcess = rq.top();
        rq.pop();

        ganttChart.push_back("P" + to_string(feasibleProcess.second[1] + 1));
        currentTime += feasibleProcess.second[2];
        CT[feasibleProcess.second[1]] = currentTime;
    }
    return ;
}

static bool cmp(vector<int> &A, vector<int> &B){
    return A[1] < B[1];
}

int main(){

    int n;
    cout << "\nEnter the number of processes: ";
    cin >> n;

    vector<int> AT(n), BT(n), priority(n);

    cout << "\nEnter the Arrival Time of " << n << " processes\n";
    for(int i=0; i<n; i++) cin >> AT[i];

    cout << "\nEnter the Burst Time of " << n << " processes" << endl;
    for(int i=0; i<n; i++) cin >> BT[i];

    cout << "\nEnter the Priority of " << n << " processes" << endl;
    for(int i=0; i<n; i++) cin >> priority[i];

    choiceLoop:
        int choice;
        cout << "\nPress 1 if the priority is low to high\nPress 2 if the priority is high to low\n";
        cin >> choice;
        
        if(choice == 1){
            increasing = true;
        }
        else if(choice == 2){
            increasing = false;
        }
        else{
            cout << "\nPlease enter a valid option!" << endl;
            goto choiceLoop;
        }

    vector<vector<int>> processes;
    for(int i=0; i<n; i++){
        processes.push_back({i, AT[i], BT[i], priority[i]});
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
    
    sort(processes.begin(), processes.end()); // again sort the processes array according to the PID

    cout << "\nPID\tAT\tBT\tPriority\tCT\tTAT\tWT\n";
    double sum1 = 0, sum2 = 0;
    for(int i=0; i<n; i++){
        int pid = i + 1;
        int at = processes[i][1];
        int bt = processes[i][2];
        int priority = processes[i][3];
        int ct = CT[i];
        int tat = ct - at;
        sum1 += tat;
        int wt = tat - bt;
        sum2 += wt;

        cout << pid << "\t" << at << "\t" << bt << "\t   " << priority << "\t\t" << ct << "\t" << tat << "\t" << wt << "\n";
    }

    double avgTAT = sum1 / (double) n;
    double avgWT = sum2 / (double) n;

    cout << "\nThe Average Turn Around Time is: " << avgTAT << endl;
    cout << "\nThe Average Waiting Time is: " << avgWT << endl << endl; 

    return 0;
}


/* ---------------- OUTPUTS ---------------- */

// OUTPUT-1

/*

Enter the number of processes: 7

Enter the Arrival Time of 7 processes
0 1 2 3 4 5 6

Enter the Burst Time of 7 processes
4 2 3 5 1 4 6

Enter the Priority of 7 processes
2 4 6 10 8 12 9

Press 1 if the priority is low to high
Press 2 if the priority is high to low
2

PID     AT      BT      Priority        CT      TAT     WT
1       0       4          2            4       4       0
2       1       2          4            25      24      22
3       2       3          6            23      21      18
4       3       5          10           9       6       1
5       4       1          8            20      16      15
6       5       4          12           13      8       4
7       6       6          9            19      13      7

The Average Turn Around Time is: 13.1429

The Average Waiting Time is: 9.57143

*/

// OUTPUT-2

/*

Enter the number of processes: 7

Enter the Arrival Time of 7 processes
0 1 2 3 4 5 6

Enter the Burst Time of 7 processes
4 2 3 5 1 4 6

Enter the Priority of 7 processes
2 4 6 10 8 12 9

Press 1 if the priority is low to high
Press 2 if the priority is high to low
1

PID     AT      BT      Priority        CT      TAT     WT
1       0       4          2            4       4       0
2       1       2          4            6       5       3
3       2       3          6            9       7       4
4       3       5          10           21      18      13
5       4       1          8            10      6       5
6       5       4          12           25      20      16
7       6       6          9            16      10      4

The Average Turn Around Time is: 10

The Average Waiting Time is: 6.42857

*/