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

    serializeTree(outputFileName);

    


}

void FileHandler::deCompress(std::string &inputFileName, std::string &outputFileName)
{

    deSerializeTree(inputFileName);

    HuffmanTree tree{};

    tree.buildTree(this->frequencies);

    tree.generateHuffmanCodes();



}

void FileHandler::serializeTree(std::string &outputFileName)
{

    std::ofstream file(outputFileName, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "file not opened" << std::endl;
        throw std::runtime_error("File not opened");
    }

    size_t mapSize{frequencies.size()};

    file.write(reinterpret_cast<char*>(&mapSize),sizeof(mapSize));


    for (const auto &[charcater, freq] : this->frequencies)
    {
        char charCopy = charcater;
        uint32_t freqCopy = freq;

        file.write(reinterpret_cast<char *>(&charCopy), sizeof(char));
        file.write(reinterpret_cast<char *>(&freqCopy), sizeof(uint32_t));
    }

    file.close();

}

void FileHandler::deSerializeTree(std::string &inputFileName)
{
    std::ifstream file(inputFileName, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "file not opened" << std::endl;
        throw std::runtime_error("File not opened");
    }

    size_t mapSize;

    file.read(reinterpret_cast<char*>(&mapSize),sizeof(mapSize));

    for (size_t i = 0; i < mapSize; i++)
    {


        char characater;
        uint32_t frequency;

        file.read(reinterpret_cast<char*> (characater),sizeof(char));
        file.read(reinterpret_cast<char*> (frequency),sizeof(uint32_t));
        
        this->frequencies[characater]=frequency;
    }


    file.close();


    tree.buildTree(frequencies);
    tree.generateHuffmanCodes();



}

std::unordered_map<char, uint32_t> FileHandler::getFrequencies() const
{
    return this->frequencies;
}
