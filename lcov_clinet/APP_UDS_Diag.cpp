/*
 * UDS_APP_Diag.c
 *
 *  Created on: Jun 4, 2024
 *      Author: Omnia
 */
extern "C"{
	#include "APP_UDS_Diag.h"
}


// there are many ser. has sub,  this var indecate for which sub servise
#define sub_func_control 1
#define Num_of_Services 5

Std_ReturnType CanTp_Transmit(uint32_t TxPduId, PduInfoType *PduInfoPtr);

PduInfoType Read_Data_Client;
PduInfoType Write_Data_Client;
PduInfoType Control_Session_Default;
PduInfoType Send_Security_Seed;
PduInfoType Control_Session_Extended;
PduInfoType PduInfoTypePtr;
volatile uint8_t global_sec_flag = 0;
volatile uint8_t global_session = DefaultSession;

volatile uint8_t Security_Service_Availability_Flag = Not_Available;
// uint8_t seed =50 ; // for example
/*Security Variables */
volatile uint32_t Sec_u32SeedValue = 0;
Security_Access_State Sec_State = Un_Secure;
volatile uint32_t Oil_Pressure_var = 0x778899AA;
volatile uint32_t Oil_Temp_var = 0x5566;
volatile uint8_t VIN_number_var[17] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
// volatile uint32_t VIN_number_var;

/*************************************************************************************/
/********************************** Test flags ***************************************/
/*************************************************************************************/
// Test Defines
#define Write_Oil_Temp_Data "0102"
#define Write_Oil_Press_Data "01020304"
#define Write_VIN_Num_Data "0102030405060708091011121314151617"

#define Security_Key_Data "01020304"
// UDS_MainFunction() Flags:
//  Negative response flags
volatile uint8_t serviceNotSupported_Flag = 0;
volatile uint8_t securityAccessDenied_Flag = 0;
volatile uint8_t subFunctionNotSupported_Flag = 0;
volatile uint8_t serviceNotSupportedInActiveSession_Flag = 0;
volatile uint8_t invalidKey_Flag = 0;

// Read flags
volatile uint8_t OilTemperature_ReadFlag = 0;
volatile uint8_t OilPressure_ReadFlag = 0;
volatile uint8_t VIN_number_ReadFlag = 0;

// Write flags
volatile uint8_t OilTemperature_WriteFlag = 0;
volatile uint8_t OilPressure_WriteFlag = 0;
volatile uint8_t VIN_number_WriteFlag = 0;

// Control Session flags
volatile uint8_t DefaultSession_Flag = 0;
volatile uint8_t ExtendedSession_Flag = 0;

// Security flags
volatile uint8_t SecuritySeed_Flag = 0;
volatile uint8_t SecurityKey_Flag = 0;

// Tester presenter flags
volatile uint8_t TesterPresent_Flag = 0;
/*************************************************************************************/
/*************************************************************************************/

// Client Functions Flag:
//  Control Session flags
volatile uint8_t Default_Session_Flag = 0;
volatile uint8_t Extended_Session_Flag = 0;
// Read Flags
volatile uint8_t Read_Oil_Temp_Flag = 0;
volatile uint8_t Read_Oil_Press_Flag = 0;
volatile uint8_t Read_VIN_Num_Flag = 0;
volatile uint8_t Read_Function_FLag = 0;
// Write Flags
volatile uint8_t Write_Oil_Temp_Flag = 0;
volatile uint8_t Write_Oil_Press_Flag = 0;
volatile uint8_t Write_VIN_Num_Flag = 0;
volatile uint8_t Write_Function_FLag = 0;
// Security Flags
volatile uint8_t Security_Seed_Flag = 0;
volatile uint8_t Security_Key_Flag = 0;
volatile uint8_t Security_Function_FLag = 0;
// Tester Representer Flags
volatile uint8_t Tester_Representer_Function_FLag = 0;
/*************************************************************************************/
/*************************************************************************************/

ServiceInfo pos_Response;

