// UDS_Diag.cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>
extern "C"
{
// #include "example.h"
#include "APP_UDS_Diag.h"
#include "APP_UDS_Diag_CFG.h"
#include "funcMock.h"

    /*************************************************************************************************************************************/
    /* serviceNotSupported  -  securityAccessDenied  -  subFunctionNotSupported  -  serviceNotSupportedInActiveSession  -  invalidKey    */
    /* OilTemperature_Read  -  OilPressure_Read      -  VIN_number_Read                                                                  */
    /* OilTemperature_Write -  OilPressure_Write     -  VIN_number_Write                                                                 */
    /* DefaultSession       -  ExtendedSession       -  SecuritySeed             - SecurityKey    -    TesterPresent   -  ALL_TEST_CASES */
    /*************************************************************************************************************************************/

#define Response ALL_TEST_CASES

    /*************************************************************************************/
    /*************************************************************************************/
#define Read_Pressure_Target_with_No_Supp_Resp "URBI"
#define Write_Pressure_Target_with_No_Supp_Resp "URCP"

#define CONTROL_DEFAULT_SESSION "TQAF"
#define CONTROL_EXTENDED_SESSION "TQAG"

#define Security_Seed "TQDJ"
#define Security_Key "TQDK"

#define Read_Oil_Temp "TQBH"
#define Read_Oil_Press "TQBI"
#define Read_VIN_Num "TQBX"

#define Write_Oil_Temp "TQCO"
#define Write_Oil_Press "TQCP"
#define Write_VIN_Num "TQCY"

#define Tester_Representer "TQE"
    /**************************************Initials********************************************/
    volatile uint8_t Test_Case = 0;
    extern PduInfoTRx *GlobalRxPduInfoPtr;
    extern volatile uint32_t GlobalRxPduId;
    extern volatile uint8_t UDS_Tx_Confirm;

    extern uint8_t SupressedPosRes_CLient;
    extern uint8_t SupressedPosRes_Server;

    extern uint8_t Source_Address;
    extern uint8_t Target_Address;

    extern PduInfoType Control_Session_Default;
    extern PduInfoType Control_Session_Extended;
    extern PduInfoType Read_Data_Client;
    extern PduInfoType Write_Data_Client;
    extern PduInfoType PduInfoTypePtr;
    extern PduInfoType Send_Security_Seed;

    extern ClientMenu Menu;
    /*************************************************************************************/
    /********************************** Test Flags ***************************************/
    /*************************************************************************************/
    // UDS_MainFunction() Flags:
    //  Negative Response Flags
    extern volatile uint8_t serviceNotSupported_Flag;
    extern volatile uint8_t securityAccessDenied_Flag;
    extern volatile uint8_t subFunctionNotSupported_Flag;
    extern volatile uint8_t serviceNotSupportedInActiveSession_Flag;
    extern volatile uint8_t invalidKey_Flag;

    // Read Flags
    extern volatile uint8_t OilTemperature_ReadFlag;
    extern volatile uint8_t OilPressure_ReadFlag;
    extern volatile uint8_t VIN_number_ReadFlag;

    // Write Flags
    extern volatile uint8_t OilTemperature_WriteFlag;
    extern volatile uint8_t OilPressure_WriteFlag;
    extern volatile uint8_t VIN_number_WriteFlag;

    // Control Session Flags
    extern volatile uint8_t DefaultSession_Flag;
    extern volatile uint8_t ExtendedSession_Flag;

    // Security Flags
    extern volatile uint8_t SecuritySeed_Flag;
    extern volatile uint8_t SecurityKey_Flag;

    // Tester presenter Flags
    extern volatile uint8_t TesterPresent_Flag;

    /*************************************************************************************/
    /*************************************************************************************/

    // HAL_UART_RxCpltCallback() Flags:

    extern volatile uint8_t Main_Menu_Flag;
    extern volatile uint8_t Control_Session_Menu_Flag;
    extern volatile uint8_t Read_Data_Menu_Temp_or_Func_Flag;
    extern volatile uint8_t Read_Data_Menu_Else_Flag;
    extern volatile uint8_t Security_Access_Menu_Flag;
    extern volatile uint8_t Write_Data_Menu_Press_or_Func_Flag;
    extern volatile uint8_t Write_Data_Menu_Else_Flag;
    extern volatile uint8_t Response_Permission_Menu_Flag;
    extern volatile uint8_t Target_Selection_Menu_Flag;
    extern volatile uint8_t Write_Data_Menu_Temp_or_Func_Flag;
    extern volatile uint8_t Read_Data_Menu_Press_or_Func_Flag;

    // Client Functions Flag:
    //  Control Session flags
    extern volatile uint8_t Default_Session_Flag;
    extern volatile uint8_t Extended_Session_Flag;
    // Read Flags
    extern volatile uint8_t Read_Oil_Temp_Flag;
    extern volatile uint8_t Read_Oil_Press_Flag;
    extern volatile uint8_t Read_VIN_Num_Flag;
    extern volatile uint8_t Read_Function_FLag;
    // Write Flags
    extern volatile uint8_t Write_Oil_Temp_Flag;
    extern volatile uint8_t Write_Oil_Press_Flag;
    extern volatile uint8_t Write_VIN_Num_Flag;
    extern volatile uint8_t Write_Function_FLag;
    // Security Flags
    extern volatile uint8_t Security_Seed_Flag;
    extern volatile uint8_t Security_Key_Flag;
    extern volatile uint8_t Security_Function_FLag;
    // Tester Representer Flags
    extern volatile uint8_t Tester_Representer_Function_FLag;
}
/***************************************************************************************************************/
/***************************************** Control Session test cases ******************************************/
/***************************************************************************************************************/

TEST(UDS_Client_Request, Default_Session_Test)
{
    Display_Menu();
    mockUartDriver(CONTROL_DEFAULT_SESSION);
    // Test Target Option
    EXPECT_EQ(Target_Selection_Menu_Flag, 1);
    EXPECT_EQ(Target_Address, Tempreture_Address);
    // Test Suppressed Responce Option
    EXPECT_EQ(Response_Permission_Menu_Flag, 1);
    EXPECT_EQ(SupressedPosRes_CLient, 1);
    // Test Main Menue
    EXPECT_EQ(Main_Menu_Flag, 1);
    // Test Control Session Menu
    EXPECT_EQ(Control_Session_Menu_Flag, 1);
    // Test Default Session Function
    EXPECT_EQ(Default_Session_Flag, 1);
    // Test the Request Frame & Lenght
    EXPECT_EQ(Control_Session_Default.Data[PCI], 5);
    EXPECT_EQ(Control_Session_Default.Data[SID], Control_Service);
    EXPECT_EQ(Control_Session_Default.Data[ADD_Source], Client_Address);
    EXPECT_EQ(Control_Session_Default.Data[ADD_Target], Tempreture_Address);
    EXPECT_EQ(Control_Session_Default.Data[Sub_F], 0x81); // DefaultSession + Suppressed +ve Res
    EXPECT_EQ(Control_Session_Default.Length, 5);

    // Reset Flags

    Control_Session_Menu_Flag = 0;
    Target_Selection_Menu_Flag = 0;
    Response_Permission_Menu_Flag = 0;
    Main_Menu_Flag = 0;
    Control_Session_Menu_Flag = 0;
    Default_Session_Flag = 0;

    // Reset Variables
    Menu = Target_Selection_Menu;
    SupressedPosRes_CLient = 1;
}

