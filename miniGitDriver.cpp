#include "miniGit.hpp"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/*--------------------Helper Function Headers--------------------*/

// this function checks if given file exists in main dir
bool checkForFileInDirectory(string filename);

/*--------------------Main Function--------------------*/

int main(){

    cout << endl << "======================Mini Git======================" << endl;
    cout << "Do you wish to initialize an empty repository? (Y/N)" << endl;

    string choice;
    getline(cin, choice);

    if(choice == "y" || choice == "Y"){

        MiniGit miniGit = MiniGit();
        miniGit.newRepository();

        cout << endl << "Repository initialized" << endl;
        string menuChoice;

        do{

            cout << endl << "==========Menu==========" << endl;
            cout << "1. Add File" << endl;
            cout << "2. Remove File" << endl;
            cout << "3. Commit" << endl;
            cout << "4. Checkout" << endl;
            cout << "5. Quit" << endl;
            cout << "Enter a selection : " << endl;
            getline(cin, menuChoice);

            switch(menuChoice[0]){

                case '1':{
                    if(!miniGit.checkCurrent()){
                        string filename;
                        do{
                            cout << "Enter the name of the file to be added : " << endl;
                            getline(cin, filename);
                            if(!checkForFileInDirectory(filename))
                                cout << "File not found in current directory" << endl;
                        }while(!checkForFileInDirectory(filename));

                        int check = miniGit.addFile(filename);
                        if(check == -1)
                            cout << "File already added to current commit" << endl;
                        else
                            cout << "File added successfully" << endl;
                    }
                    else
                        cout << "Cannot add file when in previous commit" << endl;
                    break;
                }

                case '2':{
                    if(!miniGit.checkCurrent()){
                        string filename;
                        cout << "Enter the name of the file to be removed : " << endl;
                        getline(cin, filename);
                        int check = miniGit.removeFile(filename);
                        if(check == -1)
                            cout << "This file is not in the current commit" << endl;
                        else
                            cout << "File removed successfully" << endl;
                    }
                    else
                        cout << "Cannot remove file when in previous commit" << endl;
                    break;
                }

                case '3':
                    if(!miniGit.checkCurrent()){
                        int commitNum = miniGit.commit();
                        if(commitNum == -1)
                            cout << "No changes were made to the files in current commit" << endl;
                        else
                            cout << "Commit number " << commitNum << " was successful" << endl;
                    }
                    else
                        cout << "Cannot commit when in previous commit" << endl;
                    break;

                case '4':{
                    if(miniGit.commitMade()){
                        cout << "Are you sure you want to check out a previous version?" << endl;
                        cout << "Doing so will erase any changes to local files. (Y/N)" << endl;
                        string coChoice;
                        getline(cin, coChoice);

                        if(coChoice == "Y" || coChoice == "y"){
                            cout << "Enter the commit number that you want to checkout : " << endl;
                            string sCommitNum;
                            getline(cin, sCommitNum);
                            int check = miniGit.checkout(stoi(sCommitNum));
                            if(check == 1)
                                cout << "Commit Number " << sCommitNum << " has been checked out" << endl;
                            else if(check == -1)
                                cout << "current in null" << endl;
                            else if(check == -2)
                                cout << "That commit number does not exist" << endl;
                            else if(check == -3)
                                cout << "Failed to overwrite file" << endl;
                        }

                        else if(coChoice == "N" || coChoice == "n")
                            cout << "No worries!" << endl;
                        else
                            cout << "Invalid choice" << endl;
                    }
                    else
                        cout << "You have not yet made a commit" << endl;
                    break;
                }

                case '5':
                    cout << "Bye!" << endl;
                    break;

                default:
                    cout << "Invalid selection" << endl;
            }
            
        }while(menuChoice != "5");

    }

    else if(choice == "n" || choice == "N")
        cout << "Bye!" << endl;
    else
        cout << "Invalid choice" << endl;

    return 0;
}

/*--------------------Helper Function Definitions--------------------*/

bool checkForFileInDirectory(string filename){
    ifstream file(filename);
    if(file.is_open()){
        file.close();
        return true;
    }
    return false;
}