#ifndef SCHEDULER_WORKER_H
#define SCHEDULER_WORKER_H

#include <thread>
#include "WorkOrder.h"

template <typename T>
class Worker {
private:
    int cpuId;
    std::thread t;
    Receiver<WorkOrder<T>*>* workOrderRx;

    void run() {
        while (true) {
            std::optional<WorkOrder<T>*> workOrder = workOrderRx->receive();
            if (workOrder == std::nullopt) {
                return;
            }
            workOrder.value()->execute();
        }
    }

public:
    explicit Worker(int cpuId, Receiver<WorkOrder<T>*>* rx) : cpuId(cpuId), workOrderRx(rx) {
        t = std::thread([this] {
            this->run();
        });

        // thread_affinity_policy_data_t policy = { cpuId };
        // thread_port_t mach_thread = pthread_mach_thread_np(t);
        // thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy, 1);
    }

    void join() {
        t.join();
    }
};


#endif //SCHEDULER_WORKER_H



