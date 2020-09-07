#include "cycles.cpp"
#include <thread>
#include <unistd.h>

#define PROB 70

int main(){
    PriQueue queueA('A', 20);
    PriQueue queueB('B', 20);
    PriQueue queueC('C', 20);

    std::thread prodA(producer, std::ref(queueA));
    usleep(500);
    std::thread prodB(producer, std::ref(queueB));
    usleep(500);
    std::thread prodC(producer, std::ref(queueC));
    usleep(500);

    std::thread sProd(specProducer, std::ref(queueA), std::ref(queueB), std::ref(queueC));
    usleep(500);

    std::thread consA(consumerA, std::ref(queueA), std::ref(queueB), std::ref(queueC), PROB);
    usleep(500);
    std::thread consB(consumerB, std::ref(queueA), std::ref(queueB), std::ref(queueC), PROB);
    usleep(500);
    std::thread consC(consumerC, std::ref(queueA), std::ref(queueB), std::ref(queueC), PROB);
    usleep(500);
    std::thread oProd(defProducer, std::ref(queueA), std::ref(queueB), std::ref(queueC), 500);

    prodA.join();
    prodB.join();
    prodC.join();
    sProd.join();
    consA.join();
    consB.join();
    consC.join();
    oProd.join();
}