ServiceInfo Control;
PduInfoType msg;
// extern TIM_HandleTypeDef htim6;
volatile uint8_t flag_sub_fun;
volatile uint8_t UDS_Tx_Confirm = 0;
PduInfoType *UDS_Struct;
PduInfoType *PduDataPTR;

uint8_t *seed = NULL;
uint8_t *key = NULL;

uint8_t SupressedPosRes_CLient = 1;
uint8_t SupressedPosRes_Server = 1;

uint8_t Source_Address = Client_Address;
uint8_t Target_Address = Tempreture_Address;

/***************************************************************Test**********************************************************************/
void mockUART_ReceiveAndConvert(const char *data, uint8_t *ptr)
{
	uint8_t i = 0;
	while (*data)
	{
		ptr[i] = *data++;
		i++;
	}
}

/***************************************************************Shared Fuctions**********************************************************************/

void UART_ReceiveAndConvert(uint8_t RX_BUFFER_SIZE, PduInfoType *PduInfoType_Ptr)
{
	uint8_t rxBuffer[RX_BUFFER_SIZE];
	uint8_t hexValue;
	uint8_t hexOutput[RX_BUFFER_SIZE / 2];
	uint16_t length = 0;

	// Receive ASCII characters from UART
	/*********************************************************************************************************************************************************************** */
	// For Test
	if (Write_Oil_Temp_Flag)
	{
		mockUART_ReceiveAndConvert(Write_Oil_Temp_Data, rxBuffer);
	}
	else if (Write_Oil_Press_Flag)
	{
		mockUART_ReceiveAndConvert(Write_Oil_Press_Data, rxBuffer);
	}
	else if (Write_VIN_Num_Flag)
	{
		mockUART_ReceiveAndConvert(Write_VIN_Num_Data, rxBuffer);
	}
	else if (Security_Key_Flag)
	{
		mockUART_ReceiveAndConvert(Security_Key_Data, rxBuffer);
	}
	else
	{
		// Nothing
	}
	/****************************************************************************************************************************************** */
	// if (HAL_UART_Receive(&huart2, rxBuffer, RX_BUFFER_SIZE, HAL_MAX_DELAY) == HAL_OK) {
	// Calculate the length of the received string
	length = RX_BUFFER_SIZE /*strlen((char*)rxBuffer)*/;

	// Ensure the length is even (each hex byte is represented by 2 ASCII characters)
	// if (length % 2 != 0) {
	// 	// Send an error message
	// 	char errorMsg[] = "Error: Odd number of characters received.\n";
	// 	HAL_UART_Transmit(&huart2, (uint8_t*)errorMsg, strlen(errorMsg), HAL_MAX_DELAY);
	// 	return;
	// }

	// Process each pair of ASCII characters
	for (uint16_t i = 0; i < length; i += 2)
	{
		hexValue = (charToHex(rxBuffer[i]) << 4) | charToHex(rxBuffer[i + 1]);
		// update hexvalue into frame
		if (PduInfoType_Ptr->Data[Sub_F] == Key)
		{
			PduInfoType_Ptr->Data[(i / 2) + 5] = hexValue;
		}
		else if ((PduInfoType_Ptr->Data[DID_1] == Oil_Temp_First_byte) || (PduInfoType_Ptr->Data[DID_1] == Oil_Pressure_First_byte) || (PduInfoType_Ptr->Data[DID_1] == VIN_number_First_byte))
		{
			PduInfoType_Ptr->Data[(i / 2) + 6] = hexValue;
		}

		hexOutput[i / 2] = hexValue;
		// Optionally, send the hexadecimal values back via UART
		// HAL_UART_Transmit(&huart2, hexOutput, 1/*length / 2*/, HAL_MAX_DELAY);
	}
	// sendHexArrayAsASCII(hexOutput, length / 2);

	// }//end of if
}

