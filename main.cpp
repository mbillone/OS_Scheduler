//Matt Billone and Vineeth Gutta


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <cstring>
#include <cmath>
#include "Queues.h"
//#include "Queues.cpp"

using namespace std;

#define MAX_INPUTS 1000 //aribtrary large number for amount of inputs can go into the system
#define END_TIME 9999 //will act as the longest time of the system/time that the system will be finished
#define INPUT_FILE "Users/Vineeth/Desktop/OS_project/OS_Scheduler/input.txt" //input file will be a text file of inputs that will be interperated and executed by the system. have to still make that file, will do later

//list of all the statuses for the nodes. statuses will include which queue a job is sitting in, or whether it is completed or rejected
#define SUBMIT_Q "Submit Queue"
#define HOLD_Q1 "Hold Queue 1"
#define HOLD_Q2 "Hold Queue 2"
#define READY_Q "Ready Queue"
#define WAIT_Q "Wait Queue"
#define CPU "Running on CPU"
#define COMPLETED "Completed"
#define REJECTED "Rejected"

bool running = true; //will turn false to terminate the system after D 9999 input is passed

//time and input variables/constants
int currentTime = 0; //current time of the system
int inputTime = 0; //time the input is put in
int inputNum = 0; //number of the input/job that the system is on/has been put into the system
bool allInputsEntered = false; //will turn true as soon as all the inputs have been entered into the system
bool allInputsCompleted = false; //will turn true as soon as all inputs have completed
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

int getInputTime(string input); //gets the current input's time that it should start at
void readInputCommand(string input, Node *submit, Node *wait, Node *hold1, Node *hold2, Node *ready, Node *cpu, Node *completed, Node *syst);
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
void traverseAndPrint (Node *jobs);//prints the queue
void cpuHandling(Node *syst, Node *cpu, Node *ready, Node *hold1, Node *hold2, Node *wait, Node *completed);//handles moving jobs from cpu to completed queue if completed, or waiting queue if there are not enough devices
void updateStatus(Node *syst, Node *update, string status);//updates the job's status when a job moves from one state to another
int charToInt(char *str);


void readInputCommand(string input, Node *submit, Node *wait, Node *hold1, Node *hold2, Node *ready, Node *cpu, Node *completed, Node *syst){
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
        displayStatus(input, syst, submit, wait, hold1, hold2, ready, completed);
    }
    else{
        cout << "This is an unrecognized input" << endl;
    }
    
}

