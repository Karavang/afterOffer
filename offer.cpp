#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>
#include <errno.h>

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

    istringstream iss(command);
    vector<string> args;
    string arg;

    while (iss >> arg)
    {
        args.push_back(arg);
    }

    if (args.empty())
    {
        cerr << "No command to execute." << endl;
        return;
    }

    vector<char *> argv;
    for (auto &a : args)
    {
        argv.push_back(&a[0]);
    }
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid < 0)
    {
        cerr << "Failed to fork process." << endl;
        return;
    }

    if (pid == 0)
    {

        execvp(argv[0], argv.data());

        cerr << "Failed to execute command: " << strerror(errno) << endl;
        exit(1);
    }
    else
    {

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