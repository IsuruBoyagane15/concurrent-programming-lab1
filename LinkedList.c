#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

unsigned bit;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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
    struct list_node_s* traversingNode = node;
    while (traversingNode != NULL)
    {
        printf(" %d ", traversingNode -> data);
        traversingNode = traversingNode->next;
    }
    printf("\n");
}

// #define ERR_NO_NUM -1
// #define ERR_NO_MEM -2

// int myRandom (int size) {
//     int i, n;
//     static int numNums = 0;
//     static int *numArr = NULL;

//     // Initialize with a specific size.

//     if (size >= 0) {
//         if (numArr != NULL)
//             free (numArr);
//         if ((numArr = malloc (sizeof(int) * size)) == NULL)
//             return ERR_NO_MEM;
//         for (i = 0; i  < size; i++)
//             numArr[i] = i;
//         numNums = size;
//     }

//     // Error if no numbers left in pool.

//     if (numNums == 0)
//         return ERR_NO_NUM;

//     // Get random number from pool and remove it (rnd in this
//     //   case returns a number between 0 and numNums-1 inclusive).

//     n = rand() % numNums;
//     i = numArr[n];
//     numArr[n] = numArr[numNums-1];
//     numNums--;
//     if (numNums == 0) {
//         free (numArr);
//         numArr = 0;
//     }

//     return i;
// }

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
    long numOperations = args -> numOperations;
    long threadId = args -> threadId;
    struct list_node_s **head = args->head;

    for (int i = 0; i < numOperations; ++i)
    {
        pthread_mutex_lock(&lock);
        int randNum = genUniqueRandNum(&threadId);
        pthread_mutex_unlock(&lock);
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
        else
        {
            printf("none");
        }
    }
    pthread_mutex_destroy(&lock);
    return EXIT_SUCCESS;
}

void readWriteLockProgram(long int* numOperations, struct list_node_s* head) {
    // pthread_rwlock_t rw_lock;
    // if (pthread_rwlock_init(&rw_lock, NULL) != 0) {
    //     printf("\n rw lock init has failed\n");
    //     return 1;
    // }
    // long lfsr = time(0);
    // for (int i = 0; i < *numOperations; ++i) {
    //     int randomChoice = rand() % 3;
    //     int randNum = genUniqueRandNum(&lfsr);
    //     if (randomChoice == 0) {
    //         printf("Member %d : ", randNum);
    //         pthread_rwlock_rdlock(&rw_lock);
    //         int result = Member(randNum, head);
    //         printf("%d\n", result);
    //         pthread_rwlock_unlock(&rw_lock);
    //     } else if (randomChoice ==1) {
    //         printf("Insert %d : ", randNum);
    //         pthread_rwlock_wrlock(&rw_lock);
    //         int result = Insert(randNum, &head);
    //         printf("%d\n", result);
    //         pthread_rwlock_wrlock(&rw_lock);
    //     } else if (randomChoice == 2) {
    //         printf("Delete %d : ", randNum);
    //         pthread_rwlock_wrlock(&rw_lock);
    //         int result = Delete(randNum, &head);
    //         printf("%d\n", result);
    //         pthread_rwlock_wrlock(&rw_lock);
    //     }
    // }
    // pthread_rwlock_destroy(&rw_lock);
}

int main()
{
    struct list_node_s* head = NULL;
    long numThreads;
    long numOperations;

    pthread_t *threadHandles;

    printf("Enter number of threads : ");
    scanf("%ld", &numThreads);
    printf("Enter number of operations to run : ");
    scanf("%ld", &numOperations);

    threadHandles = malloc(numThreads * sizeof(pthread_t));
    long thread;
    printf("pointer for thread id in main is %p\n",&thread);
    for (thread = 0; thread < numThreads; ++thread)
    {
        struct thread_args *thread_args_struct = malloc(sizeof(struct thread_args));
        thread_args_struct -> numOperations = numOperations;
        thread_args_struct -> head = &head;
        thread_args_struct -> threadId = thread + 1000;
        pthread_create(&threadHandles[thread], NULL, mutexProgram, (void *)thread_args_struct);
    }
    for (thread = 0; thread < numThreads; ++thread)
    {
        pthread_join(threadHandles[thread], NULL);
    }
    free(threadHandles);
    pthread_mutex_destroy(&lock);

    Traverse(head);

    return 0;
}