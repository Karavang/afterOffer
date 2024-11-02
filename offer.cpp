#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <chrono>
#include <thread>

using namespace std;

void shutdownDevice()
{
    execlp("shutdown", "shutdown", "now", nullptr);
    cerr << "Failed to execute shutdown command." << endl;
    exit(1);
}

void runAndMonitorProcess(const string &command)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        cerr << "Failed to fork process." << endl;
        return;
    }

    if (pid == 0)
    {

        execlp(command.c_str(), command.c_str(), nullptr);
        cerr << "Failed to execute command: " << command << endl;
        exit(1);
    }
    else
    {

        int status;
        if (waitpid(pid, &status, 0) == -1)
        {
            cerr << "Error waiting for child process." << endl;
            return;
        }

        if (WIFEXITED(status))
        {
            cout << "Device will be turned off in 5 seconds..." << endl;
            this_thread::sleep_for(chrono::seconds(5));
            shutdownDevice();
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <command>" << endl;
        return 1;
    }

    string command = argv[1];
    runAndMonitorProcess(command);
    return 0;
}
