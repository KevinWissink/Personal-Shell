#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

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

        //checking if the input need to be I/O redirection
        bool out, in;
        for(int i = 0; i < elements.size(); i++)
        {
            
            if(elements[i] == ">")
            {
                in = true;
                cout << "Input Redirection" << endl;
            }
            if(elements[i] == "<")
            {
                out = true;
                cout << "Output Redirection" << endl;
            }
        }

        int pid = fork();
        if (pid == -1)
        {
            cout << "Error Forking";
        }
        else if (pid == 0 )
        {
            //dealing with basic commands
            if(!in && !out)
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
                //}
            

                if(execvp (args[0], args) < 0) {
                    printf("Could not execute command..\n");
                }
            }

            if (in & !out){
                int size = elements.size();
                string file = elements[size-1] + ".txt";
                const char* filename = file.c_str();

                cout << "Filename: " << filename << endl;
                
                int iter = 0;

                vector<string>::iterator it = find(elements.begin(), elements.end(), ">");
                int index = distance(elements.begin(), it);

                cout << "Indicator at: " << index << endl;

                char* args[index + 1];
                args[index] = NULL;

                while(index != -1)
                {
                    index--;
                    args[index] = (char*) elements[index].c_str();
                }
                
                //making sure the file opens
                int fd;
                if ((fd = open(filename, O_WRONLY, 0)) < 0){
                    cout << "Error in opening file" <<  endl;
                    break;
                }    

                //redirect the standard out to the file
                dup2(fd, 1);

                close(fd);


                if(execvp(args[0], args) < 0)
                {
                    printf("Could not execute command.. \n");
                }
            }

            if (!in & out){
                int size = elements.size();
                int fd;
                string filename = elements[size-1];

                
            }

        }
        else 
        {
            wait(0);
            in = false;
            out = false;
        }
    }
}