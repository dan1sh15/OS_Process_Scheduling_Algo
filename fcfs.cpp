#include <bits/stdc++.h>
using namespace std;

vector<string> ganttChart;

void computeCompletionTime(vector<vector<int>> &processes, int* CT, int n){
    int currTime = 0, i = 0;

    while(i < n){
        if(currTime >= processes[i][0]){
            ganttChart.push_back("P" + to_string(processes[i][1] + 1));
            CT[processes[i][1]] = currTime + processes[i][2]; // add the burst time
            currTime += processes[i][2]; // increase the current time
            i++;
        }
        else{
            ganttChart.push_back("idle");
            currTime++;
        }
    }
    return ;
}

void computeTurnAroundTime(int AT[], int CT[], int* TAT, int n){
    for(int i=0; i<n; i++){
        TAT[i] = CT[i] - AT[i];
    }
    return;
}

double averageTurnAroundTime(int* TAT, int n){
    double sum = 0;
    for(int i=0; i<n; i++) sum += TAT[i];

    return (double) sum / (double) n;
}

double averageWaitingTime(int* TAT, int BT[], int n){
    double sum = 0;
    for(int i=0; i<n; i++){
        sum += (TAT[i] - BT[i]);
    }

    return (double) sum / (double) n;
}

int main(){

    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    int AT[n];

    cout << "\nEnter the Arrival Time of " << n << " processes: \n";
    for (int i = 0; i < n; i++) {
        cin >> AT[i]; // sorted order
    }

    int BT[n];
    cout << "\nEnter the Burst Time of " << n << " processes: \n";
    for (int i = 0; i < n; i++) {
        cin >> BT[i];
    }

    int* CT = new int[n];
    int* TAT = new int[n];

    vector<vector<int>> processes;

    for(int i=0; i<n; i++){
        // {Arrival time, process id, burst time}
        processes.push_back({AT[i], i, BT[i]});
    }

    sort(processes.begin(), processes.end()); // sort according to the Arrival Time

    computeCompletionTime(processes, CT, n); // calculate the completion time
    computeTurnAroundTime(AT, CT, TAT, n); // calculate the turn around time


    cout << "\nGantt Chart: \n";
    cout << "|";
    for(auto str: ganttChart){
        cout << str << "|";
    }
    cout << endl;

    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for (int i = 0; i < n; i++) {
        cout << i + 1 << "\t" << AT[i] << "\t" << BT[i] << "\t" << CT[i] << "\t"
             << TAT[i] << "\t" << TAT[i] - BT[i] << endl;
    }

    cout << "\nThe Average Turn Around Time is: " << averageTurnAroundTime(TAT, n) << endl;
    cout << "\nThe Average Waiting Time is: " << averageWaitingTime(TAT, BT, n) << endl << endl;

    delete[] CT;
    delete[] TAT;

    return 0;
}

//          OUTPUT 1

/*
Enter the number of processes: 5

Enter the Arrival Time of 5 processes:
0 1 2 3 4

Enter the Burst Time of 5 processes:
4 3 1 2 5

PID     AT      BT      CT      TAT     WT
1       0       4       4       4       0
2       1       3       7       6       3
3       2       1       8       6       5
4       3       2       10      7       5
5       4       5       15      11      6

The Average Turn Around Time is: 6.8

The Average Waiting Time is: 3.8
*/

//          OUTPUT 2
/*
Enter the number of processes: 5

Enter the Arrival Time of 5 processes:
0 2 1 4 3

Enter the Burst Time of 5 processes:
5 1 2 3 4

PID     AT      BT      CT      TAT     WT
1       0       5       5       5       0
2       2       1       8       6       5
3       1       2       7       6       4
4       4       3       15      11      8
5       3       4       12      9       5

The Average Turn Around Time is: 7.4

The Average Waiting Time is: 4.4
*/

//          OUTPUT 3
/*
Enter the number of processes: 6

Enter the Arrival Time of 6 processes:
1 4 5 6 6 8

Enter the Burst Time of 6 processes:
1 3 4 5 6 1

PID     AT      BT      CT      TAT     WT
1       1       1       2       1       0
2       4       3       7       3       0
3       5       4       11      6       2
4       6       5       16      10      5
5       6       6       22      16      10
6       8       1       23      15      14

The Average Turn Around Time is: 8.5

The Average Waiting Time is: 5.16667
*/