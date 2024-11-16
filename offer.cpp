#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring> // Include for strerror
#include <errno.h> // Include for errno

using namespace std;

void printl(const string &mystr)
{
    cout << mystr << endl;
}

void shutdownDevice()

{
    printl("Device will be shuted down after 5 seconds");
    sleep(5);
    system("shutdown now");
}

void runAndMonitorProcess(const string &command)
{
    // Split the command into arguments
    istringstream iss(command);
    vector<string> args;
    string arg;

    while (iss >> arg)
    {
        args.push_back(arg);
    }

    // Check if there are no arguments
    if (args.empty())
    {
        cerr << "No command to execute." << endl;
        return;
    }

    // Prepare the argument list for execvp
    vector<char *> argv;
    for (auto &a : args)
    {
        argv.push_back(&a[0]); // Convert string to char*
    }
    argv.push_back(nullptr); // Null-terminate the argument list

    pid_t pid = fork();
    if (pid < 0)
    {
        cerr << "Failed to fork process." << endl;
        return;
    }

    if (pid == 0)
    {
        // In child process
        execvp(argv[0], argv.data());
        // If execvp returns, it means it failed
        cerr << "Failed to execute command: " << strerror(errno) << endl; // Print the error
        exit(1);
    }
    else
    {
        // In parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            shutdownDevice();
        }
    }
}

int main(int argc, char *argv[])
{
    string command;
    for (int i = 1; i < argc; ++i)
    {
        command += argv[i];
        if (i < argc - 1)
        {
            command += " ";
        }
    }

    printl("Command to execute: " + command);
    runAndMonitorProcess(command);
    return 0;
}