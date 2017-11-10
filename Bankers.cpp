#include "Bankers.h"

Bank::Bank(const std::vector<Customer*> &customers, const std::vector<int>&available) {
    this->customerVec = customers;
    this->available = available;
    this->num_customers = customers.size();
}

Bank::~Bank(){
    if(!customerVec.empty()){
        for(Customer* customer : customerVec){ delete customer; }
    }
}

bool Bank::requestResources(int customer_num, const std::vector<int>&request){

    assert(customer_num >= 0 && customer_num < customerVec.size());
    if(!isVectorLessThan(request, customerVec[customer_num]->getNeed()) || !isVectorLessThan(request, available)){
        return false;
    }

    // 'Pretend' to release resources to Customer
    subtractVectors(available, available, request);
    addVectors(customerVec[customer_num]->getAllocation(), customerVec[customer_num]->getAllocation(), request);
    subtractVectors(customerVec[customer_num]->getNeed(), customerVec[customer_num]->getNeed(), request);


    if(!isSafeState()){
        // Take away resources
        addVectors(available, available, request);
        subtractVectors(customerVec[customer_num]->getAllocation(), customerVec[customer_num]->getAllocation(), request);
        addVectors(customerVec[customer_num]->getNeed(), customerVec[customer_num]->getNeed(), request);
        return false;
    }

    return true;
}

bool Bank::releaseResources(int customer_num, const std::vector<int>&release){

    if(!isVectorLessThan(release, customerVec[customer_num]->getAllocation())){ return false; }

    addVectors(available, available, release);
    subtractVectors(customerVec[customer_num]->getAllocation(), customerVec[customer_num]->getAllocation(), release);
    addVectors(customerVec[customer_num]->getNeed(), customerVec[customer_num]->getNeed(), release);

    return true;
}


void Bank::addVectors(std::vector<int>&source, const std::vector<int>&a, const std::vector<int>&b){
    assert(a.size() == b.size());
    for(int i = 0; i < source.size(); i++){ source[i] = a[i] + b[i]; }
}

void Bank::subtractVectors(std::vector<int>&source, const std::vector<int>&a, const std::vector<int>&b){
    assert(a.size() == b.size());
    for(int i = 0; i < source.size(); i++){ source[i] = a[i] - b[i]; }
}

// Returns true if the vector a is less than or equal to the vector b
bool Bank::isVectorLessThan(const std::vector<int>&a, const std::vector<int>&b){
    assert(a.size() == b.size());
    for(int i = 0; i < a.size(); i++){
        if(a[i] > b[i]){ return false; }
    }
    return true;
}

// Helper function for method isSafeState()
bool Bank::customerNotFinished(std::vector<int> &work, std::vector<bool> &finish){
    bool done = false;
    for(int i = 0; i < customerVec.size(); i++){
        Customer *customer = customerVec[i];
        if(!finish[i] && isVectorLessThan(customer->getNeed(), work)){
            addVectors(work, work, customer->getAllocation());
            finish[i] = true;
            done = true;
        }
    }
    return done;
}

// Returns true if the system is in a deadlock state
bool Bank::isSafeState() {
    std::vector<int> work = available;
    std::vector<bool> finish(customerVec.size(), false);
    bool done = false;
    do{
        if(!customerNotFinished(work, finish)){ done = true; }
    }while(!done);

    for(bool b : finish) { if(!b) return false; }
    return true;
}

void Bank::printAll() const{
    if(!customerVec.empty()){
        std::cout << "************************ SYSTEM SNAPSHOT *************************\n\n";
        std::cout << std::left << std::setw(18) << "    ALLOCATION" << std::setw(10) << "MAX" << std::setw(10) << "NEED" << "AVAILABLE: " << printAvailable() << "\n";
        for(auto customer : customerVec){
            std::cout << "P" << customer->getId() << "  " << std::setw(14) << customer->printAllocation()
                      << std::setw(10) << customer->printMaximum() << customer->printNeed() << "\n";
        }
        std::cout << "\n******************************************************************\n\n";
    }
}


std::vector<Customer*>& Bank::getCustomerVec() { return customerVec; }

Customer* Bank::getCustomer(int customer_num){
    if(customer_num < 0 || customer_num >= customerVec.size()) { return nullptr; }
    return customerVec[customer_num];
}

size_t Bank::size() { return num_customers; }

std::string Bank::printAvailable() const{
    std::stringstream ss;
    for(auto i : available) { ss << i << " "; }
    return ss.str();
}