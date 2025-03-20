#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include<HuffmanNode.h>
#include<string>


#include<unordered_map> 

class HuffmanTree
{
private:

    HuffmanNode * root;
    std::unordered_map<char,std::string>huffmanCodes;

public:
    HuffmanTree();
    ~HuffmanTree();
    HuffmanNode * getRoot()const;
    void buildTree(const std::unordered_map<char,uint32_t> &frequencies);
    std::unordered_map<char ,std::string> getHuffmanCodes() const ;
    void generateHuffmanCodes();
    void printTree();


private:
    void printTreeInternal(HuffmanNode * root);
    void generateHuffmanCodesInternal(HuffmanNode *node,const  std::string& code);
};


#endif // HUFFMAN_TREE_H
