
extern "C"
{
    #include "../inc/APP_UDS_Mock.h"
}


volatile uint8_t CANTP_FLAG=0;
volatile uint8_t ResetTimer_FLAG=0;
volatile uint8_t StopTimer_FLAG=0;
volatile uint8_t StartTimer_FLAG=0;
void CanTp_Transmit(uint32_t TxPduId, PduInfoType* PduInfoPtr)
{
    CANTP_FLAG=1;

}

void reset_timer(void)
{
    ResetTimer_FLAG=1;
}

void stop_timer(void)
{
    StopTimer_FLAG=1;
}
void start_timer(void)
{
    StartTimer_FLAG=1;
}