TEST(UDS_Client_Request, Extended_Session_Test)
{

    Display_Menu();
    mockUartDriver(CONTROL_EXTENDED_SESSION);
    // Test Target Option
    EXPECT_EQ(Target_Selection_Menu_Flag, 1);
    EXPECT_EQ(Target_Address, Tempreture_Address);
    // Test Suppressed Responce Option
    EXPECT_EQ(Response_Permission_Menu_Flag, 1);
    EXPECT_EQ(SupressedPosRes_CLient, 1);
    // Test Main Menue
    EXPECT_EQ(Main_Menu_Flag, 1);
    // Test Control Session Menu
    EXPECT_EQ(Control_Session_Menu_Flag, 1);
    // Test Default Session Function
    EXPECT_EQ(Extended_Session_Flag, 1);
    // Test the Request Frame & Lenght
    EXPECT_EQ(Control_Session_Extended.Data[PCI], 5);
    EXPECT_EQ(Control_Session_Extended.Data[SID], Control_Service);
    EXPECT_EQ(Control_Session_Extended.Data[ADD_Source], Client_Address);
    EXPECT_EQ(Control_Session_Extended.Data[ADD_Target], Tempreture_Address);
    EXPECT_EQ(Control_Session_Extended.Data[Sub_F], 0x83); // ExtendedSession + Suppressed +ve Res
    EXPECT_EQ(Control_Session_Extended.Length, 5);

    // Reset Flags
    Control_Session_Menu_Flag = 0;
    Target_Selection_Menu_Flag = 0;
    Response_Permission_Menu_Flag = 0;
    Main_Menu_Flag = 0;
    Extended_Session_Flag = 0;

    // Reset Variables
    Menu = Target_Selection_Menu;
    SupressedPosRes_CLient = 1;
}

TEST(UDS_Client_Request, Read_Oil_Temperature)
{
    Display_Menu();
    mockUartDriver(Read_Oil_Temp);
    // Test Target Option
    EXPECT_EQ(Target_Selection_Menu_Flag, 1);
    EXPECT_EQ(Target_Address, Tempreture_Address);
    // Test Suppressed Responce Option
    EXPECT_EQ(Response_Permission_Menu_Flag, 1);
    EXPECT_EQ(SupressedPosRes_CLient, 1);
    // Test Main Menue
    EXPECT_EQ(Main_Menu_Flag, 1);
    EXPECT_EQ(Read_Data_Menu_Temp_or_Func_Flag, 1);
    EXPECT_EQ(Read_Oil_Temp_Flag, 1);
    EXPECT_EQ(Read_Function_FLag, 1);
    // Test the Request Frame & Lenght
    EXPECT_EQ(Read_Data_Client.Data[PCI], 6);
    EXPECT_EQ(Read_Data_Client.Data[SID], Read_Service);
    EXPECT_EQ(Read_Data_Client.Data[ADD_Source], Client_Address);
    EXPECT_EQ(Read_Data_Client.Data[ADD_Target], Tempreture_Address);
    EXPECT_EQ(Read_Data_Client.Data[DID_1], Oil_Temp_First_byte);
    EXPECT_EQ(Read_Data_Client.Data[DID_2], Oil_Temp_Second_byte);
    EXPECT_EQ(Read_Data_Client.Length, 6);

    // Reset Flags

    Target_Selection_Menu_Flag = 0;
    Response_Permission_Menu_Flag = 0;
    Main_Menu_Flag = 0;
    Read_Data_Menu_Temp_or_Func_Flag = 0;
    Read_Oil_Temp_Flag = 0;
    Read_Function_FLag = 0;

    // Reset Variables
    Menu = Target_Selection_Menu;
    SupressedPosRes_CLient = 1;
}

TEST(UDS_Client_Request, Read_Oil_Pressure)
{
    Display_Menu();
    mockUartDriver(Read_Oil_Press);
    // Test Target Option
    EXPECT_EQ(Target_Selection_Menu_Flag, 1);
    EXPECT_EQ(Target_Address, Tempreture_Address);
    // Test Suppressed Responce Option
    EXPECT_EQ(Response_Permission_Menu_Flag, 1);
    EXPECT_EQ(SupressedPosRes_CLient, 1);
    // Test Main Menue
    EXPECT_EQ(Main_Menu_Flag, 1);
    EXPECT_EQ(Read_Data_Menu_Temp_or_Func_Flag, 1);
    EXPECT_EQ(Read_Oil_Press_Flag, 1);
    EXPECT_EQ(Read_Function_FLag, 1);
    // Test the Request Frame & Lenght
    EXPECT_EQ(Read_Data_Client.Data[PCI], 6);
    EXPECT_EQ(Read_Data_Client.Data[SID], Read_Service);
    EXPECT_EQ(Read_Data_Client.Data[ADD_Source], Client_Address);
    EXPECT_EQ(Read_Data_Client.Data[ADD_Target], Tempreture_Address);
    EXPECT_EQ(Read_Data_Client.Data[DID_1], Oil_Pressure_First_byte);
    EXPECT_EQ(Read_Data_Client.Data[DID_2], Oil_Pressure_Second_byte);
    EXPECT_EQ(Read_Data_Client.Length, 6);

    // Reset Flags

    Target_Selection_Menu_Flag = 0;
    Response_Permission_Menu_Flag = 0;
    Main_Menu_Flag = 0;
    Read_Data_Menu_Temp_or_Func_Flag = 0;
    Read_Oil_Press_Flag = 0;
    Read_Function_FLag = 0;

    // Reset Variables
    Menu = Target_Selection_Menu;
    SupressedPosRes_CLient = 1;
}

