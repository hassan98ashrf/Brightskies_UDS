// CanTpTest.cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>
// #include <typeinfo>
 //#include "../inc/CanTp.h"
extern "C"{
#include "../inc/CanTp.h"
extern volatile uint8_t CANIF_FLAG;
extern volatile uint8_t Decoded_Flag;
extern volatile uint8_t infinity_loop_flag ;
extern volatile uint8_t error_flag ;
extern volatile uint32_t GlobalRxPduId;
extern PduInfoTRx *GlobalRxPduInfoPtr;
extern PduInfoType CompletePduInfo;
extern volatile uint32_t numberOfRemainingBytesToReceive;
extern volatile uint32_t numberOfRemainingBytesToSend;
extern volatile uint32_t numberOfConsecutiveFramesToReceive ;
extern Frame_States expectedFrameState;
extern PduInfoTRx EncodedPduInfo;
extern volatile int8_t CanTp_Rx;
extern volatile uint32_t numberOfConsecutiveFramesToSend ;
}

//#include <stdio.h>
//#include <stddef.h>

// extern void CanTp_MainFunction();
// extern Std_ReturnType CanTp_Transmit(uint32_t TxPduId, PduInfoType *PduInfoPtr);

TEST(CanTpTest, RxIndicationTest1)
{
    uint32_t testRxPduId = 0;
    PduInfoTRx testPduInfo;
    testPduInfo.Length = 4;
    testPduInfo.Data[0] = 0x12;
    testPduInfo.Data[1] = 0x34;
    testPduInfo.Data[2] = 0x56;
    testPduInfo.Data[3] = 0x78;

    // Call the function
    Std_ReturnType result = CanTp_RxIndication(testRxPduId, &testPduInfo);

    // Check the return value
    EXPECT_EQ(result, E_OK);
}
// Test case: Invalid RxPduId Test
TEST(CanTpTest, RxIndicationTest2)
{
    uint32_t testRxPduId = 1; // the ID must be 0 so the test will failed
    PduInfoTRx testPduInfo;
    testPduInfo.Length = 4;
    testPduInfo.Data[0] = 0x12;
    testPduInfo.Data[1] = 0x34;
    testPduInfo.Data[2] = 0x56;
    testPduInfo.Data[3] = 0x78;

    // Call the function
    Std_ReturnType result = CanTp_RxIndication(testRxPduId, &testPduInfo);

    // Check the return value
    EXPECT_EQ(result, E_NOT_OK);
}
// Test case: test global variables
TEST(CanTpTest, RxIndicationTest3)
{
    uint32_t testRxPduId = 0;
    PduInfoTRx testPduInfo;
    testPduInfo.Length = 4;
    testPduInfo.Data[0] = 0x12;
    testPduInfo.Data[1] = 0x34;
    testPduInfo.Data[2] = 0x56;
    testPduInfo.Data[3] = 0x78;

    // Call the function
    Std_ReturnType result = CanTp_RxIndication(testRxPduId, &testPduInfo);

    // Check the return value
    // Check the global variables
    EXPECT_EQ(GlobalRxPduId, testRxPduId);
    EXPECT_EQ(CanTp_Rx, 1);
    EXPECT_EQ(GlobalRxPduInfoPtr, &testPduInfo);
}
// Test case: Null Pointer for PduInfoPtr
TEST(CanTpTest, RxIndicationTest4)
{
    uint32_t testRxPduId = 0;
    PduInfoTRx testPduInfo;
    testPduInfo.Length = 4;
    testPduInfo.Data[0] = (uint8_t)NULL;
    testPduInfo.Data[1] = (uint8_t)NULL;
    testPduInfo.Data[2] = (uint8_t)NULL;
    testPduInfo.Data[3] = (uint8_t)NULL;

    // Call the function
    Std_ReturnType result = CanTp_RxIndication(testRxPduId, &testPduInfo);

    // Check the return value
    EXPECT_EQ(result, E_NOT_OK);
}
// Test case: Null Pointer for PduInfoPtr
TEST(CanTpTest, RxIndicationTest5)
{
    uint32_t testRxPduId = 0;
    PduInfoTRx testPduInfo;
    testPduInfo.Length = 0;
    // testPduInfo.Data[0] = NULL;
    // testPduInfo.Data[1] = NULL;
    // testPduInfo.Data[2] = NULL;
    // testPduInfo.Data[3] = NULL;

    // Call the function
    Std_ReturnType result = CanTp_RxIndication(testRxPduId, &testPduInfo);

    // Check the return value
    EXPECT_EQ(result, E_OK);
}
// Test case: Maximum PDU Data Size
TEST(CanTpTest, RxIndicationTest6)
{
    uint32_t testRxPduId = 0;
    PduInfoTRx testPduInfo;
    testPduInfo.Length = 8;
    for (int x = 0; x < 8; x++)
    {
        testPduInfo.Data[x] = x;
    }

    // Call the function
    Std_ReturnType result = CanTp_RxIndication(testRxPduId, &testPduInfo);

    // Check the return value
    EXPECT_EQ(result, E_OK);
}
// Test case: Basic Reception Test
TEST(CANTP_UnitTest, ReceptionOfSingleFrame)
{
    // Arrange
    uint8_t ID = 0;
    PduInfoTRx pdu;
    pdu.Data[0] = 0x03; // Single Frame with length 3
    pdu.Data[1] = 0x11;
    pdu.Data[2] = 0x22;
    pdu.Data[3] = 0x33;
    pdu.Length = 4;

    // Act
    CanTp_RxIndication(ID, &pdu);
    CanTp_MainFunction();

    // Assert
    EXPECT_EQ(Decoded_Flag, 1);
    EXPECT_EQ(CompletePduInfo.Length, 3);
    EXPECT_EQ(CompletePduInfo.Data[0], 0x11);
    EXPECT_EQ(CompletePduInfo.Data[1], 0x22);
    EXPECT_EQ(CompletePduInfo.Data[2], 0x33);
}

