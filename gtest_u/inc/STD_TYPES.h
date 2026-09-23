#ifndef INC_STD_TYPES_H_
#define INC_STD_TYPES_H_

#include <stdint.h>
typedef enum
{
	E_OK=0,
	E_NOK
}Std_ReturnType;

typedef struct
{
	uint8_t Data[8];
	uint32_t Length;
}PduInfoTRx;

typedef enum
{
	Single_Frame,
	First_Frame,
	Consecutive_Frame,
	FlowControl_Frame,
	None
}Frame_Type;

typedef enum
{
	Any_State,
	Consecutive_Frame_State,
	FlowControl_Frame_State
}Frame_States;

typedef struct
{
	uint8_t Data[4096];
	uint32_t Length;
}PduInfoType;
#endif