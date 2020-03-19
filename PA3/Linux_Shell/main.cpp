#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(){

    while (true)
    {
        cout << "Kevin's Shell$ "; //print a prompt
        string inputline;
        getline (cin, inputline);
        
        //my exit element
        if (inputline == string("exit"))
        {
            cout << "Bye!! End of Shell" << endl;
            break;
        }

        //from geeks for geeks to parse a string with spaces
        stringstream separatedWords(inputline);
        istream_iterator<string> begin(separatedWords);
        istream_iterator<string> end;
        vector<string> elements(begin, end);

        int pid = fork();
        if (pid == -1)
        {
            cout << "Error Forking";
        }
        else if (pid == 0 )
        {
            int size = elements.size();
            char* args[size+1]; 
            for (int i = 0; i < size; ++i) {
                args[i] = (char*) elements[i].c_str();
            }

            args[size] = NULL;

            //testing purposes
            //for (char* cs : args) {
            //     cout << cs << " ";
            ///}
            

            if(execvp (args[0], args) < 0) {
                printf("Could not execute command..\n");
            }
        }
        else 
        {
            wait(0);
        }
    }
}