TEST(CANTP_UnitTest, ReceptionOfFirstFrame)
{
    // Arrange
    uint8_t ID = 0;
    PduInfoTRx pdu;
    pdu.Data[0] = 0x10; // First Frame with length 12 (0x0C)
    pdu.Data[1] = 0x0C;
    pdu.Data[2] = 0x11;
    pdu.Data[3] = 0x22;
    pdu.Data[4] = 0x33;
    pdu.Data[5] = 0x44;
    pdu.Data[6] = 0x55;
    pdu.Data[7] = 0x66;
    pdu.Length = 8;

    // Act
    CanTp_RxIndication(ID, &pdu);
    CanTp_MainFunction();

    // Assert
    EXPECT_EQ(numberOfRemainingBytesToReceive, 6);
    EXPECT_EQ(CompletePduInfo.Length, 12);
    EXPECT_EQ(expectedFrameState, FlowControl_Frame_State);
    EXPECT_EQ(Decoded_Flag, 1);
}



TEST(CANTP_UnitTest, ReceptionOfConsecutiveFrame)
{
    // Arrange
    uint8_t ID = 0;
    PduInfoTRx pdu;
    pdu.Data[0] = 0x21; // Consecutive Frame with sequence number 1
    pdu.Data[1] = 0x77;
    pdu.Data[2] = 0x88;
    pdu.Data[3] = 0x99;
    pdu.Data[4] = 0x69;
    pdu.Data[5] = 0x69;
    pdu.Data[6] = 0x69;
    pdu.Data[7] = 0x69;
    pdu.Data[8] = 0x69;
    pdu.Data[9] = 0x55;
    pdu.Length = 10;

    numberOfRemainingBytesToReceive = 6; // Simulate that there are remaining bytes to receive
    numberOfConsecutiveFramesToReceive =0;
    numberOfRemainingBytesToReceive = 2;
    // Act
    CanTp_RxIndication(ID, &pdu);
     
    CanTp_MainFunction();

    // Assert
    EXPECT_EQ(Decoded_Flag, 1);
    //EXPECT_EQ(CompletePduInfo.Data[2], 0x77);
    // EXPECT_EQ(CompletePduInfo.Data[7], 0x88);
    // EXPECT_EQ(CompletePduInfo.Data[8], 0x99);
    // EXPECT_EQ(numberOfRemainingBytesToReceive, 6);
}

