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
                cout << "Output Redirection" << endl;
            }
            if(elements[i] == "<")
            {
                out = true;
                cout << "Input Redirection" << endl;
            }
        }

        // check is there are any pipes
        bool pipe;
        for(int i = 0; i < elements.size(); i++)
        {
            if(elements[i] == "|")
            {
                pipe = true;
                cout << "Pipe Detected" << endl;
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
            if(!in && !out && !pipe)
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

            if (in || out){
                int size = elements.size();
                string file = elements[size-1] + ".txt";
                const char* filename = file.c_str();

                cout << "Filename: " << filename << endl;
                
                int iter = 0;
                int in_index = 0;
                int out_index = 0;
                int first_indicator = 0;

                //find first
                for(int i = 0; i < elements.size(); i++)
                {
                    if(elements[i] == "<" || elements[i] == ">")
                    {
                        first_indicator = i;
                        break;
                    }
                }

                cout << "First Indicator at: " << first_indicator << endl;

                if(in)
                {
                    vector<string>::iterator it = find(elements.begin(), elements.end(), ">");
                    in_index = distance(elements.begin(), it);
                }
                if(out)
                {
                    vector<string>::iterator it = find(elements.begin(), elements.end(), "<");
                    out_index = distance(elements.begin(), it);
                }

                if(in_index == elements.size())
                    in_index = 0;
                if(out_index == elements.size())
                    out_index = 0;
                
                cout  << "Output Indicator at: " << in_index << endl;
                cout  << "Input Indicator at: " << out_index << endl;

                string ifile = elements[in_index+1] + ".txt";
                string ofile = elements[out_index+1] + ".txt";

                const char* inputfile = ifile.c_str();
                const char* outputfile = ofile.c_str();

                cout << "output Filename: " << outputfile << endl;
                cout << "Input Filename: " <<  inputfile << endl;
                


                char* args[first_indicator + 1];
                args[first_indicator] = NULL;

                while(first_indicator != -1)
                {
                    first_indicator--;
                    args[first_indicator] = (char*) elements[first_indicator].c_str();
                }

                //cout << args[0] << args[1] << endl;
                
                //making sure the file opens
                int infd;
                int fd;
                if(in)
                {
                    if ((infd = open(inputfile, O_WRONLY)) < 0){
                        cout << "Error in opening file" <<  endl;
                        break;
                    }    

                    //redirect the standard out to the file
                    //cout << infd << endl;

                    dup2(infd, 1);

                    //cout << infd << endl;

                    close(infd);
                }
                if(out)
                {
                    if ((fd = open(outputfile, O_RDONLY)) < 0)
                    {
                        cout << "Error in opening file" <<  endl;
                        break;
                    }

                    //cout << fd << endl;

                    dup2(fd, 0);

                    //cout << fd << endl;

                    close(fd);
                }


                if(execvp(args[0], args) < 0)
                {
                    printf("Could not execute command.. \n");
                }
            }

            if(pipe)
            {
                
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