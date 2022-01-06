#include "miniGit.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

/*--------------------Helper Functions--------------------*/


// Given the head, this function deletes the contents of an SLL
void deleteSLL(singlyNode *head){
    if(head){
        singlyNode *crawler = head->next;
        while(crawler){
            delete head;
            head = crawler;
            crawler = crawler->next;
        }
        delete head;
    }
}

// this function returns true if a file with a matching filename is found in the SLL
bool checkForFileInSLL(string filename, singlyNode *head){
    singlyNode *crawler = head;
    while(crawler){
        if(crawler->filename == filename)
            return true;
        crawler = crawler->next;
    }
    return false;
}

// this function checks if the current DLL is the latest one
bool MiniGit::checkCurrent(){
    return current->next;
}

// this function checks if a commit has been made. If the DLL has a previous or next node,
// a commit must have been made
bool MiniGit::commitMade(){
    return current->next || current->previous;
}

/*--------------------Constructor/Desctructor--------------------*/

MiniGit::MiniGit(){
    current = nullptr;
}

MiniGit::~MiniGit(){
    // if there is a DLL node
    if(current){
        // go to the first DLL node
        while(current->previous){
            current = current->previous;
        }

        // delete the SLL at the node. Then, delete the DLL node
        doublyNode *crawler = current->next;
        while(crawler){
            deleteSLL(current->head);
            delete current;
            current = crawler;
            crawler = crawler->next;
        }
        deleteSLL(current->head);
        delete current;
    }
}

/*--------------------Initialize New Repository--------------------*/

// this function creates the first DLL node, and sets up the .minigit directory
void MiniGit::newRepository(){
    fs::remove_all(".minigit");
    fs::create_directory(".minigit");
    current = new doublyNode;
    current->commitNumber = 0;
    current->next = current->previous = nullptr;
    current->head = nullptr;
}

/*--------------------Add File--------------------*/

int MiniGit::addFile(string filename){

    // if the file doesn't exist in SLL, throw exception
    if(checkForFileInSLL(filename, current->head))
        return -1;
    
    // initialize the new node to store the new file info
    singlyNode *newNode = new singlyNode;
    newNode->filename = filename;
    newNode->next = nullptr;
    int i = 0;

    // find the index of the period in the filename
    while(i < filename.length() && filename[i] != '.'){
        ++i;
    }

    // set the fileversion
    newNode->fileVersion = filename.substr(0,i) + "_00" + filename.substr(i);

    // if no files stores yet, set head equal to new file
    if(!current->head)
        current->head = newNode;
    // else, find last node and add new file to next of last node
    else{
        singlyNode *crawler = current->head;
        while(crawler->next != nullptr){
            crawler = crawler->next;
        }
        crawler->next = newNode;
    }
    return 1;
}

/*--------------------Remove File--------------------*/

int MiniGit::removeFile(string filename){
    // check for the file in the SLL
    if(!checkForFileInSLL(filename, current->head))
        return -1;
    else{
        // crawl through SLL, delete the file, and connect previous and next files
        singlyNode *crawler = current->head;
        singlyNode *crawler2 = nullptr;
        while(crawler->filename != filename){
            crawler2 = crawler;
            crawler = crawler->next;
        }
        if(!crawler2)
            current->head = nullptr;
        else
            crawler2->next = crawler->next;;
        delete crawler;
        return 1;
    }
}

/*--------------------Commit File--------------------*/

