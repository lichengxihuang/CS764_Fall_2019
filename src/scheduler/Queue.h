#ifndef SCHEDULER_QUEUE_H
#define SCHEDULER_QUEUE_H


#include <iostream>
#include <queue>
#include <tuple>
#include <optional>

template <typename T> class Queue;
template <typename T> class Sender;
template <typename T> class Receiver;
template <typename T> std::pair<Sender<T*>*, Receiver<T*>*> channel();


template <typename T>
class Queue {
private:
    friend class Sender<T>;
    friend class Receiver<T>;

    unsigned numSender;
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;

    // Add an element to the queue.
    void enqueue(T t) {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_one();
    }

    // Get the "front"-element.
    // If the queue is empty, wait till a element is avaiable.
    std::optional<T> dequeue() {
        std::unique_lock<std::mutex> lock(m);
        while(q.empty())
        {
            if (numSender == 0) {
                return std::nullopt;
            }
            // release lock as long as the wait and reaquire it afterwards.
            c.wait(lock);
        }
        T val = q.front();
        q.pop();
        return val;
    }

public:
    Queue(): numSender(), q(), m(), c() {}

    ~Queue() = default;

    // bool isEmpty() {
    //     std::unique_lock<std::mutex> lock(m);
    //     return q.empty();
    // }

    void incNumSender() {
        std::lock_guard<std::mutex> lock(m);
        numSender++;
    }

    void decNumSender() {
        std::lock_guard<std::mutex> lock(m);
        numSender--;
        if (numSender == 0) {
            c.notify_one();
        }
    }
};


template <typename T>
class Sender {
public:
    explicit Sender(Queue<T>* tx): tx(tx) {
        this->tx->incNumSender();
    }

    ~Sender() = default;

    void free() {
        tx->decNumSender();
    }

    Sender<T>* clone() {
        tx->incNumSender();
        return this;
    }

    void send(T val) {
        return tx->enqueue(val);
    }

private:
    Queue<T>* tx;
};



template <typename T>
class Receiver {
public:
    explicit Receiver(Queue<T>* rx): rx(rx) {}

    ~Receiver() = default;

    std::optional<T> receive() {
        return rx->dequeue();
    }

private:
    Queue<T>* rx;
};

//
// template <class T> std::pair<Sender<T>*, Receiver<T>*> channel() {
//     auto queue = new Queue<T>();
//     auto sender = new Sender<T>(queue);
//     auto receiver = new Receiver<T>(queue);
//     return std::make_pair(sender, receiver);
// }

template <typename T>
std::pair<Sender<T*>*, Receiver<T*>*> channel() {
    auto queue = new Queue<T*>();
    auto sender = new Sender<T*>(queue);
    auto receiver = new Receiver<T*>(queue);
    return std::make_pair(sender, receiver);
}




#endif //SCHEDULER_QUEUE_H






