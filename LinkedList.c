#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//int Member(int value, struct list_node_s * head_p);
struct list_node_s
{
    int data;
    struct list_node_s * next;
};

int Member(int value, struct list_node_s * head_p){
    struct list_node_s * curr_p = head_p;

    while (curr_p != NULL && curr_p ->data < value)
        curr_p = curr_p ->next;

    if (curr_p == NULL || curr_p ->data > value){
        return 0;
    }
    else{
        return 1;
    }
}

int Insert(int value, struct list_node_s** head_pp){
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while(curr_p != NULL && curr_p ->data <value){
        pred_p = curr_p;
        curr_p =curr_p->next;
    }
    if (curr_p == NULL || curr_p->data > value){
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL)
            *head_pp = temp_p;
        else
            pred_p->next = temp_p;
        return 1;
    }
    else{
        return 0;
    }

}

int Delete(int value, struct list_node_s** head_pp){
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;

    while(curr_p != NULL && curr_p->data < value){
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value){
        if (pred_p == NULL){
            *head_pp = curr_p->next;
            free(curr_p);
        } else{
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    } else{
        return 0;
    }
}

void Traverse(struct list_node_s * node) {
    while (node != NULL) {
        printf(" %d ", node->data);
        node = node->next;
    }
}

#define ERR_NO_NUM -1
#define ERR_NO_MEM -2

int myRandom (int size) {
    int i, n;
    static int numNums = 0;
    static int *numArr = NULL;

    // Initialize with a specific size.

    if (size >= 0) {
        if (numArr != NULL)
            free (numArr);
        if ((numArr = malloc (sizeof(int) * size)) == NULL)
            return ERR_NO_MEM;
        for (i = 0; i  < size; i++)
            numArr[i] = i;
        numNums = size;
    }

    // Error if no numbers left in pool.

    if (numNums == 0)
        return ERR_NO_NUM;

    // Get random number from pool and remove it (rnd in this
    //   case returns a number between 0 and numNums-1 inclusive).

    n = rand() % numNums;
    i = numArr[n];
    numArr[n] = numArr[numNums-1];
    numNums--;
    if (numNums == 0) {
        free (numArr);
        numArr = 0;
    }

    return i;
}


int main() {
    struct list_node_s * head = NULL;
    long thread_count;
    long n;
    long m;

//    printf("Enter number of threads :\n");
//    scanf("%ld", &thread_count);

    printf("Enter  n :\n");
    scanf("%ld", &n);

//    printf("Enter m :\n");
//    scanf("%ld", &m);

//    Insert(76, &head);
//    Insert(7, &head);
//    Insert(2, &head);
//    printf("76 is in ll %d\n",Member(76, head));
//    Delete(76,&head);
//    printf("76 is in ll %d\n",Member(76, head));
//    Traverse(head);

    int i;

    srand (time (NULL));
    i = myRandom (20);
    while (i >= 0) {
        printf ("Number = %3d\n", i);
        i = myRandom (-1);
    }
    printf ("Final  = %3d\n", i);
    return 0;
}