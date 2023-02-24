#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *newhead = malloc(sizeof(*newhead));

    if (!newhead)  // if malloc failed//
        return NULL;


    INIT_LIST_HEAD(newhead);
    return newhead;
}

/* Free all storage used by queue */


void q_free(struct list_head *l)
{
    if (!l)
        return;

    element_t *entry, *safe;

    list_for_each_entry_safe (entry, safe, l, list)
        q_release_element(entry);

    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *newnode = malloc(sizeof(*newnode));
    if (!newnode)
        return false;

    int len = strlen(s) + 1;
    newnode->value = malloc(len * sizeof(char));

    if (!(newnode->value)) {
        free(newnode);
        return false;
    }

    memcpy(newnode->value, s, len);
    list_add(&newnode->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *newnode = malloc(sizeof(*newnode));
    if (!newnode)
        return false;

    int len = strlen(s) + 1;
    newnode->value = malloc(len * sizeof(char));

    if (!(newnode->value)) {
        free(newnode);
        return false;
    }
    memcpy(newnode->value, s, len);
    list_add_tail(&newnode->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || !sp)
        return NULL;
    if (list_empty(head))
        return NULL;

    struct list_head *node = head->next;
    if (node == head)
        return NULL;

    element_t *rem = list_entry(node, element_t, list);
    int len = strnlen(rem->value, bufsize - 1);
    memcpy(sp, rem->value, len);
    *(sp + len * sizeof(char)) = '\0';

    list_del(node);

    return rem;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || !sp)
        return NULL;

    struct list_head *node = head->prev;
    if (node == head)
        return NULL;

    element_t *el = list_entry(node, element_t, list);
    int len = strnlen(el->value, bufsize - 1);
    memcpy(sp, el->value, len);
    *(sp + len * sizeof(char)) = '\0';

    list_del(node);

    return el;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    int mid = (q_size(head) / 2) + 1;

    for (int i = 0; i < mid; i++) {
        head = head->next;
    }

    element_t *del;
    list_del(head);
    del = list_entry(head, element_t, list);
    q_release_element(del);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    struct list_head *now, *safe;

    list_for_each_safe (now, safe, head) {
        if (list_entry(now, element_t, list)->value ==
            list_entry(safe, element_t, list)->value)
            list_del(safe->prev);
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
