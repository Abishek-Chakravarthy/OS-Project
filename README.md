# Multilevel Feedback Queue (MLFQ) Scheduling Algorithm in C

This project implements a **Multilevel Feedback Queue (MLFQ) Scheduling Algorithm** in C. MLFQ is a process scheduling technique where processes are organized into multiple priority queues, each with a distinct time quantum. Lower-priority queues handle processes in **First-Come-First-Serve (FCFS)** mode, while higher-priority queues follow **Round-Robin (RR)** scheduling.

---

## Key Features

### **Task Structure**
Each task maintains the following details:
- **Arrival Time (AT):** The time at which a task arrives in the system.
- **Burst Time (BT):** The total time required by a task for execution.
- **Completion Time (CT):** The time at which a task completes execution.
- **Remaining Time (Rem):** The time remaining for a task to complete execution.
- **Current Queue:** The queue level where the task is currently assigned.
- **Turnaround Time (TAT):** The total time taken by a task from arrival to completion.
- **Waiting Time (WT):** The total time a task waits in queues before execution.
- **Response Time (RT):** The time from arrival to the first execution of the task.

---

### **Enqueue/Dequeue Operations**
Helper functions are used to:
- Manage tasks in queues.
- Ensure smooth transitions between different levels of the scheduler.

---

### **Scheduler Execution**
- **New Task Handling:** Newly arrived tasks are added to the **highest-priority queue**.
- **Queue Processing:**
  - Tasks in higher-priority queues are processed first using their respective time quantum.
  - If a task isn’t completed within its time quantum, it is moved to a **lower-priority queue**.
- **Lowest-Priority Queue:** Tasks in the lowest-priority queue are processed using **FCFS** until completion.

---

### **Performance Metrics**
The program calculates and displays the following metrics for each task:
- **Turnaround Time (TAT):** The total time taken by a task from arrival to completion.
- **Waiting Time (WT):** The total time a task waits in queues before execution.
- **Response Time (RT):** The time from arrival to the first execution of the task.

---

## Output Details
- **Queue Status:** Displays the status of all queues at each time step.
- **Task Completions:** Highlights completed tasks.
- **Final Results:**
  - Metrics for each task, including TAT, WT, and RT.
  - **Average Metrics:**
    - **Average Turnaround Time (ATAT)**
    - **Average Waiting Time (AWT)**
    - **Average Response Time (ART)**

---

## Simulation Summary
The program generates random **arrival times** and **burst times** for processes, sorts them by arrival time, and simulates the scheduling process across four levels of queues.

### Key Features Demonstrated:
- Dynamic task priority management.
- Context switches.
- Task preemption.

This project provides a comprehensive understanding of MLFQ scheduling, showcasing its adaptability to varying task requirements while maintaining efficiency.

---

## How to Run
1. Clone the repository:
   ```bash
   git clone <repository_url>
