#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;

// Function to Calculate waiting time and average waiting time for FCFS
void CalculateWaitingTimeFCFS(int at[], int bt[], int N) {
    int wt[N];
    wt[0] = 0;

    cout << "PN\t\tAT\t\tBT\t\tWT\n\n";
    cout << "1" << "\t\t" << at[0] << "\t\t" << bt[0] << "\t\t" << wt[0] << endl;

    for (int i = 1; i < N; i++) {
        wt[i] = (at[i - 1] + bt[i - 1] + wt[i - 1]) - at[i];
        cout << i + 1 << "\t\t" << at[i] << "\t\t" << bt[i] << "\t\t" << wt[i] << endl;
    }

    float average = 0, sum = 0;
    for (int i = 0; i < N; i++) {
        sum += wt[i];
    }

    average = sum / N;
    cout << "\nAverage waiting time (FCFS) = " << average << endl;
}

// Function to Calculate waiting time and average waiting time for SJN
void CalculateWaitingTimeSJN(int at[], int bt[], int N) {
    int wt[N];
    int processes[N];
    
    // Create an array of processes and sort based on burst time
    for (int i = 0; i < N; i++) {
        processes[i] = i;
    }
    
    // Sort burst time array along with the process order
    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            if (bt[processes[i]] > bt[processes[j]]) {
                swap(processes[i], processes[j]);
            }
        }
    }

    // Waiting time for the first process
    wt[processes[0]] = 0;
    cout << "PN\t\tAT\t\tBT\t\tWT\n\n";
    
    for (int i = 1; i < N; i++) {
        wt[processes[i]] = bt[processes[i - 1]] + wt[processes[i - 1]] - at[processes[i]];
        cout << processes[i] + 1 << "\t\t" << at[processes[i]] << "\t\t" << bt[processes[i]] << "\t\t" << wt[processes[i]] << endl;
    }

    float average = 0, sum = 0;
    for (int i = 0; i < N; i++) {
        sum += wt[i];
    }

    average = sum / N;
    cout << "\nAverage waiting time (SJN) = " << average << endl;
}

// Function to implement Round Robin Scheduling
void CalculateWaitingTimeRoundRobin(int at[], int bt[], int N, int quantum) {
    int wt[N], rem_bt[N];
    float average = 0;
    
    // Initialize remaining burst time to be the same as burst time
    for (int i = 0; i < N; i++) {
        rem_bt[i] = bt[i];
        wt[i] = 0;
    }

    queue<int> q;
    q.push(0);
    int current_time = 0;
    
    while (!q.empty()) {
        int i = q.front();
        q.pop();

        if (rem_bt[i] > quantum) {
            current_time += quantum;
            rem_bt[i] -= quantum;
            q.push(i);
        } else {
            current_time += rem_bt[i];
            wt[i] = current_time - at[i] - bt[i];
            rem_bt[i] = 0;
        }
    }

    cout << "PN\t\tAT\t\tBT\t\tWT\n\n";
    for (int i = 0; i < N; i++) {
        cout << i + 1 << "\t\t" << at[i] << "\t\t" << bt[i] << "\t\t" << wt[i] << endl;
    }

    for (int i = 0; i < N; i++) {
        average += wt[i];
    }

    average /= N;
    cout << "\nAverage waiting time (Round Robin) = " << average << endl;
}

int main() {
    int N = 5;
    int at[] = {0, 1, 2, 3, 4}; // Arrival Time
    int bt[] = {4, 3, 1, 2, 5}; // Burst Time

    int choice;
    
    
    int exit = 1;
	while(exit){
		cout << "Select Scheduling Algorithm:" << endl << "1. FCFS (First-Come, First-Served)" << endl<< "2. SJN (Shortest Job Next)" << endl<< "3. Round Robin" << endl <<"4. Exit" <<endl;
    	cout << "Enter your choice (1-4): ";
		cin >> choice;
    	switch (choice) {
        	case 1:
            	CalculateWaitingTimeFCFS(at, bt, N);
            	break;
        	case 2:
            	CalculateWaitingTimeSJN(at, bt, N);
            	break;
        	case 3: {
            	int quantum;
            	cout << "Enter time quantum for Round Robin: ";
            	cin >> quantum;
            	CalculateWaitingTimeRoundRobin(at, bt, N, quantum);
            	break;
        	}
        	case 4:
        		exit = 0;
    	}
	}
    return 0;
}

