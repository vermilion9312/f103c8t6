/*
 * pwm.c
 *
 *  Created on: Mar 16, 2026
 *      Author: LeeJooHo
 */


#include <pwm.h>

void PWM_Init(PWM* this, TIM_TypeDef* tim, PWM_Channel channerl, uint32_t ccrMin, uint32_t ccrMax, MotorDirection direction)
{
	this->tim       = tim;
	this->ccrMin    = ccrMin;
	this->ccrMax    = ccrMax;
	this->direction = direction;

	switch (channerl)
	{
	case CH1: this->channel = &(this->tim->CCR1); break;
	case CH2: this->channel = &(this->tim->CCR2); break;
	case CH3: this->channel = &(this->tim->CCR3); break;
	case CH4: this->channel = &(this->tim->CCR4); break;
	default: break;
	}
}

void PWM_SetAngle(PWM* this, uint8_t angle)
{
    if (angle > 180) angle = 180;

    uint8_t targetAngle = (this->direction == CCW) ? (180 - angle) : angle;

    uint32_t range = this->ccrMax - this->ccrMin;
	uint32_t offset = (range * targetAngle) / 180;

	*(this->channel) = this->ccrMin + offset;
}

void PWM_SetCcrMin(PWM* this)
{
	*(this->channel) = this->ccrMin;
}

void PWM_SetCcrMax(PWM* this)
{
	*(this->channel) = this->ccrMax;
}

void PWM_SetCcr(PWM* this, uint32_t ccr)
{
	*(this->channel) = ccr;
}
