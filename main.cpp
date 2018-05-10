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


//method instantiation, syst is a linked list that tracks all the inputs that go into the system in the order that they went into the system

int getCurrentInputTime(string input); //gets the current input's time that it should start at
void readInputCommand(string input, Node *submit, Node *wait, Node *hold1, Node *hold2, Node *ready, Node *completed, Node *syst);
void displayStatus(string input, Node *syst, Node *submit, Node *wait, Node *hold1, Node *hold2, Node *ready, Node *completed);
void configSystem(char *str); //called when the system configuration input command is read
void createJob(char *str, Node *syst, Node *submit);
void requestDevices(char *str, Node *cpu, Node *ready, Node *wait, Node *syst);//called when the job wants to request devices while on the cpu
void releaseDevices(char *str, Node *cpu, Node *ready, Node *wait, Node *syst);//called when the job wants to releast devices while on the cpu
void submitQHandling(Node *syst, Node *hold1, Node *hold2, Node *submit); //handles moving jobs from the submit queue to its respective holding queue
void hold1QHanlding(Node *syst, Node *hold1, Node *ready);//handles moving jobs from holding queue 1 to ready queue
void hold2QHandling(Node *syst, Node *hold2, Node *ready);//handles moving jobs from holding queue 2 to the ready queue
void waitQHandling(Node *syst, Node *wait, Node *ready);//handles moving jobs from the wait queue to the ready queue
void readyQHandling(Node *syst, Node *ready, Node *cpu);//handles moving jobs from the ready queue to the CPU
void cpuHandling(Node *syst, Node *cpu, Node *completed);//handles moving jobs from cpu to completed queues
void updateSystem(Node *syst, Node *update, string status);//updates the system when a job moves from one state to another



void readInputCommand(string input, Node *submit, Node *wait, Node *hold1, Node *hold2, Node *ready, Node *completed, Node *syst){
    char in[input.length()];//creates character array
    strcpy(in, input.c_str());//converts the string into an array of characters
    char *str;
    str = strtok(in, " ");//spaces to seperate each part of the input
    
    if (input[0] == 'C'){
        configSystem(str);
    }
    else if(input[0] == 'A'){
        createJob(str, syst, submit);
    }
    else if(input[0] == 'Q'){
        requestDevices(str, cpu, ready, wait, syst);
    }
    else if(input[0] == 'L'){
        releaseDevices(str, cpu, ready, wait, syst);
    }
    else if(input[0] == 'D'){
        displayStatus(input, syst, submit, wait, hold1, hold2, ready, completed)
    }
    else{
        cout << "This is an unrecognized input" << endl;
    }
    
}

void displayStatus(string input, Node *syst, Node *submit, Node *wait, Node *hold1, Node *hold2, Node *ready, Node *completed){
    if (input == "D 9999" || input == "D 9999 "){ //need to take into account whether the terminate command is at the end of an input file or at the end
        inputsCompleted = true;
        return; //signifies the end of the simulation
    }
    
    cout << "System status at" << currentTime << "is : " << endl;
    
    printStatus(syst);
    
    cout << "Submit Queue contains: " << endl;
    
    printQ(submit);
    
    cout << "Hold Queue 1 contains: " << endl;
    
    printQ(hold1);
    
    cout << "Hold Queue 2 contains: " << endl;
    
    printQ(hold2);
    
    cout << "Ready Queue contains: " << endl;
    
    printQ(ready);
    
    cout << "Wait Queue contains: " << endl;
    
    printQ(wait);
    
    cout << "CPU contains: " << endl; 
    
    printQ(cpu);
    
    cout << "Completed jobs : " << end;
    
    printQ(completed);
    
}

void configSystem(char *str){
    
}















int main()
{
    
}
