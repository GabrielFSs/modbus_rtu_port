#ifndef _HAL_STORAGE_H_
#define _HAL_STORAGE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ===== STATUS ===== */
typedef enum
{
    HAL_STORAGE_OK = 0,
    HAL_STORAGE_ERROR
} hal_storage_status_t;

/* ===== DRIVER API ===== */
typedef struct hal_storage_drv_imp_s
{
    hal_storage_status_t (*init)(void);
    hal_storage_status_t (*deinit)(void);

    hal_storage_status_t (*write_file)(
        const char *path,
        const uint8_t *data,
        size_t len);

    hal_storage_status_t (*read_file)(
        const char *path,
        uint8_t *data,
        size_t maxlen,
        size_t *out_len);

    /* 🔥 NOVO */
    hal_storage_status_t (*append_file)(
        const char *path,
        const uint8_t *data,
        size_t len);

} hal_storage_drv_imp_t;

/* ===== DRIVER INSTANCE ===== */
extern hal_storage_drv_imp_t HAL_STORAGE_DRV;

/* ===== HAL API ===== */
hal_storage_status_t hal_storage_init(void);
hal_storage_status_t hal_storage_deinit(void);

hal_storage_status_t hal_storage_write_file(
    const char *path,
    const uint8_t *data,
    size_t len);

hal_storage_status_t hal_storage_read_file(
    const char *path,
    uint8_t *data,
    size_t maxlen,
    size_t *out_len);

/* 🔥 NOVO */
hal_storage_status_t hal_storage_append_file(
    const char *path,
    const uint8_t *data,
    size_t len);

#endif
