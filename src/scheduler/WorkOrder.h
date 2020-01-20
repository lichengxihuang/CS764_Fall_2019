#ifndef SCHEDULER_WORKORDER_H
#define SCHEDULER_WORKORDER_H

#include "Queue.h"


template <typename T>
class WorkOrder {
protected:
    T* block;
    Sender<T*>* tx;

public:
    virtual void execute() = 0;
};



template <typename T>
class TableReferenceWorkOrder: public WorkOrder<T> {
public:
    TableReferenceWorkOrder(T* block, Sender<T*>* tx) {
        this->block = block;
        this->tx = tx;
    }

    void execute() override {
        this->tx->send(this->block);
        this->tx->free();
        std::cout << "TableReferenceWorkOrder execute:\t" << this->block->toString() << std::endl;
    }
};

template <typename T>
class SelectWorkOrder: public WorkOrder<T> {
public:
    SelectWorkOrder(T* block, Sender<T*>* tx) {
        this->block = block;
        this->tx = tx;
    }

    void execute() override {
        this->tx->send(this->block);
        this->tx->free();
        std::cout << "SelectWorkOrder execute:\t" << this->block->toString() << std::endl;
    }
};


template <typename T>
class ProjectWorkOrder: public WorkOrder<T> {
public:
    ProjectWorkOrder(T* block, Sender<T*>* tx) {
        this->block = block;
        this->tx = tx;
    }

    void execute() override {
        this->tx->send(this->block);
        this->tx->free();
        std::cout << "ProjectWorkOrder execute:\t" << this->block->toString() << std::endl;
    }
};




#endif //SCHEDULER_WORKORDER_H




// #ifndef SCHEDULER_WORKORDER_H
// #define SCHEDULER_WORKORDER_H
//
// #include "Block.h"
// #include "Queue.h"
//
//
// class WorkOrder {
// protected:
//     T* block;
//     Sender<T*>* tx;
//
// public:
//     virtual void execute() = 0;
// };
//
//
//
// class TableReferenceWorkOrder: public WorkOrder {
// public:
//     TableReferenceWorkOrder(T* block, Sender<T*>* tx) {
//         this->block = block;
//         this->tx = tx;
//     }
//
//     void execute() override {
//         tx->send(block);
//         tx->free();
//         std::cout << "TableReferenceWorkOrder execute:\t" << block->toString() << std::endl;
//     }
// };
//
//
// class SelectWorkOrder: public WorkOrder {
// public:
//     SelectWorkOrder(T* block, Sender<T*>* tx) {
//         this->block = block;
//         this->tx = tx;
//     }
//
//     void execute() override {
//         tx->send(block);
//         tx->free();
//         std::cout << "SelectWorkOrder execute:\t" << block->toString() << std::endl;
//     }
// };
//
//
// class ProjectWorkOrder: public WorkOrder {
// public:
//     ProjectWorkOrder(T* block, Sender<T*>* tx) {
//         this->block = block;
//         this->tx = tx;
//     }
//
//     void execute() override {
//         tx->send(block);
//         tx->free();
//         std::cout << "ProjectWorkOrder execute:\t" << block->toString() << std::endl;
//     }
// };
//
//
//
// #endif //SCHEDULER_WORKORDER_H



