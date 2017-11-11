#include "Customer.h"

Customer::Customer(const std::vector<int> &allocation,
                   const std::vector<int> &maximum,
                   int id){
    assert(maximum.size() == allocation.size());
    this->id = id;
    this->maximum = maximum;
    this->allocation = allocation;
    calculateNeed();
}


Customer::~Customer(){ }

void Customer::calculateNeed() {
    for(int i = 0; i < maximum.size(); i++){
        need.push_back(maximum[i] - allocation[i]);
    }
}

std::vector<int>&Customer::getMaximum() {
    return maximum;
}

std::vector<int>&Customer::getAllocation() {
    return allocation;
}

std::vector<int>&Customer::getNeed() {
    return need;
}

int Customer::getId() { return id; }

std::string Customer::printNeed() {
    std::stringstream ss;
    for(auto i : need) { ss << i << " "; }
    return ss.str();
}

std::string Customer::printMaximum() {
    std::stringstream ss;
    for(auto i : maximum) { ss << i << " "; }
    return ss.str();
}

std::string Customer::printAllocation() {
    std::stringstream ss;
    for(auto i : allocation) { ss << i << " "; }
    return ss.str();
};
