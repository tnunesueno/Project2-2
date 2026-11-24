//=================================================================
// Implementation for DLL module.
//
// Copyright 2025 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================
#include <stdlib.h>
#include <stdio.h>
#include "doubly_linked_list.h"

LLNode* create_llnode(void* data) {
    // Your code here
    LLNode* this_node = (LLNode*) malloc(sizeof(LLNode)); 
    this_node-> data = data; 
    this_node-> next = NULL;
    this_node-> prev = NULL; 
    return this_node; // replace this
}

DLinkedList* create_dlinkedlist(void) {
    DLinkedList* newList = (DLinkedList*)malloc(sizeof(DLinkedList));
    newList->head = NULL;
    newList->tail = NULL;
    newList->size = 0;
    return newList;
}

int getSize(DLinkedList* dLinkedList){

  // Your code here
  return dLinkedList->size; 
}

LLNode* getHead(DLinkedList* dLinkedList){

  // Your code here
  if (!dLinkedList->head){
    printf("Could not fetch head because the linked list is empty."); 
    return NULL;
  }
  return dLinkedList->head; 
}

LLNode* getTail(DLinkedList* dLinkedList){

  // Your code here
  if (!dLinkedList->tail){
    printf("Could not fetch tail because linked list is empty."); 
    return NULL; 
  }
  return dLinkedList->tail; 
}

LLNode* getNext(LLNode* node){

  // Your code here
  if (!node->next){
    printf("Could not fetch next node because next node is null");
    return NULL; 
  }
  return node->next;
}

LLNode* getPrev(LLNode* node){

  // Your code here
  if(!node->prev){
    printf("Could not return previous node because it is null. ");
    return NULL;
  }
   return node->prev;
}

void* getData(LLNode* node){

  // Your code here
  if (!node->data){
    printf("Could not return data because data is null. ");
    return NULL;
  }

  return node->data; 
}

void insertAfter(DLinkedList* dLinkedList, LLNode* prev_node, void* data){
  if (prev_node == NULL) {
    printf("the given previous node cannot be NULL");
    return;
  }

  // Your code here
  // no empty list condition
  LLNode* this_node = create_llnode(data);

  if(prev_node->next){
    // prev node is in the middle
    this_node->next = prev_node->next; 
    this_node->next->prev = this_node; 
  } else {
    // this node is the tail
    this_node->next = NULL; 
    dLinkedList->tail = this_node;
  }

  prev_node->next = this_node; 
  this_node->prev = prev_node; 
  dLinkedList->size ++; 

  return;
}

void insertBefore(DLinkedList* dLinkedList, LLNode* next_node, void* data){
  if (next_node == NULL) {
    printf("the given next node cannot be NULL");
    return;
  }
  // Your code here
  // no empty list condition at this point 
  LLNode* this_node = create_llnode(data);
  if (next_node->prev){
    // next_node is in the middle 
    this_node->prev = next_node->prev;
    this_node->prev->next = this_node; 
  } else {
    // next node is the head 
    this_node->prev = NULL; 
    dLinkedList->head = this_node;
  }

  next_node->prev = this_node; 
  this_node->next = next_node; 
  dLinkedList->size ++; 

  return;
}

void insertHead(DLinkedList* dLinkedList, void* data){
  if(dLinkedList->head == NULL){
    LLNode* newNode = create_llnode(data);
    dLinkedList->head = newNode;
    dLinkedList->tail = newNode;
    dLinkedList->size++; 
  }else{
    insertBefore(dLinkedList, dLinkedList->head, data);
    //return; // can you call a different method here instead? Do you need a return statement here?
  }
}

void insertTail(DLinkedList* dLinkedList, void* data){

  // Your code here
  if(dLinkedList->tail == NULL){
    LLNode* newNode = create_llnode(data);
    dLinkedList->head = newNode;
    dLinkedList->tail = newNode;
    dLinkedList->size++; 
  } else {
    insertAfter(dLinkedList, dLinkedList->tail, data);
  }
}

void deleteNode(DLinkedList* dLinkedList, LLNode* Node){

  // Your code here
  if(!Node){
    printf("Could not delete the node because it does not exist.");
    return;
  }
  if(!dLinkedList){
    printf("Could not delete node because there is no list.");
    return;
  }


  if (Node->prev){
    // there is a node before this, it's next needs to be fized 
    Node->prev->next = Node->next; 
    if(!Node->next){
      // this node is the tail 
      dLinkedList->tail = Node->prev; 
    }
  } else {
    dLinkedList->head = Node->next; 
  }

  if (Node->next){
    // there is a node after this, it's prev needs to be fixed
    Node->next->prev = Node->prev; 
    if(!Node->prev){
      // this node is the head bc it has a next but no prev 
      dLinkedList->head = Node->next; 
    }
  } else {
    dLinkedList->tail = Node->prev;
  }

  // the case where the data is a pointers 
  if(Node->data != NULL){
    free(Node->data);
  }
  free(Node); 
  dLinkedList->size --;
}

void destroyList(DLinkedList* dLinkedList){

  // Your code here
  // make sure you go through each node or else you wil get meemory leaks.

  if(!dLinkedList){
    printf("Don't need to destroy the list, it doesn't exist");
    return; 
  }
  //LLNode* this_node = dLinkedList->head; 
  // start at the head and destroy everything;
  while(dLinkedList->size>0){
    deleteNode(dLinkedList, dLinkedList->tail);  
  }

  printf("Iterated through and destroyed all nodes");
  free(dLinkedList); 
} 

DLinkedList* split(DLinkedList* dLinkedList, LLNode* Node){

  // Your code here

  if (!dLinkedList){
    printf("No list to split"); 
    return NULL;
  }

  if (!Node){
    printf("No node to split on");
    return NULL;
  }

  if (Node == dLinkedList->tail){
    printf("Node is the tail"); 
    return NULL; 
  }

  DLinkedList* newDLL = create_dlinkedlist();

  // start new list after  node 
  newDLL->head = Node->next;
  newDLL->tail = dLinkedList->tail;
  newDLL->head->prev = NULL;

  // fix old list 
  Node->next = NULL;
  dLinkedList->tail = Node;

  // add all data after node->next 
  LLNode* current = getHead(newDLL); 

  while(current){
    //insertAfter(newDLL, current, current->next->data); 
    newDLL->size ++; 
    current = getNext(current); 
  }
  dLinkedList->size = dLinkedList->size - newDLL->size; 
  return newDLL; 
  
}