TEST(CANTP_UnitTest, TransmissionOfSingleFrame)
{
    // Arrange
    uint8_t ID = 0;
    PduInfoType pdu;
    pdu.Data[0] = 0x11;
    pdu.Data[1] = 0x22;
    pdu.Data[2] = 0x33;
    pdu.Length = 3;

    // Act
    numberOfRemainingBytesToSend =0;
    expectedFrameState = Any_State;
    CanTp_Transmit(ID, &pdu);
    
    CanTp_MainFunction();

    // Assert
    EXPECT_EQ(CANIF_FLAG, 1);
    EXPECT_EQ(EncodedPduInfo.Data[0], 0x03); // Single Frame with length 3
    // EXPECT_EQ(EncodedPduInfo.Data[1], 0x22);
    // EXPECT_EQ(EncodedPduInfo.Data[2], 0x33);
    // EXPECT_EQ(EncodedPduInfo.Data[3], );
}

TEST(CANTP_UnitTest, TransmissionOfFirstFrame)
{
    // Arrange
    uint8_t ID = 0;
    PduInfoType pdu;
    pdu.Data[0] = 0x11;
    pdu.Data[1] = 0x22;
    pdu.Data[2] = 0x33;
    pdu.Data[3] = 0x44;
    pdu.Data[4] = 0x55;
    pdu.Data[5] = 0x66;
    pdu.Data[6] = 0x77;
    pdu.Data[7] = 0x88;
    pdu.Length = 8;

    // Act
    CanTp_Transmit(ID, &pdu);
    CanTp_MainFunction();

    // Assert
    EXPECT_EQ(CANIF_FLAG, 1);
    // EXPECT_EQ(EncodedPduInfo.Data[0], 0x11); // First Frame indicator
    // EXPECT_EQ(EncodedPduInfo.Data[1], 0x22); // Length 8
    // EXPECT_EQ(EncodedPduInfo.Data[2], 0x33);
    // EXPECT_EQ(EncodedPduInfo.Data[3], 0x44);
    // EXPECT_EQ(EncodedPduInfo.Data[4], 0x55);
    // EXPECT_EQ(EncodedPduInfo.Data[5], 0x66);
    // EXPECT_EQ(EncodedPduInfo.Data[6], 0x77);
    // EXPECT_EQ(EncodedPduInfo.Data[7], 0x88);
}

TEST(CANTP_UnitTest, TransmissionOfConsecutiveFrame)
{
    // Arrange
    uint8_t ID = 0;
    PduInfoType pdu;
    pdu.Data[0] = 0x11;
    pdu.Data[1] = 0x22;
    pdu.Data[2] = 0x33;
    pdu.Data[3] = 0x44;
    pdu.Data[4] = 0x55;
    pdu.Data[5] = 0x66;
    pdu.Data[6] = 0x77;
    pdu.Data[7] = 0x88;
    pdu.Data[8] = 0x99;
    pdu.Length = 9;

    expectedFrameState = Consecutive_Frame_State;
    numberOfRemainingBytesToSend = pdu.Length;
    numberOfConsecutiveFramesToSend = 2;
    // Act
    CanTp_Transmit(ID, &pdu);
    CanTp_MainFunction();

    // Assert
    EXPECT_EQ(CANIF_FLAG, 1);
    // EXPECT_EQ(EncodedPduInfo.Data[0], 0x21); // Consecutive Frame with SN=1
    // EXPECT_EQ(EncodedPduInfo.Data[1], 0x77);
    // EXPECT_EQ(EncodedPduInfo.Data[2], 0x88);
    // EXPECT_EQ(EncodedPduInfo.Data[3], 0x99);
}

