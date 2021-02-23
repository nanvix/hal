
#include <arch/stdout/pl011.h>

void exception_svc(void)
{
        __asm__("svc #0xdead");
}

int _main() {
        uart_pl011_init();
        exception_svc();
        uart_pl011_write("Hello World", 11);
        while(1);
        return 0;
}



void teste() {
        uart_pl011_init();

        uart_pl011_write("Exception", 9);
}