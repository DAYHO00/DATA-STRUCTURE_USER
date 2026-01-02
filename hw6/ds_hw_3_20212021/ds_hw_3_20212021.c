#define _CRT_SECURE_NO_WARNINGS
#define INF 1e10
#define MAX_N 201

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

typedef struct Link {
    double len;
    bool congested;
    int dest;
    struct Link* next;
} Link;

typedef struct {
    Link* head;

} Adj_List[MAX_N];

typedef struct {
    int node;
    double time;

} PQ_Node;

typedef struct {
    PQ_Node* heap;
    int size;

} P_Queue;

void swap(PQ_Node* a, PQ_Node* b) {

    PQ_Node temp = *a;
    *a = *b;
    *b = temp;
}

bool is_empty(P_Queue* pq) {
    return pq->size == 0;
}

void push(P_Queue* pq, int node, double time) {

    pq->heap[pq->size].node = node;
    pq->heap[pq->size].time = time;

    int cur = pq->size++;

    while (cur > 0) {

        int par = (cur - 1) / 2;

        if (pq->heap[cur].time >= pq->heap[par].time) {
            break;
        }

        swap(&pq->heap[cur], &pq->heap[par]);
        cur = par;
    }
}

PQ_Node pop(P_Queue* pq) {

    int cur = 0;

    PQ_Node top = pq->heap[0];
    pq->heap[0] = pq->heap[--pq->size];

    while (1) {

        int left = 2 * cur + 1;
        int right = 2 * cur + 2;
        int smallest = cur;

        if (left < pq->size && pq->heap[left].time < pq->heap[smallest].time) {
            smallest = left;
        }

        if (right < pq->size && pq->heap[right].time < pq->heap[smallest].time) {
            smallest = right;
        }

        if (smallest == cur) {
            break;
        }

        swap(&pq->heap[cur], &pq->heap[smallest]);

        cur = smallest;
    }

    return top;
}


void ADD_edge(Adj_List GRAPH, int from, int to, double len, bool congest) {

    Link* link = malloc(sizeof(Link));
    link->len = len;
    link->dest = to;

    link->congested = congest;
    link->next = GRAPH[from].head;

    GRAPH[from].head = link;

}

double cal_time(double cur_time, double len, bool congested, double start, double end) {

    if (!congested) {
        return len;
    }

    double t_time = 0;
    double remain_dist = len;

    if (cur_time < start) {

        double n_time;

        if (start - cur_time < remain_dist) {
            n_time = start - cur_time;
        }
        else {
            n_time = remain_dist;
        }

        t_time += n_time;
        remain_dist -= n_time;
        cur_time += n_time;
    }

    if (cur_time >= start && cur_time < end && remain_dist > 0) {

        double congest_dist;
        if ((end - cur_time) / 2 < remain_dist) {
            congest_dist = (end - cur_time) / 2;
        }
        else {
            congest_dist = remain_dist;
        }

        t_time += congest_dist * 2;
        remain_dist -= congest_dist;
        cur_time += congest_dist * 2;
    }


    if (remain_dist > 0) {
        t_time += remain_dist;
    }

    return t_time;
}

void dijk(Adj_List GRAPH, int start, int node, double start_time, double end_time, double* dist, int* prev) {

    P_Queue PQ = { malloc(node * sizeof(PQ_Node)), 0 };

    bool visited[MAX_N] = { false };

    for (int i = 1; i <= node; ++i) {
        dist[i] = INF;
    }

    dist[start] = 0;
    push(&PQ, start, 0);

    while (!is_empty(&PQ)) {

        PQ_Node current = pop(&PQ);
        int cur_node = current.node;

        if (visited[cur_node]) {
            continue;
        }

        visited[cur_node] = true;

        for (Link* link = GRAPH[cur_node].head; link != NULL; link = link->next) {

            int next_node = link->dest;
            double t_time = cal_time(dist[cur_node], link->len, link->congested, start_time, end_time);

            if (dist[cur_node] + t_time < dist[next_node]) {
                dist[next_node] = dist[cur_node] + t_time;
                prev[next_node] = cur_node;
                push(&PQ, next_node, dist[next_node]);
            }
        }
    }

    free(PQ.heap);
}

void tracking_path(int* prev, int start, int end) {

    if (start == end) {

        printf("%d ", start);
        return;
    }

    tracking_path(prev, start, prev[end]);
    printf("%d ", end);
}

int main() {

    FILE* IN = fopen("input.txt", "r");
    if (!IN) {
        printf("WARNINGS : FILE OPEN ERROR");
        return 1;
    }

    int N;
    int M;

    double F1;
    double F2;

    fscanf(IN, "%d %d %lf %lf", &N, &M, &F1, &F2);

    Adj_List GRAPH = { NULL };

    for (int i = 0; i < M; ++i) {
        int A, B;
        double len;
        int congest_from, congest_to;

        fscanf(IN, "%d %d %lf %d %d", &A, &B, &len, &congest_from, &congest_to);
        ADD_edge(GRAPH, A, B, len, congest_from);
        ADD_edge(GRAPH, B, A, len, congest_to);
    }

    fclose(IN);

    double dist[MAX_N];
    int prev[MAX_N] = { 0 };

    dijk(GRAPH, 1, N, F1, F2, dist, prev);

    int F_node = 0;
    int S_node = 0;

    double F_time = INF;
    double S_time = 0;

    for (int i = 2; i <= N; ++i) {

        if (dist[i] < F_time) {
            F_time = dist[i];
            F_node = i;
        }

        if (dist[i] > S_time) {
            S_time = dist[i];
            S_node = i;
        }

    }

    printf("%f\n", F_time);
    tracking_path(prev, 1, F_node);

    printf("\n%f\n", S_time);
    tracking_path(prev, 1, S_node);

    printf("\n");

    return 0;
}
