#include "HuffmanNode.h"

HuffmanNode::HuffmanNode(char characther, uint32_t frequency)
    : character(characther), frequency(frequency), left(nullptr), right(nullptr)
{
}

HuffmanNode::HuffmanNode(uint32_t frequency,HuffmanNode *left, HuffmanNode *right):
    character('\0'),frequency(frequency),left(left),right(right)    
{
}

HuffmanNode::~HuffmanNode()
{
    delete left;
    delete right;
}

bool HuffmanNode::isLeaf() const
{
    return !left && !right;
}
