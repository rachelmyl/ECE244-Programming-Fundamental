#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  // Delete all registers in the list
  while (head != nullptr){
      Register* temp = head;
      head = head -> get_next();
      delete temp;
  }
}

Register* RegisterList::get_head() { return head; }

int RegisterList::get_size() { 
  // return number of registers
  int count = 0;
  Register* temp = head;
  while (temp != NULL){
      count++;
      temp = temp -> get_next();
  }
  return count;
}


Register* RegisterList::get_min_items_register() {
    // loop all registers to find the register with least number of items
    if (head == nullptr){
      return nullptr;
    }
    Register *temp_reg = head, *min_reg = head;

    //traverse through the first register
    QueueList* queue = temp_reg->get_queue_list();
    int min_items = queue -> get_items();
    temp_reg = temp_reg -> get_next();

    while (temp_reg != nullptr) {
        queue = temp_reg -> get_queue_list();
        int items = queue -> get_items();
        
        //refreshes min_items_register to the lowest items register
        if (items < min_items) {
            min_items = items;
            min_reg = temp_reg;
        }
        temp_reg = temp_reg->get_next();
    }

    return min_reg;

}

Register* RegisterList::get_free_register() {
  // return the register with no customers
  // if all registers are occupied, return nullptr
  Register* temp = head;
  while (temp != nullptr){
      if (temp -> get_queue_list() -> get_head() == NULL)
          return temp;
      temp = temp -> get_next();
  }
  return nullptr;
}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size 
  if (head == nullptr){
      head = newRegister;
  } else{
      Register* temp = head;
      while (temp -> get_next() != NULL){
          temp = temp -> get_next();
      }
      temp -> set_next(newRegister);
  }
}

bool RegisterList::foundRegister(int ID) {
  // look for a register with the given ID
  // return true if found, false otherwise
    Register* temp = head;
    while (temp != nullptr){
        if (temp -> get_ID() == ID)
            return true;
        temp = temp -> get_next();
    }
    return false;
}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID

  // return the dequeued register
  // return nullptr if register was not found
  if (foundRegister(ID)){
      Register* temp = head;
      while (temp->get_next() != nullptr){
          if (temp->get_next() -> get_ID() == ID){
              Register* delRegister = temp->get_next();
              temp -> set_next( delRegister->get_next() );
              delRegister -> set_next(nullptr);
              return delRegister;
          }
          temp = temp -> get_next();
      }
      return nullptr;
  } else
      return nullptr;
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr
  Register *least = head, *temp_register = head;
  double min = 0, temp_time;
  while (temp_register != NULL){
      temp_time = temp_register -> calculateDepartTime();
      if (temp_time < min){
          least = temp_register;
          min = temp_time;
      }
  }
  return least;
}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
