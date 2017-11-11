#include "DiningPhilosophersMonitor.h"

// Monitor class to ensure mutual exclusion between the philosopher threads.
DiningPhilosophersMonitor::DiningPhilosophersMonitor(size_t numPhilosophers){

    this->numPhilosophers = numPhilosophers;
    this->hasEaten.resize(numPhilosophers);
    this->state.resize(numPhilosophers);
    this->self.resize(numPhilosophers);

    pthread_mutex_init(&mutex, nullptr);

    for(int i = 0; i < numPhilosophers; i++){
        state[i] = THINKING;
        hasEaten[i] = false;
        pthread_cond_init(&self[i], nullptr);
    }
}

DiningPhilosophersMonitor::~DiningPhilosophersMonitor(){
    pthread_mutex_destroy(&mutex);
    for(auto &i : self) pthread_cond_destroy(&i);
}

void DiningPhilosophersMonitor::pickupFork(int i){
    state[i] = HUNGRY;
    printf("Thread %d hungry\n", i);
    test(i);
    if(state[i] != EATING){
        pthread_mutex_lock(&mutex);
        printf("Thread %d waiting to eat\n", i);
        pthread_cond_wait(&self[i], &mutex);
        test(i);
        pthread_mutex_unlock(&mutex);
    }
}

void DiningPhilosophersMonitor::putdownFork(int i){
    printf("Thread %d putting down fork\n", i);
    state[i] = THINKING;
    test((i+4) % numPhilosophers);
    test((i+1) % numPhilosophers);
}

int DiningPhilosophersMonitor::hasStarvationOccured() const{
    for(const auto &i : hasEaten) { if(!i) return i; }
    return -1;
}

// Tests to make sure a philosopher is allowed to eat (i.e. its neighbors aren't eating)
void DiningPhilosophersMonitor::test(int i){
    if ((state[(i + 4) % 5] != EATING) &&
        (state[i] == HUNGRY) &&
        (state[(i + 1) % 5] != EATING)) {
        state[i] = EATING;
        hasEaten[i] = true;
        printf("Thread %d eating\n", i);
        pthread_mutex_lock(&mutex);
        pthread_cond_signal(&self[i]);
        pthread_mutex_unlock(&mutex);
    }
}
