#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

unsigned bit;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t rw_lock = PTHREAD_RWLOCK_INITIALIZER;

unsigned genUniqueRandNum(long *lfsr)
{
    bit = ((*lfsr >> 0) ^ (*lfsr >> 2) ^ (*lfsr >> 3) ^ (*lfsr >> 5)) & 1;
    return *lfsr = (*lfsr >> 1) | (bit << 15);
}

//int Member(int value, struct list_node_s * head_p);
struct list_node_s
{
    int data;
    struct list_node_s *next;
};

struct thread_args
{
    long numOperations;
    long threadId;
    struct list_node_s **head;
};

int Member(int value, struct list_node_s *head_p)
{
    struct list_node_s *curr_p = head_p;

    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int Insert(int value, struct list_node_s **head_pp)
{
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;
    struct list_node_s *temp_p;

    while (curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }
    if (curr_p == NULL || curr_p->data > value)
    {
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
    {
        return 0;
    }
}

int Delete(int value, struct list_node_s **head_pp)
{
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value)
    {
        if (pred_p == NULL)
        {
            *head_pp = curr_p->next;
            free(curr_p);
        }
        else
        {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

void Traverse(struct list_node_s *node)
{
    struct list_node_s *traversingNode = node;
    while (traversingNode != NULL)
    {
        printf(" %d ", traversingNode->data);
        traversingNode = traversingNode->next;
    }
    printf("\n");
}

void serialProgram(long int *numOperations, struct list_node_s **head)
{
    for (int i = 0; i < *numOperations; ++i)
    {
        int randomChoice = rand() % 3;
        randomChoice = rand() % 3;
        randomChoice = rand() % 3;
        // int randNum = genUniqueRandNum();
        int randNum = 5;
        if (randomChoice == 0)
        {
            printf("Member %d : ", randNum);
            printf("%d\n", Member(randNum, *head));
        }
        else if (randomChoice == 1)
        {
            printf("Insert %d : ", randNum);
            printf("%d\n", Insert(randNum, head));
            printf("Member check %d\n", Member(randNum, *head));
        }
        else if (randomChoice == 2)
        {
            printf("Delete %d : ", randNum);
            printf("%d\n", Delete(randNum, head));
        }
    }
}

void *mutexProgram(void *ptr)
{
    struct thread_args *args = (struct thread_args *)ptr;
    long numOperations = args->numOperations;
    long threadId = args->threadId;
    struct list_node_s **head = args->head;

    for (int i = 0; i < numOperations; ++i)
    {
        int randNum = genUniqueRandNum(&threadId);
        int randomChoice = randNum % 3;
        if (randomChoice == 0)
        {
            pthread_mutex_lock(&lock);
            printf("Member %d : ", randNum);
            int result = Member(randNum, *head);
            printf("%d\n", result);
            pthread_mutex_unlock(&lock);
        }
        else if (randomChoice == 1)
        {
            pthread_mutex_lock(&lock);
            printf("Insert %d : ", randNum);
            int result = Insert(randNum, head);
            printf("%d\n", result);
            pthread_mutex_unlock(&lock);
        }
        else if (randomChoice == 2)
        {
            pthread_mutex_lock(&lock);
            printf("Delete %d : ", randNum);
            int result = Delete(randNum, head);
            printf("%d\n", result);
            pthread_mutex_unlock(&lock);
        }
    }
    return EXIT_SUCCESS;
}

void *readWriteLockProgram(void *ptr)
{
    printf("rw lock %p\n", &rw_lock);
    struct thread_args *args = (struct thread_args *)ptr;
    long numOperations = args->numOperations;
    long threadId = args->threadId;
    struct list_node_s **head = args->head;

    for (int i = 0; i < numOperations; ++i)
    {
        int randNum = genUniqueRandNum(&threadId);
        int randomChoice = randNum % 3;
        if (randomChoice == 0)
        {
            pthread_rwlock_rdlock(&rw_lock);
            printf("Member %d : ", randNum);
            int result = Member(randNum, *head);
            printf("%d\n", result);
            pthread_rwlock_unlock(&rw_lock);
        }
        else if (randomChoice == 1)
        {
            pthread_rwlock_wrlock(&rw_lock);
            printf("Insert %d : ", randNum);
            int result = Insert(randNum, head);
            printf("%d\n", result);
            pthread_rwlock_unlock(&rw_lock);
        }
        else if (randomChoice == 2)
        {
            pthread_rwlock_wrlock(&rw_lock);
            printf("Delete %d : ", randNum);
            int result = Delete(randNum, head);
            printf("%d\n", result);
            pthread_rwlock_unlock(&rw_lock);
        }
    }
    return EXIT_SUCCESS;
}

int main()
{
    struct list_node_s *head = NULL;
    long numThreads;
    long numOperations;

    pthread_t *threadHandles;

    // if (pthread_rwlock_init(&rw_lock, NULL) != 0) { 
    //     printf("\n rwlock init has failed\n"); 
    //     return 1; 
    // }

    printf("Enter number of threads : ");
    scanf("%ld", &numThreads);
    printf("Enter number of operations to run : ");
    scanf("%ld", &numOperations);

    threadHandles = malloc(numThreads * sizeof(pthread_t));
    long thread;
    printf("pointer for thread id in main is %p\n", &thread);
    for (thread = 0; thread < numThreads; ++thread)
    {
        struct thread_args *thread_args_struct = malloc(sizeof(struct thread_args));
        thread_args_struct->numOperations = numOperations;
        thread_args_struct->head = &head;
        thread_args_struct->threadId = thread + 1000;
        // pthread_create(&threadHandles[thread], NULL, mutexProgram, (void *)thread_args_struct);
        pthread_create(&threadHandles[thread], NULL, readWriteLockProgram, (void *)thread_args_struct);
    }
    int t;
    for (t = 0; t < numThreads; ++t)
    {
        pthread_join(threadHandles[t], NULL);
    }
    free(threadHandles);
    pthread_mutex_destroy(&lock);
    Traverse(head);

    return 0;
}