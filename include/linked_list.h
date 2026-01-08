#ifndef LINKED_LIST_H
  #define LINKED_LIST_H
  struct node {
    void* entry;
    struct node* next;
  };
  struct node* createNode(void* entry);

  struct node* insert_front(struct node* list, void* entry);
  struct node* free_list(struct node* list);
  struct node* remove_node_by_index(struct node* list, int index);
#endif