int MiniGit::commit(){

    // this checks if a change has been made since the previous commit
    bool change = false;

    // this is used to crawl through the SLL of the current commit
    singlyNode *crawler = current->head;
    
    // this is used to check if a file has been removed since the previous commit.
    // If a file was removed, bool change will be set to true
    singlyNode *crawler2 = nullptr;
    if(current->previous)
        crawler2 = current->previous->head;

    // initialize the next commit so that I can copy things over
    // as I traverse the current commit
    doublyNode *nextDLL = new doublyNode;
    nextDLL->commitNumber = current->commitNumber + 1;
    nextDLL->previous = current;
    nextDLL->next = nullptr;
    nextDLL->head = nullptr;

    // used for copying stuff
    singlyNode *copier = nullptr;

    // for each node in the current commit
    while(crawler){

        // attempt to open file in .minigit directory
        ifstream file(".minigit/" + crawler->fileVersion);

        if(!file.is_open()){
            // if it didn't open, file is new, so change has been made
            change = true;

            // create a file in .minigit and copy the contents from main dir file into
            // .minigit file
            ofstream output(".minigit/" + crawler->fileVersion);
            ifstream input(crawler->filename);
            string line;
            if(output.is_open() && input.is_open()){
                while(getline(input, line)){
                    output << line << endl;
                }
            }
            else
                cout << "Failed file copying for " << crawler->filename << endl;
            if(output.is_open())
                output.close();
            if(input.is_open())
                input.close();
        }
        
        else{
            //*********************************************************************************
            // if the file did open, need to check for changes to the file in main dir
            ifstream mainInput(crawler->filename);
            string mainLine, mgLine;
            bool different = false;
            // compare each main dir file line to the .minigit version
            while(getline(mainInput, mainLine) && !different){
                getline(file, mgLine);
                if(mainLine != mgLine){
                    different = true;
                    change = true;
                }
            }
            // if more lines left in .minigit file, files are different
            if(getline(file, mgLine)){
                different = true;
                change = true;
            }
            //*********************************************************************************
            file.close();
            mainInput.close();

            if(different){
                
                // if files are different, update the fileversion number, and copy
                // contents of main dir file into the new .minigit file
                string newFilename = crawler->fileVersion;
                int i = 0;
                while(newFilename[i] != '_'){
                    ++i;
                }
                int version = stoi(newFilename.substr(i+1,2));
                ++version;
                string firstDigit = to_string(version / 10);
                string secondDigit = to_string(version % 10);
                newFilename[i+1] = firstDigit[0];
                newFilename[i+2] = secondDigit[0];
                crawler->fileVersion = newFilename;

                ofstream output(".minigit/" + newFilename);
                mainInput.open(crawler->filename);
                while(getline(mainInput, mainLine)){
                    output << mainLine << endl;
                }
                output.close();
                mainInput.close();
            }
        }

        //***************************
        // create a copy SLL to add to the next DLL
        singlyNode *newNode = new singlyNode;
        newNode->filename = crawler->filename;
        newNode->fileVersion = crawler->fileVersion;
        newNode->next = nullptr;
        if(copier){
            copier->next = newNode;
            copier = copier->next;
        }
        else{
            nextDLL->head = newNode;
            copier = newNode;
        }
        //***************************

        // increment the crawlers in current DLL and previous DLL(checks for removal of files)
        if(crawler2)
            crawler2 = crawler2->next;
        crawler = crawler->next;

    }

    // after going through everying in current DLL, if there are still SLL's in previous DLL,
    // a file must have been removed, so a change has been made since previous commit.
    if(crawler2)
        change = true;

    // after checking everything, if a change was not made, block the commit from happening,
    // and delete the DLL and corresponding SLL
    if(!change){
        deleteSLL(nextDLL->head);
        delete nextDLL;
        return -1;
    }

    // set the current DLL to the copy that was added next
    current->next = nextDLL;
    current = current->next;
    // return the commitNumber of the DLL that was committed
    return current->commitNumber - 1;
}

/*--------------------Checkout Version--------------------*/

int MiniGit::checkout(int commitNumber){
    if(current){

        // first, find the DLL with corresponding commit number
        doublyNode *finder = current;

        while(finder->previous){
            finder = finder->previous;
        }
        while(finder->next && finder->commitNumber != commitNumber){
            finder = finder->next;
        }
        // if not found, throw error
        if(finder->commitNumber != commitNumber)
            return -2;

        current = finder;

        // for each SLL node, delete main dir file and create new one with contents of
        // corresponding .minigit file
        singlyNode *crawler = current->head;
        while(crawler){
            remove(crawler->filename.c_str());
            ofstream output(crawler->filename);
            ifstream input(".minigit/" + crawler->fileVersion);
            if(output.is_open() && input.is_open()){
                string line;
                while(getline(input, line)){
                    output << line << endl;
                }
            }
            else
                return -3;
            if(input.is_open())
                input.close();
            if(output.is_open())
                output.close();
            crawler = crawler->next;
        }
        return 1;
    }
    else
        return -1;
}