
#include <arch/stdout/pl011.h>
#include <nanvix/const.h>

void exception_svc(void)
{
        __asm__("svc #0xdead");
}

PUBLIC void _main(int argc, const char *argv[]) {
        argc = argc;
        argv = argv;
        //uart_pl011_init();
        //exception_svc();
        uart_pl011_write("Hello World\n", 12);
        while(true) {};
}


void exception_handler()
{       
	uart_pl011_write("Erro\n", 6);
	while(1) {}
        
}

void exception_handler_return()
{
	uart_pl011_write("Erro\n", 6);
}
