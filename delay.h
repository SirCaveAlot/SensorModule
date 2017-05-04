/*
 * delay.h
 *
 * Created: 5/3/2017 1:20:15 PM
 *  Author: marwa079
 */ 


#ifndef DELAY_H_
#define DELAY_H_

void Setup_timer2(void);

void Activate_or_deactivate_counter2(bool activate_count);

void delay(uint32_t u_seconds);

extern volatile uint8_t tot_overflow_send;

void delay_100_ms(void);

#endif /* DELAY_H_ */