/*
 * app_main.c
 *
 *  Created on: Mar 16, 2026
 *      Author: LeeJooHo
 */


#include <app_main.h>
#include <output.h>
#include <pwm.h>
#include <math.h>

#define MIN_CCR 500
#define MAX_CCR 2500

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

static Output led;

static PWM pwm1Ch1;
static PWM pwm1Ch2;
static PWM pwm1Ch3;
static PWM pwm1Ch4;

static PWM pwm2Ch1;
static PWM pwm2Ch2;
static PWM pwm2Ch3;
static PWM pwm2Ch4;

static PWM pwm3Ch1;
static PWM pwm3Ch2;
static PWM pwm3Ch3;
static PWM pwm3Ch4;


static void Stepper_Stop(void);
static void Stepper_StartRotation(void);
static void Stepper_StartPause(void);
static void Stepper_Process(void);

void App_Init(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);  /* STEP */
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);  /* DIR */

	/* TIM1_CH3, CH4는 사용하지 않음 */
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0U);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0U);

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

	Stepper_StartRotation();
}

#define STEPPER_FULL_STEP_ANGLE 1.8f
#define STEPPER_ROTATE_DEGREES 180u
#define STEPPER_ROTATE_DURATION_MS 2000u
#define STEPPER_PAUSE_DURATION_MS 2000u
#define STEPPER_ROTATE_STEPS ((uint32_t)(STEPPER_ROTATE_DEGREES / STEPPER_FULL_STEP_ANGLE + 0.5f))
#define STEPPER_STEP_INTERVAL_MS (STEPPER_ROTATE_DURATION_MS / STEPPER_ROTATE_STEPS)
#define STEPPER_ACCEL_STEPS 20u

typedef enum {
	STEPPER_STATE_ROTATE,
	STEPPER_STATE_PAUSE
} StepperState;

static StepperState stepperState = STEPPER_STATE_ROTATE;
static uint32_t stepperNextTick = 0;
static uint32_t stepperStepCount = 0;
static uint8_t stepperDirection = 0;  /* 0: CW, 1: CCW */

static void Stepper_SetDirection(uint8_t dir)
{
	stepperDirection = dir;
	/* TIM1_CH2을 DIR 신호로 사용 */
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, dir ? htim1.Instance->ARR : 0U);
}

static void Stepper_Step(void)
{
	/* TIM1_CH1을 STEP 펄스로 사용 - 짧은 펄스 발생 */
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, htim1.Instance->ARR);
	HAL_Delay(1);  /* 1ms STEP 펄스 */
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0U);
}

static uint32_t Stepper_GetStepInterval(uint32_t stepCount)
{
	uint32_t totalSteps = STEPPER_ROTATE_STEPS;
	uint32_t accelSteps = STEPPER_ACCEL_STEPS;

	if (stepCount < accelSteps) {
		return STEPPER_STEP_INTERVAL_MS * 3;  /* 더 느리게 시작 */
	} else if (stepCount > (totalSteps - accelSteps)) {
		return STEPPER_STEP_INTERVAL_MS * 3;  /* 더 느리게 종료 */
	} else {
		return STEPPER_STEP_INTERVAL_MS;
	}
}

static void Stepper_Stop(void)
{
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0U);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0U);
}

static void Stepper_StartRotation(void)
{
	stepperState = STEPPER_STATE_ROTATE;
	stepperStepCount = 0;
	stepperDirection = 0;  /* CW 방향으로 시작 */
	Stepper_SetDirection(stepperDirection);
	stepperNextTick = HAL_GetTick();
}

static void Stepper_StartPause(void)
{
	stepperState = STEPPER_STATE_PAUSE;
	Stepper_Stop();
	stepperNextTick = HAL_GetTick() + STEPPER_PAUSE_DURATION_MS;
}

static void Stepper_Process(void)
{
	uint32_t now = HAL_GetTick();

	if ((int32_t)(now - stepperNextTick) < 0)
		return;

	if (stepperState == STEPPER_STATE_ROTATE)
	{
		Stepper_Step();
		stepperStepCount++;
		stepperNextTick = now + Stepper_GetStepInterval(stepperStepCount);

		if (stepperStepCount >= STEPPER_ROTATE_STEPS)
		{
			Stepper_StartPause();
		}
	}
	else
	{
		Stepper_StartRotation();
	}
}

void App_Loop(void)
{
	Stepper_Process();
}
