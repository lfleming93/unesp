#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "linkedlist.h"

static pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct _list_node_t {
  int key;
  struct _list_node_t *next;
} list_node_t;


/*
 * Initializes the list. Needs to be called only once at the
 * beginning.
 */
void list_init(list_node_t **head)
{
  *head = (list_node_t *)malloc(sizeof(list_node_t));
  (*head)->key = INT_MIN;
  (*head)->next = (list_node_t *)malloc(sizeof(list_node_t));
  (*head)->next->key = INT_MAX;
  (*head)->next->next = NULL;
}

/*
 * Returns the number of elements in the list pointed by head.
 */
size_t list_size(list_node_t *head)
{
  size_t count = 0;
  while (head != NULL) {
    count++;
    head = head->next;
  }

  return count;
}

/*
 * Adds an item to the given list.
 */
int list_add(list_node_t *head, int item)
{
  list_node_t *pred, *curr;
  pthread_mutex_lock(&global_lock);

  pred = head;
  curr = head->next;
  while (curr->key < item) {
    pred = curr;
    curr = curr->next;
  }

  list_node_t *node = (list_node_t *)malloc(sizeof(list_node_t));
  node->key = item;
  node->next = curr;
  pred->next = node;
  pthread_mutex_unlock(&global_lock);

  return 1;
}

/*
 * Removes an item from the given list.
 */
int list_remove(list_node_t *head, int item)
{
  list_node_t *pred, *curr;
  pthread_mutex_lock(&global_lock);
  int return_value = 0;

  pred = head;
  curr = head->next;
  while (curr->key < item) {
    pred = curr;
    curr = curr->next;
  }
  if (item == curr->key) { /* found */
    pred->next = curr->next;
    free(curr);

    pthread_mutex_unlock(&global_lock);

    return_value = 1;
  }
  pthread_mutex_unlock(&global_lock);

  return return_value;
}

/*
 * Searchs for an item in the given list.
 */
int list_contain(list_node_t *head, int item)
{
  list_node_t *curr;
  int flag;
  pthread_mutex_lock(&global_lock);
  curr = head->next;
  while (curr->key < item) {
    curr = curr->next;
  }
  flag = (item == curr->key);
  pthread_mutex_unlock(&global_lock);
  return flag;

}


/*
 * Prints the content of the given list in the standard output.
 */
void list_print(list_node_t *head)
{
  return;
  if (head == NULL) return;

  list_node_t *curr = head;
  fprintf(stdout, "[%d, ", curr->key);
  curr = curr->next;
  while (curr->next != NULL)
  {
    fprintf(stdout, "%d, ", curr->key);
    curr = curr->next;
  }
  fprintf(stdout, "%d]\n", curr->key);
}



#ifdef TEST_LIST
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

list_node_t *list_head;

int main()
{
  list_init(&list_head);

  list_add(list_head, 0);
  list_add(list_head, -10);
  list_add(list_head, 10);

  assert(list_contain(list_head, 10));
  assert(list_contain(list_head, 0));
  assert(list_contain(list_head, -10));

  list_print(list_head);

  list_remove(list_head, 0);
  assert(!list_contain(list_head, 0));
  list_print(list_head);


  list_remove(list_head, 10);
  assert(!list_contain(list_head, 10));
  list_print(list_head);

  list_remove(list_head, -10);
  assert(!list_contain(list_head, -10));
  list_print(list_head);

  assert(list_contain(list_head, 0));

  return 0;
}
#endif
