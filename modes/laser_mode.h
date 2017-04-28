/*
 * laser_mode.h
 *
 * Created: 4/3/2017 3:30:25 PM
 *  Author: marwa079
 */ 


#ifndef LASER_MODE_H_
#define LASER_MODE_H_

#include <stdint.h>
#include <stdbool.h>

void Setup_timer0(void);
extern uint16_t counter_val; 
extern volatile uint16_t tot_overflow;
//extern volatile uint16_t UART_data;

bool Steady_LIDAR_ang_vel(void);

void Activate_or_deactivate_counter0(bool activate_count);
void Laser_speed_mode(void);
uint16_t Calculate_angle(void);
uint16_t Single_reading_LIDAR(void);

void send_LIDAR_values(uint32_t delay_us);
void Activate_or_deactivate_hall2(bool act_or_de);

bool get_LIDAR_16bit_data(void);

#endif /* LASER_MODE_H_ */