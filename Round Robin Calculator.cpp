// 31/12/2023

#include<iostream>
#include<cstdlib>
#include<queue>
#include<cstdio>
#include<fstream>
using namespace std;

// Declaration
typedef struct process
{
    int id, ArrivalT, BurstT, FinishT;
    float WaitT, TurnaroundT;
} process;

int sumBT;
process p[3], p1[3], temp;
queue<int> q1;

// Function Prototype
int accept();		// To receive needed data
void turnwait(int n);	// To calculate 
void display(int n);     // To display and does not return value
int ganttC(int n);
int readBT();
int readAT();

int interrupt = 0; // To count the interrupt occurred

int main()
{
    int i, n, tq, leave; // i= counter index,n=condition, tq, time quantum, ch+-= cha

    p[0].TurnaroundT = 0;
    p[0].WaitT = 0;

    readBT();
    readAT();

    do {
        n = accept();
        tq = ganttC(n);
        turnwait(n);
        display(n);

        cout << "\n\nWould you like to try again?\n(Enter 0 to leave)\n>> ";
        cin >> leave;
        system("cls");
    } while (leave > 0);

    return 0;
}

int readBT() { // Read Burst Time
    int BT;
    sumBT = 0;

    fstream BT_file("use your own file directory", ios::in);

    if (!BT_file) { //checks if the file was opened successfully.
        cout << "No such file"; //If it wasn’t print this
    }
    else { //if opened

        int k = 1;
        while (!BT_file.eof()) { //loop continues as long as we haven’t reached the end of the file
            BT_file >> BT; //reads the next burst time from the file and stores it in the variable BT.
            p[k].BurstT = BT;//stores the burst time we just read into the BurstT field of the kth process in the array p.
            ++k;//increments k, so the next time through the loop we’ll be reading the burst time for the next process.
            sumBT = sumBT + p[k].BurstT; //adds the burst time we just read to a running total sumBT. This is used later to calculate the average burst time.
        }
        BT_file.close(); //After all burst times have been read and stored, this line closes the file.
    }
}

int readAT() { // Read Arrival Time
    int AT;

    fstream AT_file("use your own file directory", ios::in);

    if (!AT_file) {
        cout << "No such file";
    }
    else {

        int k = 1;
        while (!AT_file.eof()) {
            AT_file >> AT;
            p[k].ArrivalT = AT;
            ++k;
        }

        AT_file.close();
        return 0;
    }
}

int accept() // Collect the needed Data from the user
{
    int i, n = 3; // n=> number of job(s)

    /*
    p==>burst Time process
    p1==> Arrival Time process
    */
    readBT();
    for (i = 1; i <= n; i++) {
        p[i].id = i;
    }

    cout << endl;

    readAT();
    for (i = 1; i <= n; i++)
    {
        p1[i] = p[i];
    }
    return n;
}

int ganttC(int n) // --> To see the gantt chart of RR
{
    int i, tq, m, nextval, nextarr;

    nextval = p1[1].ArrivalT;
    i = 1;

    cout << "\n\tEnter the Time Quantum: ";
    cin >> tq;

    for (i = 1; i <= n && p1[i].ArrivalT <= nextval; i++)
    {
        q1.push(p1[i].id);
    }

    while (!q1.empty()) //loop continues as long as there are still processes in the queue q1 that need to be scheduled.
    {
        m = q1.front(); //gets the ID of the next process from the front of the queue and removes it from the queue.
        q1.pop();

        if (p1[m].BurstT >= tq) //checks if the remaining burst time of the process is greater than or equal to the time quantum (tq). 
        {
            nextval = nextval + tq;// If it is, the process is executed for a time period tq.
            p1[m].BurstT = p1[m].BurstT - tq; //and its remaining burst time is reduced by tq.
        }
        else //remaining bt < tq, process executed till finish and remaining bt set to 0.
        {
            nextval = nextval + p1[m].BurstT;
            p1[m].BurstT = 0;
        }

        while (i <= n && p1[i].ArrivalT <= nextval)//checks for any processes that have arrived while the current process was executing.
        {
            q1.push(p1[i].id);//If a process has arrived, it is added to the end of the queue.
            i++;
        }

        if (p1[m].BurstT > 0)//If the current process still has remaining burst time after its time quantum has expired
        {
            q1.push(m);//it is added back to the end of the queue to be scheduled again later.
        }

        if (p1[m].BurstT <= 0) //If the current process has finished executing(its burst time is 0), its finish time is recorded.
        {
            p[m].FinishT = nextval;
        }
    }

    interrupt = sumBT / tq;//calculate number of interrupts
    
    // Initialize the sum of BT back to 0
    sumBT = 0;

    // Initialize the variable to 0
    p[0].TurnaroundT = 0;
    p[0].WaitT = 0;
    return tq;
}

void turnwait(int n)
{

    int i;

    for (i = 1; i <= n; i++)
    {
        p[i].TurnaroundT = p[i].FinishT - p[i].ArrivalT;        //calculate turnaround time
        p[i].WaitT = p[i].TurnaroundT - p[i].BurstT;            // calculate waiting time 
        p[0].TurnaroundT = p[0].TurnaroundT + p[i].TurnaroundT; // calculate total (all 30) turnaournd time 
        p[0].WaitT = p[0].WaitT + p[i].WaitT;                   // calculate total (all 30) waiting time 
    }

    p[0].TurnaroundT = p[0].TurnaroundT / n;                    //calculate average turnaround time
    p[0].WaitT = p[0].WaitT / n;								//calculate average waiting time
}

void display(int n)
{
    int i;

    system("CLS"); // Clear the Screen
    cout << "Result:";

    cout << "\n ---------------------------------------------------------------------------------";
    cout << "\n | JOB  |Arrival Time| Burst Time | Finish Time| Turnaround Time |  Waiting Time |";
    cout << "\n ---------------------------------------------------------------------------------";

    float totalTurnaroundTime = 0, totalWaitingTime = 0;

    for (i = 1; i <= n; i++)
    {
        printf("\n | %3d  |  %7d   |  %7d   |  %7d   |   %11.3f   |  %11.3f  |", p[i].id, p[i].ArrivalT, p[i].BurstT, p[i].FinishT, p[i].TurnaroundT, p[i].WaitT);
        cout << "\n ---------------------------------------------------------------------------------";

        totalTurnaroundTime += p[i].TurnaroundT;
        totalWaitingTime += p[i].WaitT;
    }

    // Display total turnaround time and total waiting time
    printf("\n");
    printf("\nTotal Turnaround Time: %2.3f", totalTurnaroundTime);
    printf("\nTotal Waiting Time: %2.3f\n", totalWaitingTime);

    // Aligned outputs
    printf("\nAverage Turn Around Time: %2.3f", p[0].TurnaroundT); // Display the average TA time in floating number (with 3 precision and 2 minimum width)
    printf("\nAverage Waiting Time: %2.3f", p[0].WaitT);             // Display the average Waiting Time in floating number (with 3 precision and 2 minimum width)
	
	printf("\n");
    printf("\nNo. of Interrupt: %3d", interrupt); // Display the number of interrupt occurred (print an integer, taking minimum 3 spaces)

    // Initialize the variable to 0
    p[0].TurnaroundT = 0;
    p[0].WaitT = 0;
}