uint8_t charToHex(uint8_t ascii)
{
	if (ascii >= '0' && ascii <= '9')
	{
		return ascii - '0';
	}
	else if (ascii >= 'A' && ascii <= 'F')
	{
		return ascii - 'A' + 10;
	}
	else if (ascii >= 'a' && ascii <= 'f')
	{
		return ascii - 'a' + 10;
	}
	else
	{
		return 0; // Invalid character
	}
}

/***************************************************************Init**********************************************************************/

void UDS_Init(void)
{
}

void UDS_Tester_Presenter_Client(void)
{
	PduInfoTypePtr.Data[PCI] = 5;					  // TESTER_PRESENT_PCI
	PduInfoTypePtr.Data[ADD_Source] = Client_Address; // ADD_Source

	if (Target_Address == Tempreture_Address)
	{
		PduInfoTypePtr.Data[ADD_Target] = Tempreture_Address; // ADD_Target
	}
	else if (Target_Address == Pressure_Address)
	{
		PduInfoTypePtr.Data[ADD_Target] = Pressure_Address; // ADD_Target
	}
	else if (Target_Address == Functional_Address)
	{
		PduInfoTypePtr.Data[ADD_Target] = Functional_Address; // ADD_Target
	}
	else
	{
		// Nothing
	}

	PduInfoTypePtr.Data[SID] = Tester_Representer_Service; // TESTER_PRESENT_SID
	PduInfoTypePtr.Data[Sub_F] = 0x00;					   // TESTER_PRESENT_Sub_Fun
	PduInfoTypePtr.Length = 5;							   // TESTER_PRESENT_LENGTH

	// Check supressed Positive Responce
	PduInfoTypePtr.Data[Sub_F] |= 0b10000000;
	PduInfoTypePtr.Data[Sub_F] &= ((SupressedPosRes_CLient << 7) | 0x7F);
	// Sending Frame to Can TP
	// CanTp_Transmit(0, &PduInfoTypePtr);
	Tester_Representer_Function_FLag = 1;
}

/***************************************************************Read**********************************************************************/

// Send Read Frame
void UDS_Read_Data_Client(DID did)
{
	Read_Data_Client.Data[PCI] = 6;
	Read_Data_Client.Data[ADD_Source] = Client_Address; // ADD_Source

	if (Target_Address == Tempreture_Address)
	{
		Read_Data_Client.Data[ADD_Target] = Tempreture_Address; // ADD_Target
	}
	else if (Target_Address == Pressure_Address)
	{
		Read_Data_Client.Data[ADD_Target] = Pressure_Address; // ADD_Target
	}
	else if (Target_Address == Functional_Address)
	{
		Read_Data_Client.Data[ADD_Target] = Functional_Address; // ADD_Target
	}
	else
	{
		// Nothing
	}

	Read_Data_Client.Data[SID] = Read_Service; // SID of Read
	Read_Data_Client.Length = 6;			   // length of Read frame

	// check DID which Data
	if (did == Oil_Temp)
	{
		Read_Data_Client.Data[DID_1] = Oil_Temp_First_byte;
		Read_Data_Client.Data[DID_2] = Oil_Temp_Second_byte;
		Read_Oil_Temp_Flag = 1; // For Test
	}
	else if (did == Oil_Pressure)
	{
		Read_Data_Client.Data[DID_1] = Oil_Pressure_First_byte;
		Read_Data_Client.Data[DID_2] = Oil_Pressure_Second_byte;
		Read_Oil_Press_Flag = 1;
	}
	else if (did == VIN_number)
	{
		Read_Data_Client.Data[DID_1] = VIN_number_First_byte;
		Read_Data_Client.Data[DID_2] = VIN_number_Second_byte;
		Read_VIN_Num_Flag = 1;
	}
	else
	{
		// Another DID
	}

	// Check supressed Positive Responce
	Read_Data_Client.Data[DID_1] |= 0b10000000;
	Read_Data_Client.Data[DID_1] &= ((SupressedPosRes_CLient << 7) | 0x7F);
	// Send Frame to Can TP
	// CanTp_Transmit(0, &Read_Data_Client);
	Read_Function_FLag = 1; // For Test
							// For Debugging
							// HAL_UART_Transmit(&huart2, "\r\nRead Frame Client:", 50, HAL_MAX_DELAY);
							// sendHexArrayAsASCII(Read_Data_Client.Data, Read_Data_Client.Length);
							// HAL_UART_Transmit(&huart2, "\r\n", 50, HAL_MAX_DELAY);
}

