#include <bits/stdc++.h>
using namespace std;

bool increasing = true; // a boolen variable for PRIORITY high-to-low OR low-to-high

// this class is used for creating the custom comparator for our heap data structure which will act as the ready queue for the processes
class Compare{
public:
    bool operator()(pair<int, vector<int>> &A, pair<int, vector<int>> &B){
        if(increasing){ // is the priority is given as low to high then this logic will run
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
        else{ // this is for priority high to low
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

// completion time calculation function
void calculateCompletionTime(vector<vector<int>> &processes, vector<int> &CT, vector<pair<int,bool>> &RT, int n){
    int currentTime = 0;
    int i = 0;
    priority_queue<pair<int, vector<int>>, vector<pair<int, vector<int>>>, Compare> rq; // Ready Queue

    while(i < n){
        while(i < n && processes[i][1] <= currentTime){
            // {priority, at, pid, bt}
            rq.push({processes[i][3], {processes[i][1], processes[i][0], processes[i][2]}});
            i++;
        }
        if(!rq.empty()){ // if there is the process in ready queue schedule it
            auto feasibleProcess = rq.top();
            rq.pop();

            ganttChart.push_back("P" + to_string(feasibleProcess.second[1] + 1));
            if(!RT[feasibleProcess.second[1]].second){ // this is for calculating response time. The second parameter is the boolean to check if the process has arrived or not. If the process has arrived then we can store the time when it gets the CPU first.
                RT[feasibleProcess.second[1]].first = currentTime - feasibleProcess.second[0];
                RT[feasibleProcess.second[1]].second = true;
            }
            currentTime++;
            feasibleProcess.second[2]--; // since it is preemptive we can't schedule the whole burst time we decrease 1 unit
            if(feasibleProcess.second[2] == 0){ // if the burst time becomes zero then update it in our completion time array
                CT[feasibleProcess.second[1]] = currentTime;
            }
            else if(feasibleProcess.second[2] > 0){ // else if the process has remaining burst time then preempt it again in our ready queue
                rq.push({feasibleProcess.first, {feasibleProcess.second[0], feasibleProcess.second[1], feasibleProcess.second[2]}});
            }
        }
        else {
            currentTime++; // if no process arrive then increase the current time. 
            ganttChart.push_back("idle");
        }
    }

    while(!rq.empty()){ // this loop is for calculating the remaining process after all the process arrive in the ready queue.
        auto feasibleProcess = rq.top();
        rq.pop();

        ganttChart.push_back("P" + to_string(feasibleProcess.second[1] + 1));

        // SAME LOGIC AS ABOVE
        if(!RT[feasibleProcess.second[1]].second){
            RT[feasibleProcess.second[1]].first = currentTime - feasibleProcess.second[0];
            RT[feasibleProcess.second[1]].second = true;
        }
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
            cout << "Please enter a valid option!" << endl;
            goto choiceLoop;
        }

    vector<vector<int>> processes;
    for(int i=0; i<n; i++){
        processes.push_back({i, AT[i], BT[i], priority[i]});
    }

    sort(processes.begin(), processes.end(), cmp); // sort according to the arrival time
    vector<int> CT(n);
    vector<pair<int, bool>> RT(n); // we have used pair of int, bool because we've to find the time at which a process gets the cpu first, since it is inside the loop then we have to maintain a boolean variable so that we get the exact time when a process gets CPU.
    calculateCompletionTime(processes, CT, RT, n);

    cout << "\nGantt Chart:\n";
    cout << "|";
    for(auto& str: ganttChart){
        cout << str << "|";
    }
    cout << endl;

    sort(processes.begin(), processes.end()); // again sort the processes array according to the PID


    cout << "\nPID\tAT\tBT\tPriority\tCT\tTAT\tWT\tRT\n";
    double sum1 = 0, sum2 = 0;
    for(int i=0; i<n; i++){
        int pid = i + 1;
        int at = processes[i][1];
        int bt = processes[i][2];
        int priority = processes[i][3];
        int ct = CT[i];
        int rt = RT[i].first;
        int tat = ct - at;
        sum1 += tat;
        int wt = tat - bt;
        sum2 += wt;

        cout << pid << "\t" << at << "\t" << bt << "\t   " << priority << "\t\t" << ct << "\t" << tat << "\t" << wt << "\t" << rt << "\n";
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

PID     AT      BT      Priority        CT      TAT     WT      RT
1       0       4          2            25      25      21      0
2       1       2          4            22      21      19      0
3       2       3          6            21      19      16      0
4       3       5          10           12      9       4       0
5       4       1          8            19      15      14      14
6       5       4          12           9       4       0       0
7       6       6          9            18      12      6       6

The Average Turn Around Time is: 15

The Average Waiting Time is: 11.4286

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

PID     AT      BT      Priority        CT      TAT     WT      RT
1       0       4          2            4       4       0       0
2       1       2          4            6       5       3       3
3       2       3          6            9       7       4       4
4       3       5          10           21      18      13      13
5       4       1          8            10      6       5       5
6       5       4          12           25      20      16      16
7       6       6          9            16      10      4       4

The Average Turn Around Time is: 10

The Average Waiting Time is: 6.42857

*/