#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "periph_conf.h"
#include <periph/gpio.h>
#include "timex.h"
#include "thread.h"

#include "shell_commands.h"
#include "shell.h"

/* #if MODULE_STDIO_RTT */
#define XTIMER_BACKOFF 92
#include "xtimer.h"
/* #endif */

/* set interval to 1 second */
#define INTERVAL (1U * US_PER_SEC)

uint8_t led0_autorun = 1;
uint8_t led0_seconds = 1;

static int print_devid(int argc, char **argv) {
    (void) argc;
    (void) argv;

    /* stm32f303k8 */
    uint32_t * p = (uint32_t *)0xE0042000;
    printf("mcudevid:0x%08lX_%08lX_%08lX\n", p[0], p[1], p[2]);

    p = (uint32_t *)0x1FFFF7AC;
    printf("uniqid:0x%08lX_%08lX_%08lX\n", p[0], p[1], p[2]);

    return 0;
}

static int led_on(int argc, char **argv) {
    (void) argc;
    (void) argv;
    LED0_ON;
    printf("led on\n");

    return 0;
}

static int led_off(int argc, char **argv) {
    (void) argc;
    (void) argv;
    LED0_OFF;
    printf("led off\n");

    return 0;
}

static int led_toggle(int argc, char **argv) {
    (void) argc;
    (void) argv;
    LED0_TOGGLE;
    printf("led off\n");

    return 0;
}

static int led_settimer(int argc, char **argv) {
    (void) argc;
    (void) argv;

    if(argc == 2) {
        uint8_t tmp = led0_seconds;
        led0_seconds = atoi(argv[1]);
        /* printf("led settimer: (%i) set to %s sec\n", tmp, argv[1]); */
        printf("led settimer: was %i sec, set to %i sec\n", tmp, led0_seconds);
    } else {
        led0_seconds = 1;
        printf("led settimer: reset to 1 sec\n");
    }

    return 0;
}

static int led_autorun(int argc, char **argv) {
    (void) argc;
    (void) argv;

    led0_autorun = !led0_autorun;

#ifdef LED0_ON
    printf("led0 available\n");
#else
    printf("led0 not available\n");
#endif

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
    { "devid", "prints 96 bit unique device id", print_devid },
    { "led_on", "switch on led0", led_on },
    { "led_off", "switch off led0", led_off },
    { "led_toggle", "toggle led0", led_toggle },
    { "led_autorun", "autorun led0", led_autorun },
    { "led_settimer", "set timer led0", led_settimer },
    { "echo", "prints the input command", print_echo },
    { NULL, NULL, NULL }
};


/* thread settings */
char thread_stack[THREAD_STACKSIZE_MAIN];

void * thread_func(void *arg) {
    (void) arg;


#if MODULE_STDIO_RTT
    xtimer_init();
#endif

    xtimer_ticks32_t last_wakeup = xtimer_now();
    puts("thread entered");
    while (1) {
        printf("autorun=%i\n", led0_autorun);
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        /* printf("slept until %" PRIu32 "\n", xtimer_usec_from_ticks(xtimer_now())); */

        if(led0_autorun == 1) {
#ifdef LED0_ON
            LED0_TOGGLE;
#endif
        }

    }
    return NULL;
}

int main(void) {

    /* #if MODULE_STDIO_RTT */
    /*     xtimer_init(); */
    /* #endif */

    puts(">>> shell started");

    /* start thread */
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func, NULL, "thread_func");

    /* define buffer to be used by the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    /* define own shell commands */
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* shell run blocks */
    while(1) {
    }

    return 0;
}

