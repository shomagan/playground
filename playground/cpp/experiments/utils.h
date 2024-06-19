#define FUNCTION_TIME_MEASUREMENT(f, time_ns) do{     \
   uint32 time_start = STM0_TIM0.U;                \
   (f);                                            \
   uint32 time_end = STM0_TIM0.U;                  \
   time_ns = SUB_WITH_WRAP_UINT32(time_end,time_start)*10u;\
}while(0)
