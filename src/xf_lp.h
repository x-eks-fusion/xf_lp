/**
 * @file xf_lp.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-04
 * 
 * @copyright Copyright (c) 2025, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_LP_H__
#define __XF_LP_H__

/* ==================== [Includes] ========================================== */

#include "xf_lp_config_internal.h"
#include "xf_lp_sleep.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

// 电源策略配置结构体
typedef struct _xf_lp_config_t{
    int max_freq_mhz; 	// CPU最大频率
    int min_freq_mhz; 	// CPU最小频率
    bool is_auto_sleep;	// 是否允许自动睡眠    
} xf_lp_config_t;


// 外设电源控制句柄
typedef struct _xf_lp_device_t{
    const char *name;            // 外设名称（如 "WiFi", "BT"）
    void (*suspend)(void);       // 挂起回调
    void (*resume)(void);        // 恢复回调
    struct _xf_lp_device_t *next;
} xf_lp_device_t;

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 配置cpu最大最小的频率，是否自动进入light_sleep
 * 
 * @param config 配置参数。@ref xf_lp_config_t
 * @return xf_err_t 返回 XF_OK 表示配置成功
 */
xf_err_t xf_lp_configure(const xf_lp_config_t* config);

/**
 * @brief 获取电源锁，保证锁定期间内，不会进入 light_sleep
 * 
 * @return xf_err_t 返回 XF_OK 表示锁定成功
 */
xf_err_t xf_lp_lock_acquire(void);

/**
 * @brief 释放电源锁，释放了所有电源锁后，才能进入 light_sleep
 * 
 * @return xf_err_t 返回 XF_OK 表示释放电源锁成功
 */
xf_err_t xf_lp_lock_release(void);

/**
 * @brief 获取电源锁数值，用于判断是否锁定
 * 
 * @return int 大于0表示锁定，等于0表示未锁定，小于0表示错误
 */
int xf_lp_get_lock_value(void);

/**
 * @brief 注册外设电源控制
 * 
 * @param device 注册外设控制参数。@ref xf_lp_device_t
 * @param priority 优先级
 * @return xf_err_t 返回 XF_OK 表示注册成功
 */
xf_err_t xf_lp_register_device(const xf_lp_device_t *device, uint32_t priority);

/**
 * @brief 启动低功耗
 * 
 * @param sleep_ms 休眠时间
 */
void xf_lp_enable(uint32_t sleep_ms);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_LP_H__
