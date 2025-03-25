#include <gtest/gtest.h>
#include "HuffmanNode.h"
#include "HuffmanTree.h"
#include "FileHandler.h"
#include <fstream>
#include <string>
#include <map>
#include <random>

void readFile(std::string &reader, std::ifstream &file);

TEST(HuffmanNodeTest, CreatesLeafNode)
{
    HuffmanNode node('a', 10);
    EXPECT_EQ(node.character, 'a');
    EXPECT_EQ(node.frequency, 10);
    EXPECT_TRUE(node.isLeaf());
}
TEST(HuffmanNodeTest, CreatesInternalNode)
{
    auto left = std::make_shared<HuffmanNode>('a', 10);
    auto right = std::make_shared<HuffmanNode>('b', 20);
    HuffmanNode parent(30, left, right);

    EXPECT_EQ(parent.frequency, 30);
    EXPECT_FALSE(parent.isLeaf());
    EXPECT_EQ(parent.left->character, 'a');
    EXPECT_EQ(parent.right->character, 'b');
}
// Test per la classe HuffmanTree
TEST(HuffmanTreeTest, BuildsTreeCorrectly)
{
    std::map<char, uint32_t> frequencies = {
        {'a', 5}, {'b', 9}, {'c', 12}, {'d', 13}, {'e', 16}, {'f', 45}};

    HuffmanTree tree;
    tree.buildTree(frequencies);
    tree.generateHuffmanCodes();

    auto codes = tree.getHuffmanCodes();

    // Verifica che tutti i caratteri abbiano un codice
    EXPECT_EQ(codes.size(), 6);

    // Verifica che i caratteri più frequenti abbiano codici più corti
    EXPECT_TRUE(codes['f'].length() <= codes['a'].length());
}

TEST(FileHandlerTest, CalculatesFrequencies)
{
    // Crea un file di test
    std::string testFileName = "test_input.txt";
    std::ofstream testFile(testFileName);
    testFile << "abbcccddddeeeeeffffff";
    testFile.close();

    FileHandler handler;
    handler.calcolatefrequencies(testFileName);

    auto frequencies = handler.getFrequencies();

    EXPECT_EQ(frequencies['a'], 1);
    EXPECT_EQ(frequencies['b'], 2);
    EXPECT_EQ(frequencies['c'], 3);
    EXPECT_EQ(frequencies['d'], 4);
    EXPECT_EQ(frequencies['e'], 5);
    EXPECT_EQ(frequencies['f'], 6);

    // Pulizia
    remove(testFileName.c_str());
}
// Test di integrazione: compressione e decompressione
TEST(IntegrationTest, CompressAndDecompress)
{
    // Crea un file di test
    std::string originalFileName = "original.txt";
    std::string compressedFileName = "compressed.bin";
    std::string decompressedFileName = "decompressed.txt";

    // Crea un file con contenuto di test
    std::ofstream originalFile(originalFileName);
    originalFile << "This is a test file for Huffman compression algorithm. "
                 << "It should contain various characters with different frequencies. "
                 << "Repeating characters like 'e' and 't' should have shorter codes.";
    originalFile.close();

    FileHandler handler;

    // Comprime il file
    handler.compress(originalFileName, compressedFileName);

    // Decomprimi il file
    handler.deCompress(compressedFileName, decompressedFileName);

    // Verifica che il contenuto del file originale e decompresso siano identici
    std::ifstream original(originalFileName, std::ios::binary);
    std::ifstream decompressed(decompressedFileName, std::ios::binary);

    std::string originalContent((std::istreambuf_iterator<char>(original)),
                                std::istreambuf_iterator<char>());
    std::string decompressedContent((std::istreambuf_iterator<char>(decompressed)),
                                    std::istreambuf_iterator<char>());

    EXPECT_EQ(originalContent, decompressedContent);

    // Pulizia
    original.close();
    decompressed.close();
    remove(originalFileName.c_str());
    remove(compressedFileName.c_str());
    remove(decompressedFileName.c_str());
}
TEST(EdgeCaseTests, EmptyFile)
{
    // Crea un file vuoto
    std::string emptyFileName = "empty_file.txt";
    std::string compressedFileName = "empty_compressed.bin";
    std::string decompressedFileName = "empty_decompressed.txt";

    // Crea un file vuoto
    std::ofstream emptyFile(emptyFileName);
    emptyFile.close();

    FileHandler handler;

    // Verifica che la compressione non causi eccezioni con file vuoti
    EXPECT_NO_THROW({
        handler.compress(emptyFileName, compressedFileName);
    });

    // Verifica che la decompressione restituisca un file vuoto
    EXPECT_NO_THROW({
        handler.deCompress(compressedFileName, decompressedFileName);
    });

    // Verifica che il file decompresso sia anche vuoto
    std::ifstream decompressed(decompressedFileName);
    std::string content((std::istreambuf_iterator<char>(decompressed)),
                        std::istreambuf_iterator<char>());
    EXPECT_TRUE(content.empty());

    // Pulizia
    decompressed.close();
    remove(emptyFileName.c_str());
    remove(compressedFileName.c_str());
    remove(decompressedFileName.c_str());
}

