/**
 ******************************************************************************
 * @file    STM32RTC.h
 * @author  Zlobin Alexey
 * @brief   Provides a RTC interface for Arduino
 *
 ******************************************************************************
 */

#ifndef STM32_RTC_H_
#define STM32_RTC_H_

#include "Arduino.h"
#if defined(STM32_CORE_VERSION) && (STM32_CORE_VERSION  > 0x01090000)
	#include "rtc.h"
#endif
// Check if RTC HAL enable in variants/board_name/stm32yzxx_hal_conf.h
#ifndef HAL_RTC_MODULE_ENABLED
	#error "RTC configuration is missing. Check flag HAL_RTC_MODULE_ENABLED in variants/board_name/stm32yzxx_hal_conf.h"
#endif

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef long unsigned int ul32;
typedef unsigned long  u64;
typedef signed char    i8;
typedef signed short   i16;
typedef signed int     i32;
typedef signed long    i64;


#define IS_CLOCK_SOURCE(SRC) (((SRC) == LSI_CLOCK) || ((SRC) == LSE_CLOCK) || \
                              ((SRC) == HSI_CLOCK) || ((SRC) == HSE_CLOCK))
#define IS_HOUR_FORMAT(FMT)  (((FMT) == HOUR_FORMAT_12) || ((FMT) == HOUR_FORMAT_24))

typedef struct
{
	u8 hours;
	u8 minutes;
	u8 seconds;
	ul32 sub_seconds;
	u8 year;
	u8 month;
	u8 day;
	u8 wday;
	hourFormat_t format;
	hourAM_PM_t period;

	u8 alarm_day;
	u8 alarm_hours;
	u8 alarm_minutes;
	u8 alarm_seconds;
	ul32 alarm_sub_seconds;
	hourAM_PM_t alarm_period;
	alarmMask_t alarm_mask;
	bool alarm_enabled;

	sourceClock_t source_clock;

	bool _configured;	
	bool _reset;

} STM32RTC;



// CONTROL FUNCTIONS

/**
  * @brief Initialization default values of RTC
  * 
  * @param rtc: RTC
  * @retval None
  */
void
stm32rtc_init(STM32RTC *rtc);

/**
 * @brief Initialize RTC on chip
 * 
 * @param rtc: RTC
 * @param hour_fmt: Hour format: HOUR_FORMAT_24 or HOUR_FORMAT_12
 * @retval None
 */
void
stm32rtc_begin
(
	STM32RTC *rtc,
	hourFormat_t hour_fmt
);

/**
 * @brief Resets and reconfigure RTC
 * 
 * @param rtc: RTC
 * @param hour_fmt: Hour format: HOUR_FORMAT_24 or HOUR_FORMAT_12
 * @retval None
 */
void
stm32rtc_reset
(
	STM32RTC *rtc,
	hourFormat_t hour_fmt
);

/**
 * @brief Deinitialize and stop the RTC
 * 
 * @param rtc: RTC
 * @retval None
 */
void
stm32rtc_end(STM32RTC *rtc);

/**
 * @brief Enable RTC alarm
 * 
 * @param rtc: RTC
 * @param mask: Alarm configuration
 * @retval None
 */
void
stm32rtc_enable_alarm
(
	STM32RTC *rtc,
	alarmMask_t mask
);

/**
 * @brief Disable RTC alarm
 * 
 * @param rtc: RTC
 * @retval None
 */
void
stm32rtc_disable_alarm(STM32RTC *rtc);

/**
 * @brief Attach callback to RTC allarm intrerrupt
 * 
 * @param rtc: RTC
 * @param callback: Pointer to callback
 * @param data: Data
 * @retval None
 */
void
stm32rtc_attach_intrerrupt
(
	STM32RTC *rtc,
	voidCallbackPtr callback,
	void *data
);

/**
 * @brief Detach RTC alarm callback
 * 
 * @param rtc: RTC
 * @retval None
 */
void
stm32rtc_detach_intrerrupt(STM32RTC *rtc);

