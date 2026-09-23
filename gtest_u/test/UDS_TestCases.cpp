#include <gtest/gtest.h>
#include <stdint.h>
#include <stdio.h>
extern "C"
{
	#include "../inc/STD_TYPES.h"
	#include "../inc/APP_UDS_Mock.h"
	#include "../inc/APP_UDS_Diag.h"

	extern PduInfoType msg;
	extern volatile uint8_t CANTP_FLAG;
	extern volatile uint8_t ResetTimer_FLAG;
	extern uint8_t StopTimer_FLAG;
	extern uint8_t StartTimer_FLAG;
	extern volatile uint8_t flag_sub_fun;
    extern volatile uint32_t Oil_Pressure_var;
    extern volatile uint32_t Oil_Temp_var;
}


TEST(UDS_UnitTest, server_call_back_ControlSer_DefaultSess) {
    // Arrange
    uint8_t ID = 0;
    PduInfoType pdu;

    pdu.Data[0] = 0x05; 
    pdu.Data[1] = 0x01;
    pdu.Data[2] = 0x3D; //Temperature Address 
    pdu.Data[3] = 0x10; //control Service
    pdu.Data[4] = 0x81; //Default Session
    pdu.Length = 5;

    server_call_back(ID,&pdu);

    // Assert
    EXPECT_EQ(ResetTimer_FLAG,1);
    EXPECT_EQ(StartTimer_FLAG,0);
    EXPECT_EQ(flag_sub_fun,1);
    EXPECT_EQ(CANTP_FLAG,1); // 

}
TEST(UDS_UnitTest, server_call_back_ControlSer_ExtendedSess) {
    // Arrange
    uint8_t ID = 0;
    PduInfoType pdu;

    pdu.Data[0] = 0x05; 
    pdu.Data[1] = 0xF1;
    pdu.Data[2] = 0x3D; //Temperature Address 
    pdu.Data[3] = 0x10; //control Service
    pdu.Data[4] = 0x03; //Extended Session
    pdu.Length = 5;


    server_call_back(ID,&pdu);

    // Assert
    EXPECT_EQ(ResetTimer_FLAG,1);
    EXPECT_EQ(StartTimer_FLAG,1);
    EXPECT_EQ(CANTP_FLAG,1); // 

}


TEST(UDS_UnitTest, server_call_back_Read_Service_1) {
    // Arrange
    uint8_t ID = 0;
    PduInfoType pdu;

    pdu.Data[0] = 0x06; 
    pdu.Data[1] = 0xF1;
    pdu.Data[2] = 0x3D; //Temperature Address 
    pdu.Data[3] = 0x22;  // Read service 
    pdu.Data[4] = 0xF1; //DID_1
    pdu.Data[5] = 0x3D; //DID_2
    pdu.Length = 6;

    server_call_back(ID,&pdu);

    // Assert
    EXPECT_EQ(CANTP_FLAG, 1); // to check if it send positive resopnse to can tp or not

}




TEST(UDS_UnitTest, server_call_back_Write_Service_1) {
    // Arrange
    uint8_t ID = 0;
    PduInfoType pdu;

    pdu.Data[0] = 0x08; //pci
    pdu.Data[1] = 0xF1; 
    pdu.Data[2] = 0x3D; //Temperature Address 
    pdu.Data[3] = 0x2E;  //SID
    pdu.Data[4] = 0xF1; //DID_1
    pdu.Data[5] = 0x3D; //DID_2
    pdu.Data[6] = 0x12;
    pdu.Data[7] = 0x34;
    pdu.Length = 8;

    server_call_back(ID,&pdu);

    // Assert
    EXPECT_EQ(CANTP_FLAG, 1); // to check if it send positive resopnse to can tp or not

}

TEST(UDS_UnitTest, server_call_back_Write_Service_2) {
    // Arrange
    uint8_t ID = 0;
    PduInfoType pdu;

    pdu.Data[0] = 0x10; //pci
    pdu.Data[1] = 0xF1; 
    pdu.Data[2] = 0x4D; //Temperature Address 
    pdu.Data[3] = 0x2E;  //SID
    pdu.Data[4] = 0xF5; //DID_1
    pdu.Data[5] = 0x3D; //DID_2
    pdu.Data[6] = 0x12;
    pdu.Data[7] = 0x34;
    pdu.Data[8] = 0x54;
    pdu.Data[9] = 0x65;

    pdu.Length = 10;

    server_call_back(ID,&pdu);

    // Assert
    EXPECT_EQ(CANTP_FLAG, 1); // to check if it send positive resopnse to can tp or not

}


TEST(UDS_UnitTest, server_call_back_Read_Service_2) {
    // Arrange
    uint8_t ID = 0;
    PduInfoType pdu;

    pdu.Data[0] = 0x06; 
    pdu.Data[1] = 0xF1;
    pdu.Data[2] = 0x4D; //Temperature Address 
    pdu.Data[3] = 0x22;  // Read service 
    pdu.Data[4] = 0xF5; //DID_1
    pdu.Data[5] = 0x3D; //DID_2
    pdu.Length = 6;

    server_call_back(ID,&pdu);

    // Assert
    EXPECT_EQ(CANTP_FLAG, 1); // to check if it send positive resopnse to can tp or not

}


TEST(UDS_UnitTest, server_call_back_Tester_Present_Service) {
    // Arrange
    uint8_t ID = 0;
    PduInfoType pdu;

    pdu.Data[0] = 0x04; 
    pdu.Data[1] = 0x01;
    pdu.Data[2] = 0x3D; //Temperature Address 
    pdu.Data[3] = 0x3E;  // Tester Present service 
    pdu.Data[4] = 0x01; //seed = 0x01
    pdu.Length = 5;

    server_call_back(ID,&pdu);

    // Assert
    EXPECT_EQ(CANTP_FLAG, 1); // to check if it send positive resopnse to can tp or not

}
