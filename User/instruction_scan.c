// 扫描是否有指令，并根据指令发送对应的数据，
// 或者是当某一个功能的状态有更新，发送对应的数据
#include "instruction_scan.h"

// 用bit定义，来节省空间
volatile bit flag_get_all_status = 0;   // 获取所有功能的状态
volatile bit get_gear = 0;        // 获取挡位状态 / 得到了挡位的状态
volatile bit get_battery = 0;     // 获取电池状态 / 得到了电池的状态（电池电量，单位：百分比）
volatile bit get_brake = 0;       // 获取刹车状态 / 得到了刹车的状态
volatile bit get_left_turn = 0;    // 获取左转向灯的状态 / 得到了左转向灯的状态
volatile bit get_right_turn = 0;   // 获取右转向灯的状态 / 得到了右转向灯的状态
volatile bit get_high_beam = 0;    // 获取远光灯的状态 / 得到了远光灯的状态
volatile bit get_engine_speed = 0; // 获取发动机的转速 / 得到了发动机的转速
volatile bit flag_get_speed = 0;       // 获取时速 / 得到了时速
volatile bit getFuel = 0;        // 获取油量 / 得到了油量（单位：百分比）
volatile bit getWaterTemp = 0;   // 获取水温 / 得到了水温

volatile bit getTotalMileage = 0;    // 获取大计里程 / 得到了大计里程
volatile bit getSubTotalMileage = 0; // 获取小计里程 / 得到了小计里程

void instruction_handle(void)
{
    if (flag_get_all_status)
    {
        // 如果要获取所有功能的状态
        flag_get_all_status = 0; // 清除标志位

#if MY_DEBUG
        printf("get_all_status\n");
#endif
    }

    if (get_gear)
    {
        // 如果要获取挡位的状态
        get_gear = 0; //

#if MY_DEBUG
        printf("get_gear\n");
#endif

        send_data(SEND_GEAR, fun_info.gear); // 发送当前挡位的状态
    }

    if (get_battery)
    {
        // 如果要获取电池电量的状态
        get_battery = 0;

        send_data(SEND_BATTERY, fun_info.battery); // 发送电池电量

#if MY_DEBUG
        printf("get_battery\n");
#endif
    }

    if (get_brake)
    {
        // 如果要获取获取刹车的状态
        get_brake = 0;

#if MY_DEBUG
        printf("get_brake\n");
#endif

        send_data(SEND_BARKE, fun_info.brake); // 发送当前刹车的状态
    }

    if (get_left_turn)
    {
        // 如果要获取左转向灯的状态
        get_left_turn = 0;

#if MY_DEBUG
        printf("get_left_turn\n");
#endif

        send_data(SEND_LEFT_TURN, fun_info.left_turn); // 发送当前左转向灯的状态
    }

    if (get_right_turn)
    {
        // 如果要获取右转向灯的状态
        get_right_turn = 0;

        // uart0_sendstr("get_right_turn\n");

        send_data(SEND_RIGHT_TURN, fun_info.right_turn); // 发送当前右转向灯的状态
    }

    if (get_high_beam)
    {
        // 如果要获取远光灯的状态
        get_high_beam = 0;

        // uart0_sendstr("get_high_beam\n");

        send_data(SEND_HIGH_BEAM, fun_info.high_beam); // 发送当前远光灯的状态
    }

    if (get_engine_speed)
    {
        // 如果要获取发动机的转速
        get_engine_speed = 0;

        // uart0_sendstr("get_engine_speed\n");

        send_data(SEND_ENGINE_SPEED, fun_info.engine_speeed);
    }

    if (flag_get_speed)
    {
        // 如果要获取时速
        flag_get_speed = 0;

        // uart0_sendstr("get_speed\n");

#ifdef INTERNATIONAL // 使用公制单位

        send_data(SEND_SPEED, fun_info.speed); // 发送当前采集到的车速（时速）

#endif // INTERNATIONAL 使用公制单位

#ifdef IMPERIAL // 使用英制单位

        send_data(SEND_SPEED, fun_info.speed * 621 / 1000);

#endif
    }

    if (getFuel)
    {
        // 如果要获取油量
        getFuel = 0;

        // uart0_sendstr("getFuel\n");
    }

    if (getWaterTemp)
    {
        // 如果要获取水温
        getWaterTemp = 0;

        // uart0_sendstr("getWaterTemp\n");
    }

    if (getTotalMileage)
    {
        // 如果要获取大计里程 / 得到了大计里程新的数据
        getTotalMileage = 0;

#ifdef INTERNATIONAL // 公制单位

        // 只发送百米及以上的数据
        send_data(SEND_TOTAL_MILEAGE, mileage_info.total_mileage / 100);

#endif // INTERNATIONAL 公制单位

#ifdef IMPERIAL // 英制单位
#if MY_DEBUG
        printf("total mileage: %lu * 0.1 mile", mileage_info.total_mileage * 62137 / 10000000);
#endif // MY_DEBUG
       // 只发送0.1英里及以上的数据
        send_data(SEND_TOTAL_MILEAGE, mileage_info.total_mileage * 62137 / 10000000);

#endif // IMPERIAL 英制单位
    }

    if (getSubTotalMileage)
    {
        // 如果要获取小计里程 / 得到了小计里程新的数据
        getSubTotalMileage = 0;

#ifdef INTERNATIONAL // 公制单位

        // 只发送千米及以上的数据
        send_data(SEND_SUBTOTAL_MILEAGE, mileage_info.subtotal_mileage / 1000);

#endif // INTERNATIONAL 公制单位

#ifdef IMPERIAL // 英制单位
#if MY_DEBUG
        printf("sub total mileage: %lu mile", mileage_info.subtotal_mileage * 62137 / 100000000);
#endif // MY_DEBUG
       // 只发送英里及以上的数据
        send_data(SEND_SUBTOTAL_MILEAGE, mileage_info.subtotal_mileage * 62137 / 100000000);

#endif // IMPERIAL 英制单位
    }
}
