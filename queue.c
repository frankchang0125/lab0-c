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
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
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
