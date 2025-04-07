#include "FileHandler.h"
#include <exception>
#include <fstream>
#include <iostream>

FileHandler::FileHandler()
{
}

void FileHandler::calcolatefrequencies(const std::string &inputFileName)
{

    
    this->frequencies.clear();

    std::ifstream file(inputFileName, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "file not opened" << std::endl;
        throw std::runtime_error("File not opened");
    }

    char car;
    while (file.read(reinterpret_cast<char *>(&car), 1))
    {

        frequencies[car]++;
    }

    file.close();
}

void FileHandler::compress(std::string &inputFileName, std::string &outputFileName)
{

    this->calcolatefrequencies(inputFileName);
    std::ifstream iFile(inputFileName, std::ios::binary);

    std::ofstream oFile(outputFileName, std::ios::binary);
    if (!oFile.is_open() || !iFile.is_open())
    {
        std::cerr << "file not opened" << std::endl;
        throw std::runtime_error("oFile not opened");
    }
    if (frequencies.size() == 0)
    {
        oFile.close();
        iFile.close();
        return;
    }

    HuffmanTree tree{};

    tree.buildTree(this->frequencies);

    tree.generateHuffmanCodes();

    std::map<char, std::string> codes = tree.getHuffmanCodes();

    serializeTree(oFile);

    char currentByte{0};
    int bitCount{0};

    char byte;
    while (iFile.get(byte))
    {

        std::string code = codes[byte];

        for (char bit : code)
        {

            currentByte = (currentByte << 1) | (bit == '0' ? 0 : 1);
            bitCount++;

            if (bitCount == 8)
            {
                oFile.write(reinterpret_cast<char *>(&currentByte), sizeof(currentByte));
                currentByte = 0;
                bitCount = 0;
            }
        }
    }

    std::cerr<<"Writing remaining bits..."<<std::endl;
    std::cerr<<"Bit count: "<<bitCount<<std::endl;

    uint8_t padding{0};

    if (bitCount != 0)
    {

        currentByte <<= (8 - bitCount);
        padding = 8 - bitCount;
        oFile.write(reinterpret_cast<char *>(&currentByte), sizeof(currentByte));
    }



    oFile.write(reinterpret_cast<char *>(&padding), sizeof(uint8_t));

    iFile.close();
    oFile.close();
}

void FileHandler::deCompress(std::string &inputFileName, std::string &outputFileName)
{
    this->frequencies.clear();

    std::ifstream iFile(inputFileName, std::ios::binary);
    std::ofstream oFile(outputFileName, std::ios::binary);

    if (!oFile.is_open() || !iFile.is_open())
    {
        std::cerr << "Error: Failed to open files for decompression" << std::endl;
        throw std::runtime_error("File not opened");
    }

    deSerializeTree(iFile);

    if (frequencies.empty())
    {
        iFile.close();
        oFile.close();
        return;
    }

    HuffmanTree tree{};
    tree.buildTree(this->frequencies);
    tree.generateHuffmanCodes();
    auto codes = tree.getHuffmanCodes();

    iFile.seekg(0, std::ios::end);
    long fileSize = iFile.tellg();
    iFile.seekg(-1, std::ios::end);
    uint8_t paddingBits;
    iFile.read(reinterpret_cast<char *>(&paddingBits), sizeof(uint8_t));

    long dataStart = sizeof(uint16_t) + frequencies.size() * (sizeof(uint32_t) + sizeof(char));
    iFile.seekg(dataStart, std::ios::beg);

    std::shared_ptr<HuffmanNode> root = tree.getRoot();
    auto current = root;
    long byteProcessed = 0;
    char byte = 0;
    long compressedDataSize = fileSize - dataStart - 1;

    for (long i = 0; i < compressedDataSize - 1; i++)
    {
        iFile.read(reinterpret_cast<char *>(&byte), 1);
        byteProcessed++;

        for (int i = 0; i < 8; i++)
        {
            bool bit = (byte & (1 << (7 - i))) != 0;
            current = bit ? current->right : current->left;

            if (current->isLeaf())
            {
                oFile.write(reinterpret_cast<char *>(&current->character), 1);
                current = root;
            }
        }
    }

    // Process the last byte with padding
    if (compressedDataSize > 0)
    {
        iFile.read(reinterpret_cast<char *>(&byte), 1);
        int validBits = 8 - paddingBits;  // Calculate the number of valid bits
        
        for (int i = 0; i < validBits; i++)
        {
            bool bit = (byte & (1 << (7 - i))) != 0;
            current = bit ? current->right : current->left;

            if (current->isLeaf())
            {
                oFile.write(reinterpret_cast<char *>(&current->character), 1);
                current = root;
            }
        }
    }

    iFile.close();
    oFile.close();

    std::cerr << "Printing statistics..." << std::endl;
}

void FileHandler::serializeTree(std::ofstream &file)
{

    uint16_t mapSize{frequencies.size()};

    file.write(reinterpret_cast<char *>(&mapSize), sizeof(mapSize));

    for (const auto &[charcater, freq] : this->frequencies)
    {
        char charCopy = charcater;
        uint32_t freqCopy = freq;

        file.write(reinterpret_cast<char *>(&charCopy), sizeof(charCopy));
        file.write(reinterpret_cast<char *>(&freqCopy), sizeof(uint32_t));
    }
}

void FileHandler::deSerializeTree(std::ifstream &file)
{

    uint16_t mapSize;
    file.read(reinterpret_cast<char *>(&mapSize), sizeof(mapSize));

    for (size_t i = 0; i < mapSize; i++)
    {
        char character;
        uint32_t frequency;

        file.read(reinterpret_cast<char *>(&character), sizeof(character));
        file.read(reinterpret_cast<char *>(&frequency), sizeof(uint32_t));

        this->frequencies[character] = frequency;
    }
}

std::map<char, uint32_t> FileHandler::getFrequencies() const
{
    return this->frequencies;
}

void FileHandler::printStatics(std::string &inputFileName, std::string &outputFileName)
{

    std::ifstream inFile(inputFileName, std::ios::binary | std::ios::ate);
    std::ifstream outFile(outputFileName, std::ios::binary | std::ios::ate);

    if (!inFile || !outFile)
    {
        std::cerr << "File not opened  \n";
        throw std::runtime_error("File not opened");
    }

    std::streamsize inputSize = inFile.tellg();
    std::streamsize outputSize = outFile.tellg();

    std::cout << " Original Size:  " << inputSize << " bytes\n";
    std::cout << " Compressed Size  " << outputSize << " bytes\n";

    if (inputSize > 0)
    {
        double ratio = 100.0 - (static_cast<double>(outputSize) / inputSize * 100.0);
        std::cout << "Ratio of Compression : " << ratio << "% ("
                  << static_cast<double>(inputSize) / outputSize << ":1)\n";
    }
}

void FileHandler::writeBits(std::ifstream &iFile, std::ofstream &oFile)
{
}
