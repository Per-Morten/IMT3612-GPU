#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
    typedef enum
    {
        pms_time_unit_nanoseconds,
        pms_time_unit_microseconds,
        pms_time_unit_milliseconds,
        pms_time_unit_seconds,
        pms_time_unit_minutes,
        pms_time_unit_hours,
    } pms_time_unit;

    typedef void* pms_time_point;
    
    pms_time_point 
    pms_time_point_create();
    
    void 
    pms_time_point_destroy(pms_time_point time_point);

    void 
    pms_clock_now(pms_time_point time_point);

    float 
    pms_clock_difference(const pms_time_point lhs, 
                         const pms_time_point rhs, 
                         pms_time_unit unit);

#ifdef __cplusplus
}
#endif
