#ifndef _HAL_STORAGE_H_
#define _HAL_STORAGE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ===== HANDLE OPACO ===== */
typedef struct hal_storage_drv_s *hal_storage_t;

/* ===== STATUS ===== */
typedef enum
{
    HAL_STORAGE_OK = 0,
    HAL_STORAGE_ERROR,
    HAL_STORAGE_BUSY,
    HAL_STORAGE_TIMEOUT,
    HAL_STORAGE_INVALID
} hal_storage_status_t;

/* ===== STORAGE TYPE (informativo) ===== */
typedef enum
{
    HAL_STORAGE_FLASH,
    HAL_STORAGE_SD,
    HAL_STORAGE_EEPROM,
    HAL_STORAGE_RAM
} hal_storage_type_t;

/* ===== CONFIG ===== */
typedef struct
{
    hal_storage_type_t type;

    uint32_t block_size;     /* ex: 512 bytes */
    uint32_t block_count;    /* número total de blocos */

    bool read_only;
} hal_storage_cfg_t;

/* ===== DRIVER CALLBACKS ===== */
typedef struct hal_storage_drv_imp_s
{
    hal_storage_status_t (*init)(void);
    hal_storage_status_t (*open)(hal_storage_t *dev, const hal_storage_cfg_t *cfg);
    void (*close)(hal_storage_t dev);

    hal_storage_status_t (*read)(
        hal_storage_t dev,
        uint32_t block,
        uint8_t *data,
        uint32_t count);

    hal_storage_status_t (*write)(
        hal_storage_t dev,
        uint32_t block,
        const uint8_t *data,
        uint32_t count);

    hal_storage_status_t (*erase)(
        hal_storage_t dev,
        uint32_t block,
        uint32_t count);

    hal_storage_status_t (*sync)(hal_storage_t dev);
} hal_storage_drv_imp_t;

/* ===== DRIVER INSTANCE ===== */
extern hal_storage_drv_imp_t HAL_STORAGE_DRV;

/* ===== HAL API ===== */
hal_storage_status_t hal_storage_init(void);

hal_storage_status_t hal_storage_open(
    hal_storage_t *dev,
    const hal_storage_cfg_t *cfg);

void hal_storage_close(hal_storage_t dev);

hal_storage_status_t hal_storage_read(
    hal_storage_t dev,
    uint32_t block,
    uint8_t *data,
    uint32_t count);

hal_storage_status_t hal_storage_write(
    hal_storage_t dev,
    uint32_t block,
    const uint8_t *data,
    uint32_t count);

hal_storage_status_t hal_storage_erase(
    hal_storage_t dev,
    uint32_t block,
    uint32_t count);

hal_storage_status_t hal_storage_sync(hal_storage_t dev);

#endif /* _HAL_STORAGE_H_ */