TEST(CANTP_UnitTest, TransmissionOfFlowControlFrame)
{
    // Arrange
    uint8_t ID = 0;
    PduInfoType pdu;

    pdu.Data[0] = 0x18;
    pdu.Data[1] = 0x19;

    pdu.Length = 9;

    // ACT
    CanTp_Transmit(ID, &pdu);
    numberOfRemainingBytesToSend = 3;
    expectedFrameState = FlowControl_Frame_State;

    CanTp_MainFunction();

    // Assert
    EXPECT_EQ(CANIF_FLAG, 1);
}

TEST(CANTP_UnitTest, ReceptionIDCheck)
{
    // Arrange
    uint8_t ID = 3;
    PduInfoTRx pdu;
    pdu.Data[0] = 0x03; // Single Frame with length 3
    pdu.Data[1] = 0x11;
    pdu.Data[2] = 0x22;
    pdu.Data[3] = 0x33;
    pdu.Length = 4;

    // Act
    CanTp_RxIndication(ID, &pdu);
    CanTp_MainFunction();

    // Assert
    EXPECT_EQ(infinity_loop_flag, 1);
    
}


TEST(CANTP_UnitTest, TransmissionIDCheck)
{
    // Arrange
    uint8_t ID = 4;
    PduInfoType pdu;

    pdu.Data[0] = 0x18;
    pdu.Data[1] = 0x19;

    pdu.Length = 9;

    // ACT
    CanTp_Transmit(ID, &pdu);
    

    CanTp_MainFunction();

    // Assert
    EXPECT_EQ(infinity_loop_flag, 1);
}


TEST(CANTP_UnitTest, NoFrameDedicateReceip)
{
    // Arrange
    uint8_t ID = 0;
    PduInfoTRx pdu;
    pdu.Data[0] = 0x55; // Single Frame with length 3
    pdu.Data[1] = 0x11;
    pdu.Data[2] = 0x22;
    pdu.Data[3] = 0x33;
    pdu.Length = 4;

    // Act
    CanTp_RxIndication(ID, &pdu);
    CanTp_MainFunction();

    // Assert
    EXPECT_EQ(error_flag, 1);
    
}


TEST(CANTP_UnitTest, NoFrameDedicateTrans)
{
    // Arrange
    uint8_t ID = 0;
    PduInfoType pdu;
    pdu.Data[0] = 0x11;
    pdu.Data[1] = 0x22;
    pdu.Data[2] = 0x33;
    pdu.Data[3] = 0x44;
    pdu.Data[4] = 0x55;
    pdu.Data[5] = 0x66;
    pdu.Data[6] = 0x77;
    pdu.Data[7] = 0x88;
    pdu.Length = 8;

    expectedFrameState = NoState;
    // Act
    CanTp_Transmit(ID, &pdu);
    CanTp_MainFunction();

    // Assert
    EXPECT_EQ(error_flag, 1);
    // EXPECT_EQ(EncodedPduInfo.Data[0], 0x11); // First Frame indicator
    // EXPECT_EQ(EncodedPduInfo.Data[1], 0x22); // Length 8
    // EXPECT_EQ(EncodedPduInfo.Data[2], 0x33);
    // EXPECT_EQ(EncodedPduInfo.Data[3], 0x44);
    // EXPECT_EQ(EncodedPduInfo.Data[4], 0x55);
    // EXPECT_EQ(EncodedPduInfo.Data[5], 0x66);
    // EXPECT_EQ(EncodedPduInfo.Data[6], 0x77);
    // EXPECT_EQ(EncodedPduInfo.Data[7], 0x88);
}

