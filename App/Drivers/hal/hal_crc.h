#ifndef _HAL_CRC_H_
#define _HAL_CRC_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ===== CRC TYPE ===== */
typedef enum
{
    HAL_CRC_16,     /* Ex: Modbus RTU */
    HAL_CRC_32
} hal_crc_type_t;

/* ===== CRC POLYNOMIAL (opcional) ===== */
typedef enum
{
    HAL_CRC_POLY_DEFAULT,
    HAL_CRC_POLY_MODBUS,   /* 0xA001 */
    HAL_CRC_POLY_CCITT,
    HAL_CRC_POLY_CUSTOM
} hal_crc_poly_t;

/* ===== CONFIG ===== */
typedef struct
{
    hal_crc_type_t type;
    hal_crc_poly_t poly;
    uint32_t       init_value;
    uint32_t       xor_out;
    bool           reflect_in;
    bool           reflect_out;
} hal_crc_cfg_t;

/* ===== OPAQUE HANDLE ===== */
typedef struct hal_crc_drv_s *hal_crc_drv_t;

/* ===== DRIVER INTERFACE ===== */
typedef struct
{
    void (*init)(void);
    void (*deinit)(void);

    hal_crc_drv_t (*open)(const hal_crc_cfg_t *cfg);
    void (*close)(hal_crc_drv_t crc);

    uint32_t (*compute)(hal_crc_drv_t crc,
                        const uint8_t *data,
                        size_t len);

} hal_crc_drv_imp_t;

/* ===== DRIVER INSTANCE ===== */
extern hal_crc_drv_imp_t HAL_CRC_DRV;

/* ===== HAL API ===== */
void hal_crc_init(void);
void hal_crc_deinit(void);

hal_crc_drv_t hal_crc_open(const hal_crc_cfg_t *cfg);
void hal_crc_close(hal_crc_drv_t crc);

uint32_t hal_crc_compute(hal_crc_drv_t crc,
                         const uint8_t *data,
                         size_t len);

#endif /* _HAL_CRC_H_ */
