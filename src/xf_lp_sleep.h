/**
 * @file xf_lp_sleep.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-05
 *
 * @copyright Copyright (c) 2025, CorAL. All rights reserved.
 *
 */

#ifndef __XF_LP_SLEEP_H__
#define __XF_LP_SLEEP_H__

/* ==================== [Includes] ========================================== */

#include "xf_lp_config_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

typedef enum _xf_lp_sleep_source_t {
    XF_LP_SLEEP_WAKEUP_UNDEFINED = 0,
    XF_LP_SLEEP_WAKEUP_ALL,
    XF_LP_SLEEP_WAKEUP_TIMER,
    XF_LP_SLEEP_WAKEUP_GPIO,
} xf_lp_sleep_source_t;

/* ==================== [Global Prototypes] ================================= */

xf_err_t xf_lp_sleep_disable_wakeup_source(xf_lp_sleep_source_t source);

xf_err_t xf_lp_sleep_timer_wakeup(uint64_t us);

xf_err_t xf_lp_sleep_gpio_wakeup(int pin, int level);

xf_err_t xf_lp_sleep_start(void);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_LP_SLEEP_H__
