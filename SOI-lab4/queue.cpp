#include <string>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctime>
#include <cstdlib>
#include "pri_queue.h"

PriQueue::~PriQueue(){
    std::unique_lock<std::mutex> lock(mutex);
    while(!isEmpty())
        q.pop_back();

    lock.unlock();
    cond.notify_all();
}

void PriQueue::push(messageType data, int pri){
    std::unique_lock<std::mutex> lock(mutex);
    cond.wait(lock, [this]() { return !isFull(); });

    if(isEmpty()){
        q.push_back(data);
    }
    else if(pri == NORM){
        q.push_back(data);
    }
    else if(pri == HIGH){
        if(q[0].first == HIGH){
            queueType::iterator i = q.begin() + 1;
            while((*i).first == HIGH)
                ++i;
            q.insert(i, data);
        } else
            q.insert(q.begin(), data);
    }
    else
        q.insert(q.begin(), data);
    printQueue();

    lock.unlock();
    cond.notify_all();
}

messageType PriQueue::pop(){
    std::unique_lock<std::mutex> lock(mutex);
    cond.wait(lock, [this]() { return !isEmpty(); });

    printQueue();
    messageType msg = q[0];
    q.erase(q.begin());

    lock.unlock();
    cond.notify_all();

    return msg;
}

bool PriQueue::isEmpty() const{
    return q.size() == 0;
}

bool PriQueue::isFull() const{
    return (q.size() == queueCap);
}

size_t PriQueue::size() const{
    return q.size();
}

void PriQueue::printQueue(){
    if(isEmpty())
        std::cout << "Kolejka jest pusta!" << std::endl;
    else{
        queueType::iterator i = q.begin(), e = q.end();
        while(i != e){
            std::cout << (*i).first << " : " << (*i).second << " <- ";
            i++;
        }
        std::cout << std::endl;
    }
}
