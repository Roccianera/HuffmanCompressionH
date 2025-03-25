

#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H

#include <cstdint>
#include <memory>

class HuffmanNode
{
public:
    char character;
    uint32_t frequency;

    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(char characther, uint32_t frequency);
    HuffmanNode(uint32_t frequency, std::shared_ptr<HuffmanNode> left, std::shared_ptr<HuffmanNode> right);

    bool isLeaf() const;

    struct CompareNodes
    {

        bool operator()(const std::shared_ptr<HuffmanNode> &lhs, const std::shared_ptr<HuffmanNode> &rhs) { return lhs->frequency > rhs->frequency; }
    };
};

#endif
