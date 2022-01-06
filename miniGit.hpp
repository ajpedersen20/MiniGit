#include <string>

using namespace std;

struct singlyNode{
    string filename;        // Name of local file
    string fileVersion;     // Name of file in .minigit folder
    singlyNode *next;
};

struct doublyNode{
    int commitNumber;
    singlyNode *head;
    doublyNode *previous;
    doublyNode *next;
};

#ifndef MINIGIT
#define MINIGIT

class MiniGit{
    private:
        doublyNode *current;    // pointer to the DLL node that user is using
    public:
        MiniGit();
        ~MiniGit();

        void newRepository();
        int addFile(string filename);
        int removeFile(string filename);
        int commit();
        int checkout(int commitNumber);

        bool checkCurrent();
        bool commitMade();
};

#endif