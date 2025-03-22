
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "HuffmanNode.h"
#include "HuffmanTree.h"

class FileHandler
{
private:
    std::unordered_map<unsigned char, uint32_t> frequencies;
    HuffmanTree tree;

public:
    FileHandler();

    void calcolatefrequencies(const std::string &inputFileName);

    void compress(std::string &inputFileName, std::string &outputFileName);
    void deCompress(std::string &inputFileName, std::string &outputFileName);

    void serializeTree(std::ofstream &file);
    void deSerializeTree(std::ifstream &file);
    std::unordered_map<unsigned char, uint32_t> getFrequencies() const;

    void printStatics(std::string &inputFileName, std::string &outputFileName);
};

#endif
