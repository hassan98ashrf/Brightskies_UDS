#ifndef INC_APP_UDS_MOCK_H_
#define INC_APP_UDS_MOCK_H_

#include "STD_TYPES.h"

void CanTp_Transmit(uint32_t TxPduId, PduInfoType* PduInfoPtr);
void reset_timer(void);
void stop_timer(void);
void start_timer(void);

#endif // !INC_APP_UDS_DIAG_H_