/**
 * @file xf_lp.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-04
 *
 * @copyright Copyright (c) 2025, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_lp.h"
#include "xf_sys.h"

#if XF_LP_IS_ENABLE

/* ==================== [Defines] =========================================== */

#define TAG "xf_lp"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static xf_lp_device_t *s_lp_device_list[XF_LP_PRIORITY_MAX];
static xf_lp_config_t s_lp_config;
static int s_lp_lock_value;

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_err_t xf_lp_configure(const xf_lp_config_t *config)
{
    if (config == NULL) {
        return XF_ERR_INVALID_ARG;
    }

    s_lp_config = *config;
    s_lp_lock_value = 0;
    xf_lp_sleep_enable_wakeup_source(XF_LP_SLEEP_WAKEUP_TIMER);
    return XF_OK;
}

xf_err_t xf_lp_lock_acquire(void)
{
    s_lp_lock_value++;
    return XF_OK;
}

xf_err_t xf_lp_lock_release(void)
{
    s_lp_lock_value--;
    return XF_OK;
}

int xf_lp_get_lock_value(void)
{
    return s_lp_lock_value;
}

xf_err_t xf_lp_register_device(xf_lp_device_t *device, uint32_t priority)
{
    if (device == NULL || priority >= XF_LP_PRIORITY_MAX) {
        return XF_ERR_INVALID_ARG;
    }

    device->next = NULL;
    if (s_lp_device_list[priority] == NULL) {
        s_lp_device_list[priority] = (xf_lp_device_t *)device;
    } else {
        // 添加device至链表尾部
        xf_lp_device_t *p = s_lp_device_list[priority];
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = (xf_lp_device_t *)device;
    }
    return XF_OK;
}

void xf_lp_run(uint32_t sleep_ms)
{
    if (s_lp_config.is_auto_sleep == false) {
        return;
    }

    if (s_lp_lock_value != 0) {
        XF_LOGD(TAG, "lock value is %d", s_lp_lock_value);
        return;
    }

    for (int i = XF_LP_PRIORITY_MAX - 1; i >= 0; i--) {
        xf_lp_device_t *p = s_lp_device_list[i];
        if (p == NULL) {
            continue;
        }
        for (; p != NULL; p = p->next) {
            p->suspend();
        }
    }

    xf_lp_sleep_timer_wakeup(sleep_ms * 1000);
#if XF_LP_CPU_MODE == 1
    xf_sys_cpu_stop();
#elif XF_LP_CPU_MODE == 2
    xf_sys_set_cpu_freq(s_lp_config.min_freq_mhz);
#endif
    xf_lp_light_sleep_start();
#if XF_LP_CPU_MODE == 1
    xf_sys_cpu_run(void);
#elif XF_LP_CPU_MODE == 2
    xf_sys_set_cpu_freq(s_lp_config.max_freq_mhz);
#endif

    for (int i = 0; i < XF_LP_PRIORITY_MAX; i++) {
        xf_lp_device_t *p = s_lp_device_list[i];
        if (p == NULL) {
            continue;
        }
        for (; p != NULL; p = p->next) {
            p->resume();
        }
    }

}

/* ==================== [Static Functions] ================================== */

#endif /* XF_LP_IS_ENABLE */
