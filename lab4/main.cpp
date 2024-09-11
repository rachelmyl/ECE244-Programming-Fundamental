#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cmath>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

// Function Declarations:

// Set mode of the simulation
string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes 

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join


// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);

// Update helper function
void updateSingle();
void updateMultiple();
Register* get_least_available_time();
Register* get_least_available_time_multi();

// Global variables
RegisterList *registerList; // holding the list of registers
QueueList *doneList; // holding the list of customers served
QueueList *singleQueue; // holding customers in a single virtual queue
double expTimeElapsed; // time elapsed since the beginning of the simulation

// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  cout << "> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
        parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
        addCustomer(lineStream, mode);
    } else if (command == "^D"){
        break;
    } else {
        cout << "Invalid operation" << endl;
    }

    cout << "> ";  // Prompt for input
    getline(cin, line);
  }
  // doneList->print();

//    Finished at time 168
//    Statistics:
//    Maximum wait time: 50
//    Average wait time: 30.3333
//    Standard Deviation of wait time: 17.2498

  cout << "\nFinished at time " << expTimeElapsed << endl;
  cout << "Statistics:" << endl;

  Customer* temp_cus = doneList -> get_head();
  double wait_time = 0.0;
  double max = 0.0;
  double sum = 0.0;
  int n = 0;
  while (temp_cus != nullptr){
      wait_time = temp_cus->get_departureTime() - temp_cus->get_arrivalTime();
      if (wait_time > max){
          max = wait_time;
      }
      sum += wait_time;
      n++;
      temp_cus = temp_cus -> get_next();
  }
  double avg = sum/n;
  cout << "  Maximum wait time: " << max << endl;
  cout << "  Average wait time: " << avg << endl;

  temp_cus = doneList -> get_head();
  sum = 0;
  while (temp_cus != nullptr) {
      wait_time = temp_cus->get_departureTime() - temp_cus->get_arrivalTime();
      sum += pow(wait_time - avg, 2);
      temp_cus = temp_cus -> get_next();
  }
  double std_dev = sqrt(sum/n);
  cout << "  Standard Deviation of wait time: " << std_dev << endl;

  // You have to make sure all dynamically allocated memory is freed 
  // before return 0
  delete registerList;
  delete doneList;
  delete singleQueue;

  return 0;
}

void updateSingle(){
    /*
     * 1. check if any register is done serving the customer
     *      if yes, update the departure time for customer and available time for register
     *          add them to done list
     * 2. check if there's any customer in the queue
     *      if no, do nothing
     *      if yes, check whether available time < expTimeElapsed + timeElapse add the next customer
     *          customer update arrivalTime
     *          update available time using calculateDepartTime()
     */

    // Senario 1: there is an empty register when customer enter the queue

    // Senario 2: a register finishes and add next customer to it

      Register* reg = get_least_available_time();

      while (reg != nullptr){

          Customer* dep_cus = reg -> get_queue_list() -> get_head();
          if (dep_cus != nullptr){
            cout << "Departed a customer at register ID " << reg->get_ID() << " at " << reg->get_availableTime() << endl;
            reg -> get_queue_list() -> get_head() -> set_departureTime(reg->get_availableTime());
            reg -> departCustomer(doneList);
          }

          // add a new customer
          if (singleQueue -> get_head() != nullptr){
              reg -> get_queue_list() -> enqueue(singleQueue -> dequeue());
              reg -> set_availableTime( reg->get_availableTime() + reg->calculateDepartTime() );
              // update available time using calculateDepartTime()
              cout << "Queued a customer with free register " << reg->get_ID() << endl;
          } 

          if (dep_cus == nullptr && singleQueue -> get_head() == nullptr)
              break;

          reg = get_least_available_time();
      }

}

Register* get_least_available_time(){
    if (registerList->get_head() == nullptr){
        return nullptr;
    }
    Register *min_reg = nullptr, *temp_reg = registerList->get_head();
    double min_time, temp_time;

    while (temp_reg != nullptr && temp_reg -> get_queue_list() -> get_head() == nullptr){
      temp_reg = temp_reg->get_next();
    }
    if (temp_reg != nullptr){
        min_time = temp_reg->get_availableTime();
    } else{
      return nullptr;
    }

    while (temp_reg != nullptr){
        temp_time = temp_reg->get_availableTime();
        // cout << "expTimeElapse = " << expTimeElapsed << "; temp_time = " << temp_time << "; min_time = " << min_time << endl;
        if (expTimeElapsed >= temp_time && temp_time <= min_time && temp_reg->get_queue_list()->get_head() != nullptr){
            min_reg = temp_reg;
            min_time = temp_time;
        }
        if (temp_reg->get_next() != nullptr){
            temp_reg = temp_reg -> get_next();
        } else{
            break;
        }  
    }
    return min_reg;
}

void updateMultiple(){
    /*
     * 1. customer has already been added to queue, just have to check whether any customer is done
     * 2. add them to done list
     */

    Register* reg = get_least_available_time_multi();

    while (reg != nullptr){
        // set next customer
        cout << "Departed a customer at register ID " << reg->get_ID() << " at " << reg->get_availableTime() << endl;
        reg -> get_queue_list() -> get_head() -> set_departureTime(reg->get_availableTime());
                  // update departure time
        reg -> departCustomer(doneList);
                  // move customer to donelist
        if (reg -> get_queue_list() -> get_head() != nullptr){  // if there is a second customer lining up
            reg -> set_availableTime( reg->get_availableTime() + reg->calculateDepartTime() );
                    // update available time using calculateDepartTime()
        }
        reg = get_least_available_time_multi();
    }    
}

