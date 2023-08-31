#include<iostream>
#include<queue>
using namespace std;

#define quantum 20          //Switching time between the queues
#define q_time 4            //Quantum time of the RR queue

typedef struct Process{
    int pid;                //Process ID 
    int priority;           //Priority ot the Process
    int b_time;             //Burst time of the Process
    int burst_time;         //Burst time of the Process at the initial stage
    int waiting_time;       //waitinf time of the Process
    int turnaround_time;    //Completion time of the Process 
}Process;

bool comparator(const Process &left, const Process &right){
    return left.b_time < right.b_time;
}

//Creating vectors to store the completed Processes
vector<Process> Completed_P0;
vector<Process> Completed_P1;
vector<Process> Completed_P2;
vector<Process> Completed_P3;

static int All_total_Time0 = 0;     //Time from the beginning of the queue scheduling to the end

queue<Process> RR(queue<Process> q){
    Process p;

    if(q.empty())
        return q;

    int comp_time = 0;              //Completed time of the Process
    int rem_time;                   //Remaning time of the Process
    while(!q.empty()){
        rem_time = q.front().b_time;
        if(rem_time <= (quantum - comp_time) || q_time <= (quantum - comp_time)){
            if(rem_time <= q_time){
                comp_time += rem_time;
                All_total_Time0 += rem_time;
                q.front().turnaround_time = All_total_Time0;
                q.front().waiting_time = All_total_Time0 - q.front().burst_time;
                Completed_P0.push_back(q.front());
                q.pop(); 
            }
            else {
                comp_time += q_time;
                rem_time -= q_time;
                All_total_Time0 += q_time;
                q.front().b_time = rem_time;
                q.push(q.front());
                q.pop();
            }
        }
        else {
            p.pid = q.front().pid;
            p.priority = q.front().priority;
            p.burst_time = q.front().burst_time;

            if(quantum != comp_time){
                q.front().b_time = rem_time - (quantum - comp_time);
                All_total_Time0 += (quantum - comp_time);
                if(p.b_time == 0)
                    break;
                
                q.push(q.front());
                q.pop();
                break;
            }
            else {
                q.front().b_time = rem_time - (quantum - comp_time);
                break;
            }
        }
    
        if(q.empty()){
            cout << "PID | Turnaround Time | Waiting Time" << endl;
                for(int i = 0; i < Completed_P0.size(); i++){
                    cout << " " << Completed_P0[i].pid << "          " << Completed_P0[i].turnaround_time << "             " << Completed_P0[i].waiting_time << endl;
                }
            cout << endl;
            Completed_P0.clear();
            break;
        }
    }
    return q;
}


