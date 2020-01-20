#ifndef SCHEDULER_OPERATOR_H
#define SCHEDULER_OPERATOR_H

#include "WorkOrder.h"


template <typename T>
class Operator {
protected:
    Sender<WorkOrder<T>*>* workOrderTx;
    Receiver<T*>* rx;
    Sender<T*>* tx;

public:
    virtual void pushWorkOrders() = 0;
};



template <typename T>
class TableReference: public Operator<T> {
public:
    TableReference(Sender<WorkOrder<T>*>* workOrderTx, Receiver<T*>* rx, Sender<T*>* tx) {
        this->workOrderTx = workOrderTx;
        this->rx = rx;
        this->tx = tx;
    }

    void pushWorkOrders() override {
        std::cout << "start TableReference pushWorkOrders" << std::endl;
        while (true) {
            std::optional<T*> block = this->rx->receive();
            if (block == std::nullopt) {
                break;
            }
            auto tableReferenceWorkOrder = new TableReferenceWorkOrder(block.value(), this->tx->clone());
            this->workOrderTx->send(tableReferenceWorkOrder);
        }
        this->tx->free();
        this->workOrderTx->free();
        std::cout << "finish TableReference pushWorkOrders" << std::endl;
    }
};


template <typename T>
class Select: public Operator<T> {
public:
    Select(Sender<WorkOrder<T>*>* workOrderTx, Receiver<T*>* rx, Sender<T*>* tx) {
        this->workOrderTx = workOrderTx;
        this->rx = rx;
        this->tx = tx;
    }

    void pushWorkOrders() override {
        std::cout << "start Select pushWorkOrders" << std::endl;
        while (true) {
            std::optional<T*> block = this->rx->receive();
            if (block == std::nullopt) {
                break;
            }
            auto selectWorkOrder = new SelectWorkOrder(block.value(), this->tx->clone());
            this->workOrderTx->send(selectWorkOrder);
        }
        this->tx->free();
        this->workOrderTx->free();
        std::cout << "finish Select pushWorkOrders" << std::endl;
    }
};


template <typename T>
class Project: public Operator<T> {
public:
    Project(Sender<WorkOrder<T>*>* workOrderTx, Receiver<T*>* rx, Sender<T*>* tx) {
        this->workOrderTx = workOrderTx;
        this->rx = rx;
        this->tx = tx;
    }

    void pushWorkOrders() override {
        std::cout << "start Project pushWorkOrders" << std::endl;
        while (true) {
            std::optional<T*> block = this->rx->receive();
            if (block == std::nullopt) {
                break;
            }
            auto projectWorkOrder = new ProjectWorkOrder(block.value(), this->tx->clone());
            this->workOrderTx->send(projectWorkOrder);
        }
        this->tx->free();
        this->workOrderTx->free();
        std::cout << "finish Project pushWorkOrders" << std::endl;
    }
};

#endif //SCHEDULER_OPERATOR_H




// #ifndef SCHEDULER_OPERATOR_H
// #define SCHEDULER_OPERATOR_H
//
// #include "WorkOrder.h"
// #include "Queue.h"
// #include "Block.h"
//
//
// class Operator {
// protected:
//     Sender<WorkOrder*>* workOrderTx;
//     Receiver<Block*>* rx;
//     Sender<Block*>* tx;
//
// public:
//     virtual void pushWorkOrders() = 0;
// };
//
//
//
// class TableReference: public Operator {
// public:
//     TableReference(Sender<WorkOrder*>* workOrderTx, Receiver<Block*>* rx, Sender<Block*>* tx) {
//         this->workOrderTx = workOrderTx;
//         this->rx = rx;
//         this->tx = tx;
//     }
//
//     void pushWorkOrders() override {
//         std::cout << "start TableReference pushWorkOrders" << std::endl;
//         while (true) {
//             std::optional<Block*> block = rx->receive();
//             if (block == std::nullopt) {
//                 break;
//             }
//             auto tableReferenceWorkOrder = new TableReferenceWorkOrder(block.value(), tx->clone());
//             workOrderTx->send(tableReferenceWorkOrder);
//         }
//         tx->free();
//         workOrderTx->free();
//         std::cout << "finish TableReference pushWorkOrders" << std::endl;
//     }
// };
//
//
//
// class Select: public Operator {
// public:
//     Select(Sender<WorkOrder*>* workOrderTx, Receiver<Block*>* rx, Sender<Block*>* tx) {
//         this->workOrderTx = workOrderTx;
//         this->rx = rx;
//         this->tx = tx;
//     }
//
//     void pushWorkOrders() override {
//         std::cout << "start Select pushWorkOrders" << std::endl;
//         while (true) {
//             std::optional<Block*> block = rx->receive();
//             if (block == std::nullopt) {
//                 break;
//             }
//             auto selectWorkOrder = new SelectWorkOrder(block.value(), tx->clone());
//             workOrderTx->send(selectWorkOrder);
//         }
//         tx->free();
//         workOrderTx->free();
//         std::cout << "finish Select pushWorkOrders" << std::endl;
//     }
// };
//
//
//
//
//
// class Project: public Operator {
// public:
//     Project(Sender<WorkOrder*>* workOrderTx, Receiver<Block*>* rx, Sender<Block*>* tx) {
//         this->workOrderTx = workOrderTx;
//         this->rx = rx;
//         this->tx = tx;
//     }
//
//     void pushWorkOrders() override {
//         std::cout << "start Project pushWorkOrders" << std::endl;
//         while (true) {
//             std::optional<Block*> block = rx->receive();
//             if (block == std::nullopt) {
//                 break;
//             }
//             auto projectWorkOrder = new ProjectWorkOrder(block.value(), tx->clone());
//             workOrderTx->send(projectWorkOrder);
//         }
//         tx->free();
//         workOrderTx->free();
//         std::cout << "finish Project pushWorkOrders" << std::endl;
//     }
// };
//
// #endif //SCHEDULER_OPERATOR_H


