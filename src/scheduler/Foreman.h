#ifndef SCHEDULER_FOREMAN_H
#define SCHEDULER_FOREMAN_H

#include "Worker.h"

template <typename T>
class Foreman {
private:
    unsigned nWorkers;
    unsigned workerIndex;
    Receiver<WorkOrder<T>*>* rx;
    std::vector<Sender<WorkOrder<T>*>*> workerTxs;
    std::vector<Worker<T>*> workers;
    std::thread t;

    void run() {
        while (true) {
            std::optional<WorkOrder<T>*> workOrder = rx->receive();
            if (workOrder == std::nullopt) {
                for (Sender<WorkOrder<T>*>* tx : workerTxs) {
                    tx->free();
                }
                return;
            }
            // Sender<WorkOrder<T>*>* currTx = workerTxs.at(0);
            workerTxs.at(getNextWorkerIndex())->send(workOrder.value());
        }
    }

    int getNextWorkerIndex() {
        workerIndex = (workerIndex + 1) % nWorkers;
        return workerIndex;
    }

public:
    explicit Foreman(Receiver<WorkOrder<T>*>* rx, unsigned nWorkers) : nWorkers(nWorkers), workerIndex(nWorkers-1), rx(rx) {
        for (unsigned i = 0; i < nWorkers; i++) {
            auto[workerTx, workerRx] = channel<WorkOrder<T>>();
            workerTxs.push_back(workerTx);

            auto worker = new Worker(i, workerRx);
            workers.push_back(worker);
        }

        t = std::thread([this] {
            this->run();
        });
    }

    void join() {
        for (Worker<T>* worker : workers) {
            worker->join();
        }
        t.join();
    }
};


#endif //SCHEDULER_FOREMAN_H