/***************************************************************Write**********************************************************************/

void UDS_Write_Data_Client(DID did, uint32_t data)
{

	// PduInfoType Write_Data_Client;
	Write_Data_Client.Data[SID] = Write_Service; // SID of WDID
	Write_Data_Client.Data[ADD_Source] = Client_Address;

	if (Target_Address == Tempreture_Address)
	{
		Write_Data_Client.Data[ADD_Target] = Tempreture_Address; // ADD_Target
	}
	else if (Target_Address == Pressure_Address)
	{
		Write_Data_Client.Data[ADD_Target] = Pressure_Address; // ADD_Target
	}
	else if (Target_Address == Functional_Address)
	{
		Write_Data_Client.Data[ADD_Target] = Functional_Address; // ADD_Target
	}
	else
	{
		// Nothing
	}

	// check DID which Data
	if (did == Oil_Temp)
	{
		Write_Data_Client.Data[DID_1] = Oil_Temp_First_byte;
		Write_Data_Client.Data[DID_2] = Oil_Temp_Second_byte;

		Write_Data_Client.Length = 8; // SID + DID + Data
		Write_Data_Client.Data[PCI] = 8;
		Write_Oil_Temp_Flag = 1;
		// Assuming data is 2 bytes
		UART_ReceiveAndConvert(4, &Write_Data_Client);

		/*
		Write_Data_Client.Data[Data_DID] = data >> 8; // Most significant byte of data
		Write_Data_Client.Data[Data_DID+1] = data & 0xFF;	   // Least significant byte of data
		Write_Data_Client.Data[Data_DID+2] = 0x00;
		Write_Data_Client.Data[Data_DID+3] = 0x00;		  // Least significant byte of data
		 */
	}
	else if (did == Oil_Pressure)
	{
		Write_Data_Client.Data[DID_1] = Oil_Pressure_First_byte;
		Write_Data_Client.Data[DID_2] = Oil_Pressure_Second_byte;

		Write_Data_Client.Length = 10; // SID + DID + Data
		Write_Data_Client.Data[PCI] = 10;
		Write_Oil_Press_Flag = 1;
		// Assuming data is 4 bytes
		UART_ReceiveAndConvert(8, &Write_Data_Client);
		/*
		Write_Data_Client.Data[Data_DID] = (data >> 24) & 0xFF; // Most significant byte of data
		Write_Data_Client.Data[Data_DID+1] = (data >> 16) & 0xFF;
		Write_Data_Client.Data[Data_DID+2] = (data >> 8) & 0xFF;
		Write_Data_Client.Data[Data_DID+3] = data & 0xFF;		  // Least significant byte of data
		 */
	}
	else if (did == VIN_number)
	{
		Write_Data_Client.Data[DID_1] = VIN_number_First_byte;
		Write_Data_Client.Data[DID_2] = VIN_number_Second_byte;
		Write_Data_Client.Length = 23; // SID + DID + Data
		Write_Data_Client.Data[PCI] = 23;
		Write_VIN_Num_Flag = 1;
		// Assuming data is 4 bytes for now until we test then handle it as 17 bytes ISA
		UART_ReceiveAndConvert(34, &Write_Data_Client);
		// Write_Data_Client.Data[Data_DID] = (data >> 24) & 0xFF; // Most significant byte of data
		// Write_Data_Client.Data[Data_DID+1] = (data >> 16) & 0xFF;
		// Write_Data_Client.Data[Data_DID+2] = (data >> 8) & 0xFF;
		// Write_Data_Client.Data[Data_DID+3] = data & 0xFF;		  // Least significant byte of data
		/*
		Write_Data_Client.Length = 10; // SID + DID + Data
		Write_Data_Client.Data[PCI] = 10;
		 */

		/* to handle 17 bytes of data */
		/*	for (uint8_t i=16; i>=0; i--)
			{
				Write_Data_Client.Data[(Data_DID + (16-i))] = (data >> (i*8)) & 0xFF;
			}

		 */
	}
	else
	{
		// Another DID
	}
	// Check supressed Positive Responce
	Write_Data_Client.Data[DID_1] |= 0b10000000;
	Write_Data_Client.Data[DID_1] &= ((SupressedPosRes_CLient << 7) | 0x7F);
	// CanTp_Transmit(0, &Write_Data_Client);
	Write_Function_FLag = 1;
	// For Debugging
	// HAL_UART_Transmit(&huart2, "\r\nWrite Frame Client:", 50, HAL_MAX_DELAY);
	//	sendHexArrayAsASCII(Write_Data_Client.Data,  Write_Data_Client.Length);
	//	HAL_UART_Transmit(&huart2, "\r\n", 50, HAL_MAX_DELAY);
}

