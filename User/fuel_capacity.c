#include "fuel_capacity.h"

volatile u32 fuel_capacity_scan_cnt = 0;

volatile u32 fuel_adc_val = 0;
volatile u32 fuel_adc_scan_cnt = 0; // 在更新时间到来前，记录adc扫描的次数

volatile u16 adc_cmp_val_left = 0;
volatile u16 adc_cmp_val_right = 0;

volatile u8 fuel_percent = 0;

void fuel_capacity_scan(void)
{
    adc_val = adc_getval();
    // printf("fuel adc %u \n", adc_val);

    fuel_adc_val += adc_val;
    fuel_adc_scan_cnt++;

    fuel_capacity_scan_cnt += ONE_CYCLE_TIME_MS;
    if (fuel_capacity_scan_cnt >= FUEL_CAPACITY_SCAN_TIME_MS)
    {
        // 如果到了扫描更新时间，
        fuel_capacity_scan_cnt = 0;
        fuel_adc_val /= fuel_adc_scan_cnt; // 求出扫描时间内得到的ad平均值
        fuel_adc_scan_cnt = 0;
        printf("fuel adc val %lu \n", fuel_adc_val);

#if (FUEL_MAX_ADC_VAL > FUEL_MIN_ADC_VAL)
        // 如果满油量对应的ad值大于0油量对应的ad值

        fun_info.fuel = fuel_adc_val / ((FUEL_MAX_ADC_VAL - FUEL_MIN_ADC_VAL) / 100);
#else
        // 如果满油量对应的ad值小于0油量对应的ad值

        // if (fuel_adc_val > FUEL_MIN_ADC_VAL - FUEL_DELTA_ADC_VAL)
        // {
        //     // 大于最小油量对应的ad值-死区值时，认为是最小油量
        //     fuel_adc_val = FUEL_MIN_ADC_VAL;
        // }
        // else if (fuel_adc_val < FUEL_MAX_ADC_VAL + FUEL_DELTA_ADC_VAL)
        // {
        //     // 小于最大油量对应的ad值+死区时，认为是最大油量
        //     fuel_adc_val = FUEL_MAX_ADC_VAL;
        // }

        adc_cmp_val_left = FUEL_MIN_ADC_VAL - FUEL_DELTA_ADC_VAL;
        // adc_cmp_val_right = FUEL_MIN_ADC_VAL;
        if (fuel_adc_val >= adc_cmp_val_left)
        {
            // 如果油量已经接近0%：
            fuel_percent = 0;
        }
        else
        {
            // 如果不是0%油量
            // 判断当前油量是否在10% ~ 100%
            u8 i;
            for (i = 1; i < 9; i++)
            {
                //                   满油量和零油量的差值 * i / 10 - 阈值 + 满油量时对应的ad值()
                // adc_cmp_val_left = (FUEL_MIN_ADC_VAL - FUEL_MAX_ADC_VAL) * i / 10 - FUEL_DELTA_ADC_VAL + FUEL_MAX_ADC_VAL;
                // adc_cmp_val_right = (FUEL_MIN_ADC_VAL - FUEL_MAX_ADC_VAL) * (i + 1) / 10 + FUEL_DELTA_ADC_VAL + FUEL_MAX_ADC_VAL;

                adc_cmp_val_left = FUEL_MAX_ADC_VAL +                                  /* 基数 */
                                   ((FUEL_MIN_ADC_VAL - FUEL_MAX_ADC_VAL) * i / 10) -  /* 级数--第i级 */
                                   ((FUEL_MIN_ADC_VAL - FUEL_MAX_ADC_VAL) * 7 / 100);  /* 阈值 */
                adc_cmp_val_right = FUEL_MAX_ADC_VAL +                                 /* 基数 */
                                    ((FUEL_MIN_ADC_VAL - FUEL_MAX_ADC_VAL) * i / 10) + /* 级数--第i级 */
                                    ((FUEL_MIN_ADC_VAL - FUEL_MAX_ADC_VAL) * 7 / 100); /* 阈值 */

                // 判断油量是否接近100%
                if (fuel_adc_val <= FUEL_MAX_ADC_VAL + FUEL_DELTA_ADC_VAL)
                {
                    fuel_percent = 100;
                    break;
                }

                if (fuel_adc_val >= adc_cmp_val_left &&
                    fuel_adc_val <= adc_cmp_val_right)
                {
                    fuel_percent = 100 - i * 10; //
                    break;
                }
            }
        }

        printf("fuel percent %bu\n", fuel_percent);

#endif

        // printf("fuel %u%% \n", (u16)fun_info.fuel);

        fuel_adc_val = 0;
        flag_get_fuel = 1;
    }
}
