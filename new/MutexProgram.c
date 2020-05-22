#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define MAX_THREADS 1024
#define MAX_RANDOM 65535

int count_member;
int count_insert;
int count_delete;

int n = 0;
int m = 0;
int thread_count = 0;

float m_insert_frac, m_delete_frac, m_member_frac;

float m_insert, m_delete, m_member;

struct list_node_s *head = NULL;
pthread_mutex_t mutex;

struct list_node_s {
    int data;
    struct list_node_s *next;
};

int Insert(int value, struct list_node_s **head_pp);

int Delete(int value, struct list_node_s **head_pp);

int Member(int value, struct list_node_s *head_p);

double CalcTime(struct timeval time_begin, struct timeval time_end);

void *Thread_Operation();

int main(int argc, char *argv[]) {

    int n = 1000;
    int m = 10000;
    int caseNum;
//    int numTests;
    long thread_count;

    float probMember;
    float probInsert;
    float probDelete;

    printf("Enter case number : ");
    scanf("%d", &caseNum);

//    printf("Enter test run count: ");
//    scanf("%d", &numTests);

    printf("Enter number of threads: ");
    scanf("%ld", &thread_count);

    if (caseNum == 1)
    {
        probMember = 0.99;
        probInsert = 0.005;
        probDelete = 0.005;
    }
    else if (caseNum == 2)
    {
        probMember = 0.90;
        probInsert = 0.05;
        probDelete = 0.05;
    }

    else if (caseNum == 3)
    {
        probMember = 0.50;
        probInsert = 0.25;
        probDelete = 0.25;
    }

    pthread_t *thread_handlers;
    thread_handlers = malloc(sizeof(pthread_t) * thread_count);

    m_insert = probInsert * m;
    m_delete = probDelete * m;
    m_member = probMember * m;

    int i = 0;
    while (i < n) {
        if (Insert(rand() % 65535, &head) == 1)
            i++;
    }

    pthread_mutex_init(&mutex, NULL);

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    i = 0;
    while (i < thread_count) {
        pthread_create(&thread_handlers[i], NULL, (void *) Thread_Operation, NULL);
        i++;
    }

    i = 0;
    while (i < thread_count) {
        pthread_join(thread_handlers[i], NULL);
        i++;
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    pthread_mutex_destroy(&mutex);
    printf("Linked List with a single mutex Time Spent : %.6f secs\n", cpu_time_used);

    return 0;
}

// Linked List Membership function
int Member(int value, struct list_node_s *head_p) {
    struct list_node_s *current_p = head_p;

    while (current_p != NULL && current_p->data < value)
        current_p = current_p->next;

    if (current_p == NULL || current_p->data > value) {
        return 0;
    }
    else {
        return 1;
    }

}

// Linked List Insertion function
int Insert(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;
    struct list_node_s *temp_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;

        if (pred_p == NULL)
            *head_pp = temp_p;
        else
            pred_p->next = temp_p;

        return 1;
    }
    else
        return 0;
}

// Linked List Deletion function
int Delete(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            *head_pp = curr_p->next;
            free(curr_p);
        }
        else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }

        return 1;
    }
    else
        return 0;
}

void *Thread_Operation() {

    int count_tot = 0;

    int finished_member = 0;
    int finished_insert = 0;
    int delete_finished = 0;

    while (count_tot < m) {

        // Variable to randomly generate values for operations
        int random_value = rand() % MAX_RANDOM;

        // Variable to randomly select one of the three operations
        int random_select = rand() % 3;

        // Member operation
        if (random_select == 0 && finished_member == 0) {

            pthread_mutex_lock(&mutex);
            if (count_member < m_member) {
                Member(random_value, head);
                count_member++;
            }else
                finished_member =1;
            pthread_mutex_unlock(&mutex);
        }

        // Insert Operation
        if (random_select == 1 && finished_insert == 0) {

            pthread_mutex_lock(&mutex);
            if (count_insert < m_insert) {
                Insert(random_value, &head);
                count_insert++;
            }else
                finished_insert =1;
            pthread_mutex_unlock(&mutex);
        }

        // Delete Operation
        else if (random_select == 2 && delete_finished == 0) {

            pthread_mutex_lock(&mutex);
            if (count_delete < m_delete) {
                Delete(random_value, &head);
                count_delete++;
            }else
                delete_finished =1;
            pthread_mutex_unlock(&mutex);
        }

        // Updating the count
        count_tot = count_insert + count_member + count_delete;
    }
    return NULL;
}

// Calculating time
double CalcTime(struct timeval time_begin, struct timeval time_end) {

    return (double) (time_end.tv_usec - time_begin.tv_usec) / 1000000 + (double) (time_end.tv_sec - time_begin.tv_sec);
}