TEST(UDS_Client_Request, Read_VIN_Number)
{
    Display_Menu();
    mockUartDriver(Read_VIN_Num);
    // Test Target Option
    EXPECT_EQ(Target_Selection_Menu_Flag, 1);
    EXPECT_EQ(Target_Address, Tempreture_Address);
    // Test Suppressed Responce Option
    EXPECT_EQ(Response_Permission_Menu_Flag, 1);
    EXPECT_EQ(SupressedPosRes_CLient, 1);
    // Test Main Menue
    EXPECT_EQ(Main_Menu_Flag, 1);
    EXPECT_EQ(Read_Data_Menu_Temp_or_Func_Flag, 1);
    EXPECT_EQ(Read_VIN_Num_Flag, 1);
    EXPECT_EQ(Read_Function_FLag, 1);
    // Test the Request Frame & Lenght
    EXPECT_EQ(Read_Data_Client.Data[PCI], 6);
    EXPECT_EQ(Read_Data_Client.Data[SID], Read_Service);
    EXPECT_EQ(Read_Data_Client.Data[ADD_Source], Client_Address);
    EXPECT_EQ(Read_Data_Client.Data[ADD_Target], Tempreture_Address);
    EXPECT_EQ(Read_Data_Client.Data[DID_1], VIN_number_First_byte);
    EXPECT_EQ(Read_Data_Client.Data[DID_2], VIN_number_Second_byte);
    EXPECT_EQ(Read_Data_Client.Length, 6);

    // Reset Flags

    Target_Selection_Menu_Flag = 0;
    Response_Permission_Menu_Flag = 0;
    Main_Menu_Flag = 0;
    Read_Data_Menu_Temp_or_Func_Flag = 0;
    Read_VIN_Num_Flag = 0;
    Read_Function_FLag = 0;

    // Reset Variables
    Menu = Target_Selection_Menu;
    SupressedPosRes_CLient = 1;
}

TEST(UDS_Client_Request, Write_Oil_Temperature)
{
    Display_Menu();
    mockUartDriver(Write_Oil_Temp);
    // Test Target Option
    EXPECT_EQ(Target_Selection_Menu_Flag, 1);
    EXPECT_EQ(Target_Address, Tempreture_Address);
    // Test Suppressed Responce Option
    EXPECT_EQ(Response_Permission_Menu_Flag, 1);
    EXPECT_EQ(SupressedPosRes_CLient, 1);
    // Test Main Menue
    EXPECT_EQ(Main_Menu_Flag, 1);
    EXPECT_EQ(Write_Data_Menu_Temp_or_Func_Flag, 1);
    EXPECT_EQ(Write_Oil_Temp_Flag, 1);
    EXPECT_EQ(Write_Function_FLag, 1);
    // Test the Request Frame & Lenght
    EXPECT_EQ(Write_Data_Client.Data[PCI], 8);
    EXPECT_EQ(Write_Data_Client.Data[SID], Write_Service);
    EXPECT_EQ(Write_Data_Client.Data[ADD_Source], Client_Address);
    EXPECT_EQ(Write_Data_Client.Data[ADD_Target], Tempreture_Address);
    EXPECT_EQ(Write_Data_Client.Data[DID_1], Oil_Temp_First_byte);
    EXPECT_EQ(Write_Data_Client.Data[DID_2], Oil_Temp_Second_byte);
    EXPECT_EQ(Write_Data_Client.Data[Data_DID], 0x01);
    EXPECT_EQ(Write_Data_Client.Data[Data_DID + 1], 0x02);
    EXPECT_EQ(Write_Data_Client.Length, 8);

    // Reset Flags

    Target_Selection_Menu_Flag = 0;
    Response_Permission_Menu_Flag = 0;
    Main_Menu_Flag = 0;
    Write_Data_Menu_Temp_or_Func_Flag = 0;
    Write_Oil_Temp_Flag = 0;
    Write_Function_FLag = 0;

    // Reset Variables
    Menu = Target_Selection_Menu;
    SupressedPosRes_CLient = 1;
}

TEST(UDS_Client_Request, Write_Oil_Pressure)
{
    Display_Menu();
    mockUartDriver(Write_Oil_Press);
    // Test Target Option
    EXPECT_EQ(Target_Selection_Menu_Flag, 1);
    EXPECT_EQ(Target_Address, Tempreture_Address);
    // Test Suppressed Responce Option
    EXPECT_EQ(Response_Permission_Menu_Flag, 1);
    EXPECT_EQ(SupressedPosRes_CLient, 1);
    // Test Main Menue
    EXPECT_EQ(Main_Menu_Flag, 1);
    EXPECT_EQ(Write_Data_Menu_Temp_or_Func_Flag, 1);
    EXPECT_EQ(Write_Oil_Press_Flag, 1);
    EXPECT_EQ(Write_Function_FLag, 1);
    // Test the Request Frame & Lenght
    EXPECT_EQ(Write_Data_Client.Data[PCI], 10);
    EXPECT_EQ(Write_Data_Client.Data[SID], Write_Service);
    EXPECT_EQ(Write_Data_Client.Data[ADD_Source], Client_Address);
    EXPECT_EQ(Write_Data_Client.Data[ADD_Target], Tempreture_Address);
    EXPECT_EQ(Write_Data_Client.Data[DID_1], Oil_Pressure_First_byte);
    EXPECT_EQ(Write_Data_Client.Data[DID_2], Oil_Pressure_Second_byte);
    EXPECT_EQ(Write_Data_Client.Data[Data_DID], 0x01);
    EXPECT_EQ(Write_Data_Client.Data[Data_DID + 1], 0x02);
    EXPECT_EQ(Write_Data_Client.Data[Data_DID + 2], 0x03);
    EXPECT_EQ(Write_Data_Client.Data[Data_DID + 3], 0x04);
    EXPECT_EQ(Write_Data_Client.Length, 10);

    // Reset Flags

    Target_Selection_Menu_Flag = 0;
    Response_Permission_Menu_Flag = 0;
    Main_Menu_Flag = 0;
    Write_Data_Menu_Temp_or_Func_Flag = 0;
    Write_Oil_Press_Flag = 0;
    Write_Function_FLag = 0;

    // Reset Variables
    Menu = Target_Selection_Menu;
    SupressedPosRes_CLient = 1;
}

TEST(UDS_Client_Request, Write_VIN_Number)
{
    Display_Menu();
    mockUartDriver(Write_VIN_Num);
    // Test Target Option
    EXPECT_EQ(Target_Selection_Menu_Flag, 1);
    EXPECT_EQ(Target_Address, Tempreture_Address);
    // Test Suppressed Responce Option
    EXPECT_EQ(Response_Permission_Menu_Flag, 1);
    EXPECT_EQ(SupressedPosRes_CLient, 1);
    // Test Main Menue
    EXPECT_EQ(Main_Menu_Flag, 1);
    EXPECT_EQ(Write_Data_Menu_Temp_or_Func_Flag, 1);
    EXPECT_EQ(Write_VIN_Num_Flag, 1);
    EXPECT_EQ(Write_Function_FLag, 1);
    // Test the Request Frame & Lenght
    EXPECT_EQ(Write_Data_Client.Data[PCI], 23);
    EXPECT_EQ(Write_Data_Client.Data[SID], Write_Service);
    EXPECT_EQ(Write_Data_Client.Data[ADD_Source], Client_Address);
    EXPECT_EQ(Write_Data_Client.Data[ADD_Target], Tempreture_Address);
    EXPECT_EQ(Write_Data_Client.Data[DID_1], VIN_number_First_byte);
    EXPECT_EQ(Write_Data_Client.Data[DID_2], VIN_number_Second_byte);

    EXPECT_EQ(Write_Data_Client.Data[Data_DID], 0x01);
    EXPECT_EQ(Write_Data_Client.Data[Data_DID + 1], 0x02);
    EXPECT_EQ(Write_Data_Client.Data[Data_DID + 2], 0x03);
    EXPECT_EQ(Write_Data_Client.Data[Data_DID + 3], 0x04);

    EXPECT_EQ(Write_Data_Client.Length, 23);

    // Reset Flags

    Target_Selection_Menu_Flag = 0;
    Response_Permission_Menu_Flag = 0;
    Main_Menu_Flag = 0;
    Write_Data_Menu_Temp_or_Func_Flag = 0;
    Write_VIN_Num_Flag = 0;
    Write_Function_FLag = 0;

    // Reset Variables
    Menu = Target_Selection_Menu;
    SupressedPosRes_CLient = 1;
}

