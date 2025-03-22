#include "HuffmanTree.h"
#include <queue>
#include <iostream>

HuffmanTree::HuffmanTree() : root(nullptr)
{
}

HuffmanTree::~HuffmanTree()
{
    delete this->root;
}

HuffmanNode *HuffmanTree::getRoot() const
{
    return this->root;
}

void HuffmanTree::buildTree(const std::unordered_map< unsigned char , uint32_t> &frequencies)
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

   // printTree();
}

std::unordered_map< unsigned char , std::string> HuffmanTree::getHuffmanCodes() const
{

    return this->huffmanCodes;
}

void HuffmanTree::generateHuffmanCodes()
{
    generateHuffmanCodesInternal(this->root, "");
}

void HuffmanTree::printTree()
{

    printTree("", root, false);
}

void HuffmanTree::printTree(const std::string &prefix, const HuffmanNode *node, bool isLeft)
{
    if (!node)
        return;

    std::cout << prefix;
    std::cout << (isLeft ? "|--" : "\\--"); // Replace └── with \--

    std::cout << "(" << (node->isLeaf() ? node->character : 'F') << ':' << node->frequency << ") " << std::endl;

    // Also replace the Unicode characters in these lines
    printTree(prefix + (isLeft ? "|   " : "    "), node->left, true);
    printTree(prefix + (isLeft ? "|   " : "    "), node->right, false);
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
    if (!node)
        return;

    if (node->isLeaf())
    {
        this->huffmanCodes[node->character] = code.empty() ? "0" : code;
        return;
    }

    generateHuffmanCodesInternal(node->left, code + "0");
    generateHuffmanCodesInternal(node->right, code + "1");
}
