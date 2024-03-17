#include <bits/stdc++.h>
using namespace std;

static bool cmp(vector<int> &A, vector<int> &B){
    return A[1] < B[1]; // SORT ACCORDING TO THE ARRIVAL TIME
}

vector<string> ganttChart;

void computeCompletionTime(vector<vector<int>> &processes, int* CT, int n){
    priority_queue<pair<int,pair<int,int>>, vector<pair<int,pair<int,int>>>, greater<pair<int,pair<int,int>>>> rq; // create a minheap {BT, AT, PID}

    int currTime = 0, i = 0;
    while(i < n){
        // push into the ready queue
        while(i < n && processes[i][1] <= currTime){
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
    std::sort(processes.begin(), processes.end(), cmp); // sort according to the arrival time
    computeCompletionTime(processes, CT, n);

    cout << "\nGantt Chart: \n";
    cout << "|";
    for(auto& s: ganttChart){
        cout << s << "|";
    }
    cout << endl << endl;

    std::sort(processes.begin(), processes.end()); // sorting again according to the process id
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
2       2       5       12      10      5
3       3       1       5       2       1
4       4       2       7       3       1
5       5       8       20      15      7

The Average Turn Around Time is: 6.6

The Average Waiting Time is: 2.8
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
2       4       4       15      11      7
3       2       2       4       2       0
4       2       4       8       6       2
5       7       2       11      4       2
6       8       1       9       1       0

The Average Turn Around Time is: 4.16667

The Average Waiting Time is: 1.83333
*/