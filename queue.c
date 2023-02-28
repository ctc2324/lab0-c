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

    struct list_head *now = head->next, *next = now->next;
    bool dup = false;

    while (now != head && next != head) {
        element_t *now_entry = list_entry(now, element_t, list);
        element_t *next_entry = list_entry(next, element_t, list);

        while (next != head && !strcmp(now_entry->value, next_entry->value)) {
            list_del(next);
            q_release_element(next_entry);
            next = now->next;
            next_entry = list_entry(next, element_t, list);
            dup = true;
        }

        if (dup) {
            list_del(now);
            q_release_element(now_entry);
            dup = false;
        }

        now = next;
        next = now->next;
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head))
        return;
    struct list_head *p = head->next;
    while (p != head && p->next != head) {
        struct list_head *q = p->next;
        list_move(p, q);
        p = p->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *now, *safe, *tmp;
    list_for_each_safe (now, safe, head) {
        tmp = now->prev;
        now->prev = now->next;
        now->next = tmp;
    }
    tmp = head->prev;
    head->prev = head->next;
    head->next = tmp;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || k <= 1)
        return;

    struct list_head *now, *safe, *tmp = head;
    LIST_HEAD(rev);
    int cnt = k;
    list_for_each_safe (now, safe, head) {
        cnt--;
        if (cnt == 0) {
            list_cut_position(&rev, tmp, now);
            q_reverse(&rev);
            list_splice(&rev, tmp);
            cnt = k;
            tmp = safe->prev;
        }
    }
}

struct list_head *merge(struct list_head *l1, struct list_head *l2)
{
    struct list_head *head = NULL, **ptr = &head, **node;
    for (node = NULL; l1 && l2; *node = (*node)->next) {
        node = (strcmp(list_entry(l1, element_t, list)->value,
                       list_entry(l2, element_t, list)->value) < 0)
                   ? &l1
                   : &l2;
        *ptr = *node;
        ptr = &(*ptr)->next;
    }
    if (l1)
        *ptr = l1;
    else
        *ptr = l2;
    return head;
}

struct list_head *merge_sort(struct list_head *head)
{
    if (!head || !head->next)
        return head;

    struct list_head *fast, *slow = head, *mid;

    for (fast = head->next; fast && fast->next; fast = fast->next->next)
        slow = slow->next;

    mid = slow->next;
    slow->next = NULL;

    struct list_head *left = merge_sort(head);
    struct list_head *right = merge_sort(mid);

    return merge(left, right);
}


/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    head->prev->next = NULL;
    head->next = merge_sort(head->next);

    // back to double link-list
    struct list_head *prev = NULL, *now = head;

    while (now) {
        now->prev = prev;
        prev = now;
        now = now->next;
    }
    head->prev = prev;
    prev->next = head;
}


/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;
    int size = 0;
    struct list_head *now = head->prev;
    while (now != head) {
        size++;
        if (now->prev == head)
            break;
        element_t *now_entry = list_entry(now, element_t, list);
        element_t *prev_entry = list_entry(now->prev, element_t, list);

        if (strcmp(now_entry->value, prev_entry->value) > 0) {
            list_del(&prev_entry->list);
            q_release_element(prev_entry);
            size--;
        } else {
            now = now->prev;
        }
    }
    return size;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;
    queue_contex_t *fir = container_of(head->next, queue_contex_t, chain);
    if (head->next == head->prev)
        return fir->size;
    for (struct list_head *cur = head->next->next; cur != head;
         cur = cur->next) {
        queue_contex_t *que = container_of(cur, queue_contex_t, chain);
        list_splice_init(que->q, fir->q);
        que->size = 0;
    }
    q_sort(fir->q);
    fir->size = q_size(fir->q);
    return fir->size;
}
