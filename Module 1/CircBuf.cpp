
/*
If I am being honest, this project took me way longer than I thought it would have. I learned how the std::copy works for sure. I also learned how std::copy takes in arguments and what each one represents when copying. 
Using the heap is not something I have done in a while and I ran into an issue that I have not seen before when deallocating the dynamic memory with the heap. I kept getting this error when compiling:

`free(): double free detected in tcache 2`

And I had no ideas as to what that could mean. After some google searching, I found out that I was freeing already free memory and the compiler was mad at me. This sent me on a rabbit hole of 
adding conditions do my destructor and making additional tests in the main function. I eventually figured it out and got my expand function working correctly with no issues.

One last thing was using additional functions to test other members of the class. I made some other methods to print out data members and the return values of the some of the 
methods so I could get a better grasp of what was happening in the program. 

*/

#include "CircBuf.h"

using std::copy;
using std::cout;
using std::endl;


//constructor
CircBuf::CircBuf(size_t reserve){
    currentCapacity = ceil(float(reserve)/CHUNK) * CHUNK;
    buffer = new char[currentCapacity]{};
    bufferSize = 0;
    write = 0;
    read = 0;
}

//destructor
CircBuf::~CircBuf() {
    delete[] buffer;
}

size_t CircBuf::size() {
    return bufferSize;
}

size_t CircBuf::capacity(){
    return currentCapacity;
}

void CircBuf::printw(){
    cout << "Write: " << write << endl; 
}

void CircBuf::shrink() {
    if(bufferSize == 0){
        currentCapacity = 0;
        write = 0;
        read = 0;
        delete[] buffer;
        buffer = new char[currentCapacity];
        return;
    }

    char* newBuffer = new char[bufferSize];

    size_t endOfArray = currentCapacity;
    if (read < write) {
        copy(buffer + read, buffer + write, newBuffer);
    } else {
        copy(buffer + read, buffer + currentCapacity, newBuffer);
    }

    size_t newWrite = currentCapacity - read;

    delete[] buffer;
    buffer = newBuffer;
    currentCapacity = bufferSize + newWrite;
    read = 0;
    write = bufferSize;
}

void CircBuf::expand(size_t numChunks) {
    if(currentCapacity == 0)
    {
        currentCapacity = CHUNK * numChunks;
        delete[] buffer;
        buffer = new char[currentCapacity];
    }

    char *new_array = new char[currentCapacity];

    if(read <= write){
        copy(buffer + read, buffer + write, new_array);
    }
    else {
        size_t left = currentCapacity - read;
        size_t right = bufferSize - left;

        copy(buffer + read, buffer + currentCapacity, new_array);

        copy(buffer, buffer + right, new_array + left);

        read = 0;
        write = bufferSize;
    }

    currentCapacity = CHUNK * numChunks;
    delete[] buffer;
    buffer = new char[currentCapacity];
    copy(new_array, new_array + bufferSize, buffer);
    delete[] new_array;
}


void CircBuf::insert(char charIn) {
    if (bufferSize + 1 > currentCapacity) {
        expand(1);
    }
    buffer[write] = charIn;
    write++;
    if (write >= currentCapacity) {
        write = 0;
    }
    bufferSize++;
}

void CircBuf::insert(const char *charIn, size_t sz) {
    size_t news = bufferSize + sz;
    if(currentCapacity < news) {
        size_t newc = ceil(float(news) / CHUNK);
        expand(newc);
    }

    size_t endOfArray = currentCapacity;

    if (write + sz > endOfArray) {
        size_t left = endOfArray - write;
        size_t right = sz - left;

        std::copy(charIn, charIn + left, buffer + write);
        std::copy(charIn + left, charIn + sz, buffer);

        write = right;
    } else {
        std::copy(charIn, charIn + sz, buffer + write);
        write += sz;

        if (write >= currentCapacity) {
            write -= currentCapacity;
        }
    }
    bufferSize = news;
}

void CircBuf::insert(const string &stringIn) {
    size_t sz = stringIn.length();
    size_t newsize = bufferSize + sz;

    if (currentCapacity < newsize) {
        size_t newcapacity = ceil(float(newsize) / CHUNK);
        expand(newcapacity);
    }

    size_t endOfArray = currentCapacity;
    if (write + sz > endOfArray) {
        size_t left = endOfArray - write;
        size_t right = sz - left;

        std::copy(stringIn.begin(), stringIn.begin() + left, buffer + write);
        std::copy(stringIn.begin() + left, stringIn.end(), buffer);

        write = right;

    } else {
        std::copy(stringIn.begin(), stringIn.end(), buffer + write);

        write += sz;

        if (write > currentCapacity) {
            write -= currentCapacity;
        }
    }
    bufferSize = newsize;
}

char CircBuf::get(){
    if (bufferSize == 0) {
        return '\0';
    }
    char returnChar = buffer[read];
    //cout << "Returned: " << buffer[read] << " at " << read << endl;
    read++;

    if (read >= currentCapacity) {
        read = 0;
    }
    bufferSize--;
    return returnChar;
}

string CircBuf::get(size_t sz){
    if (bufferSize == 0 || sz ==0){
        return "";
    } 
    string returnString;

    for(size_t i = 0; i < sz; ++i) {
        if (bufferSize == 0){
            break;
        }
        returnString.push_back(buffer[read]);
        read++;

        if(read >= currentCapacity) {
            read = 0;
        }
        bufferSize--;
    }
    return returnString;
}

string CircBuf::flush(){
    string returnString = get(bufferSize);
    bufferSize = 0;
    currentCapacity = 0;
    read = 0;
    write = 0;
    shrink();
    return returnString;
}

string CircBuf::examine(){

    string returnString = "["; //begin

    // if empty
    if(read == write && bufferSize == 0){
        for(size_t i = 0; i < currentCapacity; i++){
            returnString += "-";
        }
    }

    // it is full
    else if(bufferSize == currentCapacity){
        for(size_t i = 0; i < currentCapacity; i++){
            returnString += buffer[i];
        }
    }

    else if(read < write){
        for (size_t i = 0; i < currentCapacity; i++){
            if(i >= read && i < write){
                returnString += buffer[i];
            }
            else {
                returnString += "-";
            }
        }
    }

    else if(read > write){
        for (size_t i = 0; i < currentCapacity; i++){
            if(i < write || i >= read){
                returnString += buffer[i];
            }
            else {
                returnString += "-";
            }
        }
    }
    returnString += "]";
    return returnString;
}

void CircBuf::printbuff(){
    std::cout << "Buffer Content: [";

    if (bufferSize == 0){
        std::cout << "Empty";
    } else{
        for (size_t i = 0; i < currentCapacity; ++i){
            std::cout << buffer[i];
        }
    }

    std::cout << "]" << std::endl;
}

