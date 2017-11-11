#include "ProducerConsumer.h"
#include <random>

void *produce(void *params){
    auto monitor = (ProducerConsumer*) params;
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> gen_random_num(1, 10);
    while(true){
        monitor->produce(gen_random_num(e1));
    }
}

void *consume(void *params){
    auto monitor = (ProducerConsumer*) params;
    while(true){
        monitor->consume();
    }
}

// Driver function
int main() {

    ProducerConsumer *producerConsumer = new ProducerConsumer(5);
    pthread_t produce_thread, consume_thread;

    if(pthread_create(&produce_thread,nullptr,produce,producerConsumer) ||
            pthread_create(&consume_thread,nullptr,consume,producerConsumer)){
        fprintf(stderr, "Failed to create one or more threads\n");
        exit(1);
    }

    pthread_join(produce_thread, nullptr);
    pthread_join(consume_thread, nullptr);

    delete producerConsumer;

    return 0;
}