/***************************************************************Control Session**********************************************************************/
void UDS_Control_Session_Default(void)
{
	// init struct var  to use for send TP

	// fill the struct data
	Control_Session_Default.Data[PCI] = 5;
	Control_Session_Default.Data[SID] = Control_Service;

	Control_Session_Default.Data[ADD_Source] = Client_Address;

	if (Target_Address == Tempreture_Address)
	{
		Control_Session_Default.Data[ADD_Target] = Tempreture_Address; // ADD_Target
	}
	else if (Target_Address == Pressure_Address)
	{
		Control_Session_Default.Data[ADD_Target] = Pressure_Address; // ADD_Target
	}
	else if (Target_Address == Functional_Address)
	{
		Control_Session_Default.Data[ADD_Target] = Functional_Address; // ADD_Target
	}
	else
	{
		// Nothing
	}
	Control_Session_Default.Data[Sub_F] = DefaultSession;
	// Check supressed Positive Responce
	Control_Session_Default.Data[Sub_F] |= 0b10000000;
	Control_Session_Default.Data[Sub_F] &= ((SupressedPosRes_CLient << 7) | 0x7F);

	Control_Session_Default.Length = 5;

	// send to can tp
	// CanTp_Transmit(0, &Control_Session_Default);
	Default_Session_Flag = 1;
	// For Debugging
	// HAL_UART_Transmit(&huart2, "\r\nControl_Session_Default:", 100, HAL_MAX_DELAY);
	// sendHexArrayAsASCII(Control_Session_Default.Data,  Control_Session_Default.Length);
	// HAL_UART_Transmit(&huart2, "\r\n", 50, HAL_MAX_DELAY);
}

void UDS_Control_Session_Extended(void)
{
	// init struct var  to use for send TP

	// fill the struct data
	Control_Session_Extended.Data[PCI] = 5;
	Control_Session_Extended.Data[SID] = Control_Service;

	Control_Session_Extended.Data[ADD_Source] = Client_Address;

	if (Target_Address == Tempreture_Address)
	{
		Control_Session_Extended.Data[ADD_Target] = Tempreture_Address; // ADD_Target
	}
	else if (Target_Address == Pressure_Address)
	{
		Control_Session_Extended.Data[ADD_Target] = Pressure_Address; // ADD_Target
	}
	else if (Target_Address == Functional_Address)
	{
		Control_Session_Extended.Data[ADD_Target] = Functional_Address; // ADD_Target
	}
	else
	{
		// Nothing
	}
	Control_Session_Extended.Data[Sub_F] = ExtendedSession;
	// Check supressed Positive Responce
	Control_Session_Extended.Data[Sub_F] |= 0b10000000;
	Control_Session_Extended.Data[Sub_F] &= ((SupressedPosRes_CLient << 7) | 0x7F);

	Control_Session_Extended.Length = 5;

	// send to can tp
	// CanTp_Transmit(0, &Control_Session_Extended);
	Extended_Session_Flag = 1;
	// For Debugging
	// HAL_UART_Transmit(&huart2, "\r\Control_Session_Extended:", 100, HAL_MAX_DELAY);
	// sendHexArrayAsASCII(Control_Session_Extended.Data,  Control_Session_Extended.Length);
	// HAL_UART_Transmit(&huart2, "\r\n", 50, HAL_MAX_DELAY);
}