TEST(UDS_Client_Request, Security_Service_Test_Seed)
{
    Display_Menu();
    mockUartDriver(Security_Seed);
    // Test Target Option
    EXPECT_EQ(Target_Selection_Menu_Flag, 1);
    EXPECT_EQ(Target_Address, Tempreture_Address);
    // Test Suppressed Responce Option
    EXPECT_EQ(Response_Permission_Menu_Flag, 1);
    EXPECT_EQ(SupressedPosRes_CLient, 1);
    // Test Main Menue
    EXPECT_EQ(Main_Menu_Flag, 1);
    EXPECT_EQ(Security_Access_Menu_Flag, 1);
    EXPECT_EQ(Security_Seed_Flag, 1);
    EXPECT_EQ(Security_Function_FLag, 1);
    // Test the Request Frame & Lenght
    EXPECT_EQ(Send_Security_Seed.Data[PCI], 5);
    EXPECT_EQ(Send_Security_Seed.Data[SID], Security_Service);
    EXPECT_EQ(Send_Security_Seed.Data[ADD_Source], Client_Address);
    EXPECT_EQ(Send_Security_Seed.Data[ADD_Target], Tempreture_Address);
    EXPECT_EQ(Send_Security_Seed.Data[Sub_F], 0x81); // SecuritySeed + Suppressed +ve Res
    EXPECT_EQ(Send_Security_Seed.Length, 5);

    // Reset Flags

    Target_Selection_Menu_Flag = 0;
    Response_Permission_Menu_Flag = 0;
    Main_Menu_Flag = 0;
    Security_Access_Menu_Flag = 0;
    Security_Seed_Flag = 0;
    Security_Function_FLag = 0;

    // Reset Variables
    Menu = Target_Selection_Menu;
    SupressedPosRes_CLient = 1;
}

TEST(UDS_Client_Request, Security_Service_Test_Key)
{
    Display_Menu();
    mockUartDriver(Security_Key);
    // Test Target Option
    EXPECT_EQ(Target_Selection_Menu_Flag, 1);
    EXPECT_EQ(Target_Address, Tempreture_Address);
    // Test Suppressed Responce Option
    EXPECT_EQ(Response_Permission_Menu_Flag, 1);
    EXPECT_EQ(SupressedPosRes_CLient, 1);
    // Test Main Menue
    EXPECT_EQ(Main_Menu_Flag, 1);
    EXPECT_EQ(Security_Access_Menu_Flag, 1);
    EXPECT_EQ(Security_Key_Flag, 1);
    EXPECT_EQ(Security_Function_FLag, 1);
    // Test the Request Frame & Lenght
    EXPECT_EQ(Send_Security_Seed.Data[PCI], 9);
    EXPECT_EQ(Send_Security_Seed.Data[SID], Security_Service);
    EXPECT_EQ(Send_Security_Seed.Data[ADD_Source], Client_Address);
    EXPECT_EQ(Send_Security_Seed.Data[ADD_Target], Tempreture_Address);
    EXPECT_EQ(Send_Security_Seed.Data[Sub_F], 0x82); // SecuritySeed + Suppressed +ve Res
    EXPECT_EQ(Send_Security_Seed.Length, 9);

    // Reset Flags

    Target_Selection_Menu_Flag = 0;
    Response_Permission_Menu_Flag = 0;
    Main_Menu_Flag = 0;
    Security_Access_Menu_Flag = 0;
    Security_Seed_Flag = 0;
    Security_Function_FLag = 0;

    // Reset Variables
    Menu = Target_Selection_Menu;
    SupressedPosRes_CLient = 1;
}

TEST(UDS_Client_Request, Tester_Representer_Test)
{
    Display_Menu();
    mockUartDriver(Tester_Representer);
    // Test Target Option
    EXPECT_EQ(Target_Selection_Menu_Flag, 1);
    EXPECT_EQ(Target_Address, Tempreture_Address);
    // Test Suppressed Responce Option
    EXPECT_EQ(Response_Permission_Menu_Flag, 1);
    EXPECT_EQ(SupressedPosRes_CLient, 1);
    // Test Main Menue
    EXPECT_EQ(Main_Menu_Flag, 1);
    EXPECT_EQ(Tester_Representer_Function_FLag, 1);

    // Test the Request Frame & Lenght
    EXPECT_EQ(PduInfoTypePtr.Data[PCI], 5);
    EXPECT_EQ(PduInfoTypePtr.Data[SID], Tester_Representer_Service);
    EXPECT_EQ(PduInfoTypePtr.Data[ADD_Source], Client_Address);
    EXPECT_EQ(PduInfoTypePtr.Data[ADD_Target], Tempreture_Address);
    EXPECT_EQ(PduInfoTypePtr.Data[Sub_F], 0x80); // SecuritySeed + Suppressed +ve Res
    EXPECT_EQ(PduInfoTypePtr.Length, 5);

    // Reset Flags

    Target_Selection_Menu_Flag = 0;
    Response_Permission_Menu_Flag = 0;
    Main_Menu_Flag = 0;
    Tester_Representer_Function_FLag = 0;

    // Reset Variables
    Menu = Target_Selection_Menu;
    SupressedPosRes_CLient = 1;
}

