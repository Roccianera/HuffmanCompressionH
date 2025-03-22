#include"HuffmanTree.h"
#include<iostream>
#include"FileHandler.h"

int main()
{

/*
    FileHandler fileHandler;
    
    fileHandler.calcolatefrequencies("test/cose.txt");
    

    std::unordered_map<char, uint32_t> frequencyMap=fileHandler.getFrequencies();

    // Create a Huffman tree from the frequency map
    HuffmanTree huffmanTree{};
    huffmanTree.buildTree(frequencyMap);

    std::cout << "\nfrequencie codes:" << std::endl;
    for (const auto& pair : frequencyMap) {
        std::cout << "'" << pair.first << "': " << pair.second << std::endl;
    }  

    
    // huffmanTree.printTree();
    huffmanTree.generateHuffmanCodes();
    
    auto codes =huffmanTree.getHuffmanCodes();
    
    // Display the Huffman codes
    std::cout << "\nHuffman codes:" << std::endl;
    for (const auto& pair : codes) {
        std::cout << "'" << pair.first << "': " << pair.second << std::endl;
    }
    */
    FileHandler fileHandler;
    
    
    std::string inputFileName{"test/cose.txt"};
    std::string outputFileName{"test/cose.huffman"};
    
    
    
    fileHandler.compress(inputFileName,outputFileName);
    
    




    FileHandler fileHandler2;
    std::string inputFileName2{"test/cose.huffman"};
    std::string outputFileName2{"test/coseCopy.txt"};



    fileHandler2.deCompress(inputFileName2,outputFileName2);

    
    return 0;
}
