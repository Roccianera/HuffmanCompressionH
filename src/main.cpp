
#include <iostream>
#include "FileHandler.h"
#include <chrono>

class Timer
{
private:
    // Type aliases to make accessing nested type easier
    using Clock = std::chrono::steady_clock;
    using Second = std::chrono::duration<double, std::ratio<1> >;

    std::chrono::time_point<Clock> m_beg{ Clock::now() };

public:

    void reset()
    {
        m_beg = Clock::now();
    }

    double elapsed() const
    {
        return std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
    }
};














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

    Timer t ;
    

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

    std::cout << "Operation completed in " << t.elapsed() << " seconds." << std::endl;

    return 0;
}