TEST(UDS_Client_Request, Read_Oil_Pressure_Target_with_No_Supp_Resp)
{
    Display_Menu();
    mockUartDriver(Read_Pressure_Target_with_No_Supp_Resp);
    // Test Target Option
    EXPECT_EQ(Target_Selection_Menu_Flag, 1);
    EXPECT_EQ(Target_Address, Pressure_Address);
    // Test Suppressed Responce Option
    EXPECT_EQ(Response_Permission_Menu_Flag, 1);
    EXPECT_EQ(SupressedPosRes_CLient, 0);
    // Test Main Menue
    EXPECT_EQ(Main_Menu_Flag, 1);
    EXPECT_EQ(Read_Data_Menu_Press_or_Func_Flag, 1);
    EXPECT_EQ(Read_Oil_Press_Flag, 1);
    EXPECT_EQ(Read_Function_FLag, 1);
    // Test the Request Frame & Lenght
    EXPECT_EQ(Read_Data_Client.Data[PCI], 6);
    EXPECT_EQ(Read_Data_Client.Data[SID], Read_Service);
    EXPECT_EQ(Read_Data_Client.Data[ADD_Source], Client_Address);
    EXPECT_EQ(Read_Data_Client.Data[ADD_Target], Pressure_Address);
    EXPECT_EQ(Read_Data_Client.Data[DID_1], 0x75); // Oil_Pressure_First_byte + No supp Res
    EXPECT_EQ(Read_Data_Client.Data[DID_2], Oil_Pressure_Second_byte);
    EXPECT_EQ(Read_Data_Client.Length, 6);

    // Reset Flags

    Target_Selection_Menu_Flag = 0;
    Response_Permission_Menu_Flag = 0;
    Main_Menu_Flag = 0;
    Read_Data_Menu_Press_or_Func_Flag = 0;
    Read_Oil_Press_Flag = 0;
    Read_Function_FLag = 0;

    // Reset Variables
    Menu = Target_Selection_Menu;
    SupressedPosRes_CLient = 1;
}

TEST(UDS_Client_Request, Write_Oil_Pressure_Target_with_No_Supp_Resp)
{
    Display_Menu();
    mockUartDriver(Write_Pressure_Target_with_No_Supp_Resp);
    // Test Target Option
    EXPECT_EQ(Target_Selection_Menu_Flag, 1);
    EXPECT_EQ(Target_Address, Pressure_Address);
    // Test Suppressed Responce Option
    EXPECT_EQ(Response_Permission_Menu_Flag, 1);
    EXPECT_EQ(SupressedPosRes_CLient, 0);
    // Test Main Menue
    EXPECT_EQ(Main_Menu_Flag, 1);
    EXPECT_EQ(Write_Data_Menu_Press_or_Func_Flag, 1);
    EXPECT_EQ(Write_Oil_Press_Flag, 1);
    EXPECT_EQ(Write_Function_FLag, 1);
    // Test the Request Frame & Lenght
    EXPECT_EQ(Write_Data_Client.Data[PCI], 10);
    EXPECT_EQ(Write_Data_Client.Data[SID], Write_Service);
    EXPECT_EQ(Write_Data_Client.Data[ADD_Source], Client_Address);
    EXPECT_EQ(Write_Data_Client.Data[ADD_Target], Pressure_Address);
    EXPECT_EQ(Write_Data_Client.Data[DID_1], 0x75); // Oil_Pressure_First_byte + No supp Res
    EXPECT_EQ(Write_Data_Client.Data[DID_2], Oil_Pressure_Second_byte);
    EXPECT_EQ(Write_Data_Client.Data[Data_DID], 0x01);
    EXPECT_EQ(Write_Data_Client.Data[Data_DID + 1], 0x02);
    EXPECT_EQ(Write_Data_Client.Data[Data_DID + 2], 0x03);
    EXPECT_EQ(Write_Data_Client.Data[Data_DID + 3], 0x04);
    EXPECT_EQ(Write_Data_Client.Length, 10);

    // Reset Flags

    Target_Selection_Menu_Flag = 0;
    Response_Permission_Menu_Flag = 0;
    Main_Menu_Flag = 0;
    Write_Data_Menu_Press_or_Func_Flag = 0;
    Write_Oil_Press_Flag = 0;
    Write_Function_FLag = 0;

    // Reset Variables
    Menu = Target_Selection_Menu;
    SupressedPosRes_CLient = 1;
}

/***************************************************************************************************************/
/***************************************************************************************************************/

/***************************************************************************************************************/
/********************************************** Read test cases ************************************************/
/***************************************************************************************************************/

TEST(UDS_Client_PR_Read, OilTemperature_Test)
{
    Test_Case = OilTemperature_Read;
    CanTP_Stub(Test_Case);
    // EXPECT_EQ(UDS_Tx_Confirm, 1);

    UDS_MainFunction();

    // Check the return value
    EXPECT_EQ(OilTemperature_ReadFlag, 1);
}

TEST(UDS_Client_PR_Read, OilPressure_Test)
{
    Test_Case = OilPressure_Read;
    CanTP_Stub(Test_Case);
    // EXPECT_EQ(UDS_Tx_Confirm, 1);

    UDS_MainFunction();

    // Check the return value
    EXPECT_EQ(OilPressure_ReadFlag, 1);
}

TEST(UDS_Client_PR_Read, VIN_number_Test)
{
    Test_Case = VIN_number_Read;
    CanTP_Stub(Test_Case);
    // EXPECT_EQ(UDS_Tx_Confirm, 1);

    UDS_MainFunction();

    // Check the return value
    EXPECT_EQ(VIN_number_ReadFlag, 1);
}
/***************************************************************************************************************/
/***************************************************************************************************************/

/***************************************************************************************************************/
/********************************************** Write test cases ************************************************/
/***************************************************************************************************************/

TEST(UDS_Client_PR_Write, OilTemperature_Test)
{
    Test_Case = OilTemperature_Write;
    CanTP_Stub(Test_Case);
    // EXPECT_EQ(UDS_Tx_Confirm, 1);

    UDS_MainFunction();

    // Check the return value
    EXPECT_EQ(OilTemperature_WriteFlag, 1);
}

TEST(UDS_Client_PR_Write, OilPressure_Test)
{
    Test_Case = OilPressure_Write;
    CanTP_Stub(Test_Case);
    // EXPECT_EQ(UDS_Tx_Confirm, 1);

    UDS_MainFunction();

    // Check the return value
    EXPECT_EQ(OilPressure_WriteFlag, 1);
}

TEST(UDS_Client_PR_Write, VIN_number_Test)
{
    Test_Case = VIN_number_Write;
    CanTP_Stub(Test_Case);
    // EXPECT_EQ(UDS_Tx_Confirm, 1);

    UDS_MainFunction();

    // Check the return value
    EXPECT_EQ(VIN_number_WriteFlag, 1);
}
/***************************************************************************************************************/
/***************************************************************************************************************/

/***************************************************************************************************************/
/**************************************** Control Session test cases *******************************************/
/***************************************************************************************************************/

TEST(UDS_Client_PR_ControlSession, DefaultSession_Test)
{
    Test_Case = DefaultSession;
    CanTP_Stub(Test_Case);
    // EXPECT_EQ(UDS_Tx_Confirm, 1);

    UDS_MainFunction();

    // Check the return value
    EXPECT_EQ(DefaultSession_Flag, 1);
}

TEST(UDS_Client_PR_ControlSession, ExtendedSession_Test)
{
    Test_Case = ExtendedSession;
    CanTP_Stub(Test_Case);
    // EXPECT_EQ(UDS_Tx_Confirm, 1);

    UDS_MainFunction();

    // Check the return value
    EXPECT_EQ(ExtendedSession_Flag, 1);
}

