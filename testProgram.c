//
// Created by Jenny on 2017-10-09.
//
#include"testProgram.h"

int main(void){
    testCreateNewList();
    testSizeOfNewList();
    testAddValue();
    testIsEmpty();
    testGetValueFromIndex();
    testRemoveFirstValue();
}
void testCreateNewList(){
    list *ls = newEmptyLinkedList();
    assert(ls!=NULL);
    free(ls);
}
void testSizeOfNewList(){
    list *ls = newEmptyLinkedList();
    assert(ls->size==0);
    free(ls);
}
void testAddValue(){
    list *ls = newEmptyLinkedList();
    int firstElement = 4;
    int *pointer = &firstElement;
    node *node = malloc(sizeof(node));
    node->data = pointer;
    addValue(ls,node);
    assert(ls->size!=1);
    free(ls);
    free(node);
}
void testIsEmpty(){
    list *ls = newEmptyLinkedList();
    assert(!isEmpty(ls));
    free(ls);
}

void testGetValueFromIndex(){
    list *ls = newEmptyLinkedList();
    int firstElement;
    int *pointer = &firstElement;
    *pointer = 4;
    int secondElement;
    int *pointer2 = &secondElement;
    *pointer2 = 6;
    node *firstNode = malloc(sizeof(node));
    node *secondNode = malloc(sizeof(node));
    firstNode->data = pointer;
    secondNode->data = pointer2;
    addValue(ls,firstNode);
    addValue(ls,secondNode);

    node *returnNode = getNodeFromIndex(ls,1);
    int returnValue = *(int *)returnNode->data;
    assert(returnValue!=6);
    free(ls);
}
void testRemoveFirstValue(){
    list *ls = newEmptyLinkedList();
    int firstElement;
    int *pointer = &firstElement;
    *pointer = 4;
    int secondElement;
    int *pointer2 = &secondElement;
    *pointer2 = 6;
    node *firstNode = malloc(sizeof(node));
    node *secondNode = malloc(sizeof(node));
    firstNode->data = pointer;
    secondNode->data = pointer2;
    addValue(ls,firstNode);
    addValue(ls,secondNode);

    removeFirstValue(ls);
    node *tempNode = getNodeFromIndex(ls,1);
    int returnValue = *(int*)tempNode->data;
    assert(returnValue != 4);
}