int charToInt(char *str){ //method used to get the integer values out of the character array made from the input
    int a = 0;
    int b = strlen(str) - 1;
    int c = 0;
    
    while (str[b] != '='){ //makes sure that the correct character in the input string is being converted. EX(M=43) -> 43 will be converted to an int
        
        a += pow(10,c) * (str[b] - '0');
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
    
    while (str != NULL){ //going to do the same thing as configSystem with truncate and read beginning of char array
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

void requestDevices(char *str, Node *cpu, Node *ready, Node *wait, Node *syst){
    quantumSlice = 0;
    int job;
    int devicesRequested;
    
    while (str != NULL){//grabs job number and devices the job is requesting for the input
        if (str[0] == 'J'){
            job = charToInt(str);
        }
        else if (str[0] == 'D'){
            devicesRequested = charToInt(str);
        }
        
        str = strtok(NULL, " ");
        
    }
    
    cpu->next->requestedDevices = devicesRequested;
    
    if (job != cpu->next->jobNum){
        cout << "Job number" << job << "could not request" << devicesRequested << "devices because the job is not running on the CPU" << endl;
        
        return;
        
    }
    
    if (cpu->next->requestedDevices > cpu->next->maxDevices){
        cout << "Job number" << job << "could not request" << devicesRequested << "devices because the requested devices is more than the maximum devices alotted for that job" << endl;
        
        return;
    }
    
    if (cpu->next->requestedDevices > availableDevices){
        cout << "Job number" << job << "could not request" << devicesRequested << "devices because the devices needed are not available" << endl;
        
        cpu->next->devicesGranted = false;
        
        Node *cpuToWait = remove(cpu, cpu->next->jobNum);//remove the denied job from the cpu queue
        addToEnd(wait, cpuToWait);//puts the denied job into the wait queue
        
        Node *readyToCpu = remove(ready, ready->next->jobNum);//remove the next available job from the ready queue
        addToEnd(cpu, readyToCpu);//puts the available job into the cpu to request devices
    }
    
    if (devicesRequested <= availableDevices && cpu->next->jobNum == job){
        cout << "Job number" << job << "request for" << devicesRequested << "granted" << endl;
        
        cpu->next->devicesGranted = true;
        
        availableDevices = availableDevices - devicesRequested;//update available devices
        
        Node *cpuToReady = remove(cpu, cpu->next->jobNum);//remove device from cpu after devices have been allocated
        addToEnd(ready, cpuToReady);//put job into ready queue to be cycled
        }
    
    else{
        cout << "Error: the device was neither granted nor denied it's request" << endl;
    }
}

void releaseDevices(char *str, Node *cpu, Node *ready, Node *wait, Node *syst){
    quantumSlice = 0;
    int job;
    int devicesReleased;
    
    while (str != NULL){//get job number and amount of devices to release from input string
        if (str[0] == 'J'){
            job = charToInt(str);
        }
        else if (str[0] == 'D'){
            devicesReleased = charToInt(str);
        }
        
        str = strtok(NULL, " ");
    }
    
    if (job != cpu->next->jobNum){
        cout << "Job number" << job << "could not release" << devicesReleased << "devices because the job is not running on CPU" << endl;
        
        return;
    }
    
    if (cpu->next->devicesGranted == true){
        if (devicesReleased > cpu->next->requestedDevices){
            cout << "Job number" << job << "could not release" << devicesReleased << "devices because the amount to release is more than the amount requested from the job" << endl;
            
            return;
        }
        
        cout << "Job number" << job << "released" << devicesReleased << "devices successfully" << endl;
        
        availableDevices = availableDevices + devicesReleased;//update available devices
        
        waitQHandling(syst, wait, ready);//check to see if there are jobs waiting in the wait queue
    }
    
    else{
        cout << "Job number" << job << "could not release devices because the devices were not granted" << endl;
    }
}

void readyQHandling(Node *syst, Node *ready, Node *cpu){
    if (ready->next != NULL){
        if(cpu->next == NULL){//put job onto cpu if the cpu is free
            Node *readyToCpu = remove(ready, ready->next->jobNum);
            addToFront(cpu, readyToCpu);
            quantumSlice = 0;
            updateStatus(syst, readyToCpu, CPU);
        }
        else{
            cout << "CPU is occupied" << endl;
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

void submitQHandling(Node *syst, Node *hold1, Node *hold2, Node *submit){
    if (submit->next != NULL){
        Node *temp = submit;
        while (temp != NULL){
            if (temp->head == false){
                if (temp->maxMemory > memory || temp->maxDevices > devices){//rejects the job if the system can not handle it (either not enough memory or devices to complete it)
                    remove(submit, temp->jobNum);
                    updateStatus(syst, temp, REJECTED);//change the job status to rejected
                }    
            }
            
            Node *submitTohandling = remove(submit, temp->jobNum);
            
            if (submitTohandling != NULL){
                if (submitTohandling->priority == 1){
                    addToEnd(hold1, submitTohandling);
                    updateStatus(syst, temp, HOLD_Q1);
                }
                else if (submitTohandling->priority == 2){
                    addToFront(hold2, submitTohandling);
                    updateStatus(syst, submitTohandling, HOLD_Q2);
                }
            }
            temp = temp->next;
        }
    }
}

void cpuHandling(Node *syst, Node *cpu, Node *ready, Node *hold1, Node *hold2, Node *wait, Node *completed){
    if (cpu->next != NULL){
        
        cpu->next->timeLeft--;
        quantumSlice++;
        
        Node *temp = syst;
        while (temp->jobNum != cpu->next->jobNum){
            temp = temp->next;
        }
        
    
        temp->timeLeft--;
    
        if (cpu->next->timeLeft == 0){
            quantumSlice = 0;
            cpu->next->runTime = currentTime + 1;
            cpu->next->turnAroundTime = currentTime - cpu->next->arriveTime;
            cpu->next->weightedTurnAround = cpu->next->turnAroundTime / cpu->next->runTime;
        
            availableMemory = availableMemory + cpu->next->maxMemory;
        
            if (cpu->next->devicesGranted){
                availableDevices = availableDevices + cpu->next->requestedDevices;
            }
        
            Node *temp = syst;
            while (temp->jobNum != cpu->next->jobNum){
                temp = temp->next;
            }
        
            temp->runTime = currentTime + 1;
            temp->turnAroundTime = cpu->next->turnAroundTime;
            temp->weightedTurnAround = cpu->next->weightedTurnAround;
            Node *cpuToComplete = remove(cpu, cpu->next->jobNum);
            addToEnd(completed, cpuToComplete);
            updateStatus(syst, cpuToComplete, COMPLETED);
            waitQHandling(syst, wait, ready);
            hold1QHanlding(syst, hold1, ready);
            hold2QHandling(syst, hold2, ready);
            readyQHandling(syst, ready, cpu);
            
        
        
        }
    }
    
}

void hold1QHanlding(Node *syst, Node *hold1, Node *ready){
    if(hold1->next != NULL){
        Node *temp = hold1;
        int shortestJob = 0;
        int shortestJobTime = END_TIME;

        while(temp != NULL){
            if(temp->head == false){
                if(temp->maxMemory <= availableMemory && temp->maxDevices <= availableDevices){
                    if(temp->runTime < shortestJobTime){
                        shortestJobTime = temp->runTime;
                        shortestJob = temp->jobNum;
                    }
                }
            }
            temp = temp->next;
        }
        if(shortestJob > 0){
            Node *transfer = remove(hold1, shortestJob);
            addToEnd(ready, transfer);
            availableMemory -= transfer->maxMemory;

            updateStatus(syst, transfer, READY_Q);
        }
    }
}

void hold2QHandling(Node *syst, Node *hold2, Node *ready){
    if(hold2->next != NULL){
        Node *temp = hold2;

        while(temp->next != NULL){
            temp = temp->next;
        }

        if(temp->maxMemory <= availableMemory && temp->maxDevices <= availableDevices){
            Node *transfer = remove(hold2, temp->jobNum);
            addToEnd(ready, transfer);
            availableMemory -= transfer->maxMemory;

            updateStatus(syst, transfer, READY_Q);
        }
    }
}


void waitQHandling(Node *syst, Node *wait, Node *ready){
    if(wait->next != NULL){
        Node *temp = wait;
        while(temp != NULL){
            if(temp->head == false){
                if(temp->maxDevices <= availableDevices){
                    Node *waitToReady = remove(wait, temp->jobNum);
                    addToEnd(ready, waitToReady);

                    updateStatus(syst, waitToReady, READY_Q);
                }
            }
            temp = temp->next;
        }
    }
}

int getInputTime(string input) {
	char parsed[input.length()];
	strcpy(parsed, input.c_str()); 
	char * temp;
	temp = strtok(parsed, " "); 
	int i = 0;
	int t = 0;
	while (temp != NULL) {
		if (i == 1) { 
			for (int j = 0; j < strlen(temp); j++) { 
				t += pow(10, strlen(temp) - j - 1) * (temp[j] - '0');
			}
		}
		temp = strtok(NULL, " ");
		i++;
	}
	return t;
}

void updateStatus(Node *syst, Node *update, string status){//needed to update the statuses of all the jobs when moving queues
    Node *temp = syst;
    while (temp != NULL){
        if(temp->jobNum == update->jobNum){
            temp->status = status;
        }
        temp->next = temp;
    }
}

void displayStatus(string input, Node *syst, Node *submit, Node *wait, Node *hold1, Node *hold2, Node *ready, Node *cpu, Node *completed){
    if (input == "D 9999 " || input == "D 9999") {//makes sure it gets detected if it is in the middle of the input file and not at the end
		allInputRead = true;
		return;
	}
	cout << "System at " << currentTime << ": " << endl;
	printSystem(sys);
	cout <<  "Submit Queue contains: " << endl;
	traverseAndPrint(submit);
	cout <<  "Hold Queue 1 contains: " << endl;
	traverseAndPrint(hold1);
	cout <<  "Hold Queue 2 contains: " << endl;
	traverseAndPrint(hold2);
	cout <<  "Ready Queue contains: " << endl;
	traverseAndPrint(ready);
	cout <<  "Using the CPU: " << endl;
	if (cpu->next != NULL) {
		cout << "Job Number: " << cpu->next->jobNumber << endl;
	} else {
		cout << "No job on CPU" << endl;
	}
	cout << "Wait Queue contains: " << endl;
	traverseAndPrint(wait);
	cout << "Completed Jobs: " << endl;
	traverseAndPrint(complete);
}


int main(){
    string input;
    string inputQ[MAX_INPUTS];
    ifstream inputFile (INPUT_FILE);
    
    //to open input text file
    if (inputFile.is_open()){
        int line = 0;
        while (getline(inputFile, input)){
            if (input[input.length() - 1] != ' '){
                cout << "Line" << line + 1 << "does not end with a space, so the input can not be accepted" << endl;
                line--;
            }
            else{
                inputQ[line] = input;
            }
            line++;
        }
        
        numOfInputs = line;
        inputFile.close();
    }
    
    else{
        cout << "Cannot open file" << endl;
        return 0;
    }
    //system queue. holds the job history
    struct Node *syst = new Node;
    syst->head = true;
    syst->next = NULL;
    
    //submit queue
    struct Node *submit = new Node;
    submit->head = true;
    submit->next = NULL;
    
    //hold queue 1
    struct Node *hold1 = new Node;
    hold1->head = true;
    hold1->next = NULL;
    
    //hold queue 2
    struct Node *hold2 = new Node;
    hold2->head = true;
    hold2->next = NULL;
    
    //ready queue
    struct Node *ready = new Node;
    ready->head = true;
    ready->next = NULL;
    
    //cpu queue
    struct Node *cpu = new Node;
    cpu->head = true;
    cpu->next = NULL;
    
    //wait queue
    struct Node *wait = new Node;
    wait->head = true;
    wait->next = NULL;
    
    //complete queue
    struct Node *complete = new Node;
    complete->head = true;
    complete->next = NULL;
    
    while (running){
        
        string currentLine = inputQ[inputNum];
        
        if (!allInputsCompleted){
            inputTime = getInputTime(currentLine);
        }
        
        if (!allInputsCompleted && currentTime >= inputTime && !multipleInputs){//checks if multiple inputs are getting put in
            readInputCommand(currentLine, submit, wait, hold1, hold2, ready, cpu, complete, syst);
            inputNum++;
            
            if (!allInputsCompleted){//only happens if multiple inputs get put in
                int nextInputTime = getInputTime(inputQ[inputNum]);
                if(inputTime == nextInputTime){
                    multipleInputs = true;
                }
            }
        }
        if (multipleInputs == true){//handles multiple inputs
            readInputCommand(inputQ[inputNum], submit, wait, hold1, hold2, ready, cpu, complete, syst);
            inputNum++;
            
            if(inputNum >= numOfInputs - 1){
                allInputsCompleted = true;
            }
            
            if(!allInputsCompleted){
                int nextInputTime = getInputTime(inputQ[inputNum]);
                if (inputNum == nextInputTime){
                    multipleInputs = true;
                }
                else{
                    multipleInputs = false;
                }
            }
            else{
                multipleInputs = false;
            }
        }
        //handle all the queues
        submitQHandling(syst, hold1, hold2, submit);
        waitQHandling(syst, wait, ready);
        hold1QHanlding(syst, hold1, ready);
        hold2QHandling(syst, hold2, ready);
        readyQHandling(syst, ready, cpu);
        
        if (!multipleInputs){
            cpuHandling(syst, cpu, ready, hold1, hold2, wait, complete);
        }
        
        if (quantumSlice == quantum){//for time slice switching
            quantumSlice = 0;
            
            if (ready->next != NULL){
                struct Node *cpuToReady = remove(cpu, cpu->next->jobNum);
                addToEnd(ready, cpuToReady);
                struct Node *readyToCpu = remove(cpu, cpu->next->jobNum);
                addToEnd(cpu, readyToCpu);
                updateStatus(syst, cpuToReady, READY_Q);
                updateStatus(syst, readyToCpu, CPU);
            }
        }
        
        if (!multipleInputs){//increments the current time
            currentTime++;
        }
        
        if (cpu->next == NULL && allInputsCompleted){//terminates when all inputs 
            running = false;
        }
    }
    
    cout << "Final System Status: " << endl;
    printStatus(syst);
    cout << "Submit Queue contains: " << endl;
    traverseAndPrint(submit);
    cout << "Hold Queue 1 contains: " << endl;
    traverseAndPrint(hold1);
    cout << "Hold Queue 2 contains: " << endl;
    traverseAndPrint(hold2);
    cout << "Ready Queue contains: " << endl;
    traverseAndPrint(ready);
    cout << "Wait Queue contains: " << endl;
    traverseAndPrint(wait);
    cout <<"Currently using CPU: " << endl;
    if (cpu->next != NULL){
        traverseAndPrint(cpu);
    }
    else{
        cout << "No job using CPU" << endl;
    }
    cout << "Jobs Completed are: " << endl;
    traverseAndPrint(complete);
    
    //print turnaround time and weightedTurnAround for the whole system
    
    int i = 0;
    int k = 0;
    
    int j = 0;
    Node *temp = syst;
    while (temp->next != NULL){
        i = i + temp->turnAroundTime;
        k = k + temp->weightedTurnAround;
        j++; 
    }
    
    i = i + temp->turnAroundTime;
    j = j + temp->weightedTurnAround;
    i = i / j;
    k = k / j;
    
    cout << "Average turn around time for the system: " << endl;
    printf("%.2d", i);
    cout << "Average weigheted turn around for the system: " << endl;
    printf("%.2d", k);
    
    return 1;
    
}
