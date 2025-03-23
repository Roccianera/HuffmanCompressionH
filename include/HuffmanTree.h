#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <HuffmanNode.h>
#include <string>

#include <map>

class HuffmanTree
{
private:
    std::shared_ptr<HuffmanNode> root;
    std::map<unsigned char, std::string> huffmanCodes;

public:
    HuffmanTree();
    std::shared_ptr<HuffmanNode> getRoot() const;
    void buildTree(const std::map<unsigned char, uint32_t> &frequencies);
    std::map<unsigned char, std::string> getHuffmanCodes() const;
    void generateHuffmanCodes();
    void printTree();
    void printTree(const std::string &prefix, const std::shared_ptr<HuffmanNode> node, bool isLeft);

private:
    void generateHuffmanCodesInternal(std::shared_ptr<HuffmanNode> node, const std::string &code);
};

#endif // HUFFMAN_TREE_H
