/*
Jonathan Pedoeem
DSA I - Spring 2018
Professor Sable
Project 1 - c++ implementation of a Stack and Queue using an Abstract Base class SimpleList
*/
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <list>


using namespace std;

//This is the Abstract Base class simplelist.
//It creates the scaffolding for the Stack and Queue class
template <class l>
class  SimpleList {
  private:
    class Node;
    Node *startNode = NULL;
    Node *endNode   =  NULL;
    string name;
  protected:
    void insertAtStart(l &newStart);
    void insertAtEnd(l &newEnd);
    l  deleteFromStart(void);
  public:
    string getName() const {return name;};
    void setName(string &n) {name = n;};
    bool  isempty();
    virtual void push(l &newItem) = 0;
    virtual l pop() = 0;
};

//Here we define the member class of SimpleList called Node
// The node contains the data and a pointer to the next node in the simplelist
template <class l>
class SimpleList<l>::Node {
  public:
    Node *next;
    l     data;
    Node(l &d, Node *n){
      data = d;
      next = n;
    };
    Node(){};
};

//member function to insert at start of SimpleList
template <class l>
void SimpleList<l>::insertAtStart(l &newStart) {
  //save the current start address, make a new node that points to the current start address and then change
  //the start address to point to this one.
  Node *pNode      = new Node;
  *pNode           = Node(newStart, startNode);
  startNode        = pNode;
}

//member function to insert at end of SimpleList
template <class l>
void SimpleList<l>::insertAtEnd(l &newEnd){
  //create a new node, go to current end add the address of this node to the pointer of that one and change
  //the end node pointers
  Node *pNode     = new Node;
  *pNode          = Node(newEnd,NULL);
  //if there is no start node then the new node is made both the start and the end.
  if (startNode == 0) {
    endNode         = pNode;
    startNode       = pNode;
  } else {
    endNode -> next = pNode;
    endNode         = pNode;
  }
}

//member function to delete from start of SimpleList. It also returns the popped value
template<class l>
l  SimpleList<l>::deleteFromStart(){
    // find the next node and then make that start.
    Node currentStart = *startNode;
    Node *nextNode    = currentStart.next;
    delete(startNode);
    //make sure there is a next node! if we deleted the only node then startnode will be null.
    if (nextNode != 0) {
      startNode = nextNode;
    } else {
      startNode = NULL;
      endNode   = NULL;
    }
    return currentStart.data;
}

//The function checks if the simpleList is empty
template<class l>
bool  SimpleList<l>::isempty(){
  if (startNode == 0){
    return true;
  } else {
    return false;
  }
}

//This is the derived class of simpleList, stack. It defines its own push and pop functions
template<class l>
class Stack: public SimpleList<l> {
  public:
    void push(l &newItem);
    l   pop();
    Stack(string name);
};

//Push is the function to add to the list. As a stack is LIFO we need to delete and insert at the same size of the list.
//Therefore we insert at start and delete at start
template<class l>
void Stack<l>::push(l &newItem){
  this -> insertAtStart(newItem);
}

//pop removes from the list and returns the removed value
template<class l>
l Stack<l>::pop(){
  l deletedItem  = this -> deleteFromStart();
  return deletedItem;
}

//Here we set up the constructor for Stack
template<class l>
Stack<l>::Stack(string name){
  this -> setName(name);
};

//This is the derived class of simpleList, Queue. It defines its own push and pop functions
template<class l>
class Queue: public SimpleList<l>{
  public:
    void push(l &newItem);
    l   pop();
    Queue(string name);
};

// The push adds the new element to the end of the Queue. This is due to the fact that
// a queue is FIFO, therefore the items are added to the end and removed from the start.
template<class l>
void Queue<l>::push(l &newItem){
  this -> insertAtEnd(newItem);
}

//the pop command returns and removes the element at the start of the list.
template<class l>
l Queue<l>::pop(){
  l deletedItem  = this -> deleteFromStart();
  return deletedItem;
}

//This function is the constructor for the Queue class
template<class l>
Queue<l>::Queue(string name){
  this -> setName(name);
};

//This function takes in a string of a command and splits it up to three parts based on spaces
void parseCommand(string command, string &commandType,string &name,string &commandValue){
  // find the first space from start - first space is the first command
  int space1  = command.find(' ');
  commandType = command.substr(0,space1);
  int space2  = command.find(' ',space1 + 1);
  // to get the second command with out the extra space we need to subtract an 1
  name        = command.substr(space1+1, space2-space1-1);
  commandValue= command.substr(space2+1,command.length()-space2);
}

//this function returns the pointer to an object with the name list.
//if no object with that name exsists it returns the NULL pointer.
template <class l>
SimpleList<l> * findObj(list<SimpleList<l> *> &li, string n){
  for(typename list<SimpleList<l>*>::iterator it=li.begin(); it != li.end(); ++it){
    if ((*it)->getName() == n){
      return *it;
    }
  }
  return NULL;
}

