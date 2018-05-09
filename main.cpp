//Matt Billone and Vineeth Gutta


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cmath>
#include "Node.h"

using namespace std;

#define MAX_INPUTS 50 //arbitrary number used to 
#define END_TIME 9999 //will act as the longest time of the system/time that the system will be finished
#define INPUT_FILE "" //input file will be a text file of inputs that will be interperated and executed by the system. have to still make that file, will do later

//list of all the statuses for the nodes. statuses will include which queue a job is sitting in, or whether it is completed or rejected
#define SUBMIT_Q "Submit Queue"
#define HOLD_Q1 "Hold Queue 1"
#define HOLD_Q2 "Hold Queue 2"
#define READY_Q "Ready Queue"
#define WAIT_Q "Wait Queue"
#define CPU "Running on CPU"
#define COMPLETED "Completed"
#define REJECTED "Rejected"

//time and input variables/constants
int currentTime = 0; //current time of the system
int inputNum = 0; //number of the input/job that the system is on/has been put into the system
bool allInputsEntered = false; //will turn true as soon as all the inputs have been entered into the system
bool inputsCompleted = false; //will turn true as soon as all inputs have completed
bool multipleInputs = false; //will turn true if multiple inputs enter the system at the same time. used as a failsafe
int numOfInputs = 0; //total amount of inputs that are presented to the system and are in the Submit Queue

//memory/resources variables and constants
int memory = 0; //total amount of memory the system has, will be determined by the system input call
int availableMemory = 0; //amount of memory that the system has available
int devices = 0; //amount of serial devices that the system has
int availableDevices = 0; //amount of devices available for jobs to use
int quantum = 0; //I know that I need these but do not know what they do right now, will change comment when I figure that out
int quantumSlice = 0;


//function instantiation

int getCurrentInputTime(string input) //gets the current input's time that it should start at
void readInputCommand(string input, Node *)
















int main()
{
    
}
