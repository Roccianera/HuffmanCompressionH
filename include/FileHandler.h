
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include"HuffmanNode.h"
#include "HuffmanTree.h"

class FileHandler
{
private:

    std::unordered_map<char,uint32_t> frequencies;
    HuffmanTree tree;

    

public:
    FileHandler();

    void calcolatefrequencies(const std::string& inputFileName );

    void compress(std::string &inputFileName, std::string &outputFileName);
    void deCompress(std::string &inputFileName, std::string &outputFileName);


    void serializeTree(std::string&outputFileName);
    void deSerializeTree(std::string&inputFileName);
    std::unordered_map<char,uint32_t> getFrequencies() const;

    



};

#endif
