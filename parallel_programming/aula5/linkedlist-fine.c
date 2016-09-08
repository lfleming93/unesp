#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "linkedlist.h"


typedef struct _list_node_t {
  int key;
  struct _list_node_t *next;
  pthread_mutex_t lock;

} list_node_t;


/*
 * Initializes the list. Needs to be called only once at the
 * beginning.
 */
void list_init(list_node_t **head)
{
  *head = (list_node_t *)malloc(sizeof(list_node_t));
  (*head)->key = INT_MIN;

  pthread_mutex_init(&(*head)->lock, NULL);

  (*head)->next = (list_node_t *)malloc(sizeof(list_node_t));
  (*head)->next->key = INT_MAX;

  pthread_mutex_init(&(*head)->next->lock, NULL);

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

  pthread_mutex_lock(&head->lock);

  pred = head;
  curr = head->next;

  pthread_mutex_lock(&curr->lock);

  while (curr->key < item) {
    pthread_mutex_unlock(&pred->lock);
    pred = curr;
    curr = curr->next;
    pthread_mutex_lock(&curr->lock);
  }

  list_node_t *node = (list_node_t *)malloc(sizeof(list_node_t));
  pthread_mutex_init(&node->lock, NULL);
  node->key = item;
  node->next = curr;
  pred->next = node;

  pthread_mutex_unlock(&curr->lock);
  pthread_mutex_unlock(&pred->lock);

  return 1;
}

/*
 * Removes an item from the given list.
 */
int list_remove(list_node_t *head, int item)
{
  list_node_t *pred, *curr;
  int retorno = 0;

  pthread_mutex_lock(&head->lock);

  pred = head;
  curr = head->next;
  pthread_mutex_lock(&curr->lock);

  while (curr->key < item) {
    pthread_mutex_unlock(&pred->lock);
    pred = curr;
    curr = curr->next;
    pthread_mutex_lock(&curr->lock);
  }
  if (item == curr->key) { /* found */
    pred->next = curr->next;
    pthread_mutex_unlock(&curr->lock);
    pthread_mutex_destroy(&curr->lock);
    free(curr);
    retorno =  1;
  }
  else
    pthread_mutex_unlock(&curr->lock);

  pthread_mutex_unlock(&pred->lock);
  return retorno;
}

/*
 * Searchs for an item in the given list.
 */
int list_contain(list_node_t *head, int item)
{
  list_node_t *curr, *pred;

  pthread_mutex_lock(&head->lock);
  int retorno;
  pred = head;
  curr = head->next;

  curr = head->next;
  while (curr->key < item) {
    pthread_mutex_unlock(&pred->lock);
    pred = curr;
    curr = curr->next;
    pthread_mutex_lock(&curr->lock);
  }

  retorno = (item == curr->key);

  pthread_mutex_unlock(&curr->lock);
  pthread_mutex_unlock(&pred->lock);

  return retorno;

}


/*
 * Prints the content of the given list in the standard output.
 */
void list_print(list_node_t *head)
{
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
