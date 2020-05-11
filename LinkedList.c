#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

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

int main() {
    struct list_node_s * head = NULL;

    printf("76 is in ll %d\n",Member(76, head));

//    Insert(76, &head);
//    Insert(56, &head);
//    Insert(8, &head);
//    Insert(9, &head);
//
//    printf("76 is in ll %d\n",Member(76, head));
//    printf("9 is in ll %d\n",Member(9, head));
    printf("9 is in ll %d\n",Member(9, head));
    Delete(9,&head);
    printf("9 is in ll %d\n",Member(9, head));

    return 0;
}