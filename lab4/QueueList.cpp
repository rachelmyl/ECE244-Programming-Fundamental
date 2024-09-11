#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() {
  while (head != NULL){
      Customer* temp = head;
      head = head -> get_next();
      delete temp;
  }
}

Customer* QueueList::get_head() { return head; }

void QueueList::enqueue(Customer* customer) {
  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head
  if (head == NULL){
      head = customer;
  } else{
      Customer* temp = head;
      while (temp -> get_next() != NULL){
          temp = temp -> get_next();
      }
      temp -> set_next(customer);

//      if (temp -> get_departureTime() == NULL){     // when this is queue list
//          while (temp -> get_next() != NULL){
//              temp = temp -> get_next();
//          }
//          temp -> get_next() = customer;
//
//      } else{                                       // when this is done list
//          while (temp -> get_next() != NULL
//                && temp -> get_next() -> get_departureTime() > customer -> get_departureTime()){
//                    // when the customer leave earlier than the next customer
//              temp = temp -> get_next();
//          }
//          temp -> get_next() = customer;
//      }
  }
}

Customer* QueueList::dequeue() {
  // remove a customer from the head of the queue 
  // and return a pointer to it
  Customer* temp = head;
  head = head -> get_next();
  temp -> set_next(nullptr);
  return temp;
}

int QueueList::get_items() {
  // count total number of items each customer in the queue has
  int count = 0;
  if (head != nullptr){
    Customer* temp = head;
    while (temp != NULL){
        count += temp -> get_numOfItems();
        temp = temp -> get_next();
    }
  }
  return count;
}

bool QueueList::empty_queue() {
  // if the queue is empty, return false
  // if the queue is not empty, delete all customers
  // and return true
  if (head == NULL)
      return false;
  else{
      while (head != NULL){
          Customer* temp = head;
          head = head -> get_next();
          delete temp;
      }
      return true;
  }
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
