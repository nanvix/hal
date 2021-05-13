
#include <arch/stdout/pl011.h>
#include <nanvix/hal/core/exception.h>
#define __NEED_CORE_ARM64
#include <arch/core/arm64.h>
#include <nanvix/const.h>

void svc(const struct exception *excp, const struct context *ctx) {
        UNUSED(ctx);
        UNUSED(excp);
        kprintf("SVC\n");
}

void exception_svc(void)
{
        //unsigned char *p = (unsigned char *) 0x4badbeef;

	/* Triggers the DTLB fault. */
	//*p = 0xfe;
        exceptions[21].handler = svc;
        __asm__("svc #1");
}

void teste_(void){
        __asm__ __volatile__ (
			"mrs x1, CNTFRQ_EL0\n;"
			"msr CNTP_TVAL_EL0, x1\n;"
                        "mov x0, 1\n;"
                        "msr CNTP_CTL_EL0, x0\n;"
		);
}

PUBLIC void _main(int argc, const char *argv[]) {
        argc = argc;
        argv = argv;
        uart_pl011_init();
        arm64_timer_init(10);
        //teste_();
        uart_pl011_write("Hello World\n", 12);
        exception_svc();
        while(true) {
                uart_pl011_write("To na main\n", 12);
                for (int i = 0; i < 1000000; i++);
        };
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
