#include "DiningPhilosophersMonitor.h"

// Used for passing multiple parameters to thread function
struct Parameters{
    DiningPhilosophersMonitor *dpMonitor;
    int id;
};

// Thread function in which each philosopher thread will think (for 1-3 seconds), eat, and sleep, respectively.
void *eat(void *p){
    auto params = (Parameters*)p;
    auto dpMonitor = params->dpMonitor;
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

    pthread_t philosophers[5];
    auto dpMonitor  = new DiningPhilosophersMonitor(5);
    auto parameters = new Parameters[5];

    // Simulate philosophers thinking, eating, and sleeping, respectively.
    for(int i = 0; i < 5; i++){
        parameters[i].id = i;
        parameters[i].dpMonitor = dpMonitor;
        if(pthread_create(&philosophers[i], nullptr, eat, &parameters[i])){
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
    delete [] parameters;

    return 0;
}