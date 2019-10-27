#include <stdio.h>
#include <string.h>
#include "board.h"
#include "periph_conf.h"
#include <periph/gpio.h>
#include "timex.h"
#include "thread.h"

#include "shell_commands.h"
#include "shell.h"

/* #if MODULE_STDIO_RTT */
#include "xtimer.h"
/* #endif */

static int print_teststart(int argc, char **argv) {
    (void) argc;
    (void) argv;
    printf("[TEST_START]\n");

    return 0;
}

static int print_testend(int argc, char **argv) {
    (void) argc;
    (void) argv;
    printf("[TEST_END]\n");

    return 0;
}

static int print_echo(int argc, char **argv) {
    for (int i = 0; i < argc; ++i) {
        printf("\"%s\"", argv[i]);
    }
    puts("");

    return 0;
}

static const shell_command_t shell_commands[] = {
    { "start_test", "starts a test", print_teststart },
    { "end_test", "ends a test", print_testend },
    { "echo", "prints the input command", print_echo },
    { NULL, NULL, NULL }
};

/* set interval to 1 second */
#define INTERVAL (1U * US_PER_SEC)

/* thread settings */
char thread_stack[THREAD_STACKSIZE_MAIN];
void * thread_func(void *arg) {
    (void) arg;
    xtimer_ticks32_t last_wakeup = xtimer_now();
    /* msg_t m; */
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, 3*INTERVAL);
        puts(">>> thread\n");
        /* msg_receive(&m); */
        /* printf("Got msg from %" PRIkernel_pid "\n", m.sender_pid); */
    }
    return NULL;
}

int main(void) {

    puts(">>> shell");

    /* xtimer_ticks32_t last_wakeup = xtimer_now(); */
    /* thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func, NULL, "thread_func"); */

#if MODULE_STDIO_RTT
    xtimer_init();
#endif

    /* define buffer to be used by the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];

    /* define own shell commands */
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* or use only system shell commands */
    /*
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);
    */

    while(1) {

#ifdef LED0_ON
        LED0_TOGGLE;
#endif

    }

    return 0;
}

