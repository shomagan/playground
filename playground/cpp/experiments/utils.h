#define FUNCTION_TIME_MEASUREMENT(f, time_ns) do{     \
   SuspendAllInterrupts(); \
   uint32 time_start = STM0_TIM0.U;                \
   (f);                                            \
   uint32 time_end = STM0_TIM0.U;                  \
   ResumeAllInterrupts(); \
   time_ns = SUB_WITH_WRAP_UINT32(time_end,time_start)*10u;\
}while(0)

  if (Dlt_SendBuffer[LogChannelIndex].Write>= Dlt_SendBuffer[LogChannelIndex].Read){
    free_space = ((uint32)Dlt_GetLogChannelBufferSizeOfLogChannelDescriptor(LogChannelIndex) - Dlt_SendBuffer[LogChannelIndex].Write-1) + Dlt_SendBuffer[LogChannelIndex].Read;
  } else{
    free_space = Dlt_SendBuffer[LogChannelIndex].Read - Dlt_SendBuffer[LogChannelIndex].Write-1 ; 
  }
  if (free_space >= DataLength){
    uint32 remain = 0;
    uint32 to_the_end = (uint32)Dlt_GetLogChannelBufferSizeOfLogChannelDescriptor(LogChannelIndex) - Dlt_SendBuffer[LogChannelIndex].Write;
    if(to_the_end < DataLength){
      remain = DataLength - to_the_end; 
    }else{
      to_the_end = DataLength;
    }
    VStdLib_MemCpy(&Dlt_SendBuffer[LogChannelIndex].Buffer[bufferIndex], Data, to_the_end);
    bufferIndex= (bufferIndex + to_the_end)> (uint32)Dlt_GetLogChannelBufferSizeOfLogChannelDescriptor(LogChannelIndex)?0:(bufferIndex + to_the_end);
    if(remain){
      VStdLib_MemCpy(&Dlt_SendBuffer[LogChannelIndex].Buffer[0], &Data[to_the_end], remain);
      bufferIndex = remain;
      if(bufferIndex>=Dlt_SendBuffer[LogChannelIndex].Read){
        error_counter_dlt++;
      }
    }
  }else{