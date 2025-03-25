/**
 * @file xf_lp_config_internal.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-04
 *
 * @copyright Copyright (c) 2025, CorAL. All rights reserved.
 *
 */

#ifndef __XF_LP_CONFIG_INTERNAL_H__
#define __XF_LP_CONFIG_INTERNAL_H__

/* ==================== [Includes] ========================================== */

#include "xf_lp_config.h"
#include "xf_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#if (!defined(XF_LP_ENABLE)) || (XF_LP_ENABLE) || defined(__DOXYGEN__)
#   define XF_LP_IS_ENABLE      (1)
#else
#   define XF_LP_IS_ENABLE      (0)
#endif

// 设置低功耗外设件的最大优先级
#ifndef XF_LP_PRIORITY_MAX
#define XF_LP_PRIORITY_MAX 4
#endif

// 设置低功耗CPU策略，默认为降频策略
#ifdef XF_LP_CPU_MODE
#error "user cant define XF_LP_CPU_MODE macro"
#endif

#if defined(XF_LP_CPU_MODE_NONE) // 无策略
#define XF_LP_CPU_MODE 0
#elif defined(XF_LP_CPU_MODE_SWITCH_CPU) // 开关 CPU
#define XF_LP_CPU_MODE 1
#elif defined(XF_LP_CPU_MODE_REDUCE_FREQ) // 降频策略
#define XF_LP_CPU_MODE 2
#else
#define XF_LP_CPU_MODE 2
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_LP_CONFIG_INTERNAL_H__
