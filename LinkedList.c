#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

unsigned bit;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t rw_lock = PTHREAD_RWLOCK_INITIALIZER;

unsigned genUniqueRandNum(unsigned short *lfsr)
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
    unsigned short threadId;
    float probMember;
    float probInsert;
    float probDelete;
    long numThreads;
    short unsigned seed;
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

void populateLinkedList(struct list_node_s **head, int n)
{
    unsigned short lfsr = time(0);
    for (int i = 0; i < n; ++i)
    {
        Insert(genUniqueRandNum(&lfsr), head);
    }
}

void SerialProgram(long int *numOperations, struct list_node_s **head, float probMember, float probInsert, float probDelete)
{
    for (int i = 0; i < *numOperations; ++i)
    {
        unsigned short lfsr = rand();
        int randNum = genUniqueRandNum(&lfsr);
        printf("randome num : %d\n", randNum);
        int randomChoice = rand() % 101;
        if (randomChoice < probMember)
        {
            // printf("Member %d : ", randNum);
            Member(randNum, *head);
        }
        else if (randomChoice >= probMember && randomChoice < probMember + probInsert)
        {
            // printf("Insert %d : ", randNum);
            Insert(randNum, head);
        }
        else if (randomChoice >= probMember + probInsert && randomChoice < probMember + probInsert + probDelete)
        {
            // printf("Delete %d : ", randNum);
            Delete(randNum, head);
        }
    }
}

void *mutexProgram(void *ptr)
{
    struct thread_args *args = (struct thread_args *)ptr;
    long numOperations = args->numOperations;
    unsigned short threadId = args->threadId;
    float probMember = args->probMember;
    float probInsert = args->probInsert;
    float probDelete = args->probDelete;
    long numThreads = args->numThreads;
    short unsigned seed = args->seed;
    struct list_node_s **head = args->head;

    for (int i = 0; i < numOperations; ++i){
        if (i%numThreads == threadId)
        {
            int randNum = genUniqueRandNum(&seed);
            int randomChoice = rand() % 101;
            if (randomChoice < probMember)
            {
                pthread_mutex_lock(&lock);
                // printf("Member %d : ", randNum);
                Member(randNum, *head);
                // printf("%d\n", result);
                pthread_mutex_unlock(&lock);
            }
            else if (randomChoice >= probMember && randomChoice < probMember + probInsert)
            {
                pthread_mutex_lock(&lock);
                // printf("Insert %d : ", randNum);
                Insert(randNum, head);
                // printf("%d\n", result);
                pthread_mutex_unlock(&lock);
            }
            else if (randomChoice >= probMember + probInsert && randomChoice < probMember + probInsert + probDelete)
            {
                pthread_mutex_lock(&lock);
                // printf("Delete %d : ", randNum);
                Delete(randNum, head);
                // printf("%d\n", result);
                pthread_mutex_unlock(&lock);
            }
        }
    }
    return EXIT_SUCCESS;
}

void *readWriteLockProgram(void *ptr)
{
    struct thread_args *args = (struct thread_args *)ptr;
    long numOperations = args->numOperations;
    unsigned short threadId = args->threadId;
    float probMember = args->probMember;
    float probInsert = args->probInsert;
    float probDelete = args->probDelete;
    long numThreads = args->numThreads;
    short unsigned seed = args->seed;
    struct list_node_s **head = args->head;

    for (int i = 0; i < numOperations; ++i)
    {
        if (i%numThreads == threadId){
            unsigned short randNum = genUniqueRandNum(&seed);
            int randomChoice = rand() % 101;
            if (randomChoice < probMember)
            {
                pthread_rwlock_rdlock(&rw_lock);
                // printf("Member %d : ", randNum);
                Member(randNum, *head);
                // printf("%d\n", result);
                pthread_rwlock_unlock(&rw_lock);
            }
            else if (randomChoice >= probMember && randomChoice < probMember + probInsert)
            {
                pthread_rwlock_wrlock(&rw_lock);
                // printf("Insert %d : ", randNum);
                Insert(randNum, head);
                // printf("%d\n", result);
                pthread_rwlock_unlock(&rw_lock);
            }
            else if (randomChoice >= probMember + probInsert && randomChoice < probMember + probInsert + probDelete)
            {
                pthread_rwlock_wrlock(&rw_lock);
                // printf("Delete %d : ", randNum);
                Delete(randNum, head);
                // printf("%d\n", result);
                pthread_rwlock_unlock(&rw_lock);
            }
        }
    }
    return EXIT_SUCCESS;
}