#ifdef ONESECOND_IRQn
	// Other mcu than stm32F1 will use the WakeUp feature to interrupt each second.

	/**
	 * @brief Attach callback to RTC Seconds intreerrupt
	 * 
	 * @param rtc: RTC
	 * @param callback: Pointer to callback
	 * @retval None
	 */
	void
	stm32rtc_attach_secconds_intrerrupt
	(
		STM32RTC *rtc,
		voidCallbackPtr callback
	);

	/**
	 * @brief Detach callback to RTC Seconds intrerrupt
	 * 
	 * @param rtc: RTC
	 *
	 */
	void
	stm32rtc_detach_seconds_intrerrupt(STM32RTC *rtc);

#endif /* ONESECOND_IRQn */







// GET FUNCTIONS

/**
 * @brief Get RTC clock sourse
 * 
 * @param rtc: RTC
 * @retval Clock sourse: LSI_CLOCK or HSI_CLOCK
 *                    or LSE_CLOCK or HSE_CLOCK
 */
sourceClock_t
stm32rtc_get_source_clock(STM32RTC *rtc);

/**
 * @brief Get RTC subseconds
 * 
 * @param rtc: RTC
 * @retval Return the current subseconds from the RTC.
 */
ul32
stm32rtc_get_sub_seconds(STM32RTC *rtc);

/**
 * @brief Get RTC seconds
 * 
 * @param rtc: RTC
 * @retval Return the current seconds from the RTC.
 */
u8
stm32rtc_get_seconds(STM32RTC *rtc);

/**
 * @brief Get RTC minutes
 * 
 * @param rtc: RTC
 * @retval Return the current minutes from the RTC.
 */
u8
stm32rtc_get_minutes(STM32RTC *rtc);

/**
 * @brief Get RTC hours
 * 
 * @param rtc: RTC
 * @param period: Optional (default: NULL)
 *                Pointer to the current hour period set in the RTC: AM or PM
 * @retval Return the current hours from the RTC.
 */
u8
stm32rtc_get_hours
(
	STM32RTC *rtc,
	hourAM_PM_t *period
);

/**
 * @brief Get RTC time
 * 
 * @param rtc: RTC
 * @param hours: Pointer to the current hours
 * @param minutes: Pointer to the current minutes
 * @param seconds: Pointer to the current seconds
 * @param sub_seconds: Pointer to the current subseconds
 * @param period: Optional (default: NULL)
 *                Pointer to the current hour period set in the RTC: AM or PM
 * @retval None
 */
void
stm32rtc_get_time
(
	STM32RTC *rtc,
	u8 *hours,
	u8 *minutes,
	u8 *seconds,
	ul32 *sub_seconds,
	hourAM_PM_t *period
);

/**
 * @brief Get RTC day of week
 * 
 * @param rtc: RTC
 * @retval Return the current day of week from the RTC.
 */
u8
stm32rtc_get_week_day(STM32RTC *rtc);

/**
 * @brief Get RTC day
 * 
 * @param rtc: RTC
 * @retval Return the current day from the RTC.
 */
u8
stm32rtc_get_day(STM32RTC *rtc);

/**
 * @brief Get RTC month
 * 
 * @param rtc: RTC
 * @retval Return the current month from the RTC.
 */
u8
stm32rtc_get_month(STM32RTC *rtc);

/**
 * @brief Get RTC year
 * 
 * @param rtc: RTC
 * @retval Return the current year from the RTC.
 */
u8
stm32rtc_get_year(STM32RTC *rtc);

/**
 * @brief Get RTC date
 * 
 * @param rtc: RTC
 * @param wday: Pointer to the current day of week
 * @param day: Pointer to the current day
 * @param month: Pointer to the current month
 * @param year: Pointer to the current year
 * @retval None
 */
void
stm32rtc_get_date
(
	STM32RTC *rtc,
	u8 *wday,
	u8 *day,
	u8 *month,
	u8 *year
);





// SET FUNCTIONS

/**
 * @brief Set RTC source clock source. By default LSI clock is selected. This
 *        method must be called before stm32rtc_begin().
 * 
 * @param rtc: RTC
 * @param source: 
 */
void
stm32rtc_set_source_clock
(
	STM32RTC *rtc,
	sourceClock_t source
);

/**
 * @brief Set RTC subseconds
 * 
 * @param rtc: RTC
 * @param sub_seconds: 0-999
 * @retval None
 */
void
stm32rtc_set_sub_seconds
(
	STM32RTC *rtc,
	ul32 sub_seconds
);

