//Matt Billone and Vineeth Gutta
#ifndef NODEH
#define NODEH



#include <iostream>
#include <stdlib.h>
#include <string>
#include <cctype>

using namespace std;

/**
 * need to make queues into linked lists that hold jobs
 * this creates the nodes and the functions that can be performed on the linked lists
 **/
struct Node{
    bool head;
    struct Node *next;
    string status;
    int arriveTime;
    int completionTime;
    int turnAroundTime;
    int priority;
    int jobNum;
    int runTime; //how long the job has been running for
    int timeLeft; //how long the job has left
    int maxDevices; //how many devices the job needs to complete
    int requestedDevices; //how many devices the job asks for to begin witch
    bool jobGranted;
    
};

void printStatus(Node *jobs);
void traverseAndPrint(Node *jobs);
void addToFront(Node *head, Node *added);
void addToEnd(Node *head, Node *added);
Node *remove(Node *head, int toRemove);

#endif