TEST(CANTP_UnitTest, ReceptionOfFlowControl_1)
{
    // Arrange
    uint8_t ID = 0;
    PduInfoTRx pdu;
    pdu.Data[0] = 0x30; // First Frame with length 12 (0x0C)
    pdu.Data[1] = 0x0C;
    pdu.Data[2] = 0x11;
    pdu.Data[3] = 0x22;
    pdu.Data[4] = 0x33;
    pdu.Data[5] = 0x44;
    pdu.Data[6] = 0x55;
    pdu.Data[7] = 0x66;
    pdu.Length = 8;

    // Act
    CanTp_RxIndication(ID, &pdu);
    CanTp_MainFunction();

    // Assert
  
    EXPECT_EQ(Decoded_Flag, 1);
}
TEST(CANTP_UnitTest, ReceptionOfFlowControl_2)
{
    // Arrange
    uint8_t ID = 0;
    PduInfoTRx pdu;
    pdu.Data[0] = 0x31; // First Frame with length 12 (0x0C)
    pdu.Data[1] = 0x0C;
    pdu.Data[2] = 0x11;
    pdu.Data[3] = 0x22;
    pdu.Data[4] = 0x33;
    pdu.Data[5] = 0x44;
    pdu.Data[6] = 0x55;
    pdu.Data[7] = 0x66;
    pdu.Length = 8;

    // Act
    CanTp_RxIndication(ID, &pdu);
    CanTp_MainFunction();

    // Assert
  
    EXPECT_EQ(Decoded_Flag, 1);
}
TEST(CANTP_UnitTest, ReceptionOfFlowControl_3)
{
    // Arrange
    uint8_t ID = 0;
    PduInfoTRx pdu;
    pdu.Data[0] = 0x32; // First Frame with length 12 (0x0C)
    pdu.Data[1] = 0x0C;
    pdu.Data[2] = 0x11;
    pdu.Data[3] = 0x22;
    pdu.Data[4] = 0x33;
    pdu.Data[5] = 0x44;
    pdu.Data[6] = 0x55;
    pdu.Data[7] = 0x66;
    pdu.Length = 8;

    // Act
    CanTp_RxIndication(ID, &pdu);
    CanTp_MainFunction();

    // Assert
  
    EXPECT_EQ(Decoded_Flag, 1);
}

TEST(CANTP_UnitTest, ReceptionOfFlowControl_4)
{
    // Arrange
    uint8_t ID = 0;
    PduInfoTRx pdu;
    pdu.Data[0] = 0x33; // First Frame with length 12 (0x0C)
    pdu.Data[1] = 0x0C;
    pdu.Data[2] = 0x11;
    pdu.Data[3] = 0x22;
    pdu.Data[4] = 0x33;
    pdu.Data[5] = 0x44;
    pdu.Data[6] = 0x55;
    pdu.Data[7] = 0x66;
    pdu.Length = 8;

    // Act
    CanTp_RxIndication(ID, &pdu);
    CanTp_MainFunction();

    // Assert
  
    EXPECT_EQ(Decoded_Flag, 1);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// TEST(SumFunction, Test_1)
// {
//     // arrange
//     // call
//     // action
//     EXPECT_EQ(sub(2, 3), -1);
// }
// TEST(SumFunction, Test_2)
// {
//     // arrange
//     // call
//     // action
//     EXPECT_NE(sum(8, 3), 9);
// }
// TEST(SubFunction, Test_1)
// {
//     // arrange
//     // call
//     // action
//     EXPECT_EQ(sum(2, 3), 5);
// }
// TEST(SubFunction, Test_2)
// {
//     // arrange
//     // call
//     // action
//     EXPECT_NE(sum(9, 3), 5);
// }
// int main()
// {

//     ::testing::InitGoogleTest();
//     return RUN_ALL_TESTS();
// }
