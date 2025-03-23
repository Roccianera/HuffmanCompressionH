#include "HuffmanNode.h"

HuffmanNode::HuffmanNode( unsigned char  characther, uint32_t frequency)
    : character(characther), frequency(frequency), left(nullptr), right(nullptr)
{
}

HuffmanNode::HuffmanNode(uint32_t frequency,std::shared_ptr<HuffmanNode> left, std::shared_ptr<HuffmanNode> right):
    character('\0'),frequency(frequency),left(left),right(right)    
{
}


bool HuffmanNode::isLeaf() const
{
    return !left && !right;
}