//This function excutes the commands writen in the file inputed by the user and writes the responses to the output file the user selects
void executeCommands(string inputFile,string outputFile){
    list<SimpleList<int> *> listSLi;
    list<SimpleList<double> *> listSLd;
    list<SimpleList<string> *> listSLs;
    ifstream inFile (inputFile.c_str());
    ofstream outFile (outputFile.c_str());
    string command;
    //this main look is reading in the file line by line and putting the contents of the line into the string command
    while (getline(inFile,command)){
      stringstream ss;
      outFile << "PROCESSING COMMAND: " << command  << "\n";
      string commandType;
      string name;
      string commandValue;
      parseCommand(command,commandType,name,commandValue);
      string varType = name.substr(0,1);
      if(commandType == "push"){
        if(varType == "s"){
          SimpleList<string> * pSLs = findObj(listSLs,name);
          if (pSLs != 0){
            pSLs ->push(commandValue);
          }else{
            outFile<<"ERROR: This name does not exist!\n";
          }
        }else if (varType == "i"){
          SimpleList<int> * pSLi = findObj(listSLi,name);
          if (pSLi != 0){
            int newItem = atoi(commandValue.c_str());
            pSLi ->push(newItem);
          }else{
            outFile<<"ERROR: This name does not exist!\n";
          }
        }else if (varType == "d"){
          SimpleList<double> * pSLd = findObj(listSLd,name);
          if (pSLd != 0){
            double newItem = atof(commandValue.c_str());
            pSLd ->push(newItem);
          }else{
            outFile<<"ERROR: This name does not exist!\n";
          }
        }
      }else if (commandType == "pop"){
        if(varType == "s"){
          SimpleList<string> * pSLs = findObj(listSLs,name);
          if (pSLs != 0){
            bool empty          =   pSLs ->isempty();
            if (!empty){
                string poppedString =   pSLs ->pop();
                outFile<<"Value popped: " << poppedString <<"\n";
            } else{
                outFile<<"ERROR: This list is empty!\n";
            }
          }else{
            outFile<<"ERROR: This name does not exist!\n";
          }
        }else if (varType == "i"){
          SimpleList<int> * pSLi = findObj(listSLi,name);
          if (pSLi != 0 ){
            bool empty             =   pSLi ->isempty();
            if (!empty){
                int poppedInt = pSLi ->pop();
                outFile<<"Value popped: "<< to_string(poppedInt)<< "\n";
            }else{
                outFile<<"ERROR: This list is empty!\n";
            }
          }else{
            outFile<<"ERROR: This name does not exist!\n";
          }
        }else if (varType == "d"){
          SimpleList<double> * pSLd = findObj(listSLd,name);
          if(pSLd !=0 ){
            bool empty                =   pSLd ->isempty();
            if (!empty){
              double poppedDouble = pSLd ->pop();
              //we are using the stream to truncate the double.
              ss <<poppedDouble;
              string dStr = ss.str();
              outFile<<"Value popped: " << dStr << "\n";
            }else{
                outFile<<"ERROR: This list is empty!\n";
            }
          }else{
            outFile<<"ERROR: This name does not exist!\n";
          }
        }
      }else if (commandType == "create"){
        if(varType == "s"){
          if (findObj(listSLs,name) == 0){
            SimpleList<string> *pSLs;
            if(commandValue =="stack"){
              pSLs = new Stack<string>(name);
            }else if(commandValue =="queue"){
              pSLs = new Queue<string>(name);
            }
            listSLs.push_front(pSLs);
          }else{
            outFile<<"ERROR: This name already exists!\n";
          }
        }else if (varType == "i"){
          if (findObj(listSLi,name) == 0){
            SimpleList<int> *pSLi;
            if(commandValue =="stack"){
              pSLi = new Stack<int>(name);
            }else if(commandValue =="queue"){
              pSLi = new Queue<int>(name);
            }
            listSLi.push_front(pSLi);
          }else{
             outFile<<"ERROR: This name already exists!\n";
          }
        }else if (varType == "d"){
          if(findObj(listSLd,name) == 0){
            SimpleList<double> *pSLd;
            if(commandValue =="stack"){
              pSLd = new Stack<double>(name);
            }else if(commandValue =="queue"){
              pSLd = new Queue<double>(name);
            }
            listSLd.push_front(pSLd);
          }else{
            outFile<<"ERROR: This name already exists!\n";
          }
        }
      }
    }
    outFile.close();
}

int main(){
  string input ;
  cout<<"Please provide an input file name:" <<endl;
  cin>>input;
  string output;
  cout<<"Please provide an output file name:" <<endl;
  cin>>output;
  executeCommands(input, output);
  return 0;
}