Register* get_least_available_time_multi(){
    if (registerList->get_head() == nullptr){
        return nullptr;
    }
    Register *min_reg = nullptr, *temp_reg = registerList->get_head();
    double min_time, temp_time;
    
    while (temp_reg != nullptr && temp_reg -> get_queue_list() -> get_head() == nullptr){
      temp_reg = temp_reg->get_next();
    }
    if (temp_reg != nullptr){
        min_time = temp_reg->get_availableTime();
    } else{
      return nullptr;
    }
    

    while (temp_reg != nullptr){
      // cout << "1: " << expTimeElapsed << endl;
      if (temp_reg -> get_queue_list() -> get_head() != nullptr){
        temp_time = temp_reg->get_availableTime();
        // cout << "2: " << temp_time << min_time << endl;
        // cout << "expTimeElapse = " << expTimeElapsed << "; temp_time = " << temp_time << "; min_time = " << min_time << endl;
        if (expTimeElapsed >= temp_time && temp_time <= min_time && temp_reg->get_queue_list()->get_head() != nullptr){
            // cout << "3" << endl;
            min_reg = temp_reg;
            min_time = temp_time;
        }  
      }
      if (temp_reg->get_next() != nullptr){
          temp_reg = temp_reg -> get_next();
      } else{
          break;
      }
    }
    return min_reg;
}

string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;
  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments." << endl;
    return;
  }
  // Depending on the mode of the simulation (single or multiple),
  // add the customer to the single queue or to the register with
  // fewest items

  /*
   * 1. the customer info is correct, add a customer
   * 2. if single, add to singlequeue
   *        check if any register is empty, if so, add to the register, set arrivetime
   *        update elapse time
   * 3. if double, check QueueList::get_items(), get the least item queue and add to it
   */
  
  Customer* new_cus = new Customer(expTimeElapsed + timeElapsed, items);

//  if (mode == "single"){
//      singleQueue -> enqueue(new_cus);
//  } else if (mode == "multiple"){
//      registerList -> get_min_items_register() -> get_queue_list() -> enqueue(new_cus);
//  }

  expTimeElapsed += timeElapsed;
  if (mode == "single"){
      updateSingle();

      cout << "A customer entered" << endl;
      Register* reg = registerList -> get_free_register();
      if (reg != NULL){
          reg -> get_queue_list() -> enqueue(new_cus);
          reg -> get_queue_list() -> get_head() -> set_arrivalTime(expTimeElapsed);
          // customer update arrivalTime
          reg -> set_availableTime( expTimeElapsed + reg->calculateDepartTime() );
          // update available time using calculateDepartTime()
          cout << "Queued a customer with free register " << reg->get_ID() << endl;
      } else{
          singleQueue -> enqueue(new_cus);
          cout << "No free registers" << endl;
      }

  } else if (mode == "multiple"){
      updateMultiple();
      Register* add_reg = registerList -> get_min_items_register();
      add_reg -> get_queue_list() -> enqueue(new_cus);
      cout << "A customer entered" << endl;
      cout << "Queued a customer with quickest register " << add_reg -> get_ID() << endl;

      if (add_reg -> get_queue_list() -> get_head() -> get_next() == nullptr){
          add_reg -> set_availableTime( expTimeElapsed + add_reg->calculateDepartTime() );
          // update available time using calculateDepartTime()
      }
  }
}

void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    cout << "Invalid operation" << endl;
  }

}

void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments" << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  // Check if the register is already open
                // bool RegisterList::foundRegister(int ID) {
  // If it's open, print an error message
  // Otherwise, open the register
                // Register::Register(int id, double timePerItem, double overhead,
                //         double entryTime) {
  // If we were simulating a single queue, 
  // and there were customers in line, then 
  // assign a customer to the new register
  if (registerList->foundRegister(ID)){
      cout << "Error: register " << ID << " is already open" << endl;
      return;
  }

  expTimeElapsed += timeElapsed;
  if (mode == "single"){
      updateSingle();

      Register* new_reg = new Register (ID, secPerItem, setupTime, expTimeElapsed);
      // setuptime = overhead
      registerList -> enqueue(new_reg);
      cout << "Opened register " << ID << endl;
      if (singleQueue -> get_head() != nullptr){
          new_reg -> get_queue_list() -> enqueue(singleQueue -> dequeue());
          // error: don't need to set arrival time: new_cus -> set_arrivalTime(expTimeElapsed);
          // customer update arrivalTime
          new_reg -> set_availableTime( expTimeElapsed + new_reg->calculateDepartTime() );
          // update available time using calculateDepartTime()
          cout << "Queued a customer with free register " << new_reg->get_ID() << endl;
      }

  } else if (mode == "multiple"){
      updateMultiple();

      Register* new_reg = new Register (ID, secPerItem, setupTime, expTimeElapsed);
      // setuptime = overhead
      registerList -> enqueue(new_reg);
      cout << "Opened register " << ID << endl;
      // do nothing until next customer comes
  }
}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments" << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is open
  // If it is open dequeue it and free its memory
  // Otherwise, print an error message 

    if (!registerList->foundRegister(ID)){
        cout << "Error: register " << ID << " is not open" << endl;
        return;
    }

    expTimeElapsed += timeElapsed;
    if (mode == "single"){
        updateSingle();
        // registerList -> print();
    } else if (mode == "multiple"){
        updateMultiple();
    }

    Register* del_reg = registerList -> dequeue(ID);
    delete del_reg;
    cout << "Closed register " << ID << endl;

    // Register* reg = registerList->get_head();
    // cout << "Available time 101: " << reg->get_availableTime() << endl;
    // reg = reg->get_next();
    // cout << "Available time 102: " << reg->get_availableTime() << endl;
    // cout << "Time: " << expTimeElapsed << endl;
}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}
