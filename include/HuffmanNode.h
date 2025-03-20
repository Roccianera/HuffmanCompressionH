

#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H

#include <cstdint>

class HuffmanNode
{
public:
    char character;
    uint32_t frequency;

    HuffmanNode *left;
    HuffmanNode *right;

    HuffmanNode(char characther, uint32_t frequency );
    HuffmanNode(uint32_t frequency ,HuffmanNode *left, HuffmanNode* right );
    ~HuffmanNode();
    bool isLeaf() const;

    struct CompareNodes
    {

        bool operator()(const HuffmanNode *lhs, const HuffmanNode *rhs) { return lhs->frequency > rhs->frequency; }
    };

};

#endif
