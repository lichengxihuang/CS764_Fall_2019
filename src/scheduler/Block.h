#ifndef SCHEDULER_BLOCK_H
#define SCHEDULER_BLOCK_H

#include <string>

class Block {
private:
    int blockId;

public:
    Block(int id) : blockId(id) {}

    int getBlockId() {
        return blockId;
    }
    std::string toString() {
        return "BlockID = " + std::to_string(blockId);
    }
};


#endif //SCHEDULER_BLOCK_H
