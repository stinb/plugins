#include <stdint.h>

typedef float float32;

typedef struct {
    float32 X_m_s2;
    float32 Y_m_s2;
    float32 Z_m_s2;
} acc_data_t;

typedef struct {
    float32 accel_offset_x;
    float32 accel_offset_y;
    float32 accel_offset_z;
} azwheel_calc_calib_t;

void test_member_arithmetic(acc_data_t *p_acc_data, azwheel_calc_calib_t *p_azwheel_calc_calib)
{
    // These are valid - arithmetic on dereferenced member values, not pointer arithmetic
    float32 acc_x_offset1 = (p_acc_data->X_m_s2 - p_azwheel_calc_calib->accel_offset_x); /* UndCC_Valid */
    float32 acc_x_offset2 = (p_acc_data->X_m_s2 + p_azwheel_calc_calib->accel_offset_x); /* UndCC_Valid */

    acc_x_offset1 += p_azwheel_calc_calib->accel_offset_y; /* UndCC_Valid */
    acc_x_offset2 -= p_azwheel_calc_calib->accel_offset_z; /* UndCC_Valid */
}
