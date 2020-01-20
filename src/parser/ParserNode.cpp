#include "ParserNode.h"


ParserNode::ParserNode(int id, std::string detail) : id(id), detail(std::move(detail)), next(), child() {}

ParserNode *ParserNode::findNode(int nodeId) {
    // if found
    if(this->id == nodeId) {
        return this;
    }

    ParserNode *res = nullptr;

    // search child
    if(this->hasChild()) {
        res = this->child->findNode(nodeId);
    }

    if(res != nullptr) {
        return res;
    }

    // search next
    if(this->hasNext()) {
        res = this->next->findNode(nodeId);
    }

    return res;
}

void ParserNode::addChild(ParserNode *node) {
    if (this->child == nullptr) {
        this->child = node;
        return;
    }

    ParserNode *curr = this->child;
    while(curr->hasNext()) {
        curr = curr->next;
    }

    curr->next = node;
}

bool ParserNode::hasNext() {
    return this->next != nullptr;
}

bool ParserNode::hasChild() {
    return this->child != nullptr;
}

std::string ParserNode::toString(int indent) {
    std::string str = std::string(indent, ' ') + this->detail + '\n';
    if (this->hasChild()) {
        str += this->child->toString(indent + 4);
    }

    if (this->hasNext()) {
        str += this->next->toString(indent);
    }

    return str;
}
