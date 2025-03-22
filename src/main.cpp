
#include <iostream>
#include "FileHandler.h"

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout << "Usage: " << argv[0] << " <mode> <input_file> <output_file>" << std::endl;
        std::cout << "Mode: 'c' for compression, 'd' for decompression" << std::endl;
        return 1;
    }

    FileHandler fileHandler;
    char mode = argv[1][0];
    std::string inputFileName = argv[2];
    std::string outputFileName = argv[3];

    if (mode == 'c')
    {
        fileHandler.compress(inputFileName, outputFileName);
    }
    else if (mode == 'd')
    {
        fileHandler.deCompress(inputFileName, outputFileName);
    }
    else
    {
        std::cout << "Invalid mode. Use 'c' for compression or 'd' for decompression." << std::endl;
        return 1;
    }

    return 0;
}
