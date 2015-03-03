#include "string.h"
#include "stdio.h"
#include "Cons_3032.h"
#include <Load_3032.h>

// ****************
int main(void) {
	int adval;
	init_load();
	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1);  // Capture error
	}
	while(1) {

		adval=read_load();
		printf("%.2f\n",adval/100.0- 25.4 );//(adval/10.0-250)/3.5);
		systick_delay(500);
		}

	return 0 ;
}
