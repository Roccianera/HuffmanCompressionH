#include "HuffmanTree.h"
#include <queue>
#include <iostream>

HuffmanTree::HuffmanTree():root(nullptr)
{

   
}

HuffmanTree::~HuffmanTree()
{
    delete this->root;
}

void HuffmanTree::buildTree(const std::unordered_map<char, uint32_t> &frequencies)
{

    std::priority_queue<HuffmanNode *, std::vector<HuffmanNode *>, HuffmanNode::CompareNodes> pq;

    for (const auto &pair : frequencies)
        pq.push(new HuffmanNode(pair.first, pair.second));

    if (pq.size() == 0)
        return;

    while (pq.size() > 1)
    {

        auto left = pq.top();
        pq.pop();
        auto right = pq.top();
        pq.pop();

        pq.push(new HuffmanNode(right->frequency + left->frequency, left, right));
    }

    this->root = pq.top();
    pq.pop();
}

std::unordered_map<char, std::string> HuffmanTree::getHuffmanCodes() const
{

    return this->huffmanCodes;
}

void HuffmanTree::generateHuffmanCodes()
{
    generateHuffmanCodesInternal(this->root,"");
}

void HuffmanTree::printTree()
{

    printTreeInternal(this->root);
}

void HuffmanTree::printTreeInternal(HuffmanNode *node)
{
    if (!node)
        return;

    if (node->isLeaf())
    {

        std::cout << "Leaf: '" << node->character << "' with frequency: " << node->frequency << std::endl;
    }
    else
    {
        std::cout << "Internal node with frequency: " << node->frequency << std::endl;
    }

    printTreeInternal(node->left);
    printTreeInternal(node->right);
}

void HuffmanTree::generateHuffmanCodesInternal(HuffmanNode *node, const std::string &code)
{   
    if (!node) return ;

    if (node->isLeaf()){
        this->huffmanCodes[node->character] = code.empty()?"0":code;
        return ;
    }

    generateHuffmanCodesInternal(node->left, code + "0");
    generateHuffmanCodesInternal(node->right, code + "1");
    
}
