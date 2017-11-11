#ifndef DININGPHILOSOPHER_BANKERS_H
#define DININGPHILOSOPHER_BANKERS_H


#include <vector>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "Customer.h"


class Bank{
public:
    Bank(const std::vector<Customer*>&customers, const std::vector<int>&available);
    ~Bank();

    bool requestResources(int customer_num, const std::vector<int>&request);
    bool releaseResources(int customer_num, const std::vector<int>&release);

    std::vector<Customer*>&getCustomerVec();
    Customer *getCustomer(int customer_num);
    size_t size();
    std::string printAvailable() const;
    void printAll() const;

private:
    size_t num_customers;
    std::vector<Customer*> customerVec;
    std::vector<int> available;               // the available amount of each resource

    bool customerNotFinished(std::vector<int> &work, std::vector<bool> &finish);
    void addVectors(std::vector<int>&source, const std::vector<int>&a, const std::vector<int>&b);
    void subtractVectors(std::vector<int>&source, const std::vector<int>&a, const std::vector<int>&b);
    bool isVectorLessThan(const std::vector<int>&a, const std::vector<int>&b);
    bool isSafeState();
};

#endif //DININGPHILOSOPHER_BANKERS_H
