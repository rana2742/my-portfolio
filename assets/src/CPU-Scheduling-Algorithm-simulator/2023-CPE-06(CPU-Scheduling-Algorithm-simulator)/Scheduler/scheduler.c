#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define PID_LEN 10

typedef struct {
    char pid[PID_LEN];
    int arrival;
    int burst;
    int remaining;
    int completion;
    int waiting;
    int turnaround;
    int finished;
} Process;

/* ---------- Utility ---------- */
void reset(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].remaining = p[i].burst;
        p[i].finished = 0;
    }
}

/* ---------- FCFS ---------- */
void sort_by_arrival(Process p[], int n) {
    Process temp;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (p[i].arrival > p[j].arrival) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
}

void fcfs(Process p[], int n) {
    int time = 0;
    sort_by_arrival(p, n);

    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival)
            time = p[i].arrival;

        time += p[i].burst;
        p[i].completion = time;
        p[i].turnaround = time - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
    }
}

/* ---------- SJF (Non-preemptive) ---------- */
void sjf(Process p[], int n) {
    int time = 0, completed = 0;

    while (completed < n) {
        int idx = -1;
        int min_burst = 1e9;

        for (int i = 0; i < n; i++) {
            if (!p[i].finished && p[i].arrival <= time &&
                p[i].burst < min_burst) {
                min_burst = p[i].burst;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        time += p[idx].burst;
        p[idx].completion = time;
        p[idx].turnaround = time - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;
        p[idx].finished = 1;
        completed++;
    }
}

/* ---------- Round Robin ---------- */
void round_robin(Process p[], int n, int q) {
    int queue[MAX], front = 0, rear = 0;
    int time = 0, completed = 0;
    int in_queue[MAX] = {0};

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (!p[i].finished && !in_queue[i] &&
                p[i].arrival <= time) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }

        if (front == rear) {
            time++;
            continue;
        }

        int i = queue[front++];
        int run = (p[i].remaining < q) ? p[i].remaining : q;

        p[i].remaining -= run;
        time += run;

        for (int j = 0; j < n; j++) {
            if (!p[j].finished && !in_queue[j] &&
                p[j].arrival <= time) {
                queue[rear++] = j;
                in_queue[j] = 1;
            }
        }

        if (p[i].remaining == 0) {
            p[i].finished = 1;
            completed++;
            p[i].completion = time;
            p[i].turnaround = time - p[i].arrival;
            p[i].waiting = p[i].turnaround - p[i].burst;
        } else {
            queue[rear++] = i;
        }
    }
}

/* ---------- Main ---------- */
int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage:\n");
        printf("./scheduler fcfs input.txt output.txt\n");
        printf("./scheduler sjf  input.txt output.txt\n");
        printf("./scheduler rr   input.txt output.txt quantum\n");
        return 1;
    }

    FILE *fin = fopen(argv[2], "r");
    FILE *fout = fopen(argv[3], "w");

    if (!fin || !fout) {
        printf("File error\n");
        return 1;
    }

    Process p[MAX];
    int n = 0;
    char line[100];

    while (fgets(line, sizeof(line), fin)) {
        if (line[0] == '#' || strlen(line) < 2)
            continue;

        sscanf(line, "%s %d %d",
               p[n].pid, &p[n].arrival, &p[n].burst);
        n++;
    }

    fclose(fin);
    reset(p, n);

    if (strcmp(argv[1], "fcfs") == 0)
        fcfs(p, n);
    else if (strcmp(argv[1], "sjf") == 0)
        sjf(p, n);
    else if (strcmp(argv[1], "rr") == 0) {
        if (argc < 5) {
            printf("Time quantum required\n");
            return 1;
        }
        int q = atoi(argv[4]);
        round_robin(p, n, q);
    } else {
        printf("Unknown algorithm\n");
        return 1;
    }

    double avg_wt = 0, avg_tat = 0;

    fprintf(fout, "PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        fprintf(fout, "%s\t%d\t%d\t%d\t%d\t%d\n",
                p[i].pid, p[i].arrival, p[i].burst,
                p[i].completion, p[i].turnaround, p[i].waiting);
        avg_wt += p[i].waiting;
        avg_tat += p[i].turnaround;
    }

    fprintf(fout, "\nAverage Waiting Time: %.2f\n", avg_wt / n);
    fprintf(fout, "Average Turnaround Time: %.2f\n", avg_tat / n);

    fclose(fout);
    return 0;
}
