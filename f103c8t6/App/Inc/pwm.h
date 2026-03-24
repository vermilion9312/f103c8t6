/*
 * pwm.h
 *
 *  Created on: Mar 16, 2026
 *      Author: LeeJooHo
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include <common.h>

typedef enum {
	CH1,
	CH2,
	CH3,
	CH4
} PWM_Channel;

typedef struct _PWM PWM;
struct _PWM {
	TIM_TypeDef* tim;
	volatile uint32_t* channel;
	uint32_t ccrMin;
	uint32_t ccrMax;
};

void PWM_Init(PWM*, TIM_TypeDef*, PWM_Channel, uint32_t, uint32_t);
void PWM_SetAngle(PWM*, uint8_t);
void PWM_SetCcr(PWM* this, uint32_t ccr);

#endif /* INC_PWM_H_ */