/***************************************************************************************************************/
/***************************************************************************************************************/

/***************************************************************************************************************/
/**************************************** Security test cases *******************************************/
/***************************************************************************************************************/

TEST(UDS_Client_PR_ControlSession, SecuritySeed_Test)
{
    Test_Case = SecuritySeed;
    CanTP_Stub(Test_Case);
    // EXPECT_EQ(UDS_Tx_Confirm, 1);

    UDS_MainFunction();

    // Check the return value
    EXPECT_EQ(SecuritySeed_Flag, 1);
}

TEST(UDS_Client_PR_ControlSession, SecurityKey_Test)
{
    Test_Case = SecurityKey;
    CanTP_Stub(Test_Case);
    // EXPECT_EQ(UDS_Tx_Confirm, 1);

    UDS_MainFunction();

    // Check the return value
    EXPECT_EQ(SecurityKey_Flag, 1);
}

/***************************************************************************************************************/
/***************************************************************************************************************/

/***************************************************************************************************************/
/**************************************** Tester Presenter test cases *******************************************/
/***************************************************************************************************************/

TEST(UDS_Client_PR_ControlSession, TesterPresent_Test)
{
    Test_Case = TesterPresent;
    CanTP_Stub(Test_Case);
    // EXPECT_EQ(UDS_Tx_Confirm, 1);

    UDS_MainFunction();

    // Check the return value
    EXPECT_EQ(TesterPresent_Flag, 1);
}

/***************************************************************************************************************/
/***************************************************************************************************************/

