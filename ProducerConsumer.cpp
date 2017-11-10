#include "ProducerConsumer.h"

ProducerConsumer::ProducerConsumer(size_t bufferSize) {
    this->bufferSize = bufferSize;
    this->itemCount = 0;
    this->buffer.resize(bufferSize);
    pthread_cond_init(&full, nullptr);
    pthread_cond_init(&empty, nullptr);
    pthread_mutex_init(&mutex, nullptr);
}

ProducerConsumer::~ProducerConsumer(){
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&full);
    pthread_cond_destroy(&empty);
}

void ProducerConsumer::add(int item){
    // wait for more room before adding
    if(itemCount == bufferSize){
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&full, &mutex);
        pthread_mutex_unlock(&mutex);
    }

    buffer[itemCount] = item;
    itemCount = (itemCount + 1) % bufferSize;

    if(itemCount == 1){
        pthread_cond_signal(&empty);
    }
}

int ProducerConsumer::remove(){
    // wait until more items exist before removing
    if(itemCount == 0){
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&empty, &mutex);
        pthread_mutex_unlock(&mutex);
    }

    int item = buffer[itemCount];
    itemCount--;

    if(itemCount == bufferSize - 1){
        pthread_cond_signal(&full);
    }

    return item;
}

void ProducerConsumer::produce(int item){
    printf("Producing item: %d\n", item);
    add(item);
}

void ProducerConsumer::consume(){
    printf("Consuming item: %d\n", remove());
}