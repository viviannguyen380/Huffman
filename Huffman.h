#include <string>
#include <queue>
#include <fstream>
#include "Node.h"
#include "Storage.h"
#include <unordered_map>
using namespace std;
#ifndef HUFFMAN_H
#define HUFFMAN_H
const static int TABLE_SIZE=128;
const static int nullChar=0;
const static int lengthDelimiter=28;
const static int delimiter=30;
/**
 * comparison to invoke a min priority queue
 */
struct compareNodes{
    bool operator()(Node* first, Node* second){
        return first->weight < second->weight;
    }
};
class Huffman{
public:
    /**
     * constructor for Huffman Tree
     */
    Huffman();
    /**
     * compresses the file into a string
     */
    void compress();
    /**
     * decode an encoded message and read it into another file
     *
     */
    void decode();
    /**
     * destructor for tree
     */
    ~Huffman();

private:
    Node* root;
    bool validFile;
    priority_queue<Node*, vector<Node*>,compareWeights> nodeQueue;
    int table[TABLE_SIZE];
    unordered_map<char,string> codeTable;
    string fullString;
    string fileName;
    Storage* binaryStorage;
    /**
    * sets the name of the file to fileName
    * @param fileName the fileName
    */
    void setFileName(string fileName);
    /**
    * reads in the file, filling the queue with necessary nodes
    * @param file the file to read in
    */
    void readInFile();
    /**
     * use table to create nodes, push into priority queue
     */
    void createNodes();
    /**
     * builds the tree, using the nodes from the priority queue
     */
    void buildTree();
    /**
     * uses an inorder traversal to get the codes from shortest to longest to
     * set mapCodes
     * @param tree the tree to search for nodes
     */
    void getMapCodes(Node* tree);
    /**
     * builds tree from the mapKey.
     * @param compressedFileString the string to put the fullString in
     */
    void buildTreeFromCodes();
    /**
     * adds the node with the char and going thru the 01s in key, adding \0 nodes as neccessary
     * @param key
     */
    void addNode(char ch, string key);
    /**
     * gets the result (decoding) from the 01 encoded message, stores it into fullString
     * @param compressedMessage the 01 message to decode
     * @param length the length of the 01 message to look through
     */
    void getResult(string compressedMessage, int length);
    /**
    * compresses the given text into 0s and 1s
    * @param tree the node to search for a character
    * @param result the character's resulting 01 code is built here
    */
    void buildCodes(Node* tree, string result);
    //destructor functions
    /**
     * Clears the tree
     */
    void deleteAll(Node* tree);
    /*
     * Map key is formatted as follows:
     * (corresponding char)(01 code for char)(delimiter),etc
     * Last delimiter is the length delimiter, then length of 01 code to look at
     * (multiples of 8 are loaded in, the rest is padded with 0s that aren't part of the message)
     */
    string mapKey;
};
#endif //HUFFMAN_H