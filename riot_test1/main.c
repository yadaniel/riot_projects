/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       riot test1
 *
 * @author      yadaniel
 *
 * @}
 */

#include <stdio.h>
#include "board.h"
#include "periph_conf.h"
#include <periph/gpio.h>
#include "xtimer.h"
#include "timex.h"
#include "thread.h"

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

    uint8_t nleds = 0;

#ifdef LED0_ON
    nleds += 1;
#endif
#ifdef LED1_ON
    nleds += 1;
#endif
#ifdef LED2_ON
    nleds += 1;
#endif
#ifdef LED3_ON
    nleds += 1;
#endif

    xtimer_ticks32_t last_wakeup = xtimer_now();
    uint32_t cnt = 0;

    /* gpio_t pin = GPIO_PIN(1,22); */
    /* gpio_init(pin,GPIO_OUT); */

    // see reference manual for stm32l432 ... unique device ID register 96 bits
    uint32_t * p = (uint32_t *)0x1FFF7590;
    uint32_t uniq_id[3] = {0};
    uniq_id[0] = p[0];
    uniq_id[1] = p[1];
    uniq_id[2] = p[2];
    printf("This board has UNIQ_ID 0x%4lX %4lX %4lX.\n", uniq_id[0], uniq_id[1], uniq_id[2]);

    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func, NULL, "thread_func");

    while(1) {
        cnt += 1;

        puts("Hello World!");
        printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
        printf("This board features a(n) %s MCU.\n", RIOT_MCU);
        printf("This board has %i LEDs.\n", nleds);
        printf("This board has UNIQ_ID 0x%4lX %4lX %4lX.\n", uniq_id[0], uniq_id[1], uniq_id[2]);
        printf("This board has MCU package code 0x%4lX.\n", (*((uint32_t *)0x1FFF7500)) & 0x1F );
        printf("This board has MCU flash size %lu kB.\n", (*((uint32_t *)0x1FFF75E0)) & 0xFFFF );

        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        printf("slept until %" PRIu32 "\n", xtimer_usec_from_ticks(xtimer_now()));

#ifdef LED0_ON
        LED0_TOGGLE;
        printf("LED0 toggle\n");
#endif
#ifdef LED1_ON
        LED1_TOGGLE;
        printf("LED1 toggle\n");
#endif
#ifdef LED2_ON
        LED2_TOGGLE;
        printf("LED2 toggle\n");
#endif
#ifdef LED3_ON
        LED3_TOGGLE;
        printf("LED3 toggle\n");
#endif

    }

    return 0;
}