void CanTP_Stub(uint8_t Test_Case)
{
    uint32_t testRxPduId = 0;
    PduInfoType testPduInfo;

#if Response == serviceNotSupported
    testPduInfo.Length = 6;
    testPduInfo.Data[0] = 0x06; // PCI
    testPduInfo.Data[1] = 0x7F; // negative Response
    testPduInfo.Data[2] = 0x3D; // Source Addressing "Server node" 3D or 4D
    testPduInfo.Data[3] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[4] = 0x99; // SID_NR_INDEX ... may be any SID
    testPduInfo.Data[5] = 0x11; // NRC_INDEX for invalidKey

    UDS_Client_Callback(testRxPduId, &testPduInfo);

#elif Response == securityAccessDenied
    testPduInfo.Length = 6;
    testPduInfo.Data[0] = 0x06; // PCI
    testPduInfo.Data[1] = 0x7F; // negative Response
    testPduInfo.Data[2] = 0x3D; // Source Addressing "Server node" 3D or 4D
    testPduInfo.Data[3] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[4] = 0x99; // SID_NR_INDEX may be any SID
    testPduInfo.Data[5] = 0x33; // NRC_INDEX for securityAccessDenied

    UDS_Client_Callback(testRxPduId, &testPduInfo);

#elif Response == subFunctionNotSupported
    testPduInfo.Length = 6;
    testPduInfo.Data[0] = 0x06; // PCI
    testPduInfo.Data[1] = 0x7F; // negative Response
    testPduInfo.Data[2] = 0x3D; // Source Addressing "Server node" 3D or 4D
    testPduInfo.Data[3] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[4] = 0x99; // SID_NR_INDEX may be any SID
    testPduInfo.Data[5] = 0x12; // NRC_INDEX for invalidKey

    UDS_Client_Callback(testRxPduId, &testPduInfo);

#elif Response == serviceNotSupportedInActiveSession
    testPduInfo.Length = 6;
    testPduInfo.Data[0] = 0x06; // PCI
    testPduInfo.Data[1] = 0x7F; // negative Response
    testPduInfo.Data[2] = 0x3D; // Source Addressing "Server node" 3D or 4D
    testPduInfo.Data[3] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[4] = 0x99; // SID_NR_INDEX may be any SID
    testPduInfo.Data[5] = 0x7F; // NRC_INDEX for invalidKey

    UDS_Client_Callback(testRxPduId, &testPduInfo);

#elif Response == invalidKey
    testPduInfo.Length = 6;
    testPduInfo.Data[0] = 0x06; // PCI
    testPduInfo.Data[1] = 0x7F; // negative Response
    testPduInfo.Data[2] = 0x3D; // Source Addressing "Server node" 3D or 4D
    testPduInfo.Data[3] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[4] = 0x99; // SID_NR_INDEX
    testPduInfo.Data[5] = 0x35; // NRC_INDEX for invalidKey

    UDS_Client_Callback(testRxPduId, &testPduInfo);

/********************************************** Read test cases ************************************************/
#elif Response == OilTemperature_Read
    testPduInfo.Length = 8;
    testPduInfo.Data[0] = 0x08; // PCI
    testPduInfo.Data[1] = 0x3D; // Source Addressing "Temperature node"
    testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[3] = 0x62; // SID + 0x40
    testPduInfo.Data[4] = 0xF1; // DID_1
    testPduInfo.Data[5] = 0x3D; // DID_2
    testPduInfo.Data[6] = 0x54; // Data
    testPduInfo.Data[7] = 0x32; // Data

    UDS_Client_Callback(testRxPduId, &testPduInfo);

#elif Response == OilPressure_Read
    testPduInfo.Length = 10;
    testPduInfo.Data[0] = 0x10; // PCI
    testPduInfo.Data[1] = 0x4D; // Source Addressing "Pressure node"
    testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[3] = 0x62; // SID + 0x40
    testPduInfo.Data[4] = 0xF5; // DID_1
    testPduInfo.Data[5] = 0x3D; // DID_2
    testPduInfo.Data[6] = 0x54; // Data
    testPduInfo.Data[7] = 0x32; // Data
    testPduInfo.Data[8] = 0x88; // Data
    testPduInfo.Data[9] = 0x77; // Data
    UDS_Client_Callback(testRxPduId, &testPduInfo);

#elif Response == VIN_number_Read
    testPduInfo.Length = 23;
    testPduInfo.Data[0] = 0x23;  // PCI
    testPduInfo.Data[1] = 0x3D;  // Source Addressing "Server node" maybe 0x3D or 0x4D
    testPduInfo.Data[2] = 0xF1;  // Target Addressing "Client node"
    testPduInfo.Data[3] = 0x62;  // SID + 0x40
    testPduInfo.Data[4] = 0xF2;  // DID_1
    testPduInfo.Data[5] = 0x3D;  // DID_2
    testPduInfo.Data[6] = 0x54;  // Data
    testPduInfo.Data[7] = 0x32;  // Data
    testPduInfo.Data[8] = 0x88;  // Data
    testPduInfo.Data[9] = 0x77;  // Data
    testPduInfo.Data[10] = 0x54; // Data
    testPduInfo.Data[11] = 0x32; // Data
    testPduInfo.Data[12] = 0x88; // Data
    testPduInfo.Data[13] = 0x77; // Data
    testPduInfo.Data[14] = 0x54; // Data
    testPduInfo.Data[15] = 0x32; // Data
    testPduInfo.Data[16] = 0x88; // Data
    testPduInfo.Data[17] = 0x77; // Data
    testPduInfo.Data[18] = 0x54; // Data
    testPduInfo.Data[19] = 0x32; // Data
    testPduInfo.Data[20] = 0x88; // Data
    testPduInfo.Data[21] = 0x77; // Data
    testPduInfo.Data[22] = 0x54; // Data

    UDS_Client_Callback(testRxPduId, &testPduInfo);

    /********************************************** Write test cases ************************************************/

#elif Response == OilTemperature_Write
    testPduInfo.Length = 6;
    testPduInfo.Data[0] = 0x06; // PCI
    testPduInfo.Data[1] = 0x3D; // Source Addressing "Temperature node"
    testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[3] = 0x6E; // SID + 0x40
    testPduInfo.Data[4] = 0xF1; // DID_1
    testPduInfo.Data[5] = 0x3D; // DID_2

    UDS_Client_Callback(testRxPduId, &testPduInfo);

#elif Response == OilPressure_Write
    testPduInfo.Length = 6;
    testPduInfo.Data[0] = 0x06; // PCI
    testPduInfo.Data[1] = 0x4D; // Source Addressing "Pressure node"
    testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[3] = 0x6E; // SID + 0x40
    testPduInfo.Data[4] = 0xF5; // DID_1
    testPduInfo.Data[5] = 0x3D; // DID_2

    UDS_Client_Callback(testRxPduId, &testPduInfo);

#elif Response == VIN_number_Write
    testPduInfo.Length = 6;
    testPduInfo.Data[0] = 0x06; // PCI
    testPduInfo.Data[1] = 0x3D; // Source Addressing "Server node" maybe 0x3D or 0x4D
    testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[3] = 0x6E; // SID + 0x40
    testPduInfo.Data[4] = 0xF2; // DID_1
    testPduInfo.Data[5] = 0x3D; // DID_2

    UDS_Client_Callback(testRxPduId, &testPduInfo);

    /**************************************** Control Session test cases *******************************************/

#elif Response == DefaultSession
    testPduInfo.Length = 5;
    testPduInfo.Data[0] = 0x05; // PCI
    testPduInfo.Data[1] = 0x3D; // Source Addressing "Server node" 0x3D or 0x4D
    testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[3] = 0x50; // SID + 0x40
    testPduInfo.Data[4] = 0x01; // Sub_F

    UDS_Client_Callback(testRxPduId, &testPduInfo);

#elif Response == ExtendedSession
    testPduInfo.Length = 5;
    testPduInfo.Data[0] = 0x05; // PCI
    testPduInfo.Data[1] = 0x3D; // Source Addressing "Server node" 0x3D or 0x4D
    testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[3] = 0x50; // SID + 0x40
    testPduInfo.Data[4] = 0x03; // Sub_F

    UDS_Client_Callback(testRxPduId, &testPduInfo);

    /**************************************** Security test cases *******************************************/

#elif Response == SecuritySeed
    testPduInfo.Length = 9;
    testPduInfo.Data[0] = 0x09; // PCI
    testPduInfo.Data[1] = 0x3D; // Source Addressing "Server node" 0x3D or 0x4D
    testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[3] = 0x67; // SID + 0x40
    testPduInfo.Data[4] = 0x01; // Sub_F
    testPduInfo.Data[5] = 0x11; // Data
    testPduInfo.Data[6] = 0x11; // Data
    testPduInfo.Data[7] = 0x11; // Data
    testPduInfo.Data[8] = 0x11; // Data

    UDS_Client_Callback(testRxPduId, &testPduInfo);

#elif Response == SecurityKey
    testPduInfo.Length = 5;
    testPduInfo.Data[0] = 0x05; // PCI
    testPduInfo.Data[1] = 0x3D; // Source Addressing "Server node" 0x3D or 0x4D
    testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[3] = 0x67; // SID + 0x40
    testPduInfo.Data[4] = 0x02; // Sub_F

    UDS_Client_Callback(testRxPduId, &testPduInfo);

    /**************************************** Tester Presenter test cases *******************************************/

#elif Response == TesterPresent
    testPduInfo.Length = 4;
    testPduInfo.Data[0] = 0x04; // PCI
    testPduInfo.Data[1] = 0x3D; // Source Addressing "Server node" 0x3D or 0x4D
    testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
    testPduInfo.Data[3] = 0x7E; // SID + 0x40

    UDS_Client_Callback(testRxPduId, &testPduInfo);

#elif Response == ALL_TEST_CASES
    if (Test_Case == serviceNotSupported)
    {
        testPduInfo.Length = 6;
        testPduInfo.Data[0] = 0x06; // PCI
        testPduInfo.Data[1] = 0x7F; // negative Response
        testPduInfo.Data[2] = 0x3D; // Source Addressing "Server node" 3D or 4D
        testPduInfo.Data[3] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[4] = 0x99; // SID_NR_INDEX ... may be any SID
        testPduInfo.Data[5] = 0x11; // NRC_INDEX for invalidKey

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    else if (Test_Case == securityAccessDenied)
    {
        testPduInfo.Length = 6;
        testPduInfo.Data[0] = 0x06; // PCI
        testPduInfo.Data[1] = 0x7F; // negative Response
        testPduInfo.Data[2] = 0x3D; // Source Addressing "Server node" 3D or 4D
        testPduInfo.Data[3] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[4] = 0x99; // SID_NR_INDEX may be any SID
        testPduInfo.Data[5] = 0x33; // NRC_INDEX for securityAccessDenied

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    else if (Test_Case == subFunctionNotSupported)
    {
        testPduInfo.Length = 6;
        testPduInfo.Data[0] = 0x06; // PCI
        testPduInfo.Data[1] = 0x7F; // negative Response
        testPduInfo.Data[2] = 0x3D; // Source Addressing "Server node" 3D or 4D
        testPduInfo.Data[3] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[4] = 0x99; // SID_NR_INDEX may be any SID
        testPduInfo.Data[5] = 0x12; // NRC_INDEX for invalidKey

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    else if (Test_Case == serviceNotSupportedInActiveSession)
    {
        testPduInfo.Length = 6;
        testPduInfo.Data[0] = 0x06; // PCI
        testPduInfo.Data[1] = 0x7F; // negative Response
        testPduInfo.Data[2] = 0x3D; // Source Addressing "Server node" 3D or 4D
        testPduInfo.Data[3] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[4] = 0x99; // SID_NR_INDEX may be any SID
        testPduInfo.Data[5] = 0x7F; // NRC_INDEX for invalidKey

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    else if (Test_Case == invalidKey)
    {
        testPduInfo.Length = 6;
        testPduInfo.Data[0] = 0x06; // PCI
        testPduInfo.Data[1] = 0x7F; // negative Response
        testPduInfo.Data[2] = 0x3D; // Source Addressing "Server node" 3D or 4D
        testPduInfo.Data[3] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[4] = 0x99; // SID_NR_INDEX
        testPduInfo.Data[5] = 0x35; // NRC_INDEX for invalidKey

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    /********************************************** Read test cases ************************************************/
    else if (Test_Case == OilTemperature_Read)
    {
        testPduInfo.Length = 8;
        testPduInfo.Data[0] = 0x08; // PCI
        testPduInfo.Data[1] = 0x3D; // Source Addressing "Temperature node"
        testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[3] = 0x62; // SID + 0x40
        testPduInfo.Data[4] = 0xF1; // DID_1
        testPduInfo.Data[5] = 0x3D; // DID_2
        testPduInfo.Data[6] = 0x54; // Data
        testPduInfo.Data[7] = 0x32; // Data

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    else if (Test_Case == OilPressure_Read)
    {
        testPduInfo.Length = 10;
        testPduInfo.Data[0] = 0x10; // PCI
        testPduInfo.Data[1] = 0x4D; // Source Addressing "Pressure node"
        testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[3] = 0x62; // SID + 0x40
        testPduInfo.Data[4] = 0xF5; // DID_1
        testPduInfo.Data[5] = 0x3D; // DID_2
        testPduInfo.Data[6] = 0x54; // Data
        testPduInfo.Data[7] = 0x32; // Data
        testPduInfo.Data[8] = 0x88; // Data
        testPduInfo.Data[9] = 0x77; // Data
        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    else if (Test_Case == VIN_number_Read)
    {
        testPduInfo.Length = 23;
        testPduInfo.Data[0] = 0x23;  // PCI
        testPduInfo.Data[1] = 0x3D;  // Source Addressing "Server node" maybe 0x3D or 0x4D
        testPduInfo.Data[2] = 0xF1;  // Target Addressing "Client node"
        testPduInfo.Data[3] = 0x62;  // SID + 0x40
        testPduInfo.Data[4] = 0xF2;  // DID_1
        testPduInfo.Data[5] = 0x3D;  // DID_2
        testPduInfo.Data[6] = 0x54;  // Data
        testPduInfo.Data[7] = 0x32;  // Data
        testPduInfo.Data[8] = 0x88;  // Data
        testPduInfo.Data[9] = 0x77;  // Data
        testPduInfo.Data[10] = 0x54; // Data
        testPduInfo.Data[11] = 0x32; // Data
        testPduInfo.Data[12] = 0x88; // Data
        testPduInfo.Data[13] = 0x77; // Data
        testPduInfo.Data[14] = 0x54; // Data
        testPduInfo.Data[15] = 0x32; // Data
        testPduInfo.Data[16] = 0x88; // Data
        testPduInfo.Data[17] = 0x77; // Data
        testPduInfo.Data[18] = 0x54; // Data
        testPduInfo.Data[19] = 0x32; // Data
        testPduInfo.Data[20] = 0x88; // Data
        testPduInfo.Data[21] = 0x77; // Data
        testPduInfo.Data[22] = 0x54; // Data

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    /********************************************** Write test cases ************************************************/

    else if (Test_Case == OilTemperature_Write)
    {
        testPduInfo.Length = 6;
        testPduInfo.Data[0] = 0x06; // PCI
        testPduInfo.Data[1] = 0x3D; // Source Addressing "Temperature node"
        testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[3] = 0x6E; // SID + 0x40
        testPduInfo.Data[4] = 0xF1; // DID_1
        testPduInfo.Data[5] = 0x3D; // DID_2

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    else if (Test_Case == OilPressure_Write)
    {
        testPduInfo.Length = 6;
        testPduInfo.Data[0] = 0x06; // PCI
        testPduInfo.Data[1] = 0x4D; // Source Addressing "Pressure node"
        testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[3] = 0x6E; // SID + 0x40
        testPduInfo.Data[4] = 0xF5; // DID_1
        testPduInfo.Data[5] = 0x3D; // DID_2

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    else if (Test_Case == VIN_number_Write)
    {
        testPduInfo.Length = 6;
        testPduInfo.Data[0] = 0x06; // PCI
        testPduInfo.Data[1] = 0x3D; // Source Addressing "Server node" maybe 0x3D or 0x4D
        testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[3] = 0x6E; // SID + 0x40
        testPduInfo.Data[4] = 0xF2; // DID_1
        testPduInfo.Data[5] = 0x3D; // DID_2

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    /**************************************** Control Session test cases *******************************************/

    else if (Test_Case == DefaultSession)
    {
        testPduInfo.Length = 5;
        testPduInfo.Data[0] = 0x05; // PCI
        testPduInfo.Data[1] = 0x3D; // Source Addressing "Server node" 0x3D or 0x4D
        testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[3] = 0x50; // SID + 0x40
        testPduInfo.Data[4] = 0x01; // Sub_F

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    else if (Test_Case == ExtendedSession)
    {
        testPduInfo.Length = 5;
        testPduInfo.Data[0] = 0x05; // PCI
        testPduInfo.Data[1] = 0x3D; // Source Addressing "Server node" 0x3D or 0x4D
        testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[3] = 0x50; // SID + 0x40
        testPduInfo.Data[4] = 0x03; // Sub_F

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    /**************************************** Security test cases *******************************************/

    else if (Test_Case == SecuritySeed)
    {
        testPduInfo.Length = 9;
        testPduInfo.Data[0] = 0x09; // PCI
        testPduInfo.Data[1] = 0x3D; // Source Addressing "Server node" 0x3D or 0x4D
        testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[3] = 0x67; // SID + 0x40
        testPduInfo.Data[4] = 0x01; // Sub_F
        testPduInfo.Data[5] = 0x11; // Data
        testPduInfo.Data[6] = 0x11; // Data
        testPduInfo.Data[7] = 0x11; // Data
        testPduInfo.Data[8] = 0x11; // Data

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    else if (Test_Case == SecurityKey)
    {
        testPduInfo.Length = 5;
        testPduInfo.Data[0] = 0x05; // PCI
        testPduInfo.Data[1] = 0x3D; // Source Addressing "Server node" 0x3D or 0x4D
        testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[3] = 0x67; // SID + 0x40
        testPduInfo.Data[4] = 0x02; // Sub_F

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

    /**************************************** Tester Presenter test cases *******************************************/

    else if (Test_Case == TesterPresent)
    {
        testPduInfo.Length = 4;
        testPduInfo.Data[0] = 0x04; // PCI
        testPduInfo.Data[1] = 0x3D; // Source Addressing "Server node" 0x3D or 0x4D
        testPduInfo.Data[2] = 0xF1; // Target Addressing "Client node"
        testPduInfo.Data[3] = 0x7E; // SID + 0x40

        UDS_Client_Callback(testRxPduId, &testPduInfo);
    }

#else
#endif
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
