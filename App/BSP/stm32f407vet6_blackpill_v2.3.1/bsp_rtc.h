#ifndef _BSP_RTC_H_
#define _BSP_RTC_H_

#include <stdbool.h>

/* Inicializa clock do RTC (LSE ou LSI) */
bool bsp_rtc_init(void);

#endif /* _BSP_RTC_H_ */