TEST(EdgeCaseTests, SingleCharacterFile)
{
    // Crea un file con un solo carattere ripetuto
    std::string singleCharFileName = "single_char.txt";
    std::string compressedFileName = "single_compressed.bin";
    std::string decompressedFileName = "single_decompressed.txt";

    // Crea un file con un solo carattere
    std::ofstream singleCharFile(singleCharFileName);
    singleCharFile << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"; // 32 'A'
    singleCharFile.close();

    FileHandler handler;

    // Comprime e decomprime
    handler.compress(singleCharFileName, compressedFileName);
    handler.deCompress(compressedFileName, decompressedFileName);

    // Verifica che il contenuto sia mantenuto
    std::ifstream original(singleCharFileName, std::ios::binary);
    std::ifstream decompressed(decompressedFileName, std::ios::binary);

    std::string originalContent{};
    std::string decompressedContent{};

    readFile(originalContent, original);
    readFile(decompressedContent, decompressed);

    EXPECT_EQ(originalContent, decompressedContent);

    // Pulizia
    original.close();
    decompressed.close();
    remove(singleCharFileName.c_str());
    remove(compressedFileName.c_str());
    remove(decompressedFileName.c_str());
}

// Test per la gestione degli errori
TEST(ErrorHandlingTests, NonExistentFile)
{
    std::string nonExistentFile = "non_existent_file.txt";
    std::string outputFile = "output.bin";

    FileHandler handler;

    // Verifica che venga lanciata un'eccezione per file non esistenti
    EXPECT_THROW({ handler.compress(nonExistentFile, outputFile); }, std::runtime_error);

    // Verifica che non sia stato creato alcun file di output
    std::ifstream outCheck(outputFile);
    EXPECT_FALSE(outCheck.good());
    outCheck.close();

    // Pulizia
    remove(outputFile.c_str());
}

// Test di integrità dei dati
TEST(DataIntegrityTests, BinaryDataCompression)
{
    // Crea un file binario di test
    std::string binaryFileName = "binary_data.bin";
    std::string compressedFileName = "binary_compressed.bin";
    std::string decompressedFileName = "binary_decompressed.bin";

    // Crea un file con dati binari casuali
    std::ofstream binaryFile(binaryFileName, std::ios::binary);

    // Genera 1024 byte di dati casuali
    std::vector<char> randomData(1024);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 255);

    for (auto &byte : randomData)
    {
        byte = static_cast<char>(distrib(gen));
    }

    binaryFile.write(reinterpret_cast<char *>(randomData.data()), randomData.size());
    binaryFile.close();

    FileHandler handler;

    // Comprime e decomprime
    handler.compress(binaryFileName, compressedFileName);
    handler.deCompress(compressedFileName, decompressedFileName);

    // Verifica che i contenuti binari siano identici
    std::ifstream original(binaryFileName, std::ios::binary);
    std::ifstream decompressed(decompressedFileName, std::ios::binary);

    std::vector<char> originalData((std::istreambuf_iterator<char>(original)),
                                   std::istreambuf_iterator<char>());
    std::vector<char> decompressedData((std::istreambuf_iterator<char>(decompressed)),
                                       std::istreambuf_iterator<char>());

    EXPECT_EQ(originalData.size(), decompressedData.size());
    EXPECT_TRUE(std::equal(originalData.begin(), originalData.end(), decompressedData.begin()));

    // Pulizia
    original.close();
    decompressed.close();
    remove(binaryFileName.c_str());
    remove(compressedFileName.c_str());
    remove(decompressedFileName.c_str());
}

