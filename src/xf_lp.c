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

xf_err_t xf_lp_register_device(const xf_lp_device_t *device, uint32_t priority)
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

void xf_lp_enable(uint32_t sleep_ms)
{
    if (s_lp_lock_value != 0) {
        XF_LOGD(TAG, "lock value is %d", s_lp_lock_value);
        return;
    }

    for (int i = 0; i < XF_LP_PRIORITY_MAX; i++) {
        xf_lp_device_t *p = s_lp_device_list[i];
        while (p->next != NULL) {
            p->suspend();
            p = p->next;
        }
    }

    xf_lp_sleep_timer_wakeup(sleep_ms * 1000);
    xf_sys_set_cpu_freq(s_lp_config.min_freq_mhz);
    xf_lp_sleep_start();
    xf_sys_set_cpu_freq(s_lp_config.max_freq_mhz);

    for (int i = XF_LP_PRIORITY_MAX - 1; i >= 0; i--) {
        xf_lp_device_t *p = s_lp_device_list[i];
        while (p->next != NULL) {
            p->resume();
            p = p->next;
        }
    }
}

/* ==================== [Static Functions] ================================== */
