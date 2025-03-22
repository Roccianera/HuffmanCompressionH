#include "FileHandler.h"
#include <exception>
#include <fstream>
#include <iostream>

FileHandler::FileHandler()
{
}

void FileHandler::calcolatefrequencies(const std::string &inputFileName)
{
    // TODO: insert return statement here

    this->frequencies.clear();

    std::ifstream file(inputFileName, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "file not opened" << std::endl;
        throw std::runtime_error("File not opened");
    }

    unsigned char  car;
    while (file.read(reinterpret_cast<char *>(&car), sizeof(car)))
    {

        frequencies[car]++;
    }

    
}

void FileHandler::compress(std::string &inputFileName, std::string &outputFileName)
{

    this->calcolatefrequencies(inputFileName);

    HuffmanTree tree{};

    tree.buildTree(this->frequencies);

    tree.generateHuffmanCodes();

    std::unordered_map< unsigned char , std::string> codes = tree.getHuffmanCodes();

    std::ifstream iFile(inputFileName, std::ios::binary);

    std::ofstream oFile(outputFileName, std::ios::binary);
    if (!oFile.is_open() || !iFile.is_open())
    {
        std::cerr << "file not opened" << std::endl;
        throw std::runtime_error("oFile not opened");
    }

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
                oFile.write(&currentByte, 1);
                currentByte = 0;
                bitCount = 0;
            }
        }
    }

    if (bitCount != 0)
    {

        currentByte <<= (8 - bitCount);
        oFile.write(&currentByte, 1);
    }

    uint8_t padding = 8 - bitCount;

    oFile.write(reinterpret_cast<char *>(&padding), sizeof(uint8_t));

    iFile.close();
    oFile.close();

    printStatics(inputFileName, outputFileName);
}

void FileHandler::deCompress(std::string &inputFileName, std::string &outputFileName)
{
    this->frequencies.clear();
    std::cerr << "Starting decompression from " << inputFileName << " to " << outputFileName << std::endl;

    std::ifstream iFile(inputFileName, std::ios::binary);
    std::ofstream oFile(outputFileName, std::ios::binary);
    
    if (!oFile.is_open() || !iFile.is_open())
    {
        std::cerr << "Error: Failed to open files for decompression" << std::endl;
        throw std::runtime_error("File not opened");
    }

     std::cerr << "Deserializing frequency tree..." << std::endl;
    deSerializeTree(iFile);
    
    if (frequencies.empty()) {
        std::cerr << "Error: Frequency map is empty after deserialization" << std::endl;
        throw std::runtime_error("Invalid frequency data");
    }

    HuffmanTree tree{};
    std::cerr << "Building Huffman tree..." << std::endl;
    tree.buildTree(this->frequencies);

    std::cerr << "Generating Huffman codes..." << std::endl;
    tree.generateHuffmanCodes();
 
    iFile.seekg(0, std::ios::end);
    long fileSize = iFile.tellg();
    std::cerr << "Input file size: " << fileSize << " bytes" << std::endl;

    iFile.seekg(-1, std::ios::end);
    uint8_t paddingBits;
    iFile.read(reinterpret_cast<char *>(&paddingBits), sizeof(uint8_t));
    std::cerr << "Padding bits: " << static_cast<int>(paddingBits) << std::endl;

    long dataStart = sizeof(size_t) + frequencies.size() * (sizeof(uint32_t) + sizeof(char));
    std::cerr << "Data starts at byte: " << dataStart << std::endl;

    iFile.seekg(dataStart, std::ios::beg);

    HuffmanNode *root = tree.getRoot();
    if (!root) {
        std::cerr << "Error: Huffman tree root is null" << std::endl;
        throw std::runtime_error("Invalid Huffman tree");
    }

    auto current = root;
    long byteProcessed = 0;
    char byte = 0;
    long compressedDataSize = fileSize - dataStart - 1;
    std::cerr << "Compressed data size: " << compressedDataSize << " bytes" << std::endl;

    while (iFile.read(&byte, 1))
    {
        byteProcessed++;
        int bitsToProcess = 8;

        if (byteProcessed == compressedDataSize)
        {
            bitsToProcess = 8 - paddingBits;
            std::cerr << "Processing final byte with " << bitsToProcess << " bits" << std::endl;
        }

        for (size_t i = 0; i < bitsToProcess; i++)
        {
            bool bit = (byte & (1 << (7 - i))) != 0;
            if (bit)
            {
                if (!current->right) {
                    std::cerr << "Error: Null right child in Huffman tree" << std::endl;
                    throw std::runtime_error("Corrupted Huffman tree");
                }
                current = current->right;
            }
            else
            {
                if (!current->left) {
                    std::cerr << "Error: Null left child in Huffman tree" << std::endl;
                    throw std::runtime_error("Corrupted Huffman tree");
                }
                current = current->left;
            }

            if (current->isLeaf())
            {
                oFile.write(reinterpret_cast<char *>(&current->character), sizeof(current->character));
                current = root;
            }
        }
    }

    std::cerr << "Decompression complete. Processed " << byteProcessed << " bytes." << std::endl;
    iFile.close();
    oFile.close();

    std::cerr << "Printing statistics..." << std::endl;
    
   // printStatics(inputFileName, outputFileName);
}

void FileHandler::serializeTree(std::ofstream &file)
{
    
    size_t mapSize{frequencies.size()};

    file.write(reinterpret_cast<char *>(&mapSize), sizeof(mapSize));

    for (const auto &[charcater, freq] : this->frequencies)
    {
        char charCopy = charcater;
        uint32_t freqCopy = freq;
        
      

        file.write(reinterpret_cast<char *>(&charCopy), sizeof(char));
        file.write(reinterpret_cast<char *>(&freqCopy), sizeof(uint32_t));
    }

 
}

void FileHandler::deSerializeTree(std::ifstream &file)
{
    
    size_t mapSize;
    file.read(reinterpret_cast<char *>(&mapSize), sizeof(mapSize));

    for (size_t i = 0; i < mapSize; i++)
    {
        char character;
        uint32_t frequency;

        file.read(reinterpret_cast<char *>(&character), sizeof(char));
        file.read(reinterpret_cast<char *>(&frequency), sizeof(uint32_t));

        this->frequencies[character] = frequency;
       
    }

    
}

std::unordered_map<unsigned char, uint32_t> FileHandler::getFrequencies() const
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