/***************************************************************Security**********************************************************************/

/***************************************************************Security**********************************************************************/

void UDS_Send_Security_Client(Sub_Fun sub_fun)
{

	Send_Security_Seed.Data[SID] = Security_Service; // Security SID

	// Prepare the Key
	// Security_Key security_key;
	Send_Security_Seed.Data[ADD_Source] = Client_Address;

	if (Target_Address == Tempreture_Address)
	{
		Send_Security_Seed.Data[ADD_Target] = Tempreture_Address; // ADD_Target
	}
	else if (Target_Address == Pressure_Address)
	{
		Send_Security_Seed.Data[ADD_Target] = Pressure_Address; // ADD_Target
	}
	else if (Target_Address == Functional_Address)
	{
		Send_Security_Seed.Data[ADD_Target] = Functional_Address; // ADD_Target
	}
	else
	{
		// Nothing
	}
	if (sub_fun == Seed)
	{
		Send_Security_Seed.Data[Sub_F] = Seed; // Sub_Fun Seed
		Send_Security_Seed.Length = 5;
		Send_Security_Seed.Data[PCI] = 5;
		Security_Seed_Flag = 1;
	}
	else if (sub_fun == Key)
	{
		Send_Security_Seed.Data[Sub_F] = Key; // Sub_Fun Key
		Send_Security_Seed.Length = 5 + Seed_Key_Lenght;
		Send_Security_Seed.Data[PCI] = 5 + Seed_Key_Lenght;
		Security_Key_Flag = 1;
		UART_ReceiveAndConvert(8, &Send_Security_Seed);

		// security_key.Seed = seed;
		// Prepare Key From Seed
		// uint8_t security_key_counter = Seed_Key_Lenght-1;
		// uint8_t Send_Security_Seed_counter = 2;
		/*
		while(security_key_counter >= 0 )
		{
			//sendHexArrayAsASCII(security_key.Key,4);
			//HAL_UART_Transmit(&huart2, "\r\n", 50, HAL_MAX_DELAY);
			security_key.Key[security_key_counter] = security_key.Seed[security_key_counter] + Key_Code;//+0x05
			security_key_counter--;

			//Put New Key Calculated into Frame
			Send_Security_Seed.Data[Send_Security_Seed_counter] = security_key.Key[security_key_counter];
			Send_Security_Seed_counter++;
		}
		 */
		// key = security_key.Key;
	}
	else
	{
		// Nothing
	}

	// Check supressed Positive Responce
	Send_Security_Seed.Data[Sub_F] |= 0b10000000;
	Send_Security_Seed.Data[Sub_F] &= ((SupressedPosRes_CLient << 7) | 0x7F);

	// Send Frame to Can TP
	// CanTp_Transmit(0, &Send_Security_Seed);
	Security_Function_FLag = 1;
	// For Debugging
	// HAL_UART_Transmit(&huart2, "\r\nUDS_Send_Security_Client", 50, HAL_MAX_DELAY);
	// HAL_UART_Transmit(&huart2, "\r\n", 50, HAL_MAX_DELAY);
	// sendHexArrayAsASCII(Send_Security_Seed.Data, Send_Security_Seed.Length);
	// HAL_UART_Transmit(&huart2, "\r\n", 50, HAL_MAX_DELAY);
}

