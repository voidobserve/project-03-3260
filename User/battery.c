#include "battery.h"

//

// 将ad值转换为对应的电压值
u8 conver_adc_val_to_voltage(u16 arg_adc_val)
{
    // 假设使用5V参考电压，电池满电4.2V，
    // 在电池满电下，检测脚检测到的电压是 2.1V,对应的ad值是 1720
    /*
        电压值 == arg_adc_val * 42 / 10 / 1720
        转换成单片机可以计算的形式：
        arg_adc_val * 42 / 17200 ;
    */
    return (u32)arg_adc_val * MAX_VOLTAGE_OF_BATTERY / (MAX_VOLTAGE_OF_BATTERY_AD_VAL * 10);
}

// 将电池电压转换为对应的百分比
u8 conver_voltage_of_battery_to_percentage(u8 voltage)
{
    // 假设电池电压从2.3V ~ 4.2V
    // u8 percentage = voltage / (42-23);
    return voltage / (MAX_VOLTAGE_OF_BATTERY - MIN_VOLTAGE_OF_BATTERY);
}

void battery_scan(void)
{
    u8 voltage_of_battery = 0;        // 存放电池电压
    u8 cur_percentage_of_battery = 0; // 存放当前电池电量百分比

    static u32 battery_scan_cnt = 0;
    static u32 battery_scan_time_cnt = 0;
    static u32 battery_val = 0;

    // adc_val = adc_getval();
    // voltage_of_battery = conver_adc_val_to_voltage(adc_val);
    // cur_percentage_of_battery = conver_voltage_of_battery_to_percentage(voltage_of_battery);

    battery_val += adc_getval(); // 可能要防止计数溢出
    battery_scan_cnt++;
    battery_scan_time_cnt += ONE_CYCLE_TIME_MS;
    if (battery_scan_cnt >= BATTERY_SCAN_UPDATE_TIME_MS)
    {
        battery_val /= battery_scan_cnt; // 取平均数
        voltage_of_battery = conver_adc_val_to_voltage(battery_val);
        cur_percentage_of_battery = conver_voltage_of_battery_to_percentage(voltage_of_battery);
        battery_val = 0; // 清空数值
        battery_scan_cnt = 0; // 清空计数值
        battery_scan_time_cnt = 0; // 清空时间计数值

        fun_info.battery = cur_percentage_of_battery;
        fun_info.voltage_of_battery = voltage_of_battery;

#if USE_MY_DEBUG

printf("cur voltage of battery : \n");

#endif

        flag_get_voltage_of_battery = 1;
        flag_get_battery = 1;
    }
}