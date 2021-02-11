
#include <arch/stdout/pl011.h>

int _main() {
        uart_pl011_init();

        uart_pl011_write("Hello World", 11);
        while(1);
        return 0;
}
