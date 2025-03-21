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

    char car;
    while (file.read(reinterpret_cast<char *>(&car), sizeof(car)))
    {

        frequencies[car]++;
    }

    file.close();
}

void FileHandler::compress(std::string &inputFileName, std::string &outputFileName)
{

    this->calcolatefrequencies(inputFileName);

    HuffmanTree tree{};

    tree.buildTree(this->frequencies);

    tree.generateHuffmanCodes();

    std::unordered_map<char, std::string> codes = tree.getHuffmanCodes();

    
    std::ifstream iFile(inputFileName, std::ios::binary);
    
    std::ofstream oFile(outputFileName, std::ios::binary);
    if (!oFile.is_open()|| !iFile.is_open())
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

            currentByte=(currentByte<<1)|(bit=='0'?0:1);
            bitCount++;

            if(bitCount==8){
                oFile.write(&currentByte,1);
                currentByte=0;
                bitCount=0;

            }
        }
    }



    if (bitCount!=0)
    {
        
        currentByte<<=(8-bitCount);
        oFile.write(&currentByte,1);
    }

    uint8_t padding =8-bitCount;

    oFile.write(reinterpret_cast<char*>(&padding),sizeof(uint8_t));
    



    iFile.close();
    oFile.close();

    

    printStatics(inputFileName,outputFileName);






}

void FileHandler::deCompress(std::string &inputFileName, std::string &outputFileName)
{

    
    
    std::ifstream iFile(inputFileName, std::ios::binary);
    
    std::ofstream oFile(outputFileName, std::ios::binary);
    if (!oFile.is_open()|| !iFile.is_open())
    {
        std::cerr << "file not opened" << std::endl;
        throw std::runtime_error("oFile not opened");
    }
    
    
    deSerializeTree(iFile);
    
    
    HuffmanTree tree{};
    
    tree.buildTree(this->frequencies);

    tree.generateHuffmanCodes();


    








    
}

void FileHandler::serializeTree(std::ofstream& file )
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

    file.close();
}

void FileHandler::deSerializeTree(std::ifstream&file)
{


    size_t mapSize;

    file.read(reinterpret_cast<char *>(&mapSize), sizeof(mapSize));

    for (size_t i = 0; i < mapSize; i++)
    {

        char characater;
        uint32_t frequency;

        file.read(reinterpret_cast<char *>(characater), sizeof(char));
        file.read(reinterpret_cast<char *>(frequency), sizeof(uint32_t));

        this->frequencies[characater] = frequency;
    }

    file.close();


}

std::unordered_map<char, uint32_t> FileHandler::getFrequencies() const
{
    return this->frequencies;
}

void FileHandler::printStatics(std::string &inputFileName, std::string &outputFileName)
{


        std::ifstream inFile(inputFileName, std::ios::binary | std::ios::ate);
        std::ifstream outFile(outputFileName, std::ios::binary | std::ios::ate);
        
        if (!inFile || !outFile) {
            std::cerr << "File not opened  \n";
            throw std::runtime_error("File not opened");
        }
        
        std::streamsize inputSize = inFile.tellg();
        std::streamsize outputSize = outFile.tellg();
        
        std::cout << " Original Size:  " << inputSize << " bytes\n";
        std::cout << " Compressed Size  " << outputSize << " bytes\n";
        
        if (inputSize > 0) {
            double ratio = 100.0 - (static_cast<double>(outputSize) / inputSize * 100.0);
            std::cout << "Ratio of Compression : " << ratio << "% (" 
                     << static_cast<double>(inputSize) / outputSize << ":1)\n";
        }
        
  
    }




