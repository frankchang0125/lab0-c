#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));

    if (!q) {
        return NULL;
    }

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q) {
        return;
    }

    list_ele_t *cur = q->head;

    while (cur) {
        list_ele_t *node = cur;
        cur = cur->next;
        free(node->value);
        free(node);
    }

    /* Free queue structure */
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }

    list_ele_t *node = q_allocate_node(s);

    if (!node) {
        return false;
    }

    node->next = q->head;
    q->head = node;

    if (!q->tail) {
        q->tail = node;
    }

    q->size += 1;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }

    list_ele_t *node = q_allocate_node(s);

    if (!node) {
        return false;
    }

    if (!q->head) {
        q->head = node;
    }

    if (q->tail) {
        q->tail->next = node;
    }

    q->tail = node;
    q->size += 1;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head) {
        return false;
    }

    list_ele_t *head = q->head;
    q->head = q->head->next;

    if (q->tail == head) {
        q->tail = NULL;
    }

    if (sp) {
        strncpy(sp, head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    free(head->value);
    free(head);
    q->size -= 1;

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }

    list_ele_t *prev, *cur, *next;

    prev = NULL;
    cur = q->head;
    q->tail = q->head;

    while (cur) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }

    q->head = prev;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }

    if (q->head == q->tail) {
        return;
    }

    q->head = merge_sort(q->head);

    // Update q->tail.
    q->tail = q->head;
    while (q->tail && q->tail->next) {
        q->tail = q->tail->next;
    }
}

/*
 * Allocate node space for given string.
 * Return NULL if s is NULL, empty string
 * or could not allocate space.
 * Otherwise, return the address of the allocated node.
 */
list_ele_t *q_allocate_node(char *s)
{
    if (!s) {
        return NULL;
    }

    size_t s_len = strlen(s) + 1;

    if (s_len == 1) {
        return NULL;
    }

    list_ele_t *node = malloc(sizeof(list_ele_t));

    if (!node) {
        return NULL;
    }

    node->next = NULL;
    node->value = malloc(s_len);

    if (!node->value) {
        free(node);
        return NULL;
    }

    memcpy(node->value, s, s_len);

    return node;
}

/*
 * Performs merge sort to sort the nodes
 * of given list.
 */
list_ele_t *merge_sort(list_ele_t *head)
{
    if (!head || !head->next) {
        return head;
    }

    /* Use fast/slow pointers to split list
     * into left and right parts.
     */
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    list_ele_t *left = head;
    list_ele_t *right = slow->next;
    slow->next = NULL;

    /* Perform merge sort on left part and right part lists separately. */
    left = merge_sort(left);
    right = merge_sort(right);

    /* Merge the sorted left part and right part lists into one list. */
    return merge(left, right);
}

/*
 * Merge left list and right list into one list,
 * ordered by the string value of each node in
 * ascending order.
 */
list_ele_t *merge(list_ele_t *left, list_ele_t *right)
{
    if (!left) {
        return right;
    }

    if (!right) {
        return left;
    }

    list_ele_t *result = NULL;
    list_ele_t *cur = NULL;

    // Initialize, compare left and right list's first node's value
    // string length, point result to the node with shorter string length
    // determined by strcasecmp().
    if (strcasecmp(left->value, right->value) < 1) {
        result = cur = left;
        left = left->next;
    } else {
        result = cur = right;
        right = right->next;
    }

    while (left && right) {
        if (strcasecmp(left->value, right->value) < 1) {
            cur->next = left;
            left = left->next;
        } else {
            cur->next = right;
            right = right->next;
        }

        cur = cur->next;
    }

    // Left list has nodes left.
    if (left) {
        cur->next = left;
    }

    // Right list has nodes left.
    if (right) {
        cur->next = right;
    }

    return result;
}
