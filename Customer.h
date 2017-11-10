
#ifndef DININGPHILOSOPHER_CUSTOMER_H
#define DININGPHILOSOPHER_CUSTOMER_H


#include <vector>
#include <cassert>
#include <iostream>
#include <sstream>

class Customer {
public:
    Customer(const std::vector<int> &allocation,
             const std::vector<int> &maximum,
             int id);
    ~Customer();

    std::vector<int>&getMaximum();
    std::vector<int>&getAllocation();
    std::vector<int>&getNeed();
    std::string printNeed();
    std::string printMaximum();
    std::string printAllocation();
    int getId();

private:
    int id;
    std::vector<int> maximum;
    std::vector<int> allocation;
    std::vector<int> need;
    void calculateNeed();
};

Customer::Customer(const std::vector<int> &allocation,
                   const std::vector<int> &maximum,
                   int id){
    assert(maximum.size() == allocation.size());
    this->id = id;
    this->maximum = maximum;
    this->allocation = allocation;
    calculateNeed();
}

#endif //DININGPHILOSOPHER_CUSTOMER_H
