#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* signal handler: prints the signal number and the accompanying integer */
void rt_handler(int signal, siginfo_t *info, void *arg __attribute__ ((__unused__)))
{
    int val = info->si_value.sival_int;

    sleep(1);//usleep(1500);

    // unsafe to use fprintf() in a signal handler - but will suffice for demo
    fprintf(stderr, "Rx signal: SIGRTMIN+%d, value: %d\n", signal - SIGRTMIN, val);
    return;
}


/* send 'value' along with a signal numbered 'signo' */
void send_rt_signal(int signo, int value)
{
    union sigval sivalue;
    sivalue.sival_int = value;

    /* queue the signal */
    if (sigqueue(getpid(), signo, sivalue) < 0) {
        fprintf(stderr, "sigqueue failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return;
}

int main(int argc, char **argv)
{
    struct sigaction action;
    sigset_t mask;

    // Setup handler for 3 real-time signals:
    // SIGRTMIN+1, SIGRTMIN+2, SIGRTMIN+3
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = rt_handler;
    sigemptyset(&action.sa_mask);
    if ((sigaction(SIGRTMIN+1, &action, NULL) < 0) ||
        (sigaction(SIGRTMIN+2, &action, NULL) < 0) ||
        (sigaction(SIGRTMIN+3, &action, NULL) < 0)) {
        fprintf(stderr, "sigaction error: %s\n", strerror(errno));
        return 1;
    }

    sigemptyset(&mask);
    sigaddset(&mask, SIGRTMIN+1);
    sigaddset(&mask, SIGRTMIN+2);
    sigaddset(&mask, SIGRTMIN+3);

    // Block the 3 real-time signals
    sigprocmask(SIG_BLOCK, &mask, NULL);

    // Queue the signals
    send_rt_signal(SIGRTMIN+1, 1);
    send_rt_signal(SIGRTMIN+1, 2);
    send_rt_signal(SIGRTMIN+1, 3);

    send_rt_signal(SIGRTMIN+2, 1);
    send_rt_signal(SIGRTMIN+2, 2);
    send_rt_signal(SIGRTMIN+2, 3);

    send_rt_signal(SIGRTMIN+3, 1);
    send_rt_signal(SIGRTMIN+3, 2);
    send_rt_signal(SIGRTMIN+3, 3);

    // Unblock the signals now
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    // Wait for 3 seconds to ensure that all signals are handled
    sleep(3);

    return 0;
}

