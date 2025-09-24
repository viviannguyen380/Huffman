#include "Huffman.h"
Huffman::Huffman(){
    root=nullptr;
    fullString="";
    fileName="";
    binaryStorage=nullptr;
    validFile=false;
    mapKey="";
}

void Huffman::setFileName(std::string fileName) {
    this->fileName=fileName;
}

void Huffman::readInFile() {
    ifstream ifs;
    string inFile;
    cout << "Enter the path of the file to be compressed: ";

    cin >> inFile;
    ifs.open(inFile);
    if (ifs.fail()) {
        std::cout << "Invalid file name" << std::endl;
        return;
    }

    validFile = true;
    setFileName(inFile);
    char ch;

    //fill array with amount of each char using ascii index
    while (ifs.get(ch)) {
        fullString += ch;
        int numLetter = ch;
        table[numLetter]++;

    }
}

void Huffman::createNodes(){
    if (!validFile){
        return;
    }
    for (int i=0; i<TABLE_SIZE; i++){
        if (table[i]>0){
            char char_eq=i;

            Node* to_put=new Node(char_eq,table[i]);
            nodeQueue.push(to_put);
        }
    }
}
void Huffman::buildTree() {
    if (!validFile) {
        return;
    }
    while (nodeQueue.size() > 1) {
        //pop off top 2 nodes, create a new node having the nodes as the 0/1

        Node *firstNode = nodeQueue.top();
        nodeQueue.pop();
        Node *secondNode = nodeQueue.top();
        nodeQueue.pop();
        Node *toHold;
        toHold = new Node(nullChar, (firstNode->weight + secondNode->weight), firstNode, secondNode);

        nodeQueue.push(toHold);

        root = nodeQueue.top();
    }
}

void Huffman::compress(){
    //set up nodes to
    this->readInFile();
    this->createNodes();
    this->buildTree();

    if (!validFile){
        return;
    }

    string code_result;
    buildCodes(root,code_result);
    string compressedFileString="";

    //save file
    ofstream ofs;
    string outFile;
    cout<<"Enter the file to save compressed text: ";
    cin>>outFile;
    ofs.open(outFile);
    if (ofs.fail()){
        std::cout<<"Invalid file name"<<std::endl;
        return;
    }

    binaryStorage=new Storage();

    if (!binaryStorage->open(outFile, "write")) {
        std::cout << "Invalid binary file" << std::endl;
        exit(0);
    }

    //get key for the 0s and 1s
    for (int i=0; i<fullString.length(); i++){
        codeTable[fullString.at(i)];
        compressedFileString+= codeTable[fullString.at(i)];
    }
    getMapCodes(root);

    //send in chunks, not the entire text 01s
    // insert in chunks of 8, remainder inserted in end (length may not be multiple of 8)
    int length = compressedFileString.length();

    int remainder = length % 8;

    int index=0;

    //length separator: delimiter between mapKey and length of 01 message
    char length_separator=lengthDelimiter;
    string fullHeader=mapKey+length_separator+to_string(compressedFileString.length());
    binaryStorage->setHeader(fullHeader);
    for (index; index<length-remainder; index+=8){
        binaryStorage->insert(compressedFileString.substr(index,8));
    }
    if (remainder>0){
        binaryStorage->insert(compressedFileString.substr(index,remainder));
    }

    binaryStorage->close();

}
void Huffman::buildCodes(Node *tree, std::string result) {
    if (tree==nullptr){
        return;
    }
    int numLetter=tree->letter;
    if (numLetter!=0){
        codeTable[tree->letter]= result;
        mapKey+=tree->letter;
        mapKey+=codeTable[tree->letter];
        char codeDelimiter=delimiter;
        mapKey+=delimiter;
        return;
    }
    buildCodes(tree->zero, result+"0");
    buildCodes(tree->one, result+"1");
}
void Huffman::getMapCodes(Node* tree){
    if (tree==nullptr){
        return;
    }
    getMapCodes(tree->zero);
    getMapCodes(tree->one);
    int numLetter=tree->letter;
    if (numLetter!=0){
        mapKey+=tree->letter;
        mapKey+=codeTable[tree->letter];
        mapKey+=delimiter;
        return;
    }
}
void Huffman::decode(){
    ifstream ifs;
    cout<<"Enter the binary file name to decode: ";
    string inFile;
    cin>>inFile;
    setFileName(inFile);
    binaryStorage=new Storage();
    if (!binaryStorage->open(fileName, "read")) {
        std::cout << "There was an error opening the file." << std::endl;
        exit(0);
    }

    mapKey = binaryStorage->getHeader();


    int lengthIndex=mapKey.find(lengthDelimiter);
    int fileStringLength=stoi(mapKey.substr(lengthIndex+1));

    // prep a string variable to pass by reference
    string compressedMessage ="";
    string result;
    // get 01 key for the message
    while (binaryStorage->extract(result)) {
        compressedMessage+=result;
    }
    root=new Node('\0',0);

    buildTreeFromCodes();
    getResult(compressedMessage,fileStringLength);

    //save file
    ofstream ofs;
    string outFile;
    cout<<"Enter the file to save compressed text: ";
    cin>>outFile;
    ofs.open(outFile);
    if (ofs.fail()){
        std::cout<<"Invalid file name"<<std::endl;
        return;
    }
    ofs<<fullString;
}
void Huffman::buildTreeFromCodes(){
    int previousInd=0;

    int i=0;
    char currentChar=mapKey.at(i);
    while (currentChar!=lengthDelimiter){
        // codeTable[fullString.at(i)];
        if (mapKey.at(i)==delimiter){
            char charToInsert=(mapKey.at(previousInd));
            string key=mapKey.substr(previousInd+1,i-previousInd-1);
            addNode(charToInsert, key);
            previousInd=i+1;

        }
        i++;
        currentChar=mapKey.at(i);
    }
}
void Huffman::addNode(char ch, string key){
    Node* curr=root;
    for (int i=0; i<key.length(); i++){

        if (key.substr(i,1)=="0"){
            if (curr->zero==nullptr){
                curr->zero=new Node(nullChar,0);
            }
            curr=curr->zero;
        }
        else{
            if (curr->one==nullptr){
                curr->one=new Node(nullChar,0);
            }

            curr=curr->one;
        }
    }

    curr->letter=ch;

}
void Huffman::getResult(string compressedMessage, int fileLength){
    Node* curr=root;

    for (int i=0; i<fileLength+1; i++){
        if (curr!=nullptr){
            int charVal=curr->letter;

            if (charVal!=nullChar){
                fullString+=charVal;
                curr=root;
                i--;
                continue;
            }
            if (compressedMessage.substr(i,1)=="0"){
                curr=curr->zero;
            }
            else if (compressedMessage.substr(i,1)=="1"){
                curr=curr->one;
            }
        }
    }
}

//big 3 functions
void Huffman::deleteAll(Node* tree){
    //base case
    if (tree==nullptr){
        return;
    }
    deleteAll(tree->zero);
    deleteAll(tree->one);
    delete tree;
}


Huffman::~Huffman(){
    deleteAll(root);
    root=nullptr;
}