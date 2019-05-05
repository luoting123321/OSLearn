#include "bootpack.h"

#define PIT_CTRL  0X0043
#define PIT_CNT0  0X0040

struct TIMERCTL timerctl;

void init_pit(void){
	io_out8(PIT_CTRL, 0X34);
	io_out8(PIT_CNT0, 0X9C);
	io_out8(PIT_CNT0, 0X2E);
	timerctl.count = 0;
	timerctl.timeout = 0;
	return;
}

void inthandler20(int *esp){
	io_out8(PIC0_OCW2, 0X60);
	timerctl.count++;
	if(timerctl.timeout > 0){
		timerctl.timeout -- ;
		if(timerctl.timeout == 0)
		{
			fifo8_put(timerctl.fifo, timerctl.data);
		}
	}
	return;
}

void settimer(unsigned int timeout, struct FIFO8 *fifo, unsigned char data){
	int eflags;
	eflags = io_load_eflags();
	io_cli();
	timerctl.timeout = timeout;
	timerctl.fifo = fifo;
	timerctl.data = data;
	io_store_eflags(eflags);
	return;
}