#include <cstdlib>
#include <iostream>
#include <queue>
#include <fstream> // For file handling
#include <sstream> // For string stream manipulation
#include <string>  // For string usage
using namespace std;

class process {
public:
    int p_no = 0;
    int start_AT = 0, AT = 0, BT_left = 0, BT = 0, temp_BT = 0;
    int CT = 0, TAT = 0, WT = 0, RT = 0;
    int priority = 0;

    // Function for completion time
    void set_CT(int time) {
        CT = time;
        set_TAT();
        set_WT();
    }

    void set_TAT() {
        TAT = CT - start_AT;
    }

    void set_WT() {
        WT = TAT - BT;
    }

    void P_set() {
        start_AT = AT;
        BT_left = BT;
    }

    void set_RT(int time) {
        RT = time - start_AT;
    }

    friend bool operator<(const process& a, const process& b) {
        return a.AT > b.AT;
    }
};

void readTasksFromFile(const string& filename, priority_queue<process>& ready_queue) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    getline(file, line); // Skip the header line

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<int> taskDetails;

        while (getline(ss, token, ',')) {
            taskDetails.push_back(stoi(token));
        }

        if (taskDetails.size() == 4) {
            process temp;
            temp.p_no = taskDetails[0];
            temp.AT = taskDetails[1];
            temp.BT = taskDetails[2];
            temp.priority = taskDetails[3];
            temp.P_set();
            ready_queue.push(temp);
        }
    }
    file.close();
}

priority_queue<process> RR_run(priority_queue<process> ready_queue, int Time_Slice, queue<process>* gantt) {
    priority_queue<process> completion_queue;
    process p;
    int clock = 0;

    while (!ready_queue.empty()) {
        while (clock < ready_queue.top().AT) {
            p.temp_BT++;
            clock++;
        }
        if (p.temp_BT > 0) {
            p.p_no = -1;
            p.CT = clock;
            (*gantt).push(p);
        }
        p = ready_queue.top();
        ready_queue.pop();

        if (p.AT == p.start_AT)
            p.set_RT(clock);

        while (p.BT_left > 0 && (p.temp_BT < Time_Slice || ready_queue.empty() || clock < ready_queue.top().AT)) {
            p.temp_BT++;
            p.BT_left--;
            clock++;
        }

        if (p.BT_left == 0) {
            p.AT = p.start_AT;
            p.set_CT(clock);
            (*gantt).push(p);
            p.temp_BT = 0;
            completion_queue.push(p);
        } else {
            p.AT = clock;
            p.CT = clock;
            (*gantt).push(p);
            p.temp_BT = 0;
            ready_queue.push(p);
        }
    }

    return completion_queue;
}

void disp(priority_queue<process> main_queue) {
    cout << "\nProcess details:\n";
    while (!main_queue.empty()) {
        process p = main_queue.top();
        main_queue.pop();
        cout << "Process P" << p.p_no << ": Arrival Time: " << p.start_AT
             << ", Burst Time: " << p.BT << ", Completion Time: " << p.CT
             << ", Turnaround Time: " << p.TAT << ", Waiting Time: " << p.WT
             << ", Response Time: " << p.RT << endl;
    }
}

int main() {
    priority_queue<process> ready_queue;
    queue<process> gantt;

    string filename = "tasks.csv"; // File with process details
    readTasksFromFile(filename, ready_queue);

    int tq = 2; // Time quantum for round robin

    auto completion_queue = RR_run(ready_queue, tq, &gantt);
    disp(completion_queue);

    return 0;
}

