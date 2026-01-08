#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

struct node* createNode(void* entry) {
  struct node* newNode = (struct node*) malloc(sizeof(struct node));
  newNode->entry = entry;
  return newNode
}

struct node* insert_front(struct node* list, void* entry) {
  struct node* newFront = createNode(entry);
  newFront->next = list;

  return newFront;
}

struct node* free_list(struct node* list) {
  struct node* nextNode = NULL;
  while (list) {
    nextNode = list->next;
    free(list);
    list = nextNode;
  }

  return NULL;
}

struct node* remove_node_by_index(struct node* list, int index) {
  struct node* front = list;
  if (!index) {
    front = list->next;
    free(list);
    list = NULL;
    return front;
  }

  while (list && index > 1) {
    list = list->next;
    index--;
  }

  if (index == 1) {
    struct node* removedNode = list->next;
    if (!removedNode) return front;
    list->next = removedNode->next;
    free(removedNode);
    removedNode = NULL;
  }

  return front;
}
