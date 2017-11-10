
#ifndef DININGPHILOSOPHER_PRODUCERCONSUMER_H
#define DININGPHILOSOPHER_PRODUCERCONSUMER_H

#include <pthread.h>
#include <vector>

class ProducerConsumer {
public:
    ProducerConsumer(size_t bufferSize);
    ~ProducerConsumer();

    void produce(int item);
    void consume();

private:
    size_t itemCount;
    size_t bufferSize;
    std::vector<int> buffer;
    pthread_cond_t full;
    pthread_cond_t empty;
    pthread_mutex_t mutex;

    void add(int item);
    int  remove();
};


#endif //DININGPHILOSOPHER_PRODUCERCONSUMER_H
