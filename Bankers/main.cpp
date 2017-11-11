#include <iostream>
#include <random>
#include <sstream>
#include "Bank.h"

pthread_mutex_t lock;

// Returns a random number between from and to
int nextInt(int from, int to){
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> dist(from, to);
    return dist(e1);
}

// Helper function to convert a vector of integers to a string for displaying purposes
std::string vectorToString(std::vector<int> &vec){
    std::stringstream ss;
    for(auto i : vec) { ss << i << " "; }
    return ss.str();
}

// thread to request a random number of resources from the bank
void *request(void *bank){
    pthread_mutex_lock(&lock);
    auto b = (Bank*) bank;
    int n = nextInt(0,4);
    auto customer = b->getCustomer(n);
    std::vector<int> randRequest(3, 0);
    for(int i = 0; i < customer->getNeed().size(); i++){
        if(customer->getNeed()[i] != 0){ randRequest[i] = nextInt(0, customer->getNeed()[i]); }
    }

    if(b->requestResources(n, randRequest)){
        std::cout << "\nCustomer " << n << " request for " << vectorToString(randRequest) << ": GRANTED\n";
    } else {
        std::cout << "\nCustomer " << n << " request for " << vectorToString(randRequest) << ": DENIED\n";
    }
    pthread_mutex_unlock(&lock);
    return nullptr;
}

// thread to release a random number of resources from the bank
void *release(void *bank){
    pthread_mutex_lock(&lock);
    auto b = (Bank*) bank;
    int n = nextInt(0,4);
    auto customer = b->getCustomer(n);
    std::vector<int> randRelease(3, 0);
    for(int i = 0; i < customer->getAllocation().size(); i++){
        if(customer->getAllocation()[i] != 0){ randRelease[i] = nextInt(0, customer->getAllocation()[i]); }
    }

    if(b->releaseResources(n, randRelease)){
        std::cout << "\nCustomer " << n << " released " << vectorToString(randRelease) << "\n";
    }
    pthread_mutex_unlock(&lock);
    return nullptr;
}


int main(int argc, char *args[]) {

    // Max number of resources = 3
    assert(argc == 4 && "You must enter 3 resources");
    std::vector<int> resources;
    for(int i = 1; i < argc; i++){ resources.push_back(std::stoi(args[i])); }

    // Create customers, bank, and thread objects
    std::vector<Customer*> customers;
    customers.push_back(new Customer({0,1,0}, {7,5,3}, 0));
    customers.push_back(new Customer({3,0,2}, {3,2,2}, 1));
    customers.push_back(new Customer({3,0,2}, {9,0,2}, 2));
    customers.push_back(new Customer({2,1,1}, {2,2,2}, 3));
    customers.push_back(new Customer({0,0,2}, {4,3,3}, 4));

    Bank *bank = new Bank(customers, resources);

    std::vector<pthread_t> threadVec(5);
    assert(pthread_mutex_init(&lock, nullptr) == 0);

    // Simulate customers requesting random numbers of resources.
    std::cout << "\nSimulating customers requesting random numbers of resources ... \n\n";
    bank->printAll();
    for(int i = 0; i < 5; i++){
        for(int i = 0; i < threadVec.size(); i++){ pthread_create(&threadVec[i], nullptr, request, bank); }
        for(auto thread : threadVec) { pthread_join(thread, nullptr); }
        std::cout << "\n";
        bank->printAll();
    }

    // Simulate customers releasing random numbers of resources.
    std::cout << "\nSimulating customers releasing random numbers of resources ... \n\n";
    for(int i = 0; i < 5; i++){
        for(int i = 0; i < threadVec.size(); i++){ pthread_create(&threadVec[i], nullptr, release, bank); }
        for(auto thread : threadVec) { pthread_join(thread, nullptr); }
        std::cout << "\n";
        bank->printAll();
    }

    std::cout << "\nEnding simulation ... \n";


    delete bank;
    pthread_mutex_destroy(&lock);

    return 0;
}