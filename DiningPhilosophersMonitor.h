#ifndef DININGPHILOSOPHER_DININGPHILOSOPHERSMONITOR_H
#define DININGPHILOSOPHER_DININGPHILOSOPHERSMONITOR_H

#include <iostream>
#include <zconf.h>
#include <random>

enum states{THINKING, HUNGRY, EATING};

// Thread Safe Monitor class to ensure mutual exclusion between the philosopher threads.
class DiningPhilosophersMonitor{
public:
    DiningPhilosophersMonitor(size_t numPhilosophers);
    ~DiningPhilosophersMonitor();

    void pickupFork(int i);
    void putdownFork(int i);
    int  hasStarvationOccured() const;

private:
    size_t numPhilosophers;
    pthread_mutex_t mutex;
    std::vector<pthread_cond_t> self;
    std::vector<states> state;
    std::vector<bool> hasEaten;
    // Tests to make sure a philosopher is allowed to eat (i.e. its neighbors aren't eating)
    void test(int i);
};

#endif //DININGPHILOSOPHER_DININGPHILOSOPHERSMONITOR_H