// Test di prestazioni
TEST(PerformanceTests, LargeFileCompression)
{
    // Questo test verifica le prestazioni su file di grandi dimensioni
    std::string largeFileName = "large_file.txt";
    std::string compressedFileName = "large_compressed.bin";
    std::string decompressedFileName = "large_decompressed.txt";

    // Crea un file di grandi dimensioni (5 MB di testo ripetitivo)
    std::ofstream largeFile(largeFileName);

    // Genera un testo con distribuzione non uniforme di caratteri
    std::string repeatedText = "This is a test file for Huffman compression algorithm performance testing. ";
    repeatedText += "The letter 'e' appears frequently in English text, as do 't', 'a' and 'o'. ";
    repeatedText += "Some letters like 'z', 'q', and 'x' are much rarer. ";
    repeatedText += "This non-uniform distribution makes Huffman coding effective. ";

    // Scrivi circa 5 MB di testo
    for (int i = 0; i < 10000; i++)
    {
        largeFile << repeatedText;
    }
    largeFile.close();

    FileHandler handler;

    // Misura il tempo di compressione
    auto startCompress = std::chrono::high_resolution_clock::now();
    handler.compress(largeFileName, compressedFileName);
    auto endCompress = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> compressTime = endCompress - startCompress;

    // Misura il tempo di decompressione
    auto startDecompress = std::chrono::high_resolution_clock::now();
    handler.deCompress(compressedFileName, decompressedFileName);
    auto endDecompress = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> decompressTime = endDecompress - startDecompress;

    // Verifica che i file originale e decompresso siano identici
    std::ifstream original(largeFileName, std::ios::binary);
    std::ifstream decompressed(decompressedFileName, std::ios::binary);

    bool filesEqual = true;
    char bufOrig, bufDecomp;
    while (original && decompressed)
    {
        original.read(reinterpret_cast<char *>(&bufOrig), sizeof(bufOrig));
        decompressed.read(reinterpret_cast<char *>(&bufDecomp), sizeof(bufDecomp));
        if (bufOrig != bufDecomp)
        {
            // Print the mismatched bytes for debugging
            std::cout << "Mismatch found: original byte = " << static_cast<int>(bufOrig)
                      << ", decompressed byte = " << static_cast<int>(bufDecomp)
                      << " at position " << original.tellg() - 1 << std::endl;
            filesEqual = false;
            break;
        }
    }

    EXPECT_TRUE(filesEqual);

    // Non facciamo asserzioni sui tempi di esecuzione, ma li stampiamo per informazione
    std::cout << "Large file compression time: " << compressTime.count() << " seconds\n";
    std::cout << "Large file decompression time: " << decompressTime.count() << " seconds\n";

    // Verifica che il file compresso sia effettivamente più piccolo
    std::ifstream compressedFile(compressedFileName, std::ios::binary | std::ios::ate);
    std::streamsize compressedSize = compressedFile.tellg();
    compressedFile.close();

    std::ifstream originalSizeCheck(largeFileName, std::ios::binary | std::ios::ate);
    std::streamsize originalSize = originalSizeCheck.tellg();
    originalSizeCheck.close();

    std::cout << "Original size: " << originalSize << " bytes\n";
    std::cout << "Compressed size: " << compressedSize << " bytes\n";
    std::cout << "Compression ratio: " << (double)originalSize / compressedSize << ":1\n";

    // Il file compresso dovrebbe essere più piccolo dell'originale
    EXPECT_LT(compressedSize, originalSize);

    // Pulizia
    original.close();
    decompressed.close();
    remove(largeFileName.c_str());
    remove(compressedFileName.c_str());
    remove(decompressedFileName.c_str());
}

