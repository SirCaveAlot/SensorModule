/*
 * drive_mode.h
 *
 * Created: 4/25/2017 8:57:28 AM
 *  Author: marwa079
 */ 


#ifndef DRIVE_MODE_H_
#define DRIVE_MODE_H_

extern volatile uint8_t tot_overflow_send;

void Setup_timer2(void);

void Activate_or_deactivate_counter2(bool activate_count);

#endif /* DRIVE_MODE_H_ */