void UDS_MainFunction()
{
	// while(1)
	
		if (UDS_Tx_Confirm)
		{
			UDS_Tx_Confirm = 0;
			if (UDS_Struct->Data[Neg_Res_INDEX] == 0x7f)
			{
				switch (UDS_Struct->Data[NRC_INDEX])
				{
				case serviceNotSupported:
					// HAL_UART_Transmit(&huart2, (uint8_t*) "\r\nThis Service is not Available.\r\n",strlen("\r\nThis Service is not Available.\r\n"), HAL_MAX_DELAY);
					serviceNotSupported_Flag = 1;
					break;

				case securityAccessDenied:
					// HAL_UART_Transmit(&huart2, (uint8_t*) "\r\nNot Secured, Please Enter a Key.\r\n",strlen("\r\nNot Secured, Please Enter a Key.\r\n"), HAL_MAX_DELAY);
					securityAccessDenied_Flag = 1;
					break;

				case subFunctionNotSupported:
					// HAL_UART_Transmit(&huart2, (uint8_t*) "\r\nWrong Session, Please Enter Extended Session.\r\n",strlen("\r\nWrong Session, Please Enter Extended Session.\r\n"), HAL_MAX_DELAY);
					subFunctionNotSupported_Flag = 1;
					break;

				case serviceNotSupportedInActiveSession:
					// HAL_UART_Transmit(&huart2, (uint8_t*) "\r\nWrong Session, Please Enter Extended Session.\r\n",strlen("\r\nWrong Session, Please Enter Extended Session.\r\n"), HAL_MAX_DELAY);
					serviceNotSupportedInActiveSession_Flag = 1;
					break;

				case invalidKey:
					// HAL_UART_Transmit(&huart2, (uint8_t*) "\r\nWrong Key.\r\n",strlen("\r\nWrong Key.\r\n"), HAL_MAX_DELAY);
					invalidKey_Flag = 1;
					break;

				//default:
					// Nothing
				}
			}

			else
			{
				UDS_Struct->Data[SID] = UDS_Struct->Data[SID] - 0x40;
				if (UDS_Struct->Data[SID] == Read_Service)

				{
					if (UDS_Struct->Data[DID_1] == Oil_Temp_First_byte && UDS_Struct->Data[DID_2] == Oil_Temp_Second_byte)
					{
						OilTemperature_ReadFlag = 1;
						// HAL_UART_Transmit(&huart2, (uint8_t*) "\r\nOil Temp:\r\n", strlen("\r\nOil Temp:\r\n"), HAL_MAX_DELAY);
						// sendHexArrayAsASCII((uint8_t*)&UDS_Struct->Data[Data_DID],2);
					}

					else if (UDS_Struct->Data[DID_1] == Oil_Pressure_First_byte && UDS_Struct->Data[DID_2] == Oil_Pressure_Second_byte)
					{
						OilPressure_ReadFlag = 1;
						// HAL_UART_Transmit(&huart2, (uint8_t*) "\r\nOil Pressure:\r\n", strlen("\r\nOil Pressure:\r\n"), HAL_MAX_DELAY);
						// sendHexArrayAsASCII((uint8_t*)&UDS_Struct->Data[Data_DID],4);
					}

					else if (UDS_Struct->Data[DID_1] == VIN_number_First_byte && UDS_Struct->Data[DID_2] == VIN_number_Second_byte)
					{
						VIN_number_ReadFlag = 1;
						// HAL_UART_Transmit(&huart2, (uint8_t*) "\r\nVIN Number:\r\n", strlen("\r\nVIN Number:\r\n"), HAL_MAX_DELAY);
						// sendHexArrayAsASCII((uint8_t*)&UDS_Struct->Data[Data_DID],17);
						// sendHexArrayAsASCII((uint8_t*)&UDS_Struct->Data[Data_DID],17);
					}
				}

				/*  Write IDS Message */
				else if (UDS_Struct->Data[SID] == Write_Service)

				{

					if (UDS_Struct->Data[DID_1] == Oil_Temp_First_byte && UDS_Struct->Data[DID_2] == Oil_Temp_Second_byte)
					{
						// HAL_UART_Transmit(&huart2, (uint8_t*) "\r\nOil Temperature Written successfully.\r\n", strlen("\r\nOil Temperature Written successfully .\r\n"), HAL_MAX_DELAY);
						OilTemperature_WriteFlag = 1;
					}

					else if (UDS_Struct->Data[DID_1] == Oil_Pressure_First_byte && UDS_Struct->Data[DID_2] == Oil_Pressure_Second_byte)
					{
						// HAL_UART_Transmit(&huart2, (uint8_t*) "\r\nOil Pressure Written successfully.\r\n", strlen("\r\nOil Pressure Written successfully .\r\n"), HAL_MAX_DELAY);
						OilPressure_WriteFlag = 1;
					}

					else if (UDS_Struct->Data[DID_1] == VIN_number_First_byte && UDS_Struct->Data[DID_2] == VIN_number_Second_byte)
					{
						// HAL_UART_Transmit(&huart2, (uint8_t*) "\r\nVIN Number Written successfully.\r\n", strlen("\r\nVIN Number Written successfully .\r\n"), HAL_MAX_DELAY);
						VIN_number_WriteFlag = 1;
					}
				}
				/*  Control Service  Session */

				else if (UDS_Struct->Data[SID] == Control_Service)

				{

					switch (UDS_Struct->Data[Sub_F])
					{

					case DefaultSession:
						// HAL_UART_Transmit(&huart2, (uint8_t*) "\r\nIt's Default Session!\r\n", strlen("\r\nIt's Default Session!\r\n"), HAL_MAX_DELAY);
						// UDS_Read_Data_Client(Frame_Info.DID);
						DefaultSession_Flag = 1;
						break;

					case ExtendedSession:
						// HAL_UART_Transmit(&huart2,(uint8_t*) "\r\nIt's Extended Session!\r\n", strlen("\r\nIt's Extended Session!\r\n"), HAL_MAX_DELAY);
						// UDS_Read_Data_Client(Frame_Info.DID);
						ExtendedSession_Flag = 1;
						break;
					}
				}
				/*  Security Service  */

				else if (UDS_Struct->Data[SID] == Security_Service)

				{

					switch (UDS_Struct->Data[Sub_F])
					{

					case Seed:
						// HAL_UART_Transmit(&huart2, (uint8_t*) "\r\nSeed is:\r\n", strlen ("\r\nSeed is:\r\n"), HAL_MAX_DELAY);
						// sendHexArrayAsASCII((uint8_t*)&UDS_Struct->Data[Data_Sub_Fun],4);
						SecuritySeed_Flag = 1;
						break;

					case Key:
						// HAL_UART_Transmit(&huart2, (uint8_t*) "\r\nKey is Compatible!\r\n", strlen ( "\r\nKey is Compatible!\r\n"), HAL_MAX_DELAY);
						SecurityKey_Flag = 1;
						break;
					}
				}
				/*  Tester_Representer_Service  */

				else if (UDS_Struct->Data[SID] == Tester_Representer_Service)

				{

					// HAL_UART_Transmit(&huart2,(uint8_t*) "\r\nECU Reseted!\r\n", strlen("\r\n ECU Reseted!\r\n"), HAL_MAX_DELAY);
					TesterPresent_Flag = 1;
				}

				else
				{ /* no thing  */
				}
			}
		}

		// vTaskDelay(50);
	
}

void UDS_Client_Callback(uint32_t TxPduId, PduInfoType *PduInfoPtr)
{
	UDS_Tx_Confirm = 1;
	UDS_Struct = PduInfoPtr;
}

// SA  TA  PCI  SID  DID1  DID2   D a t a
// 03  01   8    22   F1    3D     54 32

// void UDS_Client_Callback(uint32_t TxPduId,PduInfoType *PduInfoPtr)
// PduInfoPtr.data = 03  01   8    22   F1    3D     54 32
// PduInfoPtr.length = 8

// cllient 01
// oil pressure 02
// oil temp 03
// pci = 8 for temp