// Test di codifica a livello di bit
TEST(BitEncodingTests, HuffmanCodeUniqueness)
{
    // Verifica che i codici Huffman generati siano prefissi unici
    // (nessun codice è prefisso di un altro)

    std::map<char, uint32_t> frequencies = {
        {'a', 10}, {'b', 15}, {'c', 30}, {'d', 16}, {'e', 29}, {'f', 7}};

    HuffmanTree tree;
    tree.buildTree(frequencies);
    tree.generateHuffmanCodes();

    auto codes = tree.getHuffmanCodes();

    // Verifica prefissi
    bool prefixFound = false;
    for (const auto &[char1, code1] : codes)
    {
        for (const auto &[char2, code2] : codes)
        {
            if (char1 != char2)
            {
                if (code1.size() < code2.size() &&
                    code2.substr(0, code1.size()) == code1)
                {
                    prefixFound = true;
                    std::cout << "Prefix found: " << char1 << "(" << code1 << ") is prefix of "
                              << char2 << "(" << code2 << ")\n";
                }
            }
        }
    }

    EXPECT_FALSE(prefixFound);

    // Verifica che codici differenti abbiano lunghezze appropriate
    // (caratteri più frequenti dovrebbero avere codici più corti)
    std::vector<std::pair<char, uint32_t>> freqVec(frequencies.begin(), frequencies.end());
    std::sort(freqVec.begin(), freqVec.end(),
              [](auto &a, auto &b)
              { return a.second > b.second; });

    // I caratteri più frequenti dovrebbero tendenzialmente avere codici più corti
    // Verifichiamo che in generale la relazione sia rispettata
    bool lengthsLogical = true;
    double correlationSum = 0;
    for (size_t i = 0; i < freqVec.size() - 1; i++)
    {
        // Non sempre il carattere più frequente avrà il codice più corto
        // Ma dovrebbe esserci una tendenza generale
        correlationSum += (codes[freqVec[i].first].length() <= codes[freqVec[i + 1].first].length()) ? 1 : -1;
    }

    // Se la correlazione è positiva, in generale i caratteri più frequenti hanno codici più corti
    EXPECT_GT(correlationSum, 0);
}

// Test della struttura dell'albero
TEST(TreeStructureTests, TreeProperties)
{
    std::map<char, uint32_t> frequencies = {
        {'a', 5}, {'b', 9}, {'c', 12}, {'d', 13}, {'e', 16}, {'f', 45}};

    HuffmanTree tree;
    tree.buildTree(frequencies);

    auto root = tree.getRoot();

    // Verifica che la radice non sia un nodo foglia
    EXPECT_FALSE(root->isLeaf());

    // Verifica che la frequenza della radice sia la somma di tutte le frequenze
    uint32_t totalFrequency = 0;
    for (const auto &[c, freq] : frequencies)
    {
        totalFrequency += freq;
    }

    EXPECT_EQ(root->frequency, totalFrequency);

    // Funzione ricorsiva per verificare le proprietà dell'albero
    std::function<void(std::shared_ptr<HuffmanNode>)> verifyNode;
    verifyNode = [&verifyNode](std::shared_ptr<HuffmanNode> node)
    {
        if (!node)
            return;

        // Se non è una foglia, deve avere entrambi i figli
        if (!node->isLeaf())
        {
            EXPECT_TRUE(node->left != nullptr);
            EXPECT_TRUE(node->right != nullptr);

            // La frequenza del nodo deve essere la somma delle frequenze dei figli
            EXPECT_EQ(node->frequency, node->left->frequency + node->right->frequency);

            // Continua a verificare ricorsivamente
            verifyNode(node->left);
            verifyNode(node->right);
        }
    };

    verifyNode(root);
}

