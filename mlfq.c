#include <stdio.h>
#include <stdlib.h>
#include<string.h>

// Structure to represent a task
struct task {
    int id;
    int at;
    int bt;
    int ct;
    int rem;
    int tat;
    int wt;
    int rt;
    int curr_q;
    int flag;
    int add;
};

/*
Defining variables to store time quanta, number of tasks, an array to store the tasks,
array to hold arrival and burst times, time quantum for each queue, global time tracker,
context switch counter (if required)
*/

int tq[4] = {2, 4, 8, -1};
int n;
struct task tasks[100]; 
int a_t[100], b_t[100]; 
int q_tq[4]; 
int current_time = 0;
int total_context_switches = 0; 

// Function to sort tasks based on arrival time
void bubble(){
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(a_t[j]>a_t[j+1]){
                int temp=a_t[j];
                a_t[j]=a_t[j+1];
                a_t[j+1]=temp;

                temp=b_t[j];
                b_t[j]=b_t[j+1];
                b_t[j+1]=temp;
            }
        }
    }
}


// Function to enqueue a task into a queue
void enq(int queue[],int *front,int *rear,int val){
    if(*rear==99){
        printf("Error: Queue is full\n");
        return;
    }
    (*rear)++;
    queue[*rear]=val;
    if(*front==-1){
        *front=0;
    }
}

// Function to dequeue a task from a queue
int deq(int queue[],int *front,int *rear){
    if(*front==-1){
        return -1;
    }
    int val=queue[*front];
    (*front)++;
    if(*front>*rear){
        *front=*rear=-1;
    }
    return val;
}

// Function to display the current status of all queues
void disp_q(int queues[][100], int fronts[], int rears[]) {
    printf("\nQueues:\n");
    for (int i = 0; i < 4; i++) {
        printf("Queue %d: ", i + 1);
        if(fronts[i]==-1){printf("%d Front index %d, Rear Index %d\n\n",0,fronts[i],rears[i]);continue;}
        for (int j = fronts[i]; j <= rears[i]; j++) {
            printf("%d ", queues[i][j] + 1);
        }
        printf("Front index %d, Rear Index %d\n",fronts[i],rears[i]);
        printf("\n");
    }
}

// Function to display the final results
void display() {
    printf("---------------------------------------------------");
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n");

    int total_tat = 0;
    int total_wt = 0;
    int total_rt = 0; 

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               tasks[i].id, tasks[i].at, tasks[i].bt,
               tasks[i].ct, tasks[i].tat, tasks[i].wt, tasks[i].rt);

        total_tat += tasks[i].tat;
        total_wt += tasks[i].wt;
        total_rt += tasks[i].rt;
    }

    float avg_tat = (float)total_tat / n; // Average Turnaround Time
    float avg_wt = (float)total_wt / n;  // Average Waiting Time
    float avg_rt = (float)total_rt / n;  // Average Response Time

    printf("---------------------------------------------------\n");
    printf("Average Turnaround Time (ATAT): %.2f\n", avg_tat);
    printf("Average Waiting Time (AWT): %.2f\n", avg_wt);
    printf("Average Response Time (ART): %.2f\n", avg_rt);
    printf("---------------------------------------------------\n");
}

// Function to implement Multi-Level Feedback Queue (MLFQ) scheduling
void mlfq() {

    int queues[4][100]; // Queues for different priority levels
    int fronts[4]; // Front indices for queues
    int rears[4]; // Rear indices for queues
    memset(queues,-1,sizeof(queues));
    memset(fronts,-1,sizeof(fronts));
    memset(rears,-1,sizeof(rears));
    int comp=0; // Completed tasks counter

    while(comp<n){
        // Add new tasks to the first queue if they arrive
        for (int i = 0; i < n; i++) {
            if (tasks[i].at <= current_time && tasks[i].add == 0) {
                    enq(queues[0], &fronts[0], &rears[0], i);
                printf("---------------------------------------------------\n");
                printf("NEW task HAS BEEN ADDED\n");
                disp_q(queues, fronts, rears);
                tasks[i].add=1;
                tasks[i].curr_q = 0;
            }
        }
        
        int task_found = 0;
        for (int i = 0; i < 4; i++) {
            // Check if any queue has tasks
            if (fronts[i] != -1) {
                task_found = 1;
                int process_id = deq(queues[i], &fronts[i], &rears[i]);
                struct task *p = &tasks[process_id];

                // Set response time if it's the first execution
                if (p->flag == 0) {
                    p->rt = current_time - p->at;
                    p->flag = 1;
                }

                if(i==3) { // FCFS for the last queue
                    int time_used = p->rem;
                    p->rem = 0;
                    current_time += time_used;
                    p->ct=current_time;
                    p->tat = current_time - p->at;
                    p->wt = p->tat - p->bt;
                    printf("---------------------------------------------------\n");
                    printf("\nProcess %d completed.\n", p->id);
                    comp++;

                } else { // Round-robin for other queues
                    int time_used = (q_tq[i] < p->rem) ? q_tq[i] : p->rem;
                    p->rem -= time_used;
                    current_time += time_used;

                    if (p->rem == 0) {
                        p->tat = current_time - p->at;
                        p->ct=current_time;
                        p->wt = p->tat - p->bt;
                        printf("---------------------------------------------------\n");
                        printf("\nProcess %d completed.\n", p->id);
                        comp++;
                    } else {
                        enq(queues[i + 1], &fronts[i + 1], &rears[i + 1], process_id);
                        p->curr_q = i + 1;
                    }
                }
                printf("---------------------------------------------------\n");
                printf("Status of Scheduler at t=%d\n",current_time);
                total_context_switches++;
                disp_q(queues, fronts, rears);
                printf("\n");
                break;
            }
        }

        if (!task_found) {
            // Idle time if no tasks are ready
            printf("---------------------------------------------------\n");
            printf("Status of Scheduler at t=%d\n",current_time);
            printf("No tasks to be processed at time %d\n",current_time);
            current_time++;
        }
    }
}

int main() {
    // Input number of tasks
    printf("Enter the number of tasks: ");
    scanf("%d", &n);

    // Randomly generate arrival and burst times for tasks
    for(int i=0;i<n;i++){
        a_t[i]=rand()%100;
        b_t[i]=rand()%100;
        while(b_t[i]==0) b_t[i]=rand()%20;
    }

    printf("\nBefore Sorting:\n");
    for (int i = 0; i < n; i++) {
        printf("a_t[%d] = %d, b_t[%d] = %d\n", i, a_t[i], i, b_t[i]);
    }

    // Sort tasks by arrival time
    bubble();

    printf("\nAfter Sorting:\n");
    for (int i = 0; i < n; i++) {
        printf("a_t[%d] = %d, b_t[%d] = %d\n", i, a_t[i], i, b_t[i]);
    }

    // Initialize tasks
    for (int i=0;i<n;i++) {
        tasks[i].at=a_t[i];
        tasks[i].bt=b_t[i];
        tasks[i].id = i+1;
        tasks[i].rem = tasks[i].bt;
        tasks[i].curr_q = 0;
        tasks[i].flag = 0;
        tasks[i].add=0;
    }

    // Copy time quantum values
    for (int i = 0; i < 4; i++) {
        q_tq[i] = tq[i];
    }

    // Run MLFQ scheduler
    mlfq();

    // Display final results
    display();
    return 0;
}
