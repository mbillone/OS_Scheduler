//Matt Billone and Vineeth Gutta

#include <iostream>
#include <stdlib.h>
#include "Queues.h"

using namespace std;

//Prints the information for each job within the queue
void
printStatus (Node * jobs)
{
  Node *temp = jobs;
  if (temp->next == NULL)
    {
      cout << "No jobs in the system" << endl;
    }
  else
    {
      while (temp != NULL)
	{
	  if (temp->head == false)
	    {
	      cout << "Job Number: " << temp->jobNum << "Status: ";
	      if (temp->status == "Complete")
		{
		  cout <<  temp->status << " Time: " << temp->
		    completionTime << " Turn Around Time: " << temp->
		    turnAroundTime;
		}
	      else
		{
		  cout << temp->status << " Time to Completion: " << temp->
		    timeLeft;
		}
	      cout << endl;
	    }
	  temp = temp->next;
	}
    }
}


void traverseAndPrint (Node *jobs)
{
  Node *temp = jobs;
  if (temp->next == NULL);{
      cout << "Current Queue is Empty" << endl;
  }
  else{
      int i = 0;
      while(temp->next != NULL){
        if (temp->head == false){
            cout << "Job number: " << temp->jobNum << " Position in Current Queue: " << i <<endl;
            i++;
        }
        temp = temp->next;
      }
  }

}


//adds a node/job to the front of the queue
void addToFront (Node *head , Node *added){
    Node temp = new Node;
    temp->next = head;
    head->next = added;
    added->next = temp->next;
}

//add node/job to the end of the queue
void addToEnd (Node *head , Node *added){
    if (head->next == NULL){
        head->next = added;
        added->next = NULL;
    }
    else{
        Node *temp = head;
        while (temp->next != NULL){
            temp = temp->next;
        }
        temp->next = added;
        added->next = NULL;
    }
}

//remove job/node from the queue, return it to be used
void *remove(Node *head, int toRemove){
    Node *temp = head;
    Node *tempPrev; //previous node
    
    while(temp != NULL){
        if (temp->jobNum == toRemove){
            tempPrev->next = temp->next;
            temp->next = NULL;
            return temp;
        }
        tempPrev = temp;
        temp = temp->next;
    }
    return NULL;
}