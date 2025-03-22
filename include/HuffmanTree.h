#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <HuffmanNode.h>
#include <string>

#include <map>

class HuffmanTree
{
private:
    HuffmanNode *root;
    std::map<unsigned char, std::string> huffmanCodes;

public:
    HuffmanTree();
    ~HuffmanTree();
    HuffmanNode *getRoot() const;
    void buildTree(const std::map<unsigned char, uint32_t> &frequencies);
    std::map<unsigned char, std::string> getHuffmanCodes() const;
    void generateHuffmanCodes();
    void printTree();
    void printTree(const std::string &prefix, const HuffmanNode *node, bool isLeft);

private:
    void generateHuffmanCodesInternal(HuffmanNode *node, const std::string &code);
};

#endif // HUFFMAN_TREE_H
