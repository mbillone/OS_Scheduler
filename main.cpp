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
int charToInt(char *str);


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

charToInt(char *str){ //method used to get the integer values out of the character array made from the input
    int a = 0;
    int b = strlen(str) - 1;
    int c = 0;
    
    while (str[b] != '='){ //makes sure that the correct character in the input string is being converted. EX(M=43) -> 43 will be converted to an int
        
        a += pow(10,c) * (str[i] - '0');
        c++;
        b--;
    }
    return a;
}

void configSystem(char *str){
    while (str != NULL){
        if (str[0] == 'M'){
            memory = charToInt(str);
            availableMemory = memory;
        }
        else if (str[0] == 'S'){
            devices = charToInt(str);
            availableDevices = devices;
        }
        else if (str[0] == 'Q'){
            quantum = charToInt(str);
        }
        else{
            cout << "Input is not recognized" << endl;
        }
        str = strtok(NULL, " "); //truncates the character array to the next part of the system config input. EX: if M=43 S=12 is the character array then after M=43 is extracted it gets truncated off and S becomes str[0]
    }
}

void createJob(char *str, Node *syst, Node *submit){
    Node *job = new Node;
    Node *copy = new Node; //copy created to be put into syst Q for when the system history is called
    job->head = false;
    copy->head = false;
    job->status = SUBMIT_Q;//signifies entering the submit Q
    copy->status = SUBMIT_Q;
    job->arriveTime = currentTime;
    copy->arriveTime = currentTime;
    job->next = NULL;
    copy->next = NULL;
    
    while str != NULL{ //going to do the same thing as configSystem with truncate and read beginning of char array
        if (str[0] == 'J'){
            job->jobNum = charToInt(str);
            copy->jobNum = charToInt(str);
        }
        
        else if (str[0] == 'M'){
            job->maxMemory = charToInt(str);
            copy->maxMemory = charToInt(str);
        }
        else if (str[0] == 'S'){
            job->maxDevices = charToInt(str);
            copy->maxDevices = charToInt(str);
        }
        else if (str[0] == 'R'){
            job->runTime = charToInt(str);
            copy->runTime = charToInt(str);
            job->timeLeft = job->runTime;//have to make sure that run time and time left on job are the same to start
            copy->timeLeft = copy->timeLeft;
        }
        else if (str[0] == 'P'){
            job->priority = charToInt(str);
            copy->priority = charToInt(str);
        }
        else{
            cout << "Input is not recognized" << endl;
        }
        str = strtok(NULL, " ");
    }
    addToEnd(syst, copy);
    addToEnd(submit, job);
}
















int main()
{
    
}