// Test di serializzazione dell'albero
TEST(SerializationTests, TreeSerialization)
{
    std::string testFileName = "tree_serialization_test.bin";

    // Crea un set di frequenze
    std::map<char, uint32_t> originalFrequencies = {
        {'a', 10}, {'b', 20}, {'c', 30}, {'d', 40}, {'e', 50}, {'f', 60}};

    // Crea un FileHandler e imposta le frequenze
    FileHandler handler;

    // Dobbiamo modificare leggermente l'implementazione per questo test
    // Creiamo un metodo per impostare le frequenze direttamente
    std::ofstream file(testFileName, std::ios::binary);

    // Serializziamo manualmente le frequenze
    uint16_t mapSize = originalFrequencies.size();
    file.write(reinterpret_cast<char *>(&mapSize), sizeof(mapSize));

    for (const auto &[character, freq] : originalFrequencies)
    {
        char charCopy = character;
        uint32_t freqCopy = freq;

        file.write(reinterpret_cast<char *>(&charCopy), sizeof(charCopy));
        file.write(reinterpret_cast<char *>(&freqCopy), sizeof(freqCopy));
    }

    file.close();

    // Ora deserializziamo
    std::ifstream inFile(testFileName, std::ios::binary);
    handler.deSerializeTree(inFile);
    inFile.close();

    // Ottieni le frequenze caricate
    auto loadedFrequencies = handler.getFrequencies();

    // Verifica che le frequenze siano state mantenute correttamente
    EXPECT_EQ(originalFrequencies.size(), loadedFrequencies.size());

    for (const auto &[character, freq] : originalFrequencies)
    {
        EXPECT_EQ(loadedFrequencies[character], freq);
    }

    // Pulizia
    remove(testFileName.c_str());
}

// Test per la compressione di file con contenuto molto eterogeneo
TEST(ContentTypeTests, HeterogeneousContent)
{
    std::string mixedFileName = "mixed_content.txt";
    std::string compressedFileName = "mixed_compressed.bin";
    std::string decompressedFileName = "mixed_decompressed.txt";

    // Crea un file con contenuto eterogeneo (testo, numeri, caratteri speciali)
    std::ofstream mixedFile(mixedFileName);
    mixedFile << "Normal text with numbers 12345 and special characters !@#$%^&*()\n";
    mixedFile << "abcdefghijklmnopqrstuvwxyz\n";
    mixedFile << "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
    mixedFile << "1234567890\n";
    mixedFile << "!@#$%^&*()_+{}:\"|<>?~`-=[]\\;',./\n";
    mixedFile << "Repeated sequences: aaaaa bbbbb ccccc ddddd eeeee\n";
    mixedFile << "Mixed tabs and spaces:\t\t\t   \t   \t\n";
    mixedFile << "Unicode characters: áéíóúñÁÉÍÓÚÑ€£¥©®™½¼¾\n";
    mixedFile.close();

    FileHandler handler;

    // Comprime e decomprime
    handler.compress(mixedFileName, compressedFileName);
    handler.deCompress(compressedFileName, decompressedFileName);

    // Verifica che il contenuto sia mantenuto
    std::ifstream original(mixedFileName, std::ios::binary);
    std::ifstream decompressed(decompressedFileName, std::ios::binary);

    std::string originalContent((std::istreambuf_iterator<char>(original)),
                                std::istreambuf_iterator<char>());
    std::string decompressedContent((std::istreambuf_iterator<char>(decompressed)),
                                    std::istreambuf_iterator<char>());

    EXPECT_EQ(originalContent, decompressedContent);

    // Pulizia
    original.close();
    decompressed.close();
    remove(mixedFileName.c_str());
    remove(compressedFileName.c_str());
    remove(decompressedFileName.c_str());
}

void readFile(std::string &reader, std::ifstream &file)
{
    reader.clear();
    char car;
    while (file.read(&car, 1))
    {
        reader += car;
    }
}
