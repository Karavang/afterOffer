#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

void shutdownDevice()
{
    system("shutdown now");
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
        cerr << "Failed to execute command." << endl;
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
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <command>" << endl;
        return 1;
    }

    string command = argv[1];
    runAndMonitorProcess(command);
    return 0;
}