/**
 * @brief Set RTC sseconds
 * 
 * @param rtc: RTC
 * @param seconds: 0-59
 * @retval None
 */
void
stm32rtc_set_seconds
(
	STM32RTC *rtc,
	u8 seconds
);

/**
 * @brief Set RTC minutes
 * 
 * @param rtc: RTC
 * @param minutes: 0-59
 * @retval None
 */
void
stm32rtc_set_minutes
(
	STM32RTC *rtc,
	u8 minutes
);

/**
 * @brief Set RTC hours
 * 
 * @param rtc: RTC
 * @param hours: 0-23
 * @retval None
 */
void
stm32rtc_set_hours
(
	STM32RTC *rtc,
	u8 hours
);

/**
 * @brief Set RTC hours in 12H format
 * 
 * @param rtc: RTC
 * @param hours: 0-11
 * @param period: Hour period: HOUR_AM or HOUR_PM
 * @retval None
 */
void
stm32rtc_set_hours_12
(
	STM32RTC *rtc,
	u8 hours,
	hourAM_PM_t period
);

/**
 * @brief Set RTC time
 * 
 * @param rtc: RTC
 * @param hours: 0-23
 * @param minutes: 0-59
 * @param seconds: 0-59
 * @param sub_seconds: 0-999
 * @retval None
 */
void
stm32rtc_set_time
(
	STM32RTC *rtc,
	u8 hours,
	u8 minutes,
	u8 seconds,
	ul32 sub_seconds
);

/**
 * @brief Set RTC time in 12H format
 * 
 * @param rtc: RTC
 * @param hours: 0-23
 * @param minutes: 0-59
 * @param seconds: 0-59
 * @param sub_seconds: 0-999
 * @param period: Hour period: HOUR_AM or HOUR_PM
 * @retval None
 */
void
stm32rtc_set_time_12
(
	STM32RTC *rtc,
	u8 hours,
	u8 minutes,
	u8 seconds,
	ul32 sub_seconds,
	hourAM_PM_t period
);

/**
 * @brief Set RTC day of week
 * 
 * @param rtc: RTC
 * @param wday: 1-7 (Monday first)
 * @retval None
 */
void
stm32rtc_set_week_day
(
	STM32RTC *rtc,
	u8 wday
);

/**
 * @brief Set RTC day
 * 
 * @param rtc: RTC
 * @param day: 1-31
 * @retval None
 */
void
stm32rtc_set_day
(
	STM32RTC *rtc,
	u8 day
);

/**
 * @brief Set RTC month
 * 
 * @param rtc: RTC
 * @param month: 1-12
 * @retval None
 */
void
stm32rtc_set_month
(
	STM32RTC *rtc,
	u8 month
);

/**
 * @brief Set RTC year
 * 
 * @param rtc: RTC
 * @param year: 0-99
 * @retval None
 */
void
stm32rtc_set_year
(
	STM32RTC *rtc,
	u8 year
);

/**
 * @brief Set RTC date
 * 
 * @param rtc: RTC
 * @param day: 1-31
 * @param month: 1-12
 * @param year: 0-99
 * @retval None
 */
void
stm32rtc_set_date
(
	STM32RTC *rtc,
	u8 day,
	u8 month,
	u8 year
);

/**
 * @brief Set RTC date with day of week
 * 
 * @param rtc: RTC
 * @param day: 1-31
 * @param month: 1-12
 * @param year: 0-99
 * @param wday: 1-7 (Monday first)
 * @retval None
 */
void
stm32rtc_set_wdate
(
	STM32RTC *rtc,
	u8 day,
	u8 month,
	u8 year,
	u8 wday
);



// SYNCHRONISATION FUNCTIONS

/**
 * @brief Synchronise the time from the current RTC one
 * 
 * @param rtc: RTC
 * @retval None
 */
void
stm32rtc_sync_time(STM32RTC *rtc);

/**
 * @brief Synchronise the date from the current RTC one
 * 
 * @param rtc: RTC
 * @retval None
 */
void
stm32rtc_sync_date(STM32RTC *rtc);

/**
 * @brief Synchronise the alarm time from the current RTC one
 * 
 * @param rtc: RTC
 * @retval None
 */
void
stm32rtc_sync_alarm_time(STM32RTC *rtc);

#endif /* STM32_RTC_H_ */