void runSerialProgram(struct list_node_s **header, float probMember, float probInsert, float probDelete)
{
    struct list_node_s **head = header;
    long numOperations;

    printf("Enter number of operations to run : ");
    scanf("%ld", &numOperations);

    SerialProgram(&numOperations, head, probMember, probInsert, probDelete);
}

void runMutexProgram(struct list_node_s **header, float probMember, float probInsert, float probDelete)
{
    struct list_node_s **head = header;
    long numThreads;
    long numOperations;

    pthread_t *threadHandles;

    printf("Enter number of threads : ");
    scanf("%ld", &numThreads);
    printf("Enter number of operations to run : ");
    scanf("%ld", &numOperations);

    threadHandles = malloc(numThreads * sizeof(pthread_t));
    unsigned short thread;
    for (thread = 0; thread < numThreads; ++thread)
    {
        struct thread_args *thread_args_struct = malloc(sizeof(struct thread_args));
        thread_args_struct->numOperations = numOperations;
        thread_args_struct->head = head;
        thread_args_struct->threadId = thread;
        thread_args_struct->probMember = probMember;
        thread_args_struct->probInsert = probInsert;
        thread_args_struct->probDelete = probDelete;
        thread_args_struct->numThreads = numThreads;
        thread_args_struct->seed = thread + 1000;
        pthread_create(&threadHandles[thread], NULL, mutexProgram, (void *)thread_args_struct);
    }
    int t;
    for (t = 0; t < numThreads; ++t)
    {
        pthread_join(threadHandles[t], NULL);
    }
    free(threadHandles);
    pthread_mutex_destroy(&lock);
}

void runRWLockProgram(struct list_node_s **header, float probMember, float probInsert, float probDelete)
{
    struct list_node_s **head = header;
    long numThreads;
    long numOperations;

    pthread_t *threadHandles;

    printf("Enter number of threads : ");
    scanf("%ld", &numThreads);
    printf("Enter number of operations to run : ");
    scanf("%ld", &numOperations);

    threadHandles = malloc(numThreads * sizeof(pthread_t));
    unsigned short thread;
    for (thread = 0; thread < numThreads; ++thread)
    {
        struct thread_args *thread_args_struct = malloc(sizeof(struct thread_args));
        thread_args_struct->numOperations = numOperations;
        thread_args_struct->head = head;
        thread_args_struct->threadId = thread;
        thread_args_struct->probMember = probMember;
        thread_args_struct->probInsert = probInsert;
        thread_args_struct->probDelete = probDelete;
        thread_args_struct->numThreads = numThreads;
        thread_args_struct->seed = thread + 1000;

        pthread_create(&threadHandles[thread], NULL, readWriteLockProgram, (void *)thread_args_struct);
    }
    int t;
    for (t = 0; t < numThreads; ++t)
    {
        pthread_join(threadHandles[t], NULL);
    }
    free(threadHandles);
    pthread_mutex_destroy(&lock);
}

int main()
{
    // int i = 0;
    // unsigned short lfsr = 140;
    // for (i = 0; i < 1000; i++)
    // {
    //     int randNum = genUniqueRandNum(&lfsr);
    //     printf("random %d\n", randNum);
    // }
    int numKeys;
    float probMember;
    float probInsert;
    float probDelete;
    printf("Enter amount of keys to populate : ");
    scanf("%d", &numKeys);
    printf("Enter probability of Member operations : ");
    scanf("%f", &probMember);
    printf("Enter probability of Insert operations : ");
    scanf("%f", &probInsert);
    printf("Enter probability of Delete operations : ");
    scanf("%f", &probDelete);

    struct list_node_s *head = NULL;
    populateLinkedList(&head, numKeys);
//    runSerialProgram(&head, probMember * 100, probInsert * 100, probDelete * 100);
    runMutexProgram(&head, probMember * 100, probInsert * 100, probDelete * 100);
//    runRWLockProgram(&head, probMember * 100, probInsert * 100, probDelete * 100);
    Traverse(head);
    return 0;
}