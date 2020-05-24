#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <atomic>
#include <sys/types.h>
#include <sys/wait.h>

static const char* detectorBinName = "detector.py";
static std::atomic<pid_t> detectorPid;

void rt_handler(int signal, siginfo_t* info, void* arg __attribute__((__unused__)))
{
    int val = info->si_value.sival_int;

    sleep(1);//usleep(1500);

    // unsafe to use fprintf() in a signal handler - but will suffice for demo
    fprintf(stderr, "Rx signal: SIGRTMIN+%d, value: %d\n", signal - SIGRTMIN, val);
    return;
}


void send_rt_signal(int signal, int value)
{
    union sigval sivalue;
    sivalue.sival_int = value;

    /* queue the signal */
    if (sigqueue(getpid(), signal, sivalue) < 0) {
        fprintf(stderr, "sigqueue failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return;
}

pid_t run_detector() {
    pid_t  pid;
    int status;
    pid = fork();

    if (pid == 0) {
        // Child process creation 
        pid_t ppid = getppid();
        char *parent_pid = new char[sizeof(pid_t)];//malloc(sizeof(pid_t));
        sprintf(parent_pid, "%d", ppid);
fprintf(stderr, "create child %s\n", parent_pid);
        char* argv_list[] = { parent_pid, NULL };
        status = execv(detectorBinName, argv_list);
fprintf(stderr, "create child %s %d\n", parent_pid, status);
        delete []parent_pid;
        exit(status);
fprintf(stderr, "create child %s %d\n", parent_pid, status);
    }
    else if (pid > 0) {
        // Parent process check child state
fprintf(stderr, "parent process %d\n", pid);
int xxx;
        if (xxx=waitpid(pid, &status, WNOHANG) != 0)
            return -1;
fprintf(stderr, "parent process %d %d %d\n", pid, status, xxx);
    }
    return pid;
}

int main(int argc, char** argv)
{
    pid_t tmppid = run_detector();
    if (tmppid < 0) {
        fprintf(stderr, "Can't run detector process %s\n", strerror(errno));
        exit(tmppid);
    }

    printf("Run %d completed\n", tmppid);
    //sleep(120);
    exit(0);


    struct sigaction action;
    sigset_t mask;

    // Setup handler for 3 real-time signals:
    // SIGRTMIN+1, SIGRTMIN+2, SIGRTMIN+3
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = rt_handler;
    sigemptyset(&action.sa_mask);
    if ((sigaction(SIGRTMIN + 1, &action, NULL) < 0) ||
        (sigaction(SIGRTMIN + 2, &action, NULL) < 0) ||
        (sigaction(SIGRTMIN + 3, &action, NULL) < 0)) {
        fprintf(stderr, "sigaction error: %s\n", strerror(errno));
        return 1;
    }

    sigemptyset(&mask);
    sigaddset(&mask, SIGRTMIN + 1);
    sigaddset(&mask, SIGRTMIN + 2);
    sigaddset(&mask, SIGRTMIN + 3);

    // Block the 3 real-time signals
    sigprocmask(SIG_BLOCK, &mask, NULL);

    // Queue the signals
    send_rt_signal(SIGRTMIN + 1, 1);
    send_rt_signal(SIGRTMIN + 1, 2);
    send_rt_signal(SIGRTMIN + 1, 3);

    send_rt_signal(SIGRTMIN + 2, 1);
    send_rt_signal(SIGRTMIN + 2, 2);
    send_rt_signal(SIGRTMIN + 2, 3);

    send_rt_signal(SIGRTMIN + 3, 1);
    send_rt_signal(SIGRTMIN + 3, 2);
    send_rt_signal(SIGRTMIN + 3, 3);

    // Unblock the signals now
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    // Wait for 3 seconds to ensure that all signals are handled
    sleep(3);

    return 0;
}
