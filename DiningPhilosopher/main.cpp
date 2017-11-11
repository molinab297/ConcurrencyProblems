#include "DiningPhilosophersMonitor.h"

// Used for passing multiple parameters to thread function
struct Parameters{
    Monitor monitor;
    int id;
};

// Thread function in which each philosopher thread will think (for 1-3 seconds), eat, and sleep, respectively.
void *eat(void *p){
    auto params = (Parameters*)p;
    auto dpMonitor = params->monitor;
    int id = params->id;

    printf("Created philosopher: %d\n", id);

    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(1, 3);
    int sleepTime = uniform_dist(e1);

    printf("Thread %d thinking...\n", id);

    sleep(sleepTime);
    dpMonitor->pickupFork(id);
    dpMonitor->putdownFork(id);

    return nullptr;
}

// Driver function
int main() {

    const int NUM_PHILOSOPHERS = 5;

    pthread_t philosophers[NUM_PHILOSOPHERS];
    auto dpMonitor    = new DiningPhilosophersMonitor(NUM_PHILOSOPHERS);
    auto threadParams = new Parameters[NUM_PHILOSOPHERS];

    // Simulate philosophers thinking, eating, and sleeping, respectively.
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        threadParams[i].id = i;
        threadParams[i].monitor = dpMonitor;
        if(pthread_create(&philosophers[i], nullptr, eat, &threadParams[i])){
            fprintf(stderr, "Error creating thread");
            exit(1);
        }
    }

    for(auto &i : philosophers){ pthread_join(i, nullptr); }

    // Check for the occurrence of starvation
    if(int philosopherId = dpMonitor->hasStarvationOccured() > -1){
        fprintf(stderr, "Philosopher with id: %d has starved\n", philosopherId);
    }

    delete dpMonitor;
    delete [] threadParams;

    return 0;
}