queue<Process> SJF(queue<Process> q){
    Process p;
    vector<Process> v;

    if(q.empty())
        return q;

    int prio = q.front().priority;
    int id = q.front().pid;

    while(!q.empty()){
        v.push_back(q.front());
        q.pop();
    }

    sort(v.begin(), v.end(), &comparator);      //Sorting the queue by ascending order of the Burst Time

    for(int i=0; i<v.size(); i++){
        q.push(v[i]);
    }

    int total_time = 0;                 //Total time of the Processes(which started running)
    int rem_time = 0;                   //Remaining time of the Process
    int comp_time = 0;                  //Completed time of the Processes 
    while(!q.empty()){
        rem_time = q.front().b_time;
        total_time += rem_time;
        if(total_time <= quantum){
            if(prio == 1){
                All_total_Time0 += rem_time;
                q.front().turnaround_time = All_total_Time0;
                q.front().waiting_time = All_total_Time0 - q.front().burst_time;
                Completed_P1.push_back(q.front());
                q.pop();
            }
            else if(prio == 2){
                All_total_Time0 += rem_time;
                q.front().turnaround_time = All_total_Time0;
                q.front().waiting_time = All_total_Time0 - q.front().burst_time;
                Completed_P2.push_back(q.front());
                q.pop();
            }
        }
        else {
            p.pid = q.front().pid; 
            p.priority = q.front().priority;
            p.burst_time = q.front().burst_time;

            p.b_time = (total_time - quantum);
            q.front().b_time = (total_time - quantum);
            comp_time = rem_time - q.front().b_time;
            if(prio == 1){
                All_total_Time0 += comp_time;
            }
            else if(prio == 2){
                All_total_Time0 += comp_time;               
            }

            if(p.b_time == 0){
                break;
            }

            q.push(q.front());
            q.pop();
            break;
        }
            
        if(q.empty() && prio == 1){
            cout << "PID | Turnaround Time | Waiting Time" << endl;
            for(int i = 0; i < Completed_P1.size(); i++){
                cout << " " << Completed_P1[i].pid << "          " << Completed_P1[i].turnaround_time << "             " << Completed_P1[i].waiting_time << endl;
            }
            cout << endl;
            Completed_P1.clear();
            break;
        }
        else if(q.empty() && prio == 2){
            cout << "PID | Turnaround Time | Waiting Time" << endl;
            for(int i = 0; i < Completed_P2.size(); i++){
                cout << " " << Completed_P2[i].pid << "          " << Completed_P2[i].turnaround_time << "             " << Completed_P2[i].waiting_time << endl;
            }
            cout << endl;
            Completed_P2.clear();
            break;
        }
    }

    return q;
}


queue<Process> FCFS(queue<Process> q){
    Process p;

    if(q.empty())
        return q;

    int rem_time = 0;                   //Remaining time of the Process
    int total_time = 0;                 //Total time of the Processes in the queue(which started running)
    int comp_time = 0;                  //Completed time of the Process
    while(!q.empty()){
        rem_time = q.front().b_time;
        total_time += rem_time;
        
        if(total_time <= quantum){
            All_total_Time0 += rem_time;
            q.front().turnaround_time = All_total_Time0;
            q.front().waiting_time = All_total_Time0 - q.front().burst_time;
            Completed_P3.push_back(q.front());
            q.pop();
        }
        else {
            p.pid = q.front().pid;
            p.priority = q.front().priority;
            p.burst_time = q.front().burst_time;

            p.b_time = (total_time - quantum);
            q.front().b_time = (total_time - quantum);
            comp_time = rem_time - p.b_time;
            All_total_Time0 += comp_time;
            if(p.b_time == 0)
                break;
            
            q.push(q.front());
            q.pop();
            break;
        }
            
        if(q.empty()){
            cout << "PID | Turnaround Time | Waiting Time" << endl;
            for(int i = 0; i < Completed_P3.size(); i++){
                cout << " " << Completed_P3[i].pid << "          " << Completed_P3[i].turnaround_time << "             " << Completed_P3[i].waiting_time << endl;
            }
            cout << endl;
            Completed_P3.clear();
            break;
        }
    } 
    return q;
}


int main()
{
    int proc_num, quantum_time;
    cout << "Enter the number of Processes : ";
    cin >> proc_num;

    queue<Process> q0, q1, q2, q3;
    Process p0;

    int prio, burst_time;
    cout << "Enter the Priority(0-3) and the Burst Time : " << endl;
    for(int i=0; i<proc_num; i++){
        cout << "\tProcess p" << i+1 << ": " ;
        cin >> prio >> burst_time;
        p0.pid = i+1;
        p0.priority = prio;
        p0.b_time = burst_time;
        p0.burst_time = burst_time;

        if(p0.priority == 0){
            q0.push(p0);
        }
        else if(p0.priority == 1){
            q1.push(p0);
        }
        else if(p0.priority == 2){
            q2.push(p0);
        }
        else if(p0.priority == 3){
            q3.push(p0);
        }
    }

    while((!q0.empty()) || (!q1.empty()) || (!q2.empty()) || (!q3.empty())){
        q0 = RR(q0);
        q1 = SJF(q1);
        q2 = SJF(q2);
        q3 = FCFS(q3);
    }
}
