#ifndef QUEUE_HPP_INCLUDED
#define QUEUE_HPP_INCLUDED

//#include "monitor.h"
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <condition_variable>
#include <mutex>

#define NORM 0
#define HIGH 1
#define SPEC 2

using messageType = std::pair<int, std::string>;
using queueType = std::vector<messageType>;

const char letters[] = "ABC";

class PriQueue{
private:
    std::mutex mutex;
    std::condition_variable cond;
    queueType q;
    size_t queueCap;

public:
    char label;
    PriQueue(char l, size_t qSize) : queueCap(qSize), label(l) {};
    ~PriQueue();

    void push(messageType data, int pri);
    messageType pop();

    bool isEmpty() const;
    bool isFull() const;
    size_t size() const;

    void printQueue();

};

#endif // QUEUE_HPP_INCLUDED
