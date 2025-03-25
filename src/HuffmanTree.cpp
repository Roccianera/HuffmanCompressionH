#include "HuffmanTree.h"
#include <queue>
#include <iostream>

HuffmanTree::HuffmanTree() : root(nullptr)
{
}

std::shared_ptr<HuffmanNode> HuffmanTree::getRoot() const
{
    return this->root;
}

void HuffmanTree::buildTree(const std::map<char, uint32_t> &frequencies)
{

    std::priority_queue<std::shared_ptr<HuffmanNode>, std::vector<std::shared_ptr<HuffmanNode>>, HuffmanNode::CompareNodes> pq;

    for (const auto &pair : frequencies)
        pq.push(std::make_shared<HuffmanNode>(pair.first, pair.second));

    if (pq.size() == 0)
        return;

    if (pq.size() == 1)
    {

        auto node = pq.top();
        pq.pop();

        std::shared_ptr<HuffmanNode> dummyNode{std::make_shared<HuffmanNode>(node->frequency, nullptr, node)};

        this->root = dummyNode;
        return;
    }

    while (pq.size() > 1)
    {

        auto left = pq.top();
        pq.pop();
        auto right = pq.top();
        pq.pop();

        pq.push(std::make_shared<HuffmanNode>(right->frequency + left->frequency, left, right));
    }

    this->root = pq.top();
    pq.pop();
}

std::map<char, std::string> HuffmanTree::getHuffmanCodes() const
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

void HuffmanTree::printTree(const std::string &prefix, std::shared_ptr<HuffmanNode> node, bool isLeft)
{
    if (!node)
        return;

    std::cout << prefix;
    std::cout << (isLeft ? "|--" : "\\--");

    std::cout << "(" << (node->isLeaf() ? node->character : 'F') << ':' << node->frequency << ") " << std::endl;

    printTree(prefix + (isLeft ? "|   " : "    "), node->left, true);
    printTree(prefix + (isLeft ? "|   " : "    "), node->right, false);
}

void HuffmanTree::generateHuffmanCodesInternal(std::shared_ptr<HuffmanNode> node, const std::string &code)
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
