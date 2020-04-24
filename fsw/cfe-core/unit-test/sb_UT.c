/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/*
** File:
**    sb_UT.c
**
** Purpose:
**    Software Bus Services unit test
**
** References:
**    1. cFE Application Developers Guide
**    2. unit test standard 092503
**    3. C Coding Standard 102904
**
** Notes:
**    1. This is unit test code only, not for use in flight
**
*/

/*
** Includes
*/
#include "sb_UT.h"

static char    cMsg[UT_MAX_MESSAGE_LENGTH];

/*
** Functions
*/
void UtTest_Setup(void)
{
    UT_Init("sb");
    UT_Text("cFE SB Unit Test Output File\n\n");

    UT_ADD_TEST(Test_SB_AppInit);
    UT_ADD_TEST(Test_SB_MainRoutine);
    UT_ADD_TEST(Test_SB_Cmds);
    UT_ADD_TEST(Test_SB_EarlyInit);
    UT_ADD_TEST(Test_CreatePipe_API);
    UT_ADD_TEST(Test_DeletePipe_API);
    UT_ADD_TEST(Test_PipeOpts_API);
    UT_ADD_TEST(Test_GetPipeName_API);
    UT_ADD_TEST(Test_GetPipeIdByName_API);
    UT_ADD_TEST(Test_Subscribe_API);
    UT_ADD_TEST(Test_Unsubscribe_API);
    UT_ADD_TEST(Test_SendMsg_API);
    UtTest_Add(Test_RcvMsg_API, NULL, Test_CleanupApp_API, "Test_RcvMsg_API");
    UT_ADD_TEST(Test_SB_Utils);
    UtTest_Add(Test_SB_SpecialCases, NULL, UT_CheckForOpenSockets,
    		   "Test_SB_SpecialCases");

    UT_ADD_TEST(Test_SB_Macros);

} /* end main */

/*
** Function for calling SB and CCSDS Macros
** test functions
*/
void Test_SB_Macros(void)
{
    STARTBLOCK();

    Test_SB_CCSDSPriHdr_Macros();
    Test_SB_CCSDSSecHdr_Macros();

    ENDBLOCK();
} /* end Test_SB_Macros */

/*
**Test_SB_CCSDSSecHdr_Macros
*/
void Test_SB_CCSDSSecHdr_Macros(void)
{
    CFE_SB_CmdHdr_t NoParamPkt;

    START();

    SB_ResetUnitTest();

    CCSDS_CLR_CMDSEC_HDR(NoParamPkt.Sec);
    CCSDS_WR_FC(NoParamPkt.Sec, 1);

    ASSERT_TRUE(CCSDS_RD_FC(NoParamPkt.Sec) == 0x01);

    REPORT();

    START();

    SB_ResetUnitTest();

    CCSDS_CLR_CMDSEC_HDR(NoParamPkt.Sec);
    CCSDS_WR_CHECKSUM(NoParamPkt.Sec, 0xFF);

    ASSERT_TRUE(CCSDS_RD_CHECKSUM(NoParamPkt.Sec) == 0xFF);

    REPORT();

#ifdef MESSAGE_FORMAT_IS_CCSDS_VER_2

    START();

    SB_ResetUnitTest();

    CCSDS_CLR_SEC_APIDQ(NoParamPkt.SpacePacket.ApidQ);
    CCSDS_WR_EDS_VER(NoParamPkt.SpacePacket.ApidQ, 0x01);

    ASSERT_TRUE(CCSDS_RD_EDS_VER(NoParamPkt.SpacePacket.ApidQ) == 0x01);

    REPORT();

    START();

    SB_ResetUnitTest();

    CCSDS_CLR_SEC_APIDQ(NoParamPkt.SpacePacket.ApidQ);
    CCSDS_WR_ENDIAN(NoParamPkt.SpacePacket.ApidQ, 0x01);

    ASSERT_TRUE(CCSDS_RD_ENDIAN(NoParamPkt.SpacePacket.ApidQ) == 0x01);

    REPORT();

    START();

    SB_ResetUnitTest();

    CCSDS_CLR_SEC_APIDQ(NoParamPkt.SpacePacket.ApidQ);
    CCSDS_WR_PLAYBACK(NoParamPkt.SpacePacket.ApidQ, 0x01);

    ASSERT_TRUE(CCSDS_RD_PLAYBACK(NoParamPkt.SpacePacket.ApidQ) == 0x01);

    REPORT();

    START();

    SB_ResetUnitTest();

    CCSDS_CLR_SEC_APIDQ(NoParamPkt.SpacePacket.ApidQ);
    CCSDS_WR_SUBSYSTEM_ID(NoParamPkt.SpacePacket.ApidQ, 0xFF);

    ASSERT_TRUE(CCSDS_RD_SUBSYSTEM_ID(NoParamPkt.SpacePacket.ApidQ) == 0xFF);

    REPORT();

    START();

    SB_ResetUnitTest();

    CCSDS_CLR_SEC_APIDQ(NoParamPkt.SpacePacket.ApidQ);
    CCSDS_WR_SUBSYSTEM_ID(NoParamPkt.SpacePacket.ApidQ, 0xFF);

    ASSERT_TRUE(CCSDS_RD_SUBSYSTEM_ID(NoParamPkt.SpacePacket.ApidQ) == 0xFF);

    SB_ResetUnitTest();

    CCSDS_CLR_SEC_APIDQ(NoParamPkt.SpacePacket.ApidQ);
    CCSDS_WR_SYSTEM_ID(NoParamPkt.SpacePacket.ApidQ, 0xFFFF);

    ASSERT_TRUE(CCSDS_RD_SYSTEM_ID(NoParamPkt.SpacePacket.ApidQ) == 0xFFFF);

    REPORT();
#endif
} /* end Test_SB_CCSDSSecHdr_Macros */

/*
**Test_SB_CCSDSPriHdr_Macros
*/
void Test_SB_CCSDSPriHdr_Macros(void)
{
    CFE_SB_Msg_t Msg;

    START();

    SB_ResetUnitTest();

    CCSDS_CLR_PRI_HDR(Msg.Hdr);
    CCSDS_WR_SID(Msg.Hdr, 0x1899);

    ASSERT_TRUE(CCSDS_RD_SID(Msg.Hdr) == 0x1899);

    REPORT();
    
    START();

    SB_ResetUnitTest();

    CCSDS_CLR_PRI_HDR(Msg.Hdr);
    CCSDS_WR_APID(Msg.Hdr, 0x07FF);

    ASSERT_TRUE(CCSDS_RD_APID(Msg.Hdr) == 0x07FF);

    REPORT();
    
    START();

    SB_ResetUnitTest();

    CCSDS_CLR_PRI_HDR(Msg.Hdr);
    CCSDS_WR_SHDR(Msg.Hdr, 1);

    ASSERT_TRUE(CCSDS_RD_SHDR(Msg.Hdr) == 1);

    REPORT();
    
    START();

    SB_ResetUnitTest();

    CCSDS_CLR_PRI_HDR(Msg.Hdr);
    CCSDS_WR_TYPE(Msg.Hdr, 1);

    ASSERT_TRUE(CCSDS_RD_TYPE(Msg.Hdr) == 1);

    REPORT();
    
    START();

    SB_ResetUnitTest();

    CCSDS_CLR_PRI_HDR(Msg.Hdr);
    CCSDS_WR_VERS(Msg.Hdr, 1);

    ASSERT_TRUE(CCSDS_RD_VERS(Msg.Hdr) == 1);

    REPORT();
    
    START();

    SB_ResetUnitTest();

    CCSDS_CLR_PRI_HDR(Msg.Hdr);
    CCSDS_WR_SEQ(Msg.Hdr, 0x3FFF);

    ASSERT_TRUE(CCSDS_RD_SEQ(Msg.Hdr) == 0x3FFF);

    REPORT();
    
    START();

    SB_ResetUnitTest();

    CCSDS_CLR_PRI_HDR(Msg.Hdr);
    CCSDS_WR_SEQFLG(Msg.Hdr, 0x03);

    ASSERT_TRUE(CCSDS_RD_SEQFLG(Msg.Hdr) == 0x03);

    REPORT();
    
    START();

    SB_ResetUnitTest();

    CCSDS_CLR_PRI_HDR(Msg.Hdr);
    CCSDS_WR_LEN(Msg.Hdr, 0xFFFF);

    ASSERT_TRUE(CCSDS_RD_LEN(Msg.Hdr) == 0xFFFF);

    REPORT();
} /* end Test_SB_CCSDSPriHdr_Macros */

/*
** Reset variable values and sockets prior to a test
*/
void SB_ResetUnitTest(void)
{
    /* If any sockets were left open then report and close them */
    UT_CheckForOpenSockets();
    UT_InitData();
    CFE_SB_EarlyInit();
} /* end SB_ResetUnitTest */

/*
** Function for calling SB application initialization
** test functions
*/
void Test_SB_AppInit(void)
{
    STARTBLOCK();

    Test_SB_AppInit_ESRegFail();
    Test_SB_AppInit_EVSRegFail();
    Test_SB_AppInit_EVSSendEvtFail();
    Test_SB_AppInit_CrPipeFail();
    Test_SB_AppInit_Sub1Fail();
    Test_SB_AppInit_Sub2Fail();
    Test_SB_AppInit_GetPoolFail();
    Test_SB_AppInit_PutPoolFail();

    ENDBLOCK();
} /* end Test_SB_AppInit */

/*
** Test task init with ES_RegisterApp returning error
*/
void Test_SB_AppInit_ESRegFail(void)
{
    int32 ForcedRtnVal = -1;

    START();

    SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RegisterApp), 1, ForcedRtnVal);
	ASSERT_EQ(CFE_SB_AppInit(), ForcedRtnVal);

    EVTCNT(0);

    REPORT();
} /* end Test_SB_AppInit_ESRegFail */

/*
** Test task init with EVS_Register returning error
*/
void Test_SB_AppInit_EVSRegFail(void)
{
    int32 ForcedRtnVal = -1;

    START();

    SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(CFE_EVS_Register), 1, ForcedRtnVal);
    ASSERT_EQ(CFE_SB_AppInit(), ForcedRtnVal);

    EVTCNT(0);

    REPORT();
} /* end Test_SB_AppInit_EVSRegFail */

/*
** Test task init with EVS_SendEvent returning error on task init event
*/
void Test_SB_AppInit_EVSSendEvtFail(void)
{
    int32 ForcedRtnVal = -1;
    CFE_ES_TaskInfo_t TestTaskInfo;

    START();

    SB_ResetUnitTest();

    /* To get coverage on CFE_SB_GetAppTskName(), this ensures that the
     * path with different app/task names is followed on at least one event.
     */
    memset(&TestTaskInfo, 0, sizeof(TestTaskInfo));
    strncpy((char*)TestTaskInfo.TaskName, "test", sizeof(TestTaskInfo.TaskName)-1);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskInfo), &TestTaskInfo, sizeof(TestTaskInfo), false);

    /* There are three events prior to init, pipe created (1) and subscription
     * rcvd (2). Fourth is SB initialized, but it is the first to use SendEvent.
     * (The others use SendEventWithAppID which is a different counter).
     */
    UT_SetDeferredRetcode(UT_KEY(CFE_EVS_SendEvent), 1, ForcedRtnVal);
    ASSERT_EQ(CFE_SB_AppInit(), ForcedRtnVal);

    EVTCNT(5);

    TEARDOWN(CFE_SB_DeletePipe(CFE_SB.CmdPipe));

    REPORT();
} /* end Test_SB_AppInit_EVSSendEvtFail */

/*
** Test task init with pipe create failure
*/
void Test_SB_AppInit_CrPipeFail(void)
{
    START();

    SB_ResetUnitTest();

    /* To fail the pipe create, force the OS_QueueCreate call to return some
     * type of error code.
     */
    UT_SetDeferredRetcode(UT_KEY(OS_QueueCreate), 1, OS_ERROR);
    ASSERT_EQ(CFE_SB_AppInit(), CFE_SB_PIPE_CR_ERR);

    EVTCNT(1);

    EVTSENT(CFE_SB_CR_PIPE_ERR_EID);

    REPORT();
} /* end Test_SB_AppInit_CrPipeFail */

/*
** Test task init with a failure on first subscription request
*/
void Test_SB_AppInit_Sub1Fail(void)
{
    CFE_SB_PipeId_t PipeId = 0;

    START();

    SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetPoolBuf), 1, -1);
    ASSERT_EQ(CFE_SB_AppInit(), CFE_SB_BUF_ALOC_ERR);

    EVTCNT(3);

    EVTSENT(CFE_SB_DEST_BLK_ERR_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SB_AppInit_Sub1Fail */

/*
** Test task init with a failure on second subscription request
*/
void Test_SB_AppInit_Sub2Fail(void)
{
    CFE_SB_PipeId_t PipeId = 0;

    START();

    SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetPoolBuf), 2, -1);
    ASSERT_EQ(CFE_SB_AppInit(), CFE_SB_BUF_ALOC_ERR);

    EVTCNT(5);

    EVTSENT(CFE_SB_DEST_BLK_ERR_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SB_AppInit_Sub2Fail */

/*
** Test task init with a GetPool failure
*/
void Test_SB_AppInit_GetPoolFail(void)
{
    CFE_SB_PipeId_t PipeId = 0;
    int32           ForcedRtnVal = -1;

    START();

	SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetPoolBuf), 3, ForcedRtnVal);

    ASSERT_EQ(CFE_SB_AppInit(), ForcedRtnVal);

    EVTCNT(5);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SB_AppInit_GetPoolFail */

/*
** Test task init with a PutPool failure
*/
void Test_SB_AppInit_PutPoolFail(void)
{
    CFE_SB_PipeId_t PipeId = 0;
    int32           ForcedRtnVal = -1;

    START();

    SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_PutPoolBuf), 1, ForcedRtnVal);

    ASSERT_EQ(CFE_SB_AppInit(), ForcedRtnVal);

    EVTCNT(5);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SB_AppInit_PutPoolFail */

/*
** Function for calling SB main task test functions
*/
void Test_SB_MainRoutine(void)
{
    STARTBLOCK();

    Test_SB_Main_RcvErr();
    Test_SB_Main_InitErr();

    ENDBLOCK();
} /* end Test_SB_MainRoutine */

/*
** Test main task with a packet receive error
*/
void Test_SB_Main_RcvErr(void)
{
    CFE_SB_PipeId_t PipeId = 0;

    START();

    SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(OS_QueueGet), 1, -1);
    CFE_SB_TaskMain();

    EVTCNT(8);

    EVTSENT(CFE_SB_INIT_EID);

    EVTSENT(CFE_SB_Q_RD_ERR_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SB_Main_RcvErr */

/*
** Test main task with an application initialization error
*/
void Test_SB_Main_InitErr(void)
{
    CFE_SB_PipeId_t PipeId = 0;

    START();

    SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_PutPoolBuf), 1, -1);
    CFE_SB_TaskMain();

    EVTCNT(5);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));
    REPORT();
} /* end Test_SB_Main_InitErr */

/*
** Function for calling SB command test functions
*/
void Test_SB_Cmds(void)
{
    STARTBLOCK();

    Test_SB_Cmds_Noop();
    Test_SB_Cmds_RstCtrs();
    Test_SB_Cmds_Stats();
    Test_SB_Cmds_RoutingInfoDef();
    Test_SB_Cmds_RoutingInfoSpec();
    Test_SB_Cmds_RoutingInfoCreateFail();
    Test_SB_Cmds_RoutingInfoHdrFail();
    Test_SB_Cmds_RoutingInfoWriteFail();
    Test_SB_Cmds_PipeInfoDef();
    Test_SB_Cmds_PipeInfoSpec();
    Test_SB_Cmds_PipeInfoCreateFail();
    Test_SB_Cmds_PipeInfoHdrFail();
    Test_SB_Cmds_PipeInfoWriteFail();
    Test_SB_Cmds_MapInfoDef();
    Test_SB_Cmds_MapInfoSpec();
    Test_SB_Cmds_MapInfoCreateFail();
    Test_SB_Cmds_MapInfoHdrFail();
    Test_SB_Cmds_MapInfoWriteFail();
    Test_SB_Cmds_EnRouteValParam();
    Test_SB_Cmds_EnRouteNonExist();
    Test_SB_Cmds_EnRouteInvParam();
    Test_SB_Cmds_EnRouteInvParam2();
    Test_SB_Cmds_EnRouteInvParam3();
    Test_SB_Cmds_DisRouteValParam();
    Test_SB_Cmds_DisRouteNonExist();
    Test_SB_Cmds_DisRouteInvParam();
    Test_SB_Cmds_DisRouteInvParam2();
    Test_SB_Cmds_DisRouteInvParam3();
    Test_SB_Cmds_SendHK();
    Test_SB_Cmds_SendPrevSubs();
    Test_SB_Cmds_SubRptOn();
    Test_SB_Cmds_SubRptOff();
    Test_SB_Cmds_UnexpCmdCode();
    Test_SB_Cmds_BadCmdLength();
    Test_SB_Cmds_UnexpMsgId();

    ENDBLOCK();
} /* end Test_SB_Cmds */

/*
** Test no-op command
*/
void Test_SB_Cmds_Noop(void)
{
    CFE_SB_CmdHdr_t NoParamCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&NoParamCmd, CFE_SB_CMD_MID, sizeof(NoParamCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &NoParamCmd, CFE_SB_NOOP_CC);
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &NoParamCmd;
    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_CMD0_RCVD_EID);

    REPORT();
} /* end Test_SB_Cmds_Noop */

/*
** Test reset counters command
*/
void Test_SB_Cmds_RstCtrs(void)
{
    CFE_SB_CmdHdr_t NoParamCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&NoParamCmd, CFE_SB_CMD_MID, sizeof(NoParamCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &NoParamCmd, CFE_SB_RESET_COUNTERS_CC);
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &NoParamCmd;
    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_CMD1_RCVD_EID);

    REPORT();
} /* Test_SB_Cmds_RstCtrs */

/*
** Test send SB stats command
*/
void Test_SB_Cmds_Stats(void)
{
    CFE_SB_CmdHdr_t NoParamCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&NoParamCmd, CFE_SB_CMD_MID, sizeof(NoParamCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &NoParamCmd, CFE_SB_SEND_SB_STATS_CC);
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &NoParamCmd;
    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(2);

    EVTSENT(CFE_SB_SND_STATS_EID);

    EVTSENT(CFE_SB_SND_STATS_EID);

    REPORT();
} /* end Test_SB_Cmds_Stats */

/*
** Test send routing information command using the default file name
*/
void Test_SB_Cmds_RoutingInfoDef(void)
{
    CFE_SB_PipeId_t           PipeId = 0;
    CFE_SB_WriteFileInfoCmd_t WriteFileCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&WriteFileCmd, CFE_SB_CMD_MID,
                   sizeof(WriteFileCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &WriteFileCmd,
                      CFE_SB_SEND_ROUTING_INFO_CC);
    strncpy((char *)WriteFileCmd.Payload.Filename, "", sizeof(WriteFileCmd.Payload.Filename));

    /* Make some routing info by calling CFE_SB_AppInit */
    SETUP(CFE_SB_AppInit());

    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &WriteFileCmd;

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(9);

    EVTSENT(CFE_SB_INIT_EID);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    EVTSENT(CFE_SB_INIT_EID);

    EVTSENT(CFE_SB_SND_RTG_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SB_Cmds_RoutingInfoDef */

/*
** Test send routing information command using a specified file name
*/
void Test_SB_Cmds_RoutingInfoSpec(void)
{
    CFE_SB_WriteFileInfoCmd_t WriteFileCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&WriteFileCmd, CFE_SB_CMD_MID,
                   sizeof(WriteFileCmd), true);
    SETUP(CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &WriteFileCmd,
                      CFE_SB_SEND_ROUTING_INFO_CC));
    strncpy((char *)WriteFileCmd.Payload.Filename, "RoutingTstFile", sizeof(WriteFileCmd.Payload.Filename));
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &WriteFileCmd;

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_SND_RTG_EID);

    REPORT();
} /* end Test_SB_Cmds_RoutingInfoSpec */

/*
**  Test send routing information command with a file creation failure
*/
void Test_SB_Cmds_RoutingInfoCreateFail(void)
{
    CFE_SB_WriteFileInfoCmd_t WriteFileCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&WriteFileCmd, CFE_SB_CMD_MID,
                   sizeof(WriteFileCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &WriteFileCmd,
                      CFE_SB_SEND_ROUTING_INFO_CC);
    strncpy((char *)WriteFileCmd.Payload.Filename, "RoutingTstFile", sizeof(WriteFileCmd.Payload.Filename));
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &WriteFileCmd;

    /* Make function CFE_SB_SendRtgInfo return CFE_SB_FILE_IO_ERR */
    UT_SetForceFail(UT_KEY(OS_creat), OS_ERROR);

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_SND_RTG_ERR1_EID);

    REPORT();
} /* end Test_SB_Cmds_RoutingInfoCreateFail */

/*
** Test send routing information command with a file header write failure
*/
void Test_SB_Cmds_RoutingInfoHdrFail(void)
{
    START();

    SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(CFE_FS_WriteHeader), 1, -1);

    ASSERT_EQ(CFE_SB_SendRtgInfo("RoutingTstFile"), CFE_SB_FILE_IO_ERR);

    EVTCNT(1);

    EVTSENT(CFE_SB_FILEWRITE_ERR_EID);

    REPORT();
} /* end Test_SB_Cmds_RoutingInfoHdrFail */

/*
** Test send routing information command with a file write failure on
** the second write
*/
void Test_SB_Cmds_RoutingInfoWriteFail(void)
{
    CFE_SB_PipeId_t PipeId = 0;

    START();

    SB_ResetUnitTest();

    /* Make some routing info by calling CFE_SB_AppInit */
    SETUP(CFE_SB_AppInit());

    UT_SetDeferredRetcode(UT_KEY(OS_write), 2, -1);

    ASSERT_EQ(CFE_SB_SendRtgInfo("RoutingTstFile"), CFE_SB_FILE_IO_ERR);

    EVTCNT(9);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    EVTSENT(CFE_SB_INIT_EID);

    EVTSENT(CFE_SB_FILEWRITE_ERR_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SB_Cmds_RoutingInfoWriteFail */

/*
** Test send pipe information command using the default file name
*/
void Test_SB_Cmds_PipeInfoDef(void)
{
    CFE_SB_WriteFileInfoCmd_t WriteFileCmd;
    CFE_SB_PipeId_t           PipeId1;
    CFE_SB_PipeId_t           PipeId2;
    CFE_SB_PipeId_t           PipeId3;
    uint16                    PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&WriteFileCmd, CFE_SB_CMD_MID,
                   sizeof(WriteFileCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &WriteFileCmd,
                      CFE_SB_SEND_PIPE_INFO_CC);
    strncpy((char *)WriteFileCmd.Payload.Filename, "", sizeof(WriteFileCmd.Payload.Filename));

    /* Create some pipe info */
    SETUP(CFE_SB_CreatePipe(&PipeId1, PipeDepth, "TestPipe1"));
    SETUP(CFE_SB_CreatePipe(&PipeId2, PipeDepth, "TestPipe2"));
    SETUP(CFE_SB_CreatePipe(&PipeId3, PipeDepth, "TestPipe3"));
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &WriteFileCmd;

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(4);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);
    EVTSENT(CFE_SB_SND_RTG_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId1));
    TEARDOWN(CFE_SB_DeletePipe(PipeId2));
    TEARDOWN(CFE_SB_DeletePipe(PipeId3));

    REPORT();
} /* end Test_SB_Cmds_PipeInfoDef */

/*
** Test send pipe information command using a specified file name
*/
void Test_SB_Cmds_PipeInfoSpec(void)
{
    CFE_SB_WriteFileInfoCmd_t WriteFileCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&WriteFileCmd, CFE_SB_CMD_MID,
                   sizeof(WriteFileCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &WriteFileCmd,
                      CFE_SB_SEND_PIPE_INFO_CC);
    strncpy((char *)WriteFileCmd.Payload.Filename, "PipeTstFile", sizeof(WriteFileCmd.Payload.Filename));
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &WriteFileCmd;
    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_SND_RTG_EID);

    REPORT();
} /* end Test_SB_Cmds_PipeInfoSpec */

/*
** Test send pipe information command with a file creation failure
*/
void Test_SB_Cmds_PipeInfoCreateFail(void)
{
    START();

    SB_ResetUnitTest();
    UT_SetForceFail(UT_KEY(OS_creat), OS_ERROR);
    ASSERT_EQ(CFE_SB_SendPipeInfo("PipeTstFile"), CFE_SB_FILE_IO_ERR);

    EVTCNT(1);

    EVTSENT(CFE_SB_SND_RTG_ERR1_EID);

    REPORT();
} /* end Test_SB_Cmds_PipeInfoCreateFail */

/*
** Test send pipe information command with a file header write failure
*/
void Test_SB_Cmds_PipeInfoHdrFail(void)
{
    START();

    SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(CFE_FS_WriteHeader), 1, -1);
    ASSERT_EQ(CFE_SB_SendPipeInfo("PipeTstFile"), CFE_SB_FILE_IO_ERR);

    EVTCNT(1);

    EVTSENT(CFE_SB_FILEWRITE_ERR_EID);

    REPORT();
} /* end Test_SB_Cmds_PipeInfoHdrFail */

/*
** Test send pipe information command with a file write failure on
** the second write
*/
void Test_SB_Cmds_PipeInfoWriteFail(void)
{
    CFE_SB_PipeId_t PipeId1;
    CFE_SB_PipeId_t PipeId2;
    CFE_SB_PipeId_t PipeId3;
    uint16          PipeDepth = 10;

    START();

    SB_ResetUnitTest();

    SETUP(CFE_SB_CreatePipe(&PipeId1, PipeDepth, "TestPipe1"));
    SETUP(CFE_SB_CreatePipe(&PipeId2, PipeDepth, "TestPipe2"));
    SETUP(CFE_SB_CreatePipe(&PipeId3, PipeDepth, "TestPipe3"));
    UT_SetDeferredRetcode(UT_KEY(OS_write), 2, -1);

    ASSERT_EQ(CFE_SB_SendPipeInfo("PipeTstFile"), CFE_SB_FILE_IO_ERR);

    EVTCNT(4);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    EVTSENT(CFE_SB_FILEWRITE_ERR_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId1));
    TEARDOWN(CFE_SB_DeletePipe(PipeId2));
    TEARDOWN(CFE_SB_DeletePipe(PipeId3));

    REPORT();
} /* end Test_SB_Cmds_PipeInfoWriteFail */

/*
** Test send map information command using the default file name
*/
void Test_SB_Cmds_MapInfoDef(void)
{
    CFE_SB_WriteFileInfoCmd_t WriteFileCmd;
    CFE_SB_PipeId_t           PipeId1;
    CFE_SB_PipeId_t           PipeId2;
    CFE_SB_PipeId_t           PipeId3;
    CFE_SB_MsgId_t            MsgId0 = SB_UT_TLM_MID + 1;
    CFE_SB_MsgId_t            MsgId1 = SB_UT_TLM_MID + 2;
    CFE_SB_MsgId_t            MsgId2 = SB_UT_TLM_MID + 3;
    CFE_SB_MsgId_t            MsgId3 = SB_UT_TLM_MID + 4;
    CFE_SB_MsgId_t            MsgId4 = SB_UT_TLM_MID + 5;
    CFE_SB_MsgId_t            MsgId5 = SB_UT_TLM_MID + 6;
    uint16                    PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&WriteFileCmd, CFE_SB_CMD_MID,
                   sizeof(WriteFileCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &WriteFileCmd,
                      CFE_SB_SEND_MAP_INFO_CC);
    strncpy((char *)WriteFileCmd.Payload.Filename, "", sizeof(WriteFileCmd.Payload.Filename));

    /* Create some map info */
    SETUP(CFE_SB_CreatePipe(&PipeId1, PipeDepth, "TestPipe1"));
    SETUP(CFE_SB_CreatePipe(&PipeId2, PipeDepth, "TestPipe2"));
    SETUP(CFE_SB_CreatePipe(&PipeId3, PipeDepth, "TestPipe3"));
    SETUP(CFE_SB_Subscribe(MsgId0, PipeId1));
    SETUP(CFE_SB_Subscribe(MsgId0, PipeId2));
    SETUP(CFE_SB_Subscribe(MsgId1, PipeId1));
    SETUP(CFE_SB_Subscribe(MsgId2, PipeId3));
    SETUP(CFE_SB_Subscribe(MsgId3, PipeId3));
    SETUP(CFE_SB_Subscribe(MsgId4, PipeId3));
    SETUP(CFE_SB_Subscribe(MsgId5, PipeId2));
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &WriteFileCmd;

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(18);

    EVTSENT(CFE_SB_SND_RTG_EID);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId1));
    TEARDOWN(CFE_SB_DeletePipe(PipeId2));
    TEARDOWN(CFE_SB_DeletePipe(PipeId3));

    REPORT();
} /* end Test_SB_Cmds_MapInfoDef */

/*
** Test send map information command using a specified file name
*/
void Test_SB_Cmds_MapInfoSpec(void)
{
    CFE_SB_WriteFileInfoCmd_t WriteFileCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&WriteFileCmd, CFE_SB_CMD_MID,
                   sizeof(WriteFileCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &WriteFileCmd,
                      CFE_SB_SEND_MAP_INFO_CC);
    strncpy((char *)WriteFileCmd.Payload.Filename, "MapTstFile", sizeof(WriteFileCmd.Payload.Filename));
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &WriteFileCmd;
    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_SND_RTG_EID);

    REPORT();
} /* end Test_SB_Cmds_MapInfoSpec */

/*
** Test send map information command with a file creation failure
*/
void Test_SB_Cmds_MapInfoCreateFail(void)
{
    START();

    SB_ResetUnitTest();
    UT_SetForceFail(UT_KEY(OS_creat), OS_ERROR);
    ASSERT_EQ(CFE_SB_SendMapInfo("MapTstFile"), CFE_SB_FILE_IO_ERR);

    EVTCNT(1);

    EVTSENT(CFE_SB_SND_RTG_ERR1_EID);

    REPORT();
} /* end Test_SB_Cmds_MapInfoCreateFail */

/*
** Test send map information command with a file header write failure
*/
void Test_SB_Cmds_MapInfoHdrFail(void)
{
    START();

    SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(CFE_FS_WriteHeader), 1, -1);
    ASSERT_EQ(CFE_SB_SendMapInfo("MapTstFile"), CFE_SB_FILE_IO_ERR);

    EVTCNT(1);

    EVTSENT(CFE_SB_FILEWRITE_ERR_EID);

    REPORT();
} /* end Test_SB_Cmds_MapInfoHdrFail */

/*
** Test send map information command with a file write failure on
** the second write
*/
void Test_SB_Cmds_MapInfoWriteFail(void)
{
    CFE_SB_PipeId_t PipeId1;
    CFE_SB_PipeId_t PipeId2;
    CFE_SB_PipeId_t PipeId3;
    CFE_SB_MsgId_t  MsgId0 = SB_UT_TLM_MID + 1;
    CFE_SB_MsgId_t  MsgId1 = SB_UT_TLM_MID + 2;
    CFE_SB_MsgId_t  MsgId2 = SB_UT_TLM_MID + 3;
    CFE_SB_MsgId_t  MsgId3 = SB_UT_TLM_MID + 4;
    CFE_SB_MsgId_t  MsgId4 = SB_UT_TLM_MID + 5;
    CFE_SB_MsgId_t  MsgId5 = SB_UT_TLM_MID + 6;
    uint16          PipeDepth = 10;

    START();

    SB_ResetUnitTest();

    /* Create some map info */
    SETUP(CFE_SB_CreatePipe(&PipeId1, PipeDepth, "TestPipe1"));
    SETUP(CFE_SB_CreatePipe(&PipeId2, PipeDepth, "TestPipe2"));
    SETUP(CFE_SB_CreatePipe(&PipeId3, PipeDepth, "TestPipe3"));
    SETUP(CFE_SB_Subscribe(MsgId0, PipeId1));
    SETUP(CFE_SB_Subscribe(MsgId0, PipeId2));
    SETUP(CFE_SB_Subscribe(MsgId1, PipeId1));
    SETUP(CFE_SB_Subscribe(MsgId2, PipeId3));
    SETUP(CFE_SB_Subscribe(MsgId3, PipeId3));
    SETUP(CFE_SB_Subscribe(MsgId4, PipeId3));
    SETUP(CFE_SB_Subscribe(MsgId5, PipeId2));
    UT_SetDeferredRetcode(UT_KEY(OS_write), 2, -1);

    ASSERT_EQ(CFE_SB_SendMapInfo("MapTstFile"), CFE_SB_FILE_IO_ERR);

    EVTCNT(18);

    EVTSENT(CFE_SB_FILEWRITE_ERR_EID);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId1));
    TEARDOWN(CFE_SB_DeletePipe(PipeId2));
    TEARDOWN(CFE_SB_DeletePipe(PipeId3));

    REPORT();
} /* end Test_SB_Cmds_MapInfoWriteFail */

/*
** Test command to enable a specific route using a valid route
*/
void Test_SB_Cmds_EnRouteValParam(void)
{
    CFE_SB_RouteCmd_t  EnDisRouteCmd;
    CFE_SB_PipeId_t    PipeId;
    CFE_SB_MsgId_t     MsgId = SB_UT_TLM_MID;
    uint16             PipeDepth = 5;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "EnRouteTestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));
    CFE_SB_InitMsg(&EnDisRouteCmd, CFE_SB_CMD_MID,
                   sizeof(EnDisRouteCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd,
                      CFE_SB_ENABLE_ROUTE_CC);
    EnDisRouteCmd.Payload.MsgId = MsgId;
    EnDisRouteCmd.Payload.Pipe = PipeId;
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &EnDisRouteCmd;

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(4);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    EVTSENT(CFE_SB_ENBL_RTE2_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SB_Cmds_EnRouteValParam */

/*
** Test command to enable a specific route using a non-existent route
*/
void Test_SB_Cmds_EnRouteNonExist(void)
{
    CFE_SB_RouteCmd_t  EnDisRouteCmd;
    CFE_SB_PipeId_t    PipeId1;
    CFE_SB_PipeId_t    PipeId2;
    CFE_SB_MsgId_t     MsgId = SB_UT_TLM_MID;
    uint16             PipeDepth = 5;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId1, PipeDepth, "EnRouteTestPipe1"));
    SETUP(CFE_SB_CreatePipe(&PipeId2, PipeDepth, "EnRouteTestPipe2"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId1));
    CFE_SB_InitMsg(&EnDisRouteCmd, CFE_SB_CMD_MID,
                   sizeof(EnDisRouteCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd,
                      CFE_SB_ENABLE_ROUTE_CC);
    EnDisRouteCmd.Payload.MsgId = MsgId;
    EnDisRouteCmd.Payload.Pipe = PipeId2;
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &EnDisRouteCmd;

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(5);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);
    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);
    EVTSENT(CFE_SB_ENBL_RTE1_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId1));
    TEARDOWN(CFE_SB_DeletePipe(PipeId2));

    REPORT();
} /* end Test_SB_Cmds_EnRouteNonExist */

/*
** Test command to enable a specific route using an invalid pipe ID
*/
void Test_SB_Cmds_EnRouteInvParam(void)
{
    CFE_SB_RouteCmd_t  EnDisRouteCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&EnDisRouteCmd, CFE_SB_CMD_MID,
                   sizeof(EnDisRouteCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd,
                      CFE_SB_ENABLE_ROUTE_CC);
    EnDisRouteCmd.Payload.MsgId = CFE_PLATFORM_SB_HIGHEST_VALID_MSGID;
    EnDisRouteCmd.Payload.Pipe = 3;
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &EnDisRouteCmd;

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_ENBL_RTE3_EID);

    REPORT();
} /* end Test_SB_Cmds_EnRouteInvParam */

/*
** Test command to enable a specific route using an invalid message ID
*/
void Test_SB_Cmds_EnRouteInvParam2(void)
{
    CFE_SB_RouteCmd_t  EnDisRouteCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&EnDisRouteCmd, CFE_SB_CMD_MID,
                   sizeof(EnDisRouteCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd,
                      CFE_SB_ENABLE_ROUTE_CC);
    EnDisRouteCmd.Payload.MsgId = CFE_SB_INVALID_MSG_ID;
    EnDisRouteCmd.Payload.Pipe = 3;
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &EnDisRouteCmd;

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_ENBL_RTE3_EID);

    REPORT();
} /* end Test_SB_Cmds_EnRouteInvParam2 */

/*
** Test command to enable a specific route using a message ID greater
** than the maximum allowed
*/
void Test_SB_Cmds_EnRouteInvParam3(void)
{
    CFE_SB_RouteCmd_t  EnDisRouteCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&EnDisRouteCmd, CFE_SB_CMD_MID,
                   sizeof(EnDisRouteCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd,
                      CFE_SB_ENABLE_ROUTE_CC);
    EnDisRouteCmd.Payload.MsgId = CFE_PLATFORM_SB_HIGHEST_VALID_MSGID + 1;
    EnDisRouteCmd.Payload.Pipe = 0;
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &EnDisRouteCmd;

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_ENBL_RTE3_EID);

    REPORT();
} /* end Test_SB_Cmds_EnRouteInvParam3 */

/*
**  Test command to disable a specific route using a valid route
*/
void Test_SB_Cmds_DisRouteValParam(void)
{
    CFE_SB_RouteCmd_t  EnDisRouteCmd;
    CFE_SB_PipeId_t    PipeId;
    CFE_SB_MsgId_t     MsgId = SB_UT_TLM_MID;
    uint16             PipeDepth = 5;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "DisRouteTestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));
    CFE_SB_InitMsg(&EnDisRouteCmd, CFE_SB_CMD_MID,
                   sizeof(EnDisRouteCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd,
                      CFE_SB_DISABLE_ROUTE_CC);
    EnDisRouteCmd.Payload.MsgId = MsgId;
    EnDisRouteCmd.Payload.Pipe = PipeId;
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &EnDisRouteCmd;
    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(4);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    EVTSENT(CFE_SB_DSBL_RTE2_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SB_Cmds_DisRouteValParam */

/*
** Test command to disable a specific route using an invalid pipe ID
*/
void Test_SB_Cmds_DisRouteNonExist(void)
{
    CFE_SB_RouteCmd_t  EnDisRouteCmd;
    CFE_SB_PipeId_t    PipeId1, PipeId2;
    CFE_SB_MsgId_t     MsgId = SB_UT_TLM_MID;
    uint16             PipeDepth = 5;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId1, PipeDepth, "DisRouteTestPipe1"));
    SETUP(CFE_SB_CreatePipe(&PipeId2, PipeDepth, "DisRouteTestPipe2"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId1));
    CFE_SB_InitMsg(&EnDisRouteCmd, CFE_SB_CMD_MID,
                   sizeof(EnDisRouteCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd,
                      CFE_SB_DISABLE_ROUTE_CC);
    EnDisRouteCmd.Payload.MsgId = MsgId;
    EnDisRouteCmd.Payload.Pipe = PipeId2;
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &EnDisRouteCmd;

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(5);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);
    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);
    EVTSENT(CFE_SB_DSBL_RTE1_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId1));
    TEARDOWN(CFE_SB_DeletePipe(PipeId2));

    REPORT();
} /* end Test_SB_Cmds_DisRouteNonExist */

/*
** Test command to disable a specific route using an invalid pipe ID
*/
void Test_SB_Cmds_DisRouteInvParam(void)
{
    CFE_SB_RouteCmd_t  EnDisRouteCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&EnDisRouteCmd, CFE_SB_CMD_MID,
                   sizeof(EnDisRouteCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd,
                      CFE_SB_DISABLE_ROUTE_CC);
    EnDisRouteCmd.Payload.MsgId = CFE_PLATFORM_SB_HIGHEST_VALID_MSGID;
    EnDisRouteCmd.Payload.Pipe = 3;
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &EnDisRouteCmd;

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_DSBL_RTE3_EID);

    REPORT();
} /* end Test_SB_Cmds_DisRouteInvParam */

/*
** Test command to disable a specific route using an invalid message ID
*/
void Test_SB_Cmds_DisRouteInvParam2(void)
{
    CFE_SB_RouteCmd_t  EnDisRouteCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&EnDisRouteCmd, CFE_SB_CMD_MID,
                   sizeof(EnDisRouteCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd,
                      CFE_SB_DISABLE_ROUTE_CC);
    EnDisRouteCmd.Payload.MsgId = CFE_SB_INVALID_MSG_ID;
    EnDisRouteCmd.Payload.Pipe = 3;
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &EnDisRouteCmd;

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_DSBL_RTE3_EID);

    REPORT();
} /* end Test_SB_Cmds_DisRouteInvParam2 */

/*
** Test command to disable a specific route using a message ID greater
** than the maximum allowed
*/
void Test_SB_Cmds_DisRouteInvParam3(void)
{
    CFE_SB_RouteCmd_t  EnDisRouteCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&EnDisRouteCmd, CFE_SB_CMD_MID,
                   sizeof(EnDisRouteCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd,
                      CFE_SB_DISABLE_ROUTE_CC);
    EnDisRouteCmd.Payload.MsgId = CFE_PLATFORM_SB_HIGHEST_VALID_MSGID + 1;
    EnDisRouteCmd.Payload.Pipe = 0;
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &EnDisRouteCmd;

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_DSBL_RTE3_EID);

    REPORT();
} /* end Test_SB_Cmds_DisRouteInvParam3 */

/*
** Test send housekeeping information command
*/
void Test_SB_Cmds_SendHK(void)
{
    CFE_SB_CmdHdr_t NoParamCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&NoParamCmd, CFE_SB_SEND_HK_MID,
                   sizeof(NoParamCmd), true);
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &NoParamCmd;

    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_SEND_NO_SUBS_EID);

    REPORT();
} /* end Test_SB_Cmds_SendHK */

/*
** Test command to build and send a SB packet containing a complete
** list of current subscriptions
*/
void Test_SB_Cmds_SendPrevSubs(void)
{
    CFE_SB_SendPrevSubs_t NoParamCmd;
    CFE_SB_PipeId_t PipeId1;
    CFE_SB_PipeId_t PipeId2;
    CFE_SB_MsgId_t  MsgId = 0x0003;
    uint16          MsgLim = 4;
    uint16          PipeDepth = 50;
    int32           i;
    int32           NumEvts;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&NoParamCmd, CFE_SB_CMD_MID, sizeof(CFE_SB_SendPrevSubs_t), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &NoParamCmd, CFE_SB_SEND_PREV_SUBS_CC);
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &NoParamCmd;
    SETUP(CFE_SB_CreatePipe(&PipeId1, PipeDepth, "TestPipe1"));
    SETUP(CFE_SB_CreatePipe(&PipeId2, PipeDepth, "TestPipe2"));
    NumEvts = 2; /* one for each pipe create */

    /* Two full pkts to be sent plus five entries in a partial pkt, skipping MSGID 0x0D */
    for (i = 0; i < CFE_SB_SUB_ENTRIES_PER_PKT * 2 + 5; i++)
    {
        /* Skip subscribing to ALLSUBS mid. This is the one we are testing.
         * MsgID for this in CCSDS v.1 was 0x180d so this MID did not appear in the
         * SB sub list. This results in multiple NO_SUBS_EID sent which we are not 
         * testing here so it is irrelevent
         * For CCSDS v.2 CFE_SB_ALLSUBS_TLM_MID (0x0d) now appears in the
         * SB subscription list and thus we must skip adding 0x0D to the list
         * as we were going from MSGID 0-45 (0x00-0x2D)
         * */
        if (i != CFE_SB_ALLSUBS_TLM_MID)
        {
            NumEvts += 2;
            SETUP(CFE_SB_Subscribe(i, PipeId1));
        }
    }

    SETUP(CFE_SB_SubscribeLocal(MsgId, PipeId2, MsgLim));

    CFE_SB_ProcessCmdPipePkt();

    NumEvts += 8;  /* +2 for the subscribe, +6 for the SEND_PREV_SUBS_CC */
    EVTCNT(NumEvts);

    /* Round out the number to three full pkts in order to test branch path
     * coverage, MSGID 0x0D was skipped in previous subscription loop
     */
    for (; i < CFE_SB_SUB_ENTRIES_PER_PKT * 3; i++)
    {
        SETUP(CFE_SB_Subscribe(i, PipeId1));
        NumEvts += 2;
    }

    SETUP(CFE_SB_SubscribeLocal(MsgId, PipeId2, MsgLim));

    CFE_SB_ProcessCmdPipePkt();

    NumEvts += 8;  /* +2 for the subscribe, +6 for the SEND_PREV_SUBS_CC */

    EVTCNT(NumEvts);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);
    EVTSENT(CFE_SB_SEND_NO_SUBS_EID);
    EVTSENT(CFE_SB_FULL_SUB_PKT_EID);
    EVTSENT(CFE_SB_PART_SUB_PKT_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId1));
    TEARDOWN(CFE_SB_DeletePipe(PipeId2));

    REPORT();
} /* end Test_SB_Cmds_SendPrevSubs */

/*
** Test command to enable subscription reporting
*/
void Test_SB_Cmds_SubRptOn(void)
{
    CFE_SB_CmdHdr_t NoParamCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&NoParamCmd, CFE_SB_CMD_MID,
                   sizeof(NoParamCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &NoParamCmd,
                      CFE_SB_ENABLE_SUB_REPORTING_CC);
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &NoParamCmd;
    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(0);

    REPORT();
} /* end Test_SB_Cmds_SubRptOn */

/*
** Test command to disable subscription reporting
*/
void Test_SB_Cmds_SubRptOff(void)
{
    CFE_SB_CmdHdr_t NoParamCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&NoParamCmd, CFE_SB_CMD_MID,
                   sizeof(NoParamCmd), true);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &NoParamCmd,
                      CFE_SB_DISABLE_SUB_REPORTING_CC);
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &NoParamCmd;
    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(0);

    REPORT();
} /* end Test_SB_Cmds_SubRptOff */

/*
** Test command handler response to an invalid command code
*/
void Test_SB_Cmds_UnexpCmdCode(void)
{
    CFE_SB_CmdHdr_t NoParamCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&NoParamCmd, CFE_SB_CMD_MID, sizeof(NoParamCmd), true);

    /* Use a command code known to be invalid */
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &NoParamCmd, 99);
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &NoParamCmd;
    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_BAD_CMD_CODE_EID);

    REPORT();
} /* end Test_SB_Cmds_UnexpCmdCode */

/*
** Test command handler response to an incorrect length
*/
void Test_SB_Cmds_BadCmdLength(void)
{
    /*
     * Just choosing "EnableRoute" command here as it has a non-empty payload
     */
    CFE_SB_EnableRoute_t EnableRouteCmd;

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&EnableRouteCmd, CFE_SB_CMD_MID, sizeof(EnableRouteCmd) - 1, true);

    /* Use a command code known to be invalid */
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnableRouteCmd, CFE_SB_ENABLE_ROUTE_CC);
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &EnableRouteCmd;
    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_LEN_ERR_EID);

    REPORT();
} /* end Test_SB_Cmds_BadCmdLength */

/*
** Test command handler response to an invalid message ID
*/
void Test_SB_Cmds_UnexpMsgId(void)
{
    CFE_SB_MsgId_t  MsgId = SB_UT_TLM_MID;
    CFE_SB_CmdHdr_t NoParamCmd;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&NoParamCmd, MsgId, sizeof(NoParamCmd), true);
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &NoParamCmd;
    CFE_SB_ProcessCmdPipePkt();

    EVTCNT(1);

    EVTSENT(CFE_SB_BAD_MSGID_EID);

    REPORT();
} /* end Test_SB_Cmds_UnexpMsgId */

/*
** Function for calling early initialization tests
*/
void Test_SB_EarlyInit(void)
{
    STARTBLOCK();

    Test_SB_EarlyInit_SemCreateError();
    Test_SB_EarlyInit_PoolCreateError();
    Test_SB_EarlyInit_NoErrors();

    ENDBLOCK();
} /* end Test_SB_EarlyInit */

/*
** Test early initialization response to a semaphore create failure
*/
void Test_SB_EarlyInit_SemCreateError(void)
{
    START();

    SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(OS_MutSemCreate), 1, OS_ERR_NO_FREE_IDS);
    UT_Report(__FILE__, __LINE__,
              CFE_SB_EarlyInit() == OS_ERR_NO_FREE_IDS,
              "CFE_SB_EarlyInit", "Sem Create error logic");
} /* end Test_SB_EarlyInit_SemCreateError */

/*
** Test early initialization response to a pool create ex failure
*/
void Test_SB_EarlyInit_PoolCreateError(void)
{
    START();

    SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_PoolCreateEx), 1, CFE_ES_BAD_ARGUMENT);
    UT_Report(__FILE__, __LINE__,
              CFE_SB_EarlyInit() == CFE_ES_BAD_ARGUMENT,
              "CFE_SB_EarlyInit", "PoolCreateEx error logic");
} /* end Test_SB_EarlyInit_PoolCreateError */

/*
** Test successful early initialization
*/
void Test_SB_EarlyInit_NoErrors(void)
{
    START();

    SB_ResetUnitTest();
    CFE_SB_EarlyInit();
    UT_Report(__FILE__, __LINE__,
              CFE_SB_EarlyInit() == CFE_SUCCESS,
              "CFE_SB_EarlyInit", "No errors test");
} /* end Test_SB_EarlyInit_NoErrors */

/*
** Function for calling SB create pipe API test functions
*/
void Test_CreatePipe_API(void)
{
    STARTBLOCK();

    Test_CreatePipe_NullPtr();
    Test_CreatePipe_ValPipeDepth();
    Test_CreatePipe_InvalPipeDepth();
    Test_CreatePipe_MaxPipes();
    Test_CreatePipe_SamePipeName();

    ENDBLOCK();
} /* end Test_CreatePipe_API */

/*
** Test create pipe response to a null pipe ID pointer
*/
void Test_CreatePipe_NullPtr(void)
{
    uint16 PipeDepth = 10;

    START();

    SB_ResetUnitTest();

    /* This provides completion of code coverage in CFE_SB_GetAppTskName() */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetTaskInfo), 1, CFE_ES_ERR_APPID);

    UT_SetDeferredRetcode(UT_KEY(OS_QueueCreate), 1, OS_SUCCESS); /* Avoids creating socket */
    ASSERT_EQ(CFE_SB_CreatePipe(NULL, PipeDepth, "TestPipe"), CFE_SB_BAD_ARGUMENT);

    EVTCNT(1);

    EVTSENT(CFE_SB_CR_PIPE_BAD_ARG_EID);

    REPORT();
} /* end Test_CreatePipe_NullPtr */

/*
** Test create pipe response to valid pipe depths
*/
void Test_CreatePipe_ValPipeDepth(void)
{
    CFE_SB_PipeId_t PipeIdReturned[3];

    START();

    /* Test a few valid pipe depth values */
    SB_ResetUnitTest();
    ASSERT(CFE_SB_CreatePipe(&PipeIdReturned[0], 99, "TestPipe99"));
    ASSERT(CFE_SB_CreatePipe(&PipeIdReturned[1], 255, "TestPipe255"));
    ASSERT(CFE_SB_CreatePipe(&PipeIdReturned[2],
             CFE_PLATFORM_SB_MAX_PIPE_DEPTH, "TestPipeMaxDepth"));

    EVTCNT(3);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeIdReturned[0]));
    TEARDOWN(CFE_SB_DeletePipe(PipeIdReturned[1]));
    TEARDOWN(CFE_SB_DeletePipe(PipeIdReturned[2]));

    REPORT();
} /* end Test_CreatePipe_ValPipeDepth */

/*
** Test create pipe response to invalid pipe depths
*/
void Test_CreatePipe_InvalPipeDepth(void)
{
    CFE_SB_PipeId_t PipeIdReturned[3];

    START();

    /* Test a few invalid pipe depth values */
    SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(OS_QueueCreate), 1, OS_SUCCESS); /* Avoid creating socket */
    ASSERT_EQ(CFE_SB_CreatePipe(&PipeIdReturned[0], 0, "TestPipe1"), CFE_SB_BAD_ARGUMENT);
    UT_SetDeferredRetcode(UT_KEY(OS_QueueCreate), 1, OS_SUCCESS);
    ASSERT_EQ(CFE_SB_CreatePipe(&PipeIdReturned[1],
             CFE_PLATFORM_SB_MAX_PIPE_DEPTH + 1, "TestPipeMaxDepPlus1"), CFE_SB_BAD_ARGUMENT);
    UT_SetDeferredRetcode(UT_KEY(OS_QueueCreate), 1, OS_SUCCESS);
    ASSERT_EQ(CFE_SB_CreatePipe(&PipeIdReturned[2], 0xffff, "TestPipeffff"), CFE_SB_BAD_ARGUMENT);

    EVTCNT(3);

    EVTSENT(CFE_SB_CR_PIPE_BAD_ARG_EID);

    REPORT();
} /* end Test_CreatePipe_InvalPipeDepth */

/*
** Test create pipe response to too many pipes
*/
void Test_CreatePipe_MaxPipes(void)
{
    CFE_SB_PipeId_t PipeIdReturned[CFE_PLATFORM_SB_MAX_PIPES + 1];
    uint16          PipeDepth = 50;
    int32           i;
    char            PipeName[OS_MAX_API_NAME];

    START();

    SB_ResetUnitTest();

    /* Create maximum number of pipes + 1. Only one 'create pipe' failure
     * expected
     */
    for (i = 0; TestStat == CFE_PASS && i < CFE_PLATFORM_SB_MAX_PIPES + 1; i++)
    {
        snprintf(PipeName, OS_MAX_API_NAME, "TestPipe%ld", (long) i);

        if (i < CFE_PLATFORM_SB_MAX_PIPES)
        {
        	SETUP(CFE_SB_CreatePipe(&PipeIdReturned[i], PipeDepth, &PipeName[0]));
        }
        else
        {
        	ASSERT_EQ(CFE_SB_CreatePipe(&PipeIdReturned[i], PipeDepth, &PipeName[0]), CFE_SB_MAX_PIPES_MET);
        }
    }

    EVTSENT(CFE_SB_MAX_PIPES_MET_EID);

    /* Clean up */
    for (i = 0; i < CFE_PLATFORM_SB_MAX_PIPES; i++)
    {
        TEARDOWN(CFE_SB_DeletePipe(PipeIdReturned[i]));
    }

    REPORT();
} /* end Test_CreatePipe_MaxPipes */

/*
** Test create pipe response to duplicate pipe names
*/
void Test_CreatePipe_SamePipeName(void)
{
    CFE_SB_PipeId_t FirstPipeId = -1;
    CFE_SB_PipeId_t PipeId = -1;
    uint16 PipeDepth = 1;
    char PipeName[] = "Test_CFE_SB";

    START();

    SB_ResetUnitTest();

    /* Need to make sure OS_QueueCreate() stub fails on second call   */
    /* to mimick the failure expected when passing the same pipe name */
    UT_SetDeferredRetcode(UT_KEY(OS_QueueCreate), 2, OS_ERR_NAME_TAKEN);

    /* First call to CFE_SB_CreatePipe() should succeed */
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, PipeName));

    FirstPipeId = PipeId;

    /* Second call to CFE_SB_CreatePipe with same PipeName should fail */
    ASSERT_EQ(CFE_SB_CreatePipe(&PipeId, PipeDepth, PipeName), CFE_SB_PIPE_CR_ERR);

    ASSERT_EQ(PipeId, FirstPipeId);

    EVTCNT(2);

    EVTSENT(CFE_SB_CR_PIPE_NAME_TAKEN_EID);

    /* Call to CFE_SB_DeletePipe with the first pipe id created should work fine */
    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
}

/*
** Function for calling SB delete pipe API test functions
*/
void Test_DeletePipe_API(void)
{
    STARTBLOCK();

    Test_DeletePipe_NoSubs();
    Test_DeletePipe_WithSubs();
    Test_DeletePipe_InvalidPipeId();
    Test_DeletePipe_InvalidPipeOwner();
    Test_DeletePipe_WithAppid();

    ENDBLOCK();
} /* end Test_DeletePipe_API */

/*
** Test delete pipe response to deleting a pipe with no subscriptions
*/
void Test_DeletePipe_NoSubs(void)
{
    CFE_SB_PipeId_t PipedId;
    uint16          PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipedId, PipeDepth, "TestPipe"));
    ASSERT(CFE_SB_DeletePipe(PipedId));

    EVTCNT(2);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);
    EVTSENT(CFE_SB_PIPE_DELETED_EID);

    REPORT();
} /* end Test_DeletePipe_NoSubs */

/*
** Test delete pipe response to deleting a pipe with subscriptions
*/
void Test_DeletePipe_WithSubs(void)
{
    CFE_SB_PipeId_t PipedId;
    CFE_SB_MsgId_t  MsgId0 = SB_UT_CMD_MID + 1;
    CFE_SB_MsgId_t  MsgId1 = SB_UT_CMD_MID + 2;
    CFE_SB_MsgId_t  MsgId2 = SB_UT_CMD_MID + 3;
    CFE_SB_MsgId_t  MsgId3 = SB_UT_CMD_MID + 4;
    uint16          PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipedId, PipeDepth, "TestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId0, PipedId));
    SETUP(CFE_SB_Subscribe(MsgId1, PipedId));
    SETUP(CFE_SB_Subscribe(MsgId2, PipedId));
    SETUP(CFE_SB_Subscribe(MsgId3, PipedId));
    ASSERT(CFE_SB_DeletePipe(PipedId));

    EVTCNT(14);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);
    EVTSENT(CFE_SB_PIPE_DELETED_EID);

    REPORT();
} /* end Test_DeletePipe_WithSubs */

/*
** Test delete pipe response to an invalid pipe ID
*/
void Test_DeletePipe_InvalidPipeId(void)
{
    CFE_SB_PipeId_t PipeId = 30;

    START();

    SB_ResetUnitTest();

    ASSERT_EQ(CFE_SB_DeletePipe(PipeId), CFE_SB_BAD_ARGUMENT);

    EVTCNT(1);

    EVTSENT(CFE_SB_DEL_PIPE_ERR1_EID);

    REPORT();
} /* end Test_DeletePipe_InvalidPipeId */

/*
** Test delete pipe response to an invalid pipe owner
*/
void Test_DeletePipe_InvalidPipeOwner(void)
{
    CFE_SB_PipeId_t PipedId;
    uint32          RealOwner;
    uint16          PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipedId, PipeDepth, "TestPipe"));

    /* Change owner of pipe through memory corruption */
    RealOwner = CFE_SB.PipeTbl[PipedId].AppId;

    /* Choose a value that is sure not to be owner */
    CFE_SB.PipeTbl[PipedId].AppId = RealOwner + 1;
    ASSERT_EQ(CFE_SB_DeletePipe(PipedId), CFE_SB_BAD_ARGUMENT);

    EVTCNT(2);

    EVTSENT(CFE_SB_DEL_PIPE_ERR2_EID);

    /* Restore owner id and delete pipe since test is complete */
    CFE_SB.PipeTbl[PipedId].AppId = RealOwner;
    TEARDOWN(CFE_SB_DeletePipe(PipedId));

    REPORT();
} /* end Test_DeletePipe_InvalidPipeId */

/*
** Test successful pipe delete with app ID
*/
void Test_DeletePipe_WithAppid(void)
{
    CFE_SB_PipeId_t PipedId;
    uint32			AppId = 0;
    uint16          PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipedId, PipeDepth, "TestPipe"));
    SETUP(CFE_SB_Subscribe(SB_UT_CMD_MID + 1, PipedId));
    SETUP(CFE_SB_Subscribe(SB_UT_CMD_MID + 2, PipedId));
    SETUP(CFE_SB_Subscribe(SB_UT_CMD_MID + 3, PipedId));
    SETUP(CFE_SB_Subscribe(SB_UT_CMD_MID + 4, PipedId));

    ASSERT(CFE_SB_DeletePipeWithAppId(PipedId, AppId));

    EVTCNT(14);

    REPORT();
} /* end Test_DeletePipe_WithAppid */

/*
** Function for calling SB set pipe opts API test functions
*/
void Test_PipeOpts_API(void)
{
    STARTBLOCK();

    Test_SetPipeOpts_BadID();
    Test_SetPipeOpts_NotOwner();
    Test_SetPipeOpts();
    Test_GetPipeOpts_BadID();
    Test_GetPipeOpts_BadPtr();
    Test_GetPipeOpts();

    ENDBLOCK();
} /* end Test_PipeOpts_API */

/*
** Function for calling SB get pipe name by id API test functions
*/
void Test_GetPipeName_API(void)
{
    STARTBLOCK();

    Test_GetPipeName_NullPtr();
    Test_GetPipeName_InvalidId();
    Test_GetPipeName();

    ENDBLOCK();
} /* end Test_GetGetPipeName_API */

/*
** Call GetPipeName with null pointer.
*/
void Test_GetPipeName_NullPtr(void)
{
    START();

    ASSERT_EQ(CFE_SB_GetPipeName(NULL, OS_MAX_API_NAME, 0), CFE_SB_BAD_ARGUMENT);

    EVTSENT(CFE_SB_GETPIPENAME_NULL_PTR_EID);

    REPORT();
} /* end Test_GetPipeName_NullPtr */

/*
** Call to GetPipeName with an invalid pipe id.
*/
void Test_GetPipeName_InvalidId(void)
{
    char PipeName[OS_MAX_API_NAME];
    START();

    UT_SetDeferredRetcode(UT_KEY(OS_QueueGetInfo), 1, OS_ERROR);

    ASSERT_EQ(CFE_SB_GetPipeName(PipeName, OS_MAX_API_NAME, 0), CFE_SB_BAD_ARGUMENT);

    EVTSENT(CFE_SB_GETPIPENAME_ID_ERR_EID);

    REPORT();
} /* end Test_GetPipeName_InvalidId */

/*
** Successful call to GetPipeName
*/
void Test_GetPipeName(void)
{
    char PipeName[OS_MAX_API_NAME];
    CFE_SB_PipeId_t PipeId = 0;

    OS_queue_prop_t queue_info = {
        "TestPipe1", 0
    };

    START();

    SETUP(CFE_SB_CreatePipe(&PipeId, 4, "TestPipe1"));

    UT_SetDataBuffer(
        UT_KEY(OS_QueueGetInfo),
        &queue_info, sizeof(queue_info),
        false);

    ASSERT(CFE_SB_GetPipeName(PipeName, OS_MAX_API_NAME, PipeId));

    EVTSENT(CFE_SB_GETPIPENAME_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_GetPipeName */

/*
** Function for calling SB get pipe id by name API test functions
*/
void Test_GetPipeIdByName_API(void)
{
    STARTBLOCK();

    Test_GetPipeIdByName_NullPtrs();
    Test_GetPipeIdByName_InvalidName();
    Test_GetPipeIdByName();

    ENDBLOCK();
} /* end Test_GetPipeIdByName_API */

/*
** Call GetPipeIdByName with null pointers.
*/
void Test_GetPipeIdByName_NullPtrs(void)
{
    CFE_SB_PipeId_t PipeIDOut = 0;
    START();

	ASSERT_EQ(CFE_SB_GetPipeIdByName(&PipeIDOut, NULL), CFE_SB_BAD_ARGUMENT);

	EVTSENT(CFE_SB_GETPIPEIDBYNAME_NULL_ERR_EID);

    ASSERT_EQ(CFE_SB_GetPipeIdByName(NULL, "invalid"), CFE_SB_BAD_ARGUMENT);

    EVTSENT(CFE_SB_GETPIPEIDBYNAME_NULL_ERR_EID);

    REPORT();
} /* end Test_GetPipeIdByName_NullPtrs */

/*
** Call to GetPipeId with an invalid pipe name.
*/
void Test_GetPipeIdByName_InvalidName(void)
{
    CFE_SB_PipeId_t PipeIdOut = 0;
    START();

	ASSERT_EQ(CFE_SB_GetPipeIdByName(&PipeIdOut, "invalid"), CFE_SB_BAD_ARGUMENT);

    EVTSENT(CFE_SB_GETPIPEIDBYNAME_NAME_ERR_EID);

    REPORT();
} /* end Test_GetPipeIdByName_InvalidName */

/*
** Successful call to GetPipeIdByName
*/
void Test_GetPipeIdByName(void)
{
    CFE_SB_PipeId_t PipeId = 0, PipeIdOut = 0;
    START();

    SETUP(CFE_SB_CreatePipe(&PipeId, 4, "TestPipe1"));

    UT_SetDataBuffer(
        UT_KEY(OS_QueueGetIdByName),
        &(CFE_SB.PipeTbl[0].SysQueueId),
        sizeof(CFE_SB.PipeTbl[0].SysQueueId),
        false);

    ASSERT(CFE_SB_GetPipeIdByName(&PipeIdOut, "TestPipe1"));

    EVTSENT(CFE_SB_GETPIPEIDBYNAME_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_GetPipeIdByName */

/*
** Try setting pipe options on an invalid pipe ID
*/
void Test_SetPipeOpts_BadID(void)
{
    START();

	ASSERT_EQ(CFE_SB_SetPipeOpts(CFE_PLATFORM_SB_MAX_PIPES, 0), CFE_SB_BAD_ARGUMENT);

    EVTSENT(CFE_SB_SETPIPEOPTS_ID_ERR_EID);

    REPORT();
} /* end Test_SetPipeOpts_BadID */

/*
** Try setting pipe options when not pipe owner
*/
void Test_SetPipeOpts_NotOwner(void)
{
    CFE_SB_PipeId_t PipeID = 0;
    uint8 PipeTblIdx = 0;
    uint32 OrigOwner = 0;

    START();

    SETUP(CFE_SB_CreatePipe(&PipeID, 4, "TestPipe1"));

    PipeTblIdx = CFE_SB_GetPipeIdx(PipeID);

    OrigOwner = CFE_SB.PipeTbl[PipeTblIdx].AppId;
    CFE_SB.PipeTbl[PipeTblIdx].AppId = 0xFFFFFFFF;
    ASSERT_EQ(CFE_SB_SetPipeOpts(PipeID, 0), CFE_SB_BAD_ARGUMENT);

    CFE_SB.PipeTbl[PipeTblIdx].AppId = OrigOwner;

    EVTSENT(CFE_SB_SETPIPEOPTS_OWNER_ERR_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeID));

    REPORT();
} /* end Test_SetPipeOpts_NotOwner */

/*
** Set pipe options
*/
void Test_SetPipeOpts(void)
{
    CFE_SB_PipeId_t PipeID = 0;

    START();

    SETUP(CFE_SB_CreatePipe(&PipeID, 4, "TestPipe1"));

    ASSERT(CFE_SB_SetPipeOpts(PipeID, 0));

    EVTSENT(CFE_SB_SETPIPEOPTS_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeID));

    REPORT();
} /* end Test_SetPipeOpts */

/*
** Try getting pipe options on an invalid pipe ID
*/
void Test_GetPipeOpts_BadID(void)
{
    uint8 Opts = 0;

    START();

    ASSERT_EQ(CFE_SB_GetPipeOpts(CFE_PLATFORM_SB_MAX_PIPES, &Opts), CFE_SB_BAD_ARGUMENT);

    EVTSENT(CFE_SB_GETPIPEOPTS_ID_ERR_EID);

    REPORT();
} /* end Test_GetPipeOpts_BadID */

/*
** Try getting pipe options with a bad pointer
*/
void Test_GetPipeOpts_BadPtr(void)
{
    CFE_SB_PipeId_t PipeID = 0;

    START();

    SETUP(CFE_SB_CreatePipe(&PipeID, 4, "TestPipe1"));

    ASSERT_EQ(CFE_SB_GetPipeOpts(PipeID, NULL), CFE_SB_BAD_ARGUMENT);

    EVTSENT(CFE_SB_GETPIPEOPTS_PTR_ERR_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeID));

    REPORT();
} /* end Test_GetPipeOpts_BadPtr */

/*
** Successful call to GetPipeOpts
*/
void Test_GetPipeOpts(void)
{
    CFE_SB_PipeId_t PipeID = 0;
    uint8 Opts = 0;

    START();

    SETUP(CFE_SB_CreatePipe(&PipeID, 4, "TestPipe1"));

    ASSERT(CFE_SB_GetPipeOpts(PipeID, &Opts));

    EVTSENT(CFE_SB_GETPIPEOPTS_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeID));

    REPORT();
} /* end Test_GetPipeOpts */

/*
** Function for calling SB subscribe API test functions
*/
void Test_Subscribe_API(void)
{
    STARTBLOCK();

    Test_Subscribe_SubscribeEx();
    Test_Subscribe_InvalidPipeId();
    Test_Subscribe_InvalidMsgId();
    Test_Subscribe_MaxMsgLim();
    Test_Subscribe_DuplicateSubscription();
    Test_Subscribe_LocalSubscription();
    Test_Subscribe_MaxDestCount();
    Test_Subscribe_MaxMsgIdCount();
    Test_Subscribe_SendPrevSubs();
    Test_Subscribe_FindGlobalMsgIdCnt();
    Test_Subscribe_PipeNonexistent();
    Test_Subscribe_SubscriptionReporting();
    Test_Subscribe_InvalidPipeOwner();

    ENDBLOCK();
} /* end Test_Subscribe_API */

/*
** Test API to globally subscribe to a message
*/
void Test_Subscribe_SubscribeEx(void)
{
    CFE_SB_PipeId_t PipeId;
    CFE_SB_MsgId_t  MsgId = SB_UT_CMD_MID;
    CFE_SB_Qos_t    Quality = {0, 0};
    uint16          PipeDepth = 10;
    uint16          MsgLim = 8;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));

    ASSERT(CFE_SB_SubscribeEx(MsgId, PipeId, Quality, MsgLim));

    EVTCNT(3);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);
    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_Subscribe_SubscribeEx */

/*
** Test message subscription response to an invalid pipe ID
*/
void Test_Subscribe_InvalidPipeId(void)
{
    CFE_SB_PipeId_t PipeId = 2;
    CFE_SB_MsgId_t  MsgId = CFE_PLATFORM_SB_HIGHEST_VALID_MSGID + 1;

    START();

    SB_ResetUnitTest();
    ASSERT_EQ(CFE_SB_Subscribe(MsgId, PipeId), CFE_SB_BAD_ARGUMENT);

    EVTCNT(2);

    EVTSENT(CFE_SB_SUB_INV_PIPE_EID);

    REPORT();
} /* end Test_Subscribe_InvalidPipeId */

/*
** Test message subscription response to an invalid message ID
*/
void Test_Subscribe_InvalidMsgId(void)
{
    CFE_SB_PipeId_t PipeId;
    CFE_SB_MsgId_t  MsgId = CFE_PLATFORM_SB_HIGHEST_VALID_MSGID + 1;
    uint16          PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));

    ASSERT_EQ(CFE_SB_Subscribe(MsgId, PipeId), CFE_SB_BAD_ARGUMENT);

    EVTCNT(3);

    EVTSENT(CFE_SB_SUB_ARG_ERR_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_Subscribe_InvalidMsgId */

/*
** Test message subscription response to the maximum message limit
*/
void Test_Subscribe_MaxMsgLim(void)
{
    CFE_SB_PipeId_t PipeId;
    CFE_SB_MsgId_t  MsgId = SB_UT_CMD_MID;
    CFE_SB_Qos_t    Quality = {0, 0};
    uint16          PipeDepth = 10;
    uint16          MsgLim;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));
    MsgLim = 0xffff;

    ASSERT(CFE_SB_SubscribeEx(MsgId, PipeId, Quality, MsgLim));

    EVTCNT(3);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_Subscribe_MaxMsgLim */

/*
** Test message subscription response to a duplicate subscription
*/
void Test_Subscribe_DuplicateSubscription(void)
{
    CFE_SB_PipeId_t PipeId;
    CFE_SB_MsgId_t  MsgId = SB_UT_CMD_MID;
    uint16          PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));

    ASSERT(CFE_SB_Subscribe(MsgId, PipeId));
    ASSERT(CFE_SB_Subscribe(MsgId, PipeId));

    EVTCNT(5);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_Subscribe_DuplicateSubscription */

/*
** Test API to locally subscribe to a message
*/
void Test_Subscribe_LocalSubscription(void)
{
    CFE_SB_PipeId_t PipeId;
    CFE_SB_MsgId_t  MsgId = SB_UT_TLM_MID;
    uint16          PipeDepth = 10;
    uint16          MsgLim = 4;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));

    ASSERT(CFE_SB_SubscribeLocal(MsgId, PipeId, MsgLim));

    EVTCNT(3);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));
    REPORT();
} /* end Test_Subscribe_LocalSubscription */

/*
** Test message subscription response to reaching the maximum destination count
*/
void Test_Subscribe_MaxDestCount(void)
{
    CFE_SB_PipeId_t PipeId[CFE_PLATFORM_SB_MAX_DEST_PER_PKT + 1];
    CFE_SB_MsgId_t  MsgId = SB_UT_TLM_MID;
    char            PipeName[OS_MAX_API_NAME];
    uint16          PipeDepth = 50;
    int32           i;

    START();

    SB_ResetUnitTest();

    /* Create pipes */
    for (i = 0; i < CFE_PLATFORM_SB_MAX_DEST_PER_PKT + 1; i++)
    {
        snprintf(PipeName, OS_MAX_API_NAME, "TestPipe%ld", (long) i);
        SETUP(CFE_SB_CreatePipe(&PipeId[i], PipeDepth, &PipeName[0]));
    }

    /* Do subscriptions */
    for (i = 0; i < CFE_PLATFORM_SB_MAX_DEST_PER_PKT + 1; i++)
    {
        if (i < CFE_PLATFORM_SB_MAX_DEST_PER_PKT)
        {
            SETUP(CFE_SB_Subscribe(MsgId, i));
        }
        else
        {
            ASSERT_EQ(CFE_SB_Subscribe(MsgId, i), CFE_SB_MAX_DESTS_MET);
        }
    }

    EVTCNT(3 * (CFE_PLATFORM_SB_MAX_DEST_PER_PKT + 1));

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    /* Delete pipes */
    for (i = 0; i < CFE_PLATFORM_SB_MAX_DEST_PER_PKT + 1; i++)
    {
        TEARDOWN(CFE_SB_DeletePipe(PipeId[i]));
    }

    REPORT();
} /* end Test_Subscribe_MaxDestCount */

/*
** Test message subscription response to reaching the maximum message ID count
*/
void Test_Subscribe_MaxMsgIdCount(void)
{
    CFE_SB_PipeId_t PipeId0;
    CFE_SB_PipeId_t PipeId1;
    CFE_SB_PipeId_t PipeId2;
    uint16          PipeDepth = 50;
    int32           i;

    START();

    SB_ResetUnitTest();

    SETUP(CFE_SB_CreatePipe(&PipeId0, PipeDepth, "TestPipe0"));
    SETUP(CFE_SB_CreatePipe(&PipeId1, PipeDepth, "TestPipe1"));
    SETUP(CFE_SB_CreatePipe(&PipeId2, PipeDepth, "TestPipe2"));

    for (i = 0; i < CFE_PLATFORM_SB_MAX_MSG_IDS + 1; i++)
    {

        if (i < CFE_PLATFORM_SB_MAX_MSG_IDS)
        {
            SETUP(CFE_SB_Subscribe(i, PipeId2));
        }
        else
        {
            ASSERT_EQ(CFE_SB_Subscribe(i, PipeId2), CFE_SB_MAX_MSGS_MET);
        }
    }

    EVTSENT(CFE_SB_MAX_MSGS_MET_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId0));
    TEARDOWN(CFE_SB_DeletePipe(PipeId1));
    TEARDOWN(CFE_SB_DeletePipe(PipeId2));

    REPORT();
} /* end Test_Subscribe_MaxMsgIdCount */

/*
** Test obtaining the list of current message subscriptions
*/
void Test_Subscribe_SendPrevSubs(void)
{
    CFE_SB_PipeId_t PipeId0;
    CFE_SB_PipeId_t PipeId1;
    CFE_SB_PipeId_t PipeId2;
    CFE_SB_MsgId_t  MsgId0 = SB_UT_TLM_MID + 1;
    CFE_SB_MsgId_t  MsgId1 = SB_UT_TLM_MID + 2;
    CFE_SB_MsgId_t  MsgId2 = SB_UT_TLM_MID + 3;
    uint16          PipeDepth = 50;
    CFE_SB_SendPrevSubs_t SendPrevSubsMsg;

    START();

    /* note that the message is not currently used or required,
     * but creating one and initializing it for completeness, in
     * case that changes in the future */
    memset(&SendPrevSubsMsg, 0, sizeof(SendPrevSubsMsg));
    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId0, PipeDepth, "TestPipe0"));
    SETUP(CFE_SB_CreatePipe(&PipeId1, PipeDepth, "TestPipe1"));
    SETUP(CFE_SB_CreatePipe(&PipeId2, PipeDepth, "TestPipe2"));
    SETUP(CFE_SB_Subscribe(MsgId0, PipeId0));
    SETUP(CFE_SB_Subscribe(MsgId1, PipeId0));
    SETUP(CFE_SB_Subscribe(MsgId2, PipeId0));
    SETUP(CFE_SB_Subscribe(MsgId0, PipeId1));
    SETUP(CFE_SB_Subscribe(MsgId1, PipeId1));
    SETUP(CFE_SB_Subscribe(MsgId2, PipeId1));
    SETUP(CFE_SB_Subscribe(MsgId0, PipeId2));

    /* Set the last list header pointer to NULL to get branch path coverage */
    CFE_SB.RoutingTbl[2].ListHeadPtr = NULL;

    ASSERT(CFE_SB_SendPrevSubsCmd(&SendPrevSubsMsg));

    EVTCNT(19);

    EVTSENT(CFE_SB_PART_SUB_PKT_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId0));
    TEARDOWN(CFE_SB_DeletePipe(PipeId1));
    TEARDOWN(CFE_SB_DeletePipe(PipeId2));

    REPORT();
} /* end Test_Subscribe_SendPrevSubs */

/*
** Test function to get a count of the global message ids in use
*/
void Test_Subscribe_FindGlobalMsgIdCnt(void)
{
    CFE_SB_PipeId_t PipeId0;
    CFE_SB_PipeId_t PipeId1;
    CFE_SB_PipeId_t PipeId2;
    CFE_SB_MsgId_t  MsgId0 = SB_UT_TLM_MID + 1;
    CFE_SB_MsgId_t  MsgId1 = SB_UT_TLM_MID + 2;
    CFE_SB_MsgId_t  MsgId2 = SB_UT_TLM_MID + 3;
    uint16          PipeDepth = 50;
    uint16          MsgLim = 4;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId0, PipeDepth, "TestPipe0"));
    SETUP(CFE_SB_CreatePipe(&PipeId1, PipeDepth, "TestPipe1"));
    SETUP(CFE_SB_CreatePipe(&PipeId2, PipeDepth, "TestPipe2"));
    SETUP(CFE_SB_Subscribe(MsgId0, PipeId0));
    SETUP(CFE_SB_Subscribe(MsgId1, PipeId0));
    SETUP(CFE_SB_Subscribe(MsgId2, PipeId0));
    SETUP(CFE_SB_Subscribe(MsgId0, PipeId1));
    SETUP(CFE_SB_Subscribe(MsgId1, PipeId1));
    SETUP(CFE_SB_Subscribe(MsgId2, PipeId1));
    SETUP(CFE_SB_SubscribeLocal(MsgId0, PipeId2, MsgLim));

    /* Set the last list head pointer to NULL for branch path coverage */
    CFE_SB.RoutingTbl[2].ListHeadPtr = NULL;

    ASSERT_EQ(CFE_SB_FindGlobalMsgIdCnt(), 2); /* 2 unique msg ids; the third is set to skip */

    EVTCNT(17);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId0));
    TEARDOWN(CFE_SB_DeletePipe(PipeId1));
    TEARDOWN(CFE_SB_DeletePipe(PipeId2));

    REPORT();
} /* end Test_Subscribe_FindGlobalMsgIdCnt */

/*
** Test message subscription response to nonexistent pipe
*/
void Test_Subscribe_PipeNonexistent(void)
{
    CFE_SB_MsgId_t  MsgId = SB_UT_CMD_MID;
    CFE_SB_PipeId_t PipeId = 55;

    START();

    SB_ResetUnitTest();

    ASSERT_EQ(CFE_SB_Subscribe(MsgId, PipeId), CFE_SB_BAD_ARGUMENT);

    EVTCNT(2);

    EVTSENT(CFE_SB_SUB_INV_PIPE_EID);

    REPORT();
} /* end Test_Subscribe_PipeNonexistent */

/*
** Test enabling and disabling subscription reporting
*/
void Test_Subscribe_SubscriptionReporting(void)
{
    CFE_SB_PipeId_t PipeId;
    CFE_SB_MsgId_t  MsgId = SB_UT_TLM_MID;
    CFE_SB_Qos_t    Quality;
    uint16          PipeDepth = 10;

    START();

    SB_ResetUnitTest();

    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));

    /* Enable subscription reporting */
	CFE_SB_SetSubscriptionReporting(CFE_SB_ENABLE);

	/* Subscribe to message: GLOBAL */
	SETUP(CFE_SB_Subscribe(MsgId, PipeId));

	/* Unsubscribe so that a local subscription can be tested */
	SETUP(CFE_SB_Unsubscribe(MsgId, PipeId));

	/* Subscribe to message: LOCAL */
	ASSERT(CFE_SB_SubscribeFull(MsgId, PipeId, Quality, CFE_PLATFORM_SB_DEFAULT_MSG_LIMIT, CFE_SB_LOCAL));

	EVTCNT(8);

	EVTSENT(CFE_SB_SUBSCRIPTION_RPT_EID);

	/* Disable subscription reporting */
	CFE_SB_SetSubscriptionReporting(CFE_SB_DISABLE);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_Subscribe_SubscriptionReporting */

/*
** Test message subscription response to an invalid pipe owner
*/
void Test_Subscribe_InvalidPipeOwner(void)
{
    CFE_SB_PipeId_t PipeId;
    CFE_SB_MsgId_t  MsgId = SB_UT_TLM_MID;
    uint16          PipeDepth = 10;
    int32           RealOwner;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));

    /* Change owner of pipe through memory corruption */
    RealOwner = CFE_SB.PipeTbl[PipeId].AppId;

    /* Choose a value that is sure not to be owner */
    CFE_SB.PipeTbl[PipeId].AppId = RealOwner + 1;
    CFE_SB_Subscribe(MsgId, PipeId);

    EVTCNT(3);

    EVTSENT(CFE_SB_SUB_INV_CALLER_EID);

    /* Restore owner id and delete pipe since test is complete */
    CFE_SB.PipeTbl[PipeId].AppId = RealOwner;
    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_Subscribe_InvalidPipeOwner */

/*
** Function for calling SB unsubscribe API test functions
*/
void Test_Unsubscribe_API(void)
{
    STARTBLOCK();

    Test_Unsubscribe_Basic();
    Test_Unsubscribe_Local();
    Test_Unsubscribe_InvalParam();
    Test_Unsubscribe_NoMatch();
    Test_Unsubscribe_InvalidPipe();
    Test_Unsubscribe_InvalidPipeOwner();
    Test_Unsubscribe_FirstDestWithMany();
    Test_Unsubscribe_MiddleDestWithMany();
    Test_Unsubscribe_GetDestPtr();

    ENDBLOCK();
} /* end Test_Unsubscribe_API */

/*
** Test API used to unsubscribe to a message (successful)
*/
void Test_Unsubscribe_Basic(void)
{
    CFE_SB_PipeId_t TestPipe;
    CFE_SB_MsgId_t  MsgId = CFE_PLATFORM_SB_HIGHEST_VALID_MSGID / 2 + 1;
    uint16          PipeDepth = 50;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&TestPipe, PipeDepth, "TestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, TestPipe));

    ASSERT(CFE_SB_Unsubscribe(MsgId, TestPipe));

    EVTCNT(4);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    TEARDOWN(CFE_SB_DeletePipe(TestPipe));
    REPORT();
} /* end Test_Unsubscribe_Basic */

/*
** Test CFE internal API used to locally unsubscribe to a message (successful)
*/
void Test_Unsubscribe_Local(void)
{
    CFE_SB_PipeId_t TestPipe;
    CFE_SB_MsgId_t  MsgId = SB_UT_TLM_MID;
    uint16          PipeDepth = 50;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&TestPipe, PipeDepth, "TestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, TestPipe));

    ASSERT(CFE_SB_UnsubscribeLocal(CFE_PLATFORM_SB_HIGHEST_VALID_MSGID, TestPipe));

    EVTCNT(5);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    TEARDOWN(CFE_SB_DeletePipe(TestPipe));

    REPORT();
} /* end Test_Unsubscribe_Local */

/*
** Test message unsubscription response to an invalid message ID
*/
void Test_Unsubscribe_InvalParam(void)
{
    CFE_SB_PipeId_t TestPipe;
    uint32          CallerId = 0xFFFFFFFF;
    uint16          PipeDepth = 50;
    CFE_SB_PipeId_t SavedPipeId;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&TestPipe, PipeDepth, "TestPipe"));

    /* Perform test using a bad message ID */
    ASSERT_EQ(CFE_SB_Unsubscribe(CFE_PLATFORM_SB_HIGHEST_VALID_MSGID + 1, TestPipe), CFE_SB_BAD_ARGUMENT);

    /* Get the caller's Application ID */
    ASSERT(CFE_ES_GetAppID(&CallerId));

    /* Perform test using a bad scope value */
    ASSERT_EQ(CFE_SB_UnsubscribeFull(0, TestPipe, CFE_SB_LOCAL + 1, CallerId), CFE_SB_BAD_ARGUMENT);

    /* Perform test using an invalid pipe ID for branch path coverage.
     * This situation cannot happen in normal circumstances since the
     * bad pipe ID is caught by CFE_SB_GetPipeIdx() before it gets to
     * CFE_SB_ValidatePipeId()
     */
    SavedPipeId = CFE_SB.PipeTbl[0].PipeId;
    CFE_SB.PipeTbl[0].PipeId = CFE_PLATFORM_SB_MAX_PIPES;
    CFE_SB.PipeTbl[0].InUse = 1;
    ASSERT_EQ(CFE_SB_Unsubscribe(0, CFE_PLATFORM_SB_MAX_PIPES), CFE_SB_BAD_ARGUMENT);

    /* We must restore the old value so CFE_SB_DeletePipe() works */
    CFE_SB.PipeTbl[0].PipeId = SavedPipeId;

    EVTCNT(4);

    EVTSENT(CFE_SB_UNSUB_ARG_ERR_EID);

    TEARDOWN(CFE_SB_DeletePipe(TestPipe));

    REPORT();
} /* end Test_Unsubscribe_InvalParam */

/*
** Test message unsubscription response to a message ID that is not subscribed
*/
void Test_Unsubscribe_NoMatch(void)
{
    CFE_SB_PipeId_t TestPipe;
    CFE_SB_MsgId_t  MsgId = SB_UT_TLM_MID;
    CFE_SB_MsgRouteIdx_t Idx;
    uint16          PipeDepth = 50;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&TestPipe, PipeDepth, "TestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, TestPipe));

    ASSERT(CFE_SB_Unsubscribe(MsgId + 1, TestPipe));

    /* Get index into routing table */
    Idx = CFE_SB_GetRoutingTblIdx(CFE_SB_ConvertMsgIdtoMsgKey(MsgId));
    CFE_SB.RoutingTbl[CFE_SB_RouteIdxToValue(Idx)].ListHeadPtr->PipeId = 1;
    CFE_SB.RoutingTbl[CFE_SB_RouteIdxToValue(Idx)].ListHeadPtr->Next = NULL;
    ASSERT(CFE_SB_Unsubscribe(MsgId, TestPipe));

    EVTCNT(6);

    EVTSENT(CFE_SB_UNSUB_NO_SUBS_EID);

    TEARDOWN(CFE_SB_DeletePipe(TestPipe));

    REPORT();
} /* end Test_Unsubscribe_NoMatch */

/*
** Test message unsubscription response to an invalid pipe ID
*/
void Test_Unsubscribe_InvalidPipe(void)
{
    CFE_SB_PipeId_t TestPipe;
    CFE_SB_MsgId_t  MsgId = SB_UT_TLM_MID;
    uint16          PipeDepth = 50;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&TestPipe, PipeDepth, "TestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, TestPipe));

    ASSERT_EQ(CFE_SB_Unsubscribe(MsgId, TestPipe + 1), CFE_SB_BAD_ARGUMENT);

    EVTCNT(4);

    EVTSENT(CFE_SB_UNSUB_INV_PIPE_EID);

    TEARDOWN(CFE_SB_DeletePipe(TestPipe));

    REPORT();
} /* end Test_Unsubscribe_InvalidPipe */

/*
** Test message unsubscription response to an invalid pipe owner
*/
void Test_Unsubscribe_InvalidPipeOwner(void)
{
    CFE_SB_PipeId_t PipeId;
    CFE_SB_MsgId_t  MsgId = SB_UT_TLM_MID;
    uint32          RealOwner;
    uint16          PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));

    SETUP(CFE_SB_Subscribe(MsgId, PipeId));

    /* Change owner of pipe through memory corruption */
    RealOwner = CFE_SB.PipeTbl[PipeId].AppId;

    /* Choose a value that is sure not be owner */
    CFE_SB.PipeTbl[PipeId].AppId = RealOwner + 1;
    ASSERT_EQ(CFE_SB_Unsubscribe(MsgId, PipeId), CFE_SB_BAD_ARGUMENT);

    EVTCNT(4);

    EVTSENT(CFE_SB_UNSUB_INV_CALLER_EID);

    CFE_SB.PipeTbl[PipeId].AppId = RealOwner;

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_Unsubscribe_InvalidPipeOwner */

/*
** Test message unsubscription response to the first pipe destination when
** the message is subscribed to by multiple pipes
*/
void Test_Unsubscribe_FirstDestWithMany(void)
{
    CFE_SB_MsgId_t  MsgId = SB_UT_CMD_MID;
    CFE_SB_PipeId_t TestPipe1;
    CFE_SB_PipeId_t TestPipe2;
    CFE_SB_PipeId_t TestPipe3;
    uint16          PipeDepth = 50;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&TestPipe1, PipeDepth, "TestPipe1"));
    SETUP(CFE_SB_CreatePipe(&TestPipe2, PipeDepth, "TestPipe2"));
    SETUP(CFE_SB_CreatePipe(&TestPipe3, PipeDepth, "TestPipe3"));
    SETUP(CFE_SB_Subscribe(MsgId, TestPipe1));
    SETUP(CFE_SB_Subscribe(MsgId, TestPipe2));
    SETUP(CFE_SB_Subscribe(MsgId, TestPipe3));

    ASSERT(CFE_SB_Unsubscribe(MsgId, TestPipe1));

    EVTCNT(10);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    TEARDOWN(CFE_SB_DeletePipe(TestPipe1));
    TEARDOWN(CFE_SB_DeletePipe(TestPipe2));
    TEARDOWN(CFE_SB_DeletePipe(TestPipe3));

    REPORT();
} /* end Test_Unsubscribe_FirstDestWithMany */

/*
** Test message unsubscription response to a middle pipe destination when
** the message is subscribed to by multiple pipes
*/
void Test_Unsubscribe_MiddleDestWithMany(void)
{
    CFE_SB_MsgId_t  MsgId = SB_UT_CMD_MID;
    CFE_SB_PipeId_t TestPipe1;
    CFE_SB_PipeId_t TestPipe2;
    CFE_SB_PipeId_t TestPipe3;
    uint16          PipeDepth = 50;

    START();

    SB_ResetUnitTest();

    SETUP(CFE_SB_CreatePipe(&TestPipe1, PipeDepth, "TestPipe1"));
    SETUP(CFE_SB_CreatePipe(&TestPipe2, PipeDepth, "TestPipe2"));
    SETUP(CFE_SB_CreatePipe(&TestPipe3, PipeDepth, "TestPipe3"));
    SETUP(CFE_SB_Subscribe(MsgId, TestPipe1));
    SETUP(CFE_SB_Subscribe(MsgId, TestPipe2));
    SETUP(CFE_SB_Subscribe(MsgId, TestPipe3));

    ASSERT(CFE_SB_Unsubscribe(MsgId, TestPipe2));

    EVTCNT(10);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    TEARDOWN(CFE_SB_DeletePipe(TestPipe1));
    TEARDOWN(CFE_SB_DeletePipe(TestPipe2));
    TEARDOWN(CFE_SB_DeletePipe(TestPipe3));

    REPORT();
} /* end Test_Unsubscribe_MiddleDestWithMany */

/*
** Test message unsubscription by verifying the message destination pointer no
** longer points to the pipe
*/
void Test_Unsubscribe_GetDestPtr(void)
{
    CFE_SB_MsgId_t  MsgId = SB_UT_CMD_MID;
    CFE_SB_PipeId_t TestPipe1;
    CFE_SB_PipeId_t TestPipe2;
    uint16          PipeDepth = 50;

    START();

    SB_ResetUnitTest();

    SETUP(CFE_SB_CreatePipe(&TestPipe1, PipeDepth, "TestPipe1"));
    SETUP(CFE_SB_CreatePipe(&TestPipe2, PipeDepth, "TestPipe2"));
    SETUP(CFE_SB_Subscribe(MsgId, TestPipe1));
    SETUP(CFE_SB_Subscribe(MsgId, TestPipe2));
    SETUP(CFE_SB_Unsubscribe(MsgId, TestPipe2));

    ASSERT_TRUE(CFE_SB_GetDestPtr(CFE_SB_ConvertMsgIdtoMsgKey(MsgId), TestPipe2) == NULL);

    EVTCNT(7);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    TEARDOWN(CFE_SB_DeletePipe(TestPipe1));
    TEARDOWN(CFE_SB_DeletePipe(TestPipe2));

    REPORT();
} /* end Test_Unsubscribe_GetDestPtr */

/*
** Function for calling SB send message API test functions
*/
void Test_SendMsg_API(void)
{
    STARTBLOCK();

    Test_SendMsg_NullPtr();
    Test_SendMsg_InvalidMsgId();
    Test_SendMsg_NoSubscribers();
    Test_SendMsg_MaxMsgSizePlusOne();
    Test_SendMsg_BasicSend();
    Test_SendMsg_SequenceCount();
    Test_SendMsg_QueuePutError();
    Test_SendMsg_PipeFull();
    Test_SendMsg_MsgLimitExceeded();
    Test_SendMsg_GetPoolBufErr();
    Test_SendMsg_ZeroCopyGetPtr();
    Test_SendMsg_ZeroCopySend();
    Test_SendMsg_ZeroCopyPass();
    Test_SendMsg_ZeroCopyReleasePtr();
    Test_SendMsg_DisabledDestination();
    Test_SendMsg_SendWithMetadata();
    Test_SendMsg_InvalidMsgId_ZeroCopy();
    Test_SendMsg_MaxMsgSizePlusOne_ZeroCopy();
    Test_SendMsg_NoSubscribers_ZeroCopy();

    ENDBLOCK();
} /* end Test_SendMsg_API */

/*
** Test response to sending a null message on the software bus
*/
void Test_SendMsg_NullPtr(void)
{
    START();

    SB_ResetUnitTest();
    ASSERT_EQ(CFE_SB_SendMsg(NULL), CFE_SB_BAD_ARGUMENT);

    EVTCNT(1);

    EVTSENT(CFE_SB_SEND_BAD_ARG_EID);

    REPORT();
} /* end Test_SendMsg_NullPtr */

/*
** Test response to sending a message with an invalid ID
*/
void Test_SendMsg_InvalidMsgId(void)
{
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t) &TlmPkt;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&TlmPkt, CFE_PLATFORM_SB_HIGHEST_VALID_MSGID + 1,
                   sizeof(TlmPkt), true);

    CFE_SB_SetMsgId(TlmPktPtr, 0xFFFF);


    CCSDS_WR_APID(TlmPktPtr->Hdr, 0x7FF );

#ifdef MESSAGE_FORMAT_IS_CCSDS_VER_2

    CCSDS_WR_SUBSYSTEM_ID(TlmPktPtr->SpacePacket.ApidQ, 0x7E );

#endif

    ASSERT_EQ(CFE_SB_SendMsg(TlmPktPtr), CFE_SB_BAD_ARGUMENT);

    EVTCNT(1);

    EVTSENT(CFE_SB_SEND_INV_MSGID_EID);

    REPORT();
} /* end Test_SendMsg_InvalidMsgId */

/*
** Test response to sending a message which has no subscribers
*/
void Test_SendMsg_NoSubscribers(void)
{
    CFE_SB_MsgId_t   MsgId = SB_UT_TLM_MID;
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t) &TlmPkt;

    START();

    SB_ResetUnitTest();

    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), true);
    ASSERT(CFE_SB_SendMsg(TlmPktPtr));

    EVTCNT(1);

    EVTSENT(CFE_SB_SEND_NO_SUBS_EID);

    REPORT();
} /* end Test_SendMsg_NoSubscribers */

/*
** Test response to sending a message with the message size larger than allowed
*/
void Test_SendMsg_MaxMsgSizePlusOne(void)
{
    CFE_SB_MsgId_t   MsgId = SB_UT_TLM_MID;
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t) &TlmPkt;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&TlmPkt, MsgId, CFE_MISSION_SB_MAX_SB_MSG_SIZE + 1, false);

    ASSERT_EQ(CFE_SB_SendMsg(TlmPktPtr), CFE_SB_MSG_TOO_BIG);

    EVTCNT(1);

    EVTSENT(CFE_SB_MSG_TOO_BIG_EID);

    REPORT();
} /* end Test_SendMsg_MaxMsgSizePlusOne */

/*
** Test successfully sending a message on the software bus
*/
void Test_SendMsg_BasicSend(void)
{
    CFE_SB_PipeId_t  PipeId;
    CFE_SB_MsgId_t   MsgId = SB_UT_TLM_MID;
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t) &TlmPkt;
    int32            PipeDepth = 2;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));
    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), true);

    ASSERT(CFE_SB_SendMsg(TlmPktPtr));

    EVTCNT(3);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SendMsg_BasicSend */

/*
** Test successful send/receive for packet sequence count
*/
void Test_SendMsg_SequenceCount(void)
{
    CFE_SB_PipeId_t  PipeId;
    CFE_SB_MsgId_t   MsgId = SB_UT_TLM_MID;
    CFE_SB_MsgPtr_t  PtrToMsg;
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t) &TlmPkt;
    uint32           PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "SeqCntTestPipe"));
    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), true);
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));
    CCSDS_WR_SEQ(TlmPktPtr->Hdr, 22);
    SETUP(CFE_SB_SendMsg(TlmPktPtr));

    ASSERT(CFE_SB_RcvMsg(&PtrToMsg, PipeId, CFE_SB_PEND_FOREVER));

    ASSERT_TRUE(PtrToMsg != NULL);

    ASSERT_TRUE(CCSDS_RD_SEQ(PtrToMsg->Hdr) == 1);

    ASSERT(CFE_SB_PassMsg(TlmPktPtr));

    ASSERT(CFE_SB_RcvMsg(&PtrToMsg, PipeId, CFE_SB_PEND_FOREVER));

    ASSERT_TRUE(PtrToMsg != NULL);

    ASSERT_TRUE(CCSDS_RD_SEQ(PtrToMsg->Hdr) == 22);

    ASSERT(CFE_SB_SendMsg(TlmPktPtr));

    ASSERT(CFE_SB_RcvMsg(&PtrToMsg, PipeId, CFE_SB_PEND_FOREVER));

    ASSERT_TRUE(PtrToMsg != NULL);

    ASSERT_TRUE(CCSDS_RD_SEQ(PtrToMsg->Hdr) == 2);

    EVTCNT(3);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    SETUP(CFE_SB_Unsubscribe(MsgId, PipeId)); /* should have no subscribers now */

    SETUP(CFE_SB_SendMsg(TlmPktPtr)); /* increment to 3 */

    SETUP(CFE_SB_Subscribe(MsgId, PipeId)); /* resubscribe so we can receive a msg */

    SETUP(CFE_SB_SendMsg(TlmPktPtr)); /* increment to 4 */

    SETUP(CFE_SB_RcvMsg(&PtrToMsg, PipeId, CFE_SB_PEND_FOREVER));

    ASSERT_EQ(CCSDS_RD_SEQ(PtrToMsg->Hdr), 4);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SendMsg_SequenceCount */

/*
** Test send message response to a socket queue 'put' error
*/
void Test_SendMsg_QueuePutError(void)
{
    CFE_SB_PipeId_t  PipeId4Error;
    CFE_SB_MsgId_t   MsgId = SB_UT_TLM_MID;
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t) &TlmPkt;
    int32            PipeDepth = 2;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId4Error, PipeDepth, "TestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId4Error));
    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), false);
    UT_SetDeferredRetcode(UT_KEY(OS_QueuePut), 1, OS_ERROR);

    ASSERT(CFE_SB_SendMsg(TlmPktPtr));


    EVTCNT(5);

    EVTSENT(CFE_SB_Q_WR_ERR_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId4Error));

    REPORT();
} /* end Test_SendMsg_QueuePutError */

/*
** Test send message response when the socket queue is full
*/
void Test_SendMsg_PipeFull(void)
{
    CFE_SB_PipeId_t  PipeId;
    CFE_SB_MsgId_t   MsgId = SB_UT_TLM_MID;
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t) &TlmPkt;
    int32            PipeDepth = 1;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), true);
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "PipeFullTestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));

    /* This send should pass */
    ASSERT(CFE_SB_SendMsg(TlmPktPtr));

    /* Tell the QueuePut stub to return OS_QUEUE_FULL on its next call */
    UT_SetDeferredRetcode(UT_KEY(OS_QueuePut), 1, OS_QUEUE_FULL);

    /* Pipe overflow causes SendMsg to return CFE_SUCCESS */
    ASSERT(CFE_SB_SendMsg(TlmPktPtr));

    EVTCNT(5);

    EVTSENT(CFE_SB_Q_FULL_ERR_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));
    REPORT();
} /* end Test_SendMsg_PipeFull */

/*
** Test send message response to too many messages sent to the pipe
*/
void Test_SendMsg_MsgLimitExceeded(void)
{
    CFE_SB_PipeId_t  PipeId;
    CFE_SB_MsgId_t   MsgId = SB_UT_TLM_MID;
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t) &TlmPkt;
    int32            PipeDepth = 5;

    START();

    SB_ResetUnitTest();
    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), false);
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "MsgLimTestPipe"));

    /* Set maximum allowed messages on the pipe at one time to 1 */
    CFE_SB_SubscribeEx(MsgId, PipeId, CFE_SB_Default_Qos, 1);

    /* First send should pass */
    ASSERT(CFE_SB_SendMsg(TlmPktPtr));

    /* This send should produce a MsgId to Pipe Limit Exceeded message, but
     * return success
     */
    ASSERT(CFE_SB_SendMsg(TlmPktPtr));

    EVTCNT(5);

    EVTSENT(CFE_SB_MSGID_LIM_ERR_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SendMsg_MsgLimitExceeded */

/*
** Test send message response to a buffer descriptor allocation failure
*/
void Test_SendMsg_GetPoolBufErr(void)
{
    CFE_SB_PipeId_t  PipeId;
    CFE_SB_MsgId_t   MsgId = SB_UT_TLM_MID;
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t) &TlmPkt;
    int32            PipeDepth;

    START();

    SB_ResetUnitTest();
    PipeDepth = 1;
    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), true);
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "GetPoolErrPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));

    /* Have GetPoolBuf stub return error on its next call (buf descriptor
     * allocation failed)
     */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetPoolBuf), 1, CFE_ES_ERR_MEM_BLOCK_SIZE);
    ASSERT_EQ(CFE_SB_SendMsg(TlmPktPtr), CFE_SB_BUF_ALOC_ERR);

    EVTCNT(4);

    EVTSENT(CFE_SB_GET_BUF_ERR_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SendMsg_GetPoolBufErr */

/*
** Test getting a pointer to a buffer for zero copy mode with buffer
** allocation failures
*/
void Test_SendMsg_ZeroCopyGetPtr(void)
{
    CFE_SB_ZeroCopyHandle_t ZeroCpyBufHndl;
    uint16                  MsgSize = 10;

    START();

    SB_ResetUnitTest();

    /* Have GetPoolBuf stub return error on its next call (buf descriptor
     * allocation failed)
     */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetPoolBuf), 1, CFE_ES_ERR_MEM_BLOCK_SIZE);
    ASSERT_TRUE((cpuaddr) CFE_SB_ZeroCopyGetPtr(MsgSize, &ZeroCpyBufHndl) == (cpuaddr) NULL);

    /* Have GetPoolBuf stub return error on its second call (actual buffer
     * allocation failed)
     */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetPoolBuf), 2, CFE_ES_ERR_MEM_BLOCK_SIZE);
    ASSERT_TRUE((cpuaddr) CFE_SB_ZeroCopyGetPtr(MsgSize, &ZeroCpyBufHndl) == (cpuaddr) NULL);

    /* Have GetPoolBuf stub return error on its second call (null buffer
     * returned and error returning the memory to the buffer)
     */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetPoolBuf), 2, -1);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_PutPoolBuf), 1, 0);
    ASSERT_TRUE((cpuaddr) CFE_SB_ZeroCopyGetPtr(MsgSize, &ZeroCpyBufHndl) == (cpuaddr) NULL);

    EVTCNT(0);

    /* Increase the peak memory and buffers in use above the expected values in
     * order to exercise branch paths
     */
    CFE_SB.StatTlmMsg.Payload.PeakMemInUse = (MsgSize +
                                              sizeof(CFE_SB_BufferD_t)) * 5;
    CFE_SB.StatTlmMsg.Payload.PeakSBBuffersInUse =
      CFE_SB.StatTlmMsg.Payload.SBBuffersInUse + 2;
    ASSERT_TRUE((cpuaddr) CFE_SB_ZeroCopyGetPtr(MsgSize, &ZeroCpyBufHndl) != (cpuaddr) NULL);

    ASSERT_TRUE(CFE_SB.StatTlmMsg.Payload.PeakMemInUse == (MsgSize + sizeof(CFE_SB_BufferD_t)) * 5);

    ASSERT_TRUE(CFE_SB.StatTlmMsg.Payload.PeakSBBuffersInUse == CFE_SB.StatTlmMsg.Payload.SBBuffersInUse + 1);

    EVTCNT(0);

    REPORT();
} /* end Test_SendMsg_ZeroCopyGetPtr */

/*
** Test successfully sending a message in zero copy mode (telemetry source
** sequence count increments)
*/
void Test_SendMsg_ZeroCopySend(void)
{
    CFE_SB_MsgPtr_t         PtrToMsg;
    CFE_SB_PipeId_t         PipeId;
    CFE_SB_MsgId_t          MsgId = SB_UT_TLM_MID;
    CFE_SB_MsgPtr_t         ZeroCpyMsgPtr = NULL;
    uint32                  PipeDepth = 10;
    CFE_SB_ZeroCopyHandle_t ZeroCpyBufHndl = 0;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "ZeroCpyTestPipe"));

    SETUP(CFE_SB_Subscribe(MsgId, PipeId));

    if(ASSERT_TRUE((ZeroCpyMsgPtr = CFE_SB_ZeroCopyGetPtr(sizeof(SB_UT_Test_Tlm_t),
                                          &ZeroCpyBufHndl)) != NULL))
    {
        CFE_SB_InitMsg(ZeroCpyMsgPtr, MsgId, sizeof(SB_UT_Test_Tlm_t), true);
        CCSDS_WR_SEQ(ZeroCpyMsgPtr->Hdr, 22);
    }

    /* Test response to a get pool information error */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetPoolBufInfo), 1, -1);
    ASSERT_EQ(CFE_SB_ZeroCopySend(ZeroCpyMsgPtr, ZeroCpyBufHndl), CFE_SB_BUFFER_INVALID);

    /* Test a successful zero copy send */
    ASSERT(CFE_SB_ZeroCopySend(ZeroCpyMsgPtr, ZeroCpyBufHndl));

    ASSERT(CFE_SB_RcvMsg(&PtrToMsg, PipeId, CFE_SB_PEND_FOREVER));

    ASSERT_TRUE(PtrToMsg != NULL);

    ASSERT_EQ(CCSDS_RD_SEQ(PtrToMsg->Hdr), 1);

    EVTCNT(3);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SendMsg_ZeroCopySend */

/*
** Test successfully sending a message in zero copy mode (telemetry source
** sequence count is unchanged)
*/
void Test_SendMsg_ZeroCopyPass(void)
{
    CFE_SB_MsgPtr_t PtrToMsg;
    CFE_SB_PipeId_t PipeId;
    CFE_SB_MsgId_t          MsgId = SB_UT_TLM_MID;
    CFE_SB_MsgPtr_t         ZeroCpyMsgPtr = NULL;
    uint32                  PipeDepth = 10;
    CFE_SB_ZeroCopyHandle_t ZeroCpyBufHndl = 0;
    uint16                  Seq = 22;

    START();

    SB_ResetUnitTest();
    
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "ZeroCpyPassTestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));

    ZeroCpyMsgPtr = CFE_SB_ZeroCopyGetPtr(sizeof(SB_UT_Test_Tlm_t),
                                          &ZeroCpyBufHndl);

    if (ZeroCpyMsgPtr == NULL)
    {
        UT_Text("Unexpected NULL pointer returned from ZeroCopyGetPtr");
        TestStat = CFE_FAIL;
    }
    else
    {
      CFE_SB_InitMsg(ZeroCpyMsgPtr, MsgId, sizeof(SB_UT_Test_Tlm_t), true);
      CCSDS_WR_SEQ(ZeroCpyMsgPtr->Hdr, Seq);
    }

    /* Test response to a get pool information error */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetPoolBufInfo), 1, -1);
    ASSERT_EQ(CFE_SB_ZeroCopyPass(ZeroCpyMsgPtr, ZeroCpyBufHndl), CFE_SB_BUFFER_INVALID);


    /* Test a successful zero copy pass */
    ASSERT(CFE_SB_ZeroCopyPass(ZeroCpyMsgPtr, ZeroCpyBufHndl));


    ASSERT(CFE_SB_RcvMsg(&PtrToMsg, PipeId, CFE_SB_PEND_FOREVER));


    if (PtrToMsg == NULL)
    {
        UT_Text("Unexpected NULL return from receive in zero copy pass test");
        TestStat = CFE_FAIL;
    }
    else if (CCSDS_RD_SEQ(PtrToMsg->Hdr) != Seq)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "Unexpected sequence count for send in sequence count test, "
                   "exp=%d, act=%d",
                 Seq, CCSDS_RD_SEQ(PtrToMsg->Hdr));
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
    }

    EVTCNT(3);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SendMsg_ZeroCopyPass */

/*
** Test releasing a pointer to a buffer for zero copy mode
*/
void Test_SendMsg_ZeroCopyReleasePtr(void)
{
    CFE_SB_MsgPtr_t         ZeroCpyMsgPtr1 = NULL;
    CFE_SB_MsgPtr_t         ZeroCpyMsgPtr2 = NULL;
    CFE_SB_MsgPtr_t         ZeroCpyMsgPtr3 = NULL;
    CFE_SB_ZeroCopyHandle_t ZeroCpyBufHndl1 = 0;
    CFE_SB_ZeroCopyHandle_t ZeroCpyBufHndl2 = 0;
    CFE_SB_ZeroCopyHandle_t ZeroCpyBufHndl3 = 0;
    uint16                  MsgSize = 10;

    START();

    SB_ResetUnitTest();
    ZeroCpyMsgPtr1 = CFE_SB_ZeroCopyGetPtr(MsgSize, &ZeroCpyBufHndl1);
    ZeroCpyMsgPtr2 = CFE_SB_ZeroCopyGetPtr(MsgSize, &ZeroCpyBufHndl2);
    ZeroCpyMsgPtr3 = CFE_SB_ZeroCopyGetPtr(MsgSize, &ZeroCpyBufHndl3);
    SETUP(CFE_SB_ZeroCopyReleasePtr(ZeroCpyMsgPtr2, ZeroCpyBufHndl2));

    /* Test response to an invalid buffer */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetPoolBufInfo), 1, -1);
    ASSERT_EQ(CFE_SB_ZeroCopyReleasePtr(ZeroCpyMsgPtr2, ZeroCpyBufHndl2), CFE_SB_BUFFER_INVALID);

    /* Test response to a null message pointer */
    ASSERT_EQ(CFE_SB_ZeroCopyReleasePtr(NULL, ZeroCpyBufHndl2), CFE_SB_BUFFER_INVALID);

    /* Test response to an invalid message pointer */
    ASSERT_EQ(CFE_SB_ZeroCopyReleasePtr((CFE_SB_Msg_t *) 0x1234,
                                       ZeroCpyBufHndl2), CFE_SB_BUFFER_INVALID);

    /* Test path when return the descriptor to the pool fails in
     * CFE_SB_ZeroCopyReleaseDesc
     */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_PutPoolBuf), 1, -1);
    ASSERT(CFE_SB_ZeroCopyReleasePtr(ZeroCpyMsgPtr2, ZeroCpyBufHndl2));

    /* Test path when return the buffer to the pool fails in
     * CFE_SB_ZeroCopyReleasePtr
     */
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_PutPoolBuf), 2, -1);
    ASSERT(CFE_SB_ZeroCopyReleasePtr(ZeroCpyMsgPtr2, ZeroCpyBufHndl2));

    /* Test successful release of the second buffer */
    ASSERT(CFE_SB_ZeroCopyReleasePtr(ZeroCpyMsgPtr3, ZeroCpyBufHndl3));

    /* Test successful release of the third buffer */
    ASSERT(CFE_SB_ZeroCopyReleasePtr(ZeroCpyMsgPtr1, ZeroCpyBufHndl1));

    EVTCNT(0);

    REPORT();
} /* end Test_SendMsg_ZeroCopyReleasePtr */

/*
** Test send message response with the destination disabled
*/
void Test_SendMsg_DisabledDestination(void)
{
    CFE_SB_PipeId_t       PipeId;
    CFE_SB_MsgId_t        MsgId = SB_UT_TLM_MID;
    SB_UT_Test_Tlm_t      TlmPkt;
    CFE_SB_MsgPtr_t       TlmPktPtr = (CFE_SB_MsgPtr_t) &TlmPkt;
    CFE_SB_DestinationD_t *DestPtr;
    int32                 PipeDepth;

    START();

    SB_ResetUnitTest();
    PipeDepth = 2;

    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));
    DestPtr = CFE_SB_GetDestPtr(CFE_SB_ConvertMsgIdtoMsgKey(MsgId), PipeId);
    DestPtr->Active = CFE_SB_INACTIVE;
    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), true);

    ASSERT(CFE_SB_SendMsg(TlmPktPtr));

    EVTCNT(3);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SendMsg_DisabledDestination */

/*
** Test successfully sending a message with the metadata
*/
void Test_SendMsg_SendWithMetadata(void)
{
    CFE_SB_PipeId_t PipeId;
    CFE_SB_MsgId_t  MsgId = SB_UT_TLM_MID;

    struct
    {
        /* To exercise the CFE_SB_SEND_ZEROCOPY feature,
         * the packet data must already be enclosed in a
         * buffer descriptor
         */
        CFE_SB_BufferD_t  BufDesc;
        SB_UT_Test_Tlm_t  TlmPkt;
    } TlmPktBufDesc;

    CFE_SB_MsgPtr_t TlmPktPtr = (CFE_SB_MsgPtr_t) &TlmPktBufDesc.TlmPkt;
    int32           PipeDepth;

    START();

    memset(&TlmPktBufDesc, 0, sizeof(TlmPktBufDesc));
    SB_ResetUnitTest();
    PipeDepth = 2;
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));
    CFE_SB_InitMsg(&TlmPktBufDesc.TlmPkt, MsgId,
                   sizeof(TlmPktBufDesc.TlmPkt), true);

    ASSERT(CFE_SB_SendMsgFull(TlmPktPtr, CFE_SB_DO_NOT_INCREMENT,
                                CFE_SB_SEND_ZEROCOPY));

    EVTCNT(3);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SendMsg_SendWithMetadata */

/*
** Test response to sending a message with an invalid ID and ZeroCopy is set
*/
void Test_SendMsg_InvalidMsgId_ZeroCopy(void)
{
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr;

    START();

    SB_ResetUnitTest();
    TlmPktPtr = CFE_SB_ZeroCopyGetPtr(sizeof(SB_UT_Test_Tlm_t),
                                      (CFE_SB_ZeroCopyHandle_t *) &TlmPkt);
    if (TlmPktPtr == NULL)
    {
        UT_Text("Unexpected NULL pointer returned from ZeroCopyGetPtr");
        TestStat = CFE_FAIL;
    }
    else
    {
        CFE_SB_InitMsg(TlmPktPtr, CFE_PLATFORM_SB_HIGHEST_VALID_MSGID + 1,
        		       sizeof(SB_UT_Test_Tlm_t), true);
    ASSERT_EQ(CFE_SB_SendMsgFull(TlmPktPtr, CFE_SB_INCREMENT_TLM,
                                    CFE_SB_SEND_ZEROCOPY), CFE_SB_BAD_ARGUMENT);

    }

    EVTCNT(1);

    EVTSENT(CFE_SB_SEND_INV_MSGID_EID);

    REPORT();
} /* end Test_SendMsg_InvalidMsgId_ZeroCopy */

/*
** Test response to sending a message with the message size larger than allowed
** and ZeroCopy is set
*/
void Test_SendMsg_MaxMsgSizePlusOne_ZeroCopy(void)
{
    CFE_SB_MsgId_t   MsgId = SB_UT_TLM_MID;
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr;

    START();

    SB_ResetUnitTest();
    TlmPktPtr = CFE_SB_ZeroCopyGetPtr(sizeof(SB_UT_Test_Tlm_t),
    		                          (CFE_SB_ZeroCopyHandle_t *) &TlmPkt);
    if (TlmPktPtr == NULL)
    {
        UT_Text("Unexpected NULL pointer returned from ZeroCopyGetPtr");
        TestStat = CFE_FAIL;
    }
    else
    {
        CFE_SB_InitMsg(TlmPktPtr, MsgId, CFE_MISSION_SB_MAX_SB_MSG_SIZE + 1, false);
    ASSERT_EQ(CFE_SB_SendMsgFull(TlmPktPtr, CFE_SB_INCREMENT_TLM,
                                    CFE_SB_SEND_ZEROCOPY), CFE_SB_MSG_TOO_BIG);

    }

    EVTCNT(1);

    EVTSENT(CFE_SB_MSG_TOO_BIG_EID);

    REPORT();
} /* end Test_SendMsg_MaxMsgSizePlusOne_ZeroCopy */

/*
** Test response to sending a message which has no subscribers and ZeroCopy is
** set
*/
void Test_SendMsg_NoSubscribers_ZeroCopy(void)
{
    CFE_SB_MsgId_t   MsgId = SB_UT_TLM_MID;
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr;

    START();

    SB_ResetUnitTest();
    TlmPktPtr = CFE_SB_ZeroCopyGetPtr(sizeof(SB_UT_Test_Tlm_t),
    		                          (CFE_SB_ZeroCopyHandle_t *) &TlmPkt);
    if (TlmPktPtr == NULL)
    {
        UT_Text("Unexpected NULL pointer returned from ZeroCopyGetPtr");
        TestStat = CFE_FAIL;
    }
    else
    {
        CFE_SB_InitMsg(TlmPktPtr, MsgId, sizeof(SB_UT_Test_Tlm_t), true);
        ASSERT(CFE_SB_SendMsgFull(TlmPktPtr, CFE_SB_INCREMENT_TLM,
                                    CFE_SB_SEND_ZEROCOPY));
    }

    EVTCNT(1);

    EVTSENT(CFE_SB_SEND_NO_SUBS_EID);

    REPORT();
} /* end Test_SendMsg_NoSubscribers_ZeroCopy */

/*
** Function for calling SB receive message API test functions
*/
void Test_RcvMsg_API(void)
{
    STARTBLOCK();

    Test_RcvMsg_InvalidPipeId();
    Test_RcvMsg_InvalidTimeout();
    Test_RcvMsg_Poll();
    Test_RcvMsg_GetLastSenderNull();
    Test_RcvMsg_GetLastSenderInvalidPipe();
    Test_RcvMsg_GetLastSenderInvalidCaller();
    Test_RcvMsg_GetLastSenderSuccess();
    Test_RcvMsg_Timeout();
    Test_RcvMsg_PipeReadError();
    Test_RcvMsg_PendForever();
    Test_RcvMsg_InvalidBufferPtr();

    ENDBLOCK();
} /* end Test_RcvMsg_API */

/*
** Test receiving a message from the software bus with an invalid pipe ID
*/
void Test_RcvMsg_InvalidPipeId(void)
{
    CFE_SB_MsgPtr_t PtrToMsg;
    CFE_SB_PipeId_t InvalidPipeId = 20;

    START();

    SB_ResetUnitTest();
    CFE_SB.PipeTbl[InvalidPipeId].InUse = CFE_SB_NOT_IN_USE;

    ASSERT_EQ(CFE_SB_RcvMsg(&PtrToMsg, InvalidPipeId, CFE_SB_POLL), CFE_SB_BAD_ARGUMENT);


    EVTCNT(1);

    EVTSENT(CFE_SB_BAD_PIPEID_EID);

    REPORT();
} /* end Test_RcvMsg_InvalidPipeId */

/*
** Test receiving a message response to invalid timeout value (< -1)
*/
void Test_RcvMsg_InvalidTimeout(void)
{
    CFE_SB_MsgPtr_t PtrToMsg;
    CFE_SB_PipeId_t PipeId;
    uint32          PipeDepth = 10;
    int32           TimeOut = -5;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "RcvMsgTestPipe"));

    ASSERT_EQ(CFE_SB_RcvMsg(&PtrToMsg, PipeId, TimeOut), CFE_SB_BAD_ARGUMENT);

    EVTCNT(2);

    EVTSENT(CFE_SB_RCV_BAD_ARG_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_RcvMsg_InvalidTimeout */

/*
** Test receiving a message response when there is no message on the queue
*/
void Test_RcvMsg_Poll(void)
{
    CFE_SB_MsgPtr_t PtrToMsg;
    CFE_SB_PipeId_t PipeId;
    uint32          PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "RcvMsgTestPipe"));

    ASSERT_EQ(CFE_SB_RcvMsg(&PtrToMsg, PipeId, CFE_SB_POLL), CFE_SB_NO_MESSAGE);

    EVTCNT(1);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_RcvMsg_Poll */

/*
** Test receive last message response to a null sender ID
*/
void Test_RcvMsg_GetLastSenderNull(void)
{
    CFE_SB_PipeId_t PipeId;
    uint32          PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "RcvMsgTestPipe"));

    ASSERT_EQ(CFE_SB_GetLastSenderId(NULL, PipeId), CFE_SB_BAD_ARGUMENT);

    EVTCNT(2);

    EVTSENT(CFE_SB_LSTSNDER_ERR1_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_RcvMsg_GetLastSenderNull */

/*
** Test receive last message response to an invalid pipe ID
*/
void Test_RcvMsg_GetLastSenderInvalidPipe(void)
{
    CFE_SB_PipeId_t   PipeId;
    CFE_SB_PipeId_t   InvalidPipeId = 250;
    CFE_SB_SenderId_t *GLSPtr;
    uint32            PipeDepth = 10;

    START();

    SB_ResetUnitTest();

    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "RcvMsgTestPipe"));

    ASSERT_EQ(CFE_SB_GetLastSenderId(&GLSPtr, InvalidPipeId), CFE_SB_BAD_ARGUMENT);

    EVTCNT(2);

    EVTSENT(CFE_SB_LSTSNDER_ERR2_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_RcvMsg_GetLastSenderInvalidPipe */

/*
** Test receive last message response to an invalid owner ID
*/
void Test_RcvMsg_GetLastSenderInvalidCaller(void)
{
    CFE_SB_PipeId_t   PipeId;
    CFE_SB_SenderId_t *GLSPtr;
    uint32            PipeDepth = 10;
    uint32            OrigPipeOwner;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "RcvMsgTestPipe"));

    /* Change pipe owner ID to execute 'invalid caller' code */
    OrigPipeOwner = CFE_SB.PipeTbl[PipeId].AppId;
    CFE_SB.PipeTbl[PipeId].AppId = OrigPipeOwner + 1;
    ASSERT_EQ(CFE_SB_GetLastSenderId(&GLSPtr, PipeId), CFE_SB_BAD_ARGUMENT);

    EVTCNT(2);

    EVTSENT(CFE_SB_GLS_INV_CALLER_EID);

    /* Restore original pipe owner apid */
    CFE_SB.PipeTbl[PipeId].AppId = OrigPipeOwner;
    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_RcvMsg_GetLastSenderInvalidCaller */

/*
** Test successful receive last message request
*/
void Test_RcvMsg_GetLastSenderSuccess(void)
{
    CFE_SB_PipeId_t   PipeId;
    CFE_SB_SenderId_t *GLSPtr;
    uint32            PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "RcvMsgTestPipe"));

    ASSERT(CFE_SB_GetLastSenderId(&GLSPtr, PipeId));

    EVTCNT(1);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_RcvMsg_GetLastSenderSuccess */

/*
** Test receiving a message response to a timeout
*/
void Test_RcvMsg_Timeout(void)
{
    CFE_SB_MsgPtr_t PtrToMsg;
    CFE_SB_PipeId_t PipeId;
    uint32          PipeDepth = 10;
    int32           TimeOut = 200;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "RcvMsgTestPipe"));
    UT_SetDeferredRetcode(UT_KEY(OS_QueueGet), 1, OS_QUEUE_TIMEOUT);

    ASSERT_EQ(CFE_SB_RcvMsg(&PtrToMsg, PipeId, TimeOut), CFE_SB_TIME_OUT);

    EVTCNT(1);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_RcvMsg_Timeout */

/*
** Test receiving a message response to a pipe read error
*/
void Test_RcvMsg_PipeReadError(void)
{
    CFE_SB_MsgPtr_t PtrToMsg;
    CFE_SB_PipeId_t PipeId;
    uint32          PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "RcvMsgTestPipe"));
    UT_SetDeferredRetcode(UT_KEY(OS_QueueGet), 1, OS_ERROR);
    ASSERT_EQ(CFE_SB_RcvMsg(&PtrToMsg, PipeId, CFE_SB_PEND_FOREVER), CFE_SB_PIPE_RD_ERR);

    EVTCNT(3);

    EVTSENT(CFE_SB_Q_RD_ERR_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_RcvMsg_PipeReadError */

/*
** Test receiving a message response to a "pend forever" timeout
*/
void Test_RcvMsg_PendForever(void)
{
    CFE_SB_MsgPtr_t  PtrToMsg;
    CFE_SB_MsgId_t   MsgId = SB_UT_TLM_MID;
    CFE_SB_PipeId_t  PipeId;
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t) &TlmPkt;
    CFE_SB_PipeD_t   *PipeDscPtr;
    uint32           PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "RcvMsgTestPipe"));
    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), true);
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));
    SETUP(CFE_SB_SendMsg(TlmPktPtr));

    ASSERT(CFE_SB_RcvMsg(&PtrToMsg, PipeId, CFE_SB_PEND_FOREVER));

    ASSERT_TRUE(PtrToMsg != NULL);

    EVTCNT(3);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    PipeDscPtr = CFE_SB_GetPipePtr(PipeId);
    PipeDscPtr->ToTrashBuff = PipeDscPtr->CurrentBuff;
    PipeDscPtr->CurrentBuff = NULL;
    
    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_RcvMsg_PendForever */

/*
** Test releasing zero copy buffers for all pipes owned by a given app ID
*/
void Test_CleanupApp_API(void)
{
    CFE_SB_PipeId_t         PipeId;
    CFE_SB_ZeroCopyHandle_t ZeroCpyBufHndl = 0;
    uint16                  PipeDepth = 50;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));
    CFE_SB_ZeroCopyGetPtr(PipeDepth, &ZeroCpyBufHndl);
    CFE_SB_ZeroCopyGetPtr(PipeDepth, &ZeroCpyBufHndl);

    /* Set second application ID to provide complete branch path coverage */
    CFE_SB.PipeTbl[1].InUse = CFE_SB_IN_USE;
    CFE_SB.PipeTbl[1].AppId = 1;

    ASSERT_TRUE(CFE_SB.ZeroCopyTail != NULL);

    /* Attempt with a bad application ID first in order to get full branch path
     * coverage in CFE_SB_ZeroCopyReleaseAppId
     */
    CFE_SB_CleanUpApp(1);

    /* Attempt again with a valid application ID */
    CFE_SB_CleanUpApp(0);

    ASSERT_TRUE(CFE_SB.ZeroCopyTail == NULL);

    EVTCNT(3);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);

    EVTSENT(CFE_SB_PIPE_DELETED_EID);

    REPORT();
} /* end Test_CleanupApp_API */

/*
** Test receiving a message response to invalid buffer pointer (null)
*/
void Test_RcvMsg_InvalidBufferPtr(void)
{
    CFE_SB_PipeId_t PipeId;
    uint32          PipeDepth = 10;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "RcvMsgTestPipe"));

    ASSERT_EQ(CFE_SB_RcvMsg(NULL, PipeId, CFE_SB_PEND_FOREVER), CFE_SB_BAD_ARGUMENT);

    EVTCNT(2);

    EVTSENT(CFE_SB_RCV_BAD_ARG_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_RcvMsg_InvalidBufferPtr */

/*
** Test SB Utility APIs
*/
void Test_SB_Utils(void)
{
    STARTBLOCK();

    Test_CFE_SB_InitMsg();
    Test_CFE_SB_MsgHdrSize();
    Test_CFE_SB_GetUserData();
    Test_CFE_SB_SetGetMsgId();
    Test_CFE_SB_SetGetUserDataLength();
    Test_CFE_SB_SetGetTotalMsgLength();
    Test_CFE_SB_SetGetMsgTime();
    Test_CFE_SB_TimeStampMsg();
    Test_CFE_SB_SetGetCmdCode();
    Test_CFE_SB_ChecksumUtils();
    Test_CFE_SB_ValidateMsgId();

    ENDBLOCK();
} /* end Test_SB_Utils */

/*
**
*/
void Test_CFE_SB_InitMsg(void)
{
    STARTBLOCK();

    Test_CFE_SB_InitMsg_True();
    Test_CFE_SB_InitMsg_False();

    ENDBLOCK();
} /* end Test_CFE_SB_InitMsg */

/*
** Test message initialization, clearing the message content
*/
void Test_CFE_SB_InitMsg_True(void)
{
    SB_UT_Test_Cmd_t SBCmd;
    CFE_SB_MsgPtr_t  SBCmdPtr = (CFE_SB_MsgPtr_t) &SBCmd;

    START();

    SB_ResetUnitTest();

    /* Set entire cmd packet to all f's */
    memset(SBCmdPtr, 0xff, sizeof(SBCmd));
    CFE_SB_InitMsg(SBCmdPtr, CFE_SB_CMD_MID, sizeof(SBCmd), true);
    ASSERT_TRUE(SBCmd.Cmd32Param1 == 0 &&
             SBCmd.Cmd16Param1 == 0 &&
             SBCmd.Cmd16Param2 == 0 &&
             SBCmd.Cmd8Param1 == 0 &&
             SBCmd.Cmd8Param2 == 0 &&
             SBCmd.Cmd8Param3 == 0 &&
             SBCmd.Cmd8Param4 == 0);

    REPORT();
} /* end Test_CFE_SB_InitMsg_True */

/*
** Test message initialization, leaving the message content unchanged
*/
void Test_CFE_SB_InitMsg_False(void)
{
    SB_UT_Test_Cmd_t SBCmd;
    CFE_SB_MsgPtr_t  SBCmdPtr = (CFE_SB_MsgPtr_t) &SBCmd;

    START();

    SB_ResetUnitTest();

    /* Set entire cmd packet to all f's */
    memset(SBCmdPtr, 0xff, sizeof(SBCmd));
    ASSERT_TRUE(SBCmd.Cmd32Param1 == 0xffffffff &&
             SBCmd.Cmd16Param1 == 0xffff &&
             SBCmd.Cmd16Param2 == 0xffff &&
             SBCmd.Cmd8Param1 == 0xff &&
             SBCmd.Cmd8Param2 == 0xff &&
             SBCmd.Cmd8Param3 == 0xff &&
             SBCmd.Cmd8Param4 == 0xff);

    CFE_SB_InitMsg(SBCmdPtr, CFE_SB_CMD_MID, sizeof(SBCmd), false);

    REPORT();
} /* end Test_CFE_SB_InitMsg_False */

/*
** Test getting the size of a command/telemetry message header
*/
void Test_CFE_SB_MsgHdrSize(void)
{
    CCSDS_PriHdr_t   * PktPtr;
    CFE_SB_MsgId_t msgId;
    SB_UT_Test_Cmd_t testCmd;
    SB_UT_Test_Tlm_t testTlm;

    START();

    CFE_SB_MsgPtr_t MsgPtr = (CFE_SB_MsgPtr_t)&testCmd;

    PktPtr = (CCSDS_PriHdr_t*)MsgPtr;

    /* Test for cmds w/sec hdr */
    SB_ResetUnitTest();

    msgId = SB_UT_CMD_MID;

    CFE_SB_InitMsg(MsgPtr,
                    msgId,
                    sizeof(testCmd),
                    0);


    /* Set this to Command Type */
    CCSDS_WR_TYPE(*PktPtr, 1); 
    /* No sec hdr */
    CCSDS_WR_SHDR(*PktPtr, 1);

    ASSERT_EQ(CFE_SB_MsgHdrSize(MsgPtr), sizeof(CFE_SB_CmdHdr_t));

    REPORT();

    /* Test for cmds wo/sec hdr */

    START();
    SB_ResetUnitTest();

    CFE_SB_InitMsg(MsgPtr,
                    SB_UT_TLM_MID,
                    sizeof(testCmd),
                    0);

    /* Set this to Command Type */
    CCSDS_WR_TYPE(*PktPtr, 1); 
    /* No sec hdr */
    CCSDS_WR_SHDR(*PktPtr, 0);

    ASSERT_EQ(CFE_SB_MsgHdrSize(MsgPtr), sizeof(CCSDS_PriHdr_t));

    REPORT();

    START();

    /* Test for tlm w/sec hdr */
    SB_ResetUnitTest();

    MsgPtr = (CFE_SB_MsgPtr_t)&testTlm;
    PktPtr = (CCSDS_PriHdr_t*)MsgPtr;

    CFE_SB_SetMsgId(MsgPtr, SB_UT_TLM_MID);

    /* Set this to Tlm Type */
    CCSDS_WR_TYPE(*PktPtr, 0);
    CCSDS_WR_SHDR(*PktPtr, 1);

    ASSERT_EQ(CFE_SB_MsgHdrSize(MsgPtr), sizeof(CFE_SB_TlmHdr_t));

    REPORT();

    START();
    /* Test for tlm wo/sec hdr */
    SB_ResetUnitTest();

    CFE_SB_SetMsgId(MsgPtr, SB_UT_TLM_MID);

    /* Set this to Telemetry Type */
    CCSDS_WR_TYPE(*PktPtr, 0); 
    CCSDS_WR_SHDR(*PktPtr, 0);

    ASSERT_EQ(CFE_SB_MsgHdrSize(MsgPtr), sizeof(CCSDS_PriHdr_t));

    REPORT();
} /* end Test_CFE_SB_MsgHdrSize */

/*
** Test getting a pointer to the user data portion of a message
*/
void Test_CFE_SB_GetUserData(void)
{
    SB_UT_Test_Cmd_t       SBCmd;
    CFE_SB_MsgPtr_t        SBCmdPtr = (CFE_SB_MsgPtr_t) &SBCmd;
    SB_UT_Test_Tlm_t       SBTlm;
    CFE_SB_MsgPtr_t        SBTlmPtr = (CFE_SB_MsgPtr_t) &SBTlm;
    SB_UT_TstPktWoSecHdr_t SBNoSecHdrPkt;
    CFE_SB_MsgPtr_t        SBNoSecHdrPktPtr = (CFE_SB_MsgPtr_t) &SBNoSecHdrPkt;
    uint8                  *ActualAdrReturned;
    uint8                  *ExpAdrReturned;
    CFE_SB_MsgId_t         msgId;

    START();

    /* Test address returned for cmd pkts w/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    msgId = SB_UT_CMD_MID;

    ExpAdrReturned = (uint8 *) SBCmdPtr + sizeof(CFE_SB_CmdHdr_t);

    CFE_SB_InitMsg(SBCmdPtr, msgId, sizeof(SB_UT_Test_Cmd_t), true);
    ActualAdrReturned = CFE_SB_GetUserData(SBCmdPtr);


    if (ActualAdrReturned != ExpAdrReturned)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "Address of data for commands with secondary header is "
                   "packet address + 8\n PktAddr %p, Rtn %p, Exp %p",
                 (void *) SBCmdPtr, ActualAdrReturned, ExpAdrReturned);
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
    }

    REPORT();

    /* Test address returned for cmd pkts wo/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;
    CFE_SB_SetMsgId(SBNoSecHdrPktPtr, SB_UT_TLM_MID);
    CCSDS_WR_SHDR(*(CCSDS_PriHdr_t*)SBNoSecHdrPktPtr, 0);
    ActualAdrReturned = CFE_SB_GetUserData(SBNoSecHdrPktPtr);
    ExpAdrReturned = (uint8 *) SBNoSecHdrPktPtr + 6;

    if (ActualAdrReturned != ExpAdrReturned)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "Address of data for commands without secondary header is "
                   "packet address + 6\n PktAddr %p, Rtn %p, Exp %p",
                 (void *) SBNoSecHdrPktPtr, ActualAdrReturned, ExpAdrReturned);
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
    }

    REPORT();

    /* Test address returned for tlm pkts w/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;
    CFE_SB_InitMsg(SBTlmPtr, SB_UT_TLM_MID, sizeof(SB_UT_Test_Tlm_t), true);
    ActualAdrReturned = CFE_SB_GetUserData(SBTlmPtr);

    ExpAdrReturned = (uint8 *) SBTlmPtr + sizeof(CFE_SB_TlmHdr_t);

    if (ActualAdrReturned != ExpAdrReturned)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "Address of data for telemetry packets with secondary header "
                   "is Pkt Addr + 12\n PktAddr %p, Rtn %p, Exp %p",
                 (void *) SBTlmPtr, ActualAdrReturned, ExpAdrReturned);
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
    }

    REPORT();

    /* Test address returned for tlm pkts wo/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;
    CFE_SB_SetMsgId(SBNoSecHdrPktPtr, SB_UT_TLM_MID);
    CCSDS_WR_SHDR(*(CCSDS_PriHdr_t*)SBNoSecHdrPktPtr, 0);
    ActualAdrReturned = CFE_SB_GetUserData(SBNoSecHdrPktPtr);
    ExpAdrReturned = (uint8 *) SBNoSecHdrPktPtr + sizeof(CCSDS_PriHdr_t);

    if (ActualAdrReturned != ExpAdrReturned)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "Address of data for telemetry packets without secondary "
                   "header is Pkt Addr + 6\n PktAddr %p, Rtn %p, Exp %p",
                 (void *) SBNoSecHdrPktPtr, ActualAdrReturned, ExpAdrReturned);
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
    }

    REPORT();
} /* end Test_CFE_SB_GetUserData */

/*
** Test setting and getting the message ID of a message
*/
void Test_CFE_SB_SetGetMsgId(void)
{
    SB_UT_Test_Cmd_t SBCmd;
    CFE_SB_MsgPtr_t  SBCmdPtr = (CFE_SB_MsgPtr_t) &SBCmd;
    CFE_SB_MsgId_t   MsgIdReturned;
    uint32           i;

    START();

    /* Test setting and getting the message ID of a message */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    /* Set entire command packet to all f's */
    memset(SBCmdPtr, 0xff, sizeof(SBCmd));
    CFE_SB_SetMsgId(SBCmdPtr, CFE_SB_CMD_MID);
    MsgIdReturned = CFE_SB_GetMsgId(SBCmdPtr);

    if (MsgIdReturned != CFE_SB_CMD_MID)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "CFE_SB_GetMsgId returned 0x%lx, expected 0x%lx",
                 (unsigned long) MsgIdReturned,
                 (unsigned long) CFE_SB_CMD_MID);
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
    }

    REPORT();

    /* Test setting and getting the message ID of a message looping through
     * all values
     */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;


    /* Looping through every value from 0 to 0xffff */
    for (i = 0; i <= 0xFFFF; i++)
    {
        CFE_SB_SetMsgId(SBCmdPtr, i);

        if (CFE_SB_GetMsgId(SBCmdPtr) != i)
        {
            break;
        }
    }

    REPORT();
} /* end Test_CFE_SB_SetGetMsgId */

/*
** Test setting and getting the user data size of a message
*/
void Test_CFE_SB_SetGetUserDataLength(void)
{
    SB_UT_Test_Cmd_t       SBCmd;
    CFE_SB_MsgPtr_t        SBCmdPtr = (CFE_SB_MsgPtr_t) &SBCmd;
    SB_UT_Test_Tlm_t       SBTlm;
    CFE_SB_MsgPtr_t        SBTlmPtr = (CFE_SB_MsgPtr_t) &SBTlm;
    SB_UT_TstPktWoSecHdr_t SBNoSecHdrPkt;
    CFE_SB_MsgPtr_t        SBNoSecHdrPktPtr = (CFE_SB_MsgPtr_t) &SBNoSecHdrPkt;
    int32                  SetSize;
    uint16                 SizeReturned;
    int16                  ActualPktLenField;
    int16                  ExpPktLenField;
    CFE_SB_MsgId_t         msgId;

    START();

    /* CCSDS pkt length field = SecHdrSize + data - 1 */

    /* Loop through all pkt length values for cmd pkts w/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    msgId = SB_UT_CMD_MID;

    CFE_SB_InitMsg(SBCmdPtr, msgId, sizeof(SB_UT_Test_Cmd_t), true);


    for (SetSize = 0; SetSize < 0x10000; SetSize++)
    {
        CFE_SB_SetUserDataLength(SBCmdPtr, SetSize);
        SizeReturned = CFE_SB_GetUserDataLength(SBCmdPtr);
        ActualPktLenField = UT_GetActualPktLenField(SBCmdPtr);

        ExpPktLenField = sizeof(CCSDS_CommandPacket_t) + SetSize - sizeof(CCSDS_PriHdr_t) - 1; /* SecHdrSize + data - 1 */

        if (SizeReturned != SetSize ||
            ActualPktLenField != ExpPktLenField)
        {
            snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                     "SzRet=%u, SetSz=%ld, ActPL=%d, ExpPL=%d",
                     (unsigned) SizeReturned, (long) SetSize,
                     (int) ActualPktLenField, (int) ExpPktLenField);
            UT_Text(cMsg);
            TestStat = CFE_FAIL;
            break;
        }
    }

    REPORT();

    /* Loop through all pkt length values for cmd pkts wo/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    msgId = SB_UT_TLM_MID;

    CFE_SB_SetMsgId(SBNoSecHdrPktPtr, msgId);
    CCSDS_WR_SHDR(*(CCSDS_PriHdr_t*)SBNoSecHdrPktPtr, 0);

    for (SetSize = 0; SetSize < 0x10000; SetSize++)
    {
        CFE_SB_SetUserDataLength(SBNoSecHdrPktPtr, SetSize);
        SizeReturned = CFE_SB_GetUserDataLength(SBNoSecHdrPktPtr);
        ActualPktLenField = UT_GetActualPktLenField(SBNoSecHdrPktPtr);
        ExpPktLenField = 0 + SetSize - 1; /* SecHdrSize + data - 1 */

        if (SizeReturned != SetSize ||
            ActualPktLenField != ExpPktLenField)
        {
            snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                     "SzRet=%u, SetSz=%ld, ActPL=%d, ExpPL=%d",
                     (unsigned) SizeReturned, (long) SetSize,
                     (int) ActualPktLenField, (int) ExpPktLenField);
            UT_Text(cMsg);
            TestStat = CFE_FAIL;
            break;
        }
    }

    REPORT();

    /* Loop through all pkt length values for tlm pkts w/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    msgId = SB_UT_TLM_MID;

    CFE_SB_InitMsg(SBTlmPtr, msgId, sizeof(SB_UT_Test_Tlm_t), true);

    for (SetSize = 0; SetSize < 0x10000; SetSize++)
    {
        CFE_SB_SetUserDataLength(SBTlmPtr, SetSize);
        SizeReturned = CFE_SB_GetUserDataLength(SBTlmPtr);
        ActualPktLenField = UT_GetActualPktLenField(SBTlmPtr);

        ExpPktLenField = sizeof(CCSDS_TelemetryPacket_t) + SetSize - sizeof(CCSDS_PriHdr_t) - 1; /* SecHdrSize + data - 1 */

        if (SizeReturned != SetSize ||
            ActualPktLenField != ExpPktLenField)
        {
            snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                     "SzRet=%u, SetSz=%ld, ActPL=%d, ExpPL=%d",
                     (unsigned int) SizeReturned, (long) SetSize,
                     (int) ActualPktLenField, (int) ExpPktLenField);
            UT_Text(cMsg);
            TestStat = CFE_FAIL;
            break;
        }
    }

    REPORT();

    /* Loop through all pkt length values for tlm pkts wo/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    msgId = SB_UT_TLM_MID;

    CFE_SB_SetMsgId(SBNoSecHdrPktPtr, msgId);

    CCSDS_WR_SHDR(*(CCSDS_PriHdr_t*)SBNoSecHdrPktPtr, 0);

    for (SetSize = 0; SetSize < 0x10000; SetSize++)
    {
        CFE_SB_SetUserDataLength(SBNoSecHdrPktPtr, SetSize);
        SizeReturned = CFE_SB_GetUserDataLength(SBNoSecHdrPktPtr);
        ActualPktLenField = UT_GetActualPktLenField(SBNoSecHdrPktPtr);
        ExpPktLenField = 0 + SetSize - 1; /* SecHdrSize + data - 1 */

        if (SizeReturned != SetSize ||
            ActualPktLenField != ExpPktLenField)
        {
            snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                     "SzRet=%u, SetSz=%ld, ActPL=%d, ExpPL=%d",
                     (unsigned int) SizeReturned, (long) SetSize,
                     (int) ActualPktLenField, (int) ExpPktLenField);
            UT_Text(cMsg);
            TestStat = CFE_FAIL;
            break;
        }
    }

    REPORT();
} /* end Test_CFE_SB_SetGetUserDataLength */

/*
** Test setting and getting the total message size
*/
void Test_CFE_SB_SetGetTotalMsgLength(void)
{
    SB_UT_Test_Cmd_t       SBCmd;
    CFE_SB_MsgPtr_t        SBCmdPtr = (CFE_SB_MsgPtr_t) &SBCmd;
    SB_UT_Test_Tlm_t       SBTlm;
    CFE_SB_MsgPtr_t        SBTlmPtr = (CFE_SB_MsgPtr_t) &SBTlm;
    SB_UT_TstPktWoSecHdr_t SBNoSecHdrPkt;
    CFE_SB_MsgPtr_t        SBNoSecHdrPktPtr = (CFE_SB_MsgPtr_t) &SBNoSecHdrPkt;
    int32                  SetSize;
    uint16                 TotalMsgSizeReturned;
    int16                  ActualPktLenField;
    int16                  ExpPktLenField;

    START();

    /* CCSDS pkt length field = TotalPktSize - 7 */

    /* Loop through all pkt length values for cmd pkts w/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;
    CFE_SB_SetMsgId(SBCmdPtr, SB_UT_CMD_MID);

    for (SetSize = 0; SetSize < 0x10000; SetSize++)
    {
        CFE_SB_SetTotalMsgLength(SBCmdPtr, SetSize);
        TotalMsgSizeReturned = CFE_SB_GetTotalMsgLength(SBCmdPtr);
        ActualPktLenField = UT_GetActualPktLenField(SBCmdPtr);
        ExpPktLenField = SetSize - 7; /* TotalPktSize - 7 */

        if (TotalMsgSizeReturned != SetSize ||
            ActualPktLenField != ExpPktLenField)
        {
            snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                     "SzRet=%u, SetSz=%ld, ActPL=%d, ExpPL=%d",
                     (unsigned int) TotalMsgSizeReturned, (long) SetSize,
                     (int) ActualPktLenField, (int) ExpPktLenField);
            UT_Text(cMsg);
            TestStat = CFE_FAIL;
            break;
        }
    }

    REPORT();

    /* Loop through all pkt length values for cmd pkts wo/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;
    CFE_SB_SetMsgId(SBNoSecHdrPktPtr, SB_UT_TLM_MID);

    for (SetSize = 0; SetSize < 0x10000; SetSize++)
    {
        CFE_SB_SetTotalMsgLength(SBNoSecHdrPktPtr, SetSize);
        TotalMsgSizeReturned = CFE_SB_GetTotalMsgLength(SBNoSecHdrPktPtr);
        ActualPktLenField = UT_GetActualPktLenField(SBNoSecHdrPktPtr);
        ExpPktLenField = SetSize - 7; /* TotalPktSize - 7 */

        if (TotalMsgSizeReturned != SetSize ||
            ActualPktLenField != ExpPktLenField)
        {
            snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                     "SzRet=%u, SetSz=%ld, ActPL=%d, ExpPL=%d",
                     (unsigned int) TotalMsgSizeReturned, (long) SetSize,
                     (int) ActualPktLenField, (int) ExpPktLenField);
            UT_Text(cMsg);
            TestStat = CFE_FAIL;
            break;
        }
    }

    REPORT();

    /* Loop through all pkt length values for tlm pkts w/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;
    CFE_SB_SetMsgId(SBTlmPtr, SB_UT_TLM_MID);

    for (SetSize = 0; SetSize < 0x10000; SetSize++)
    {
        CFE_SB_SetTotalMsgLength(SBTlmPtr, SetSize);
        TotalMsgSizeReturned = CFE_SB_GetTotalMsgLength(SBTlmPtr);
        ActualPktLenField = UT_GetActualPktLenField(SBTlmPtr);
        ExpPktLenField = SetSize - 7; /* TotalPktSize - 7 */

        if (TotalMsgSizeReturned != SetSize ||
            ActualPktLenField != ExpPktLenField)
        {
            snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                     "SzRet=%u, SetSz=%ld, ActPL=%d, ExpPL=%d",
                     (unsigned int) TotalMsgSizeReturned, (long) SetSize,
                     (int) ActualPktLenField, (int) ExpPktLenField);
            UT_Text(cMsg);
            TestStat = CFE_FAIL;
            break;
        }
    }

    REPORT();

    /* Loop through all pkt length values for tlm pkts wo/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;
    CFE_SB_SetMsgId(SBNoSecHdrPktPtr, SB_UT_TLM_MID);

    for (SetSize = 0; SetSize < 0x10000; SetSize++)
    {
        CFE_SB_SetTotalMsgLength(SBNoSecHdrPktPtr, SetSize);
        TotalMsgSizeReturned = CFE_SB_GetTotalMsgLength(SBNoSecHdrPktPtr);
        ActualPktLenField = UT_GetActualPktLenField(SBNoSecHdrPktPtr);
        ExpPktLenField = SetSize - 7; /* TotalPktSize - 7 */

        if (TotalMsgSizeReturned != SetSize ||
            ActualPktLenField != ExpPktLenField)
        {
            snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                     "SzRet=%u, SetSz=%ld, ActPL=%d, ExpPL=%d",
                     (unsigned int) TotalMsgSizeReturned, (long) SetSize,
                     (int) ActualPktLenField, (int) ExpPktLenField);
            UT_Text(cMsg);
            TestStat = CFE_FAIL;
            break;
        }
    }

    REPORT();
} /* end Test_CFE_SB_SetGetTotalMsgLength */

/*
** Test setting and getting the message time field
*/
void Test_CFE_SB_SetGetMsgTime(void)
{
    SB_UT_Test_Cmd_t       SBCmd;
    CFE_SB_MsgPtr_t        SBCmdPtr = (CFE_SB_MsgPtr_t) &SBCmd;
    SB_UT_Test_Tlm_t       SBTlm;
    CFE_SB_MsgPtr_t        SBTlmPtr = (CFE_SB_MsgPtr_t) &SBTlm;
    SB_UT_TstPktWoSecHdr_t SBNoSecHdrPkt;
    CFE_SB_MsgPtr_t        SBNoSecHdrPktPtr = (CFE_SB_MsgPtr_t) &SBNoSecHdrPkt;
    CFE_TIME_SysTime_t     SetTime, GetTime;
    int32                  RtnFromSet;
    CFE_SB_MsgId_t         msgId;

    START();

    /* Begin test for cmd pkts w/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    /* Set MsgId to all f's */
    memset(SBCmdPtr, 0xff, sizeof(SBCmd));

    /* Set MsgId */
    CFE_SB_SetMsgId(SBCmdPtr, SB_UT_CMD_MID);


    SetTime.Seconds = 0x4321;
    SetTime.Subseconds = 0x8765;
    RtnFromSet = CFE_SB_SetMsgTime(SBCmdPtr, SetTime);
    GetTime.Seconds = 0xffff;
    GetTime.Subseconds = 0xffff;
    GetTime = CFE_SB_GetMsgTime(SBCmdPtr);

    /* Verify CFE_SB_SetMsgTime returns CFE_SB_WRONG_MSG_TYPE for cmd
     * pkts w/sec hdr
     */
    if (RtnFromSet != CFE_SB_WRONG_MSG_TYPE)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "RtnFromSet=%ld, ExpReturn=0x%lx",
                 (long) RtnFromSet, (unsigned long) CFE_SB_WRONG_MSG_TYPE);
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
        UT_DisplayPkt(SBCmdPtr, sizeof(SBCmd));
    }
    /* Verify the call to CFE_SB_GetMsgTime returns a time value of zero */
    else if (GetTime.Seconds != 0 || GetTime.Subseconds != 0)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "GetTime.Sec=%lu, GetTime.Subsec=%lu",
                 (unsigned long) GetTime.Seconds,
                 (unsigned long) GetTime.Subseconds);
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
        UT_DisplayPkt(SBCmdPtr, sizeof(SBCmd));
    }

    REPORT();

    /* Begin test for cmd pkts wo/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    /* Set MsgId to all f's */
    memset(SBNoSecHdrPktPtr, 0xff, sizeof(SBNoSecHdrPkt));

    /* Set MsgId */
    CFE_SB_SetMsgId(SBNoSecHdrPktPtr, SB_UT_TLM_MID);
    CCSDS_WR_SHDR(*(CCSDS_PriHdr_t*)SBNoSecHdrPktPtr, 0);


    SetTime.Seconds = 0x4321;
    SetTime.Subseconds = 0x8765;
    RtnFromSet = CFE_SB_SetMsgTime(SBNoSecHdrPktPtr, SetTime);
    GetTime.Seconds = 0xffff;
    GetTime.Subseconds = 0xffff;
    GetTime = CFE_SB_GetMsgTime(SBNoSecHdrPktPtr);

    /* Verify CFE_SB_SetMsgTime returns CFE_SB_WRONG_MSG_TYPE for cmd
     * pkts wo/sec hdr
     */
    if (RtnFromSet != CFE_SB_WRONG_MSG_TYPE)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "RtnFromSet=%ld, ExpReturn=0x%lx",
                 (long) RtnFromSet, (unsigned long) CFE_SB_WRONG_MSG_TYPE);
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
        UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SB_UT_TstPktWoSecHdr_t));
    }
    /* Verify the call to CFE_SB_GetMsgTime returns a time value of zero */
    else if (GetTime.Seconds != 0 || GetTime.Subseconds != 0)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "GetTime.Sec=%lu, GetTime.Subsec=%lu",
                 (unsigned long) GetTime.Seconds,
                 (unsigned long) GetTime.Subseconds);
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
        UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SB_UT_TstPktWoSecHdr_t));
    }

    REPORT();

    /* Begin test for tlm pkts w/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    /* Set MsgId to all f's */
    memset(SBTlmPtr, 0xff, sizeof(SBTlm));

    msgId = SB_UT_TLM_MID;

    /* Set MsgId to 0x0805 */
    CFE_SB_SetMsgId(SBTlmPtr, msgId);
    SetTime.Seconds = 0x01234567;
    SetTime.Subseconds = 0x89abcdef;
    RtnFromSet = CFE_SB_SetMsgTime(SBTlmPtr, SetTime);
    GetTime.Seconds = 0xffff;
    GetTime.Subseconds = 0xffff;
    GetTime = CFE_SB_GetMsgTime(SBTlmPtr);

    /* Verify CFE_SB_SetMsgTime returns CFE_SUCCESS for tlm pkts w/sec hdr */
    if (RtnFromSet != CFE_SUCCESS)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "RtnFromSet=%ld, ExpReturn=0", (long)RtnFromSet);
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
        UT_DisplayPkt(SBTlmPtr, sizeof(SBTlm));
    }
    /* Verify CFE_SB_GetMsgTime returns the SetTime value w/2 LSBytes
     * of subseconds zeroed out
     */
    else if (GetTime.Seconds != SetTime.Seconds ||
             GetTime.Subseconds != (SetTime.Subseconds & 0xffff0000))
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "SetTime.Sec=%lu, GetTime.Sec=%lu, SetTime.Subsec=%lu, "
                   "GetTime.Subsec=%lu",
                 (unsigned long) SetTime.Seconds,
                 (unsigned long) GetTime.Seconds,
                 (unsigned long) SetTime.Subseconds,
                 (unsigned long) GetTime.Subseconds);
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
        UT_DisplayPkt(SBTlmPtr, sizeof(SBTlm));
    }

    REPORT();

    /* Begin test for tlm pkts wo/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    /* Set MsgId to all f's */
    memset(SBNoSecHdrPktPtr, 0xff, sizeof(SBNoSecHdrPkt));

    /* Set MsgId to 0x0005 */
    CFE_SB_SetMsgId(SBNoSecHdrPktPtr, SB_UT_TLM_MID);
    CCSDS_WR_SHDR(*((CCSDS_PriHdr_t*)SBNoSecHdrPktPtr), 0);
    SetTime.Seconds = 0x01234567;
    SetTime.Subseconds = 0x89abcdef;
    RtnFromSet = CFE_SB_SetMsgTime(SBNoSecHdrPktPtr, SetTime);
    GetTime.Seconds = 0xffff;
    GetTime.Subseconds = 0xffff;
    GetTime = CFE_SB_GetMsgTime(SBNoSecHdrPktPtr);

    /* Verify CFE_SB_SetMsgTime returns CFE_SB_WRONG_MSG_TYPE for tlm
     * pkts wo/sec hdr
     */
    if (RtnFromSet != CFE_SB_WRONG_MSG_TYPE)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "RtnFromSet=%ld, ExpReturn=0x%lx",
                 (long) RtnFromSet, (unsigned long) CFE_SB_WRONG_MSG_TYPE);
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
        UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SBNoSecHdrPkt));
    }
    /* Verify the call to CFE_SB_GetMsgTime returns a time value of zero */
    else if (GetTime.Seconds != 0 || GetTime.Subseconds != 0)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "GetTime.Sec=%lu, GetTime.Subsec=%lu",
                 (unsigned long) GetTime.Seconds,
                 (unsigned long) GetTime.Subseconds);
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
        UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SBNoSecHdrPkt));
    }

    REPORT();
} /* end Test_CFE_SB_SetGetMsgTime */

/*
** Test setting the time field to the current time
*/
void Test_CFE_SB_TimeStampMsg(void)
{
    SB_UT_Test_Tlm_t   SBTlm;
    CFE_SB_MsgPtr_t    SBTlmPtr = (CFE_SB_MsgPtr_t) &SBTlm;
    CFE_TIME_SysTime_t GetTime;
    uint32             ExpSecs;

    START();

    /* Begin test for tlm pkts w/sec hdr */
    SB_ResetUnitTest();

    /* Note: Only Tlm Pkt with Sec Hdr is tested here because this function
     * (Test_CFE_SB_TimeStampMsg) is simply a call to
     * Test_CFE_SB_SetGetMsgTime.  Test_CFE_SB_SetGetMsgTime has covered
     * the other pkt types
     */

    /* Set MsgId to all f's */
    memset(SBTlmPtr, 0xff, sizeof(SBTlm));

    /* Set MsgId to 0x0805 */
    CFE_SB_SetMsgId(SBTlmPtr, SB_UT_TLM_MID);
    CFE_SB_TimeStampMsg(SBTlmPtr);
    GetTime.Seconds = 0xffff;
    GetTime.Subseconds = 0xffff;
    GetTime = CFE_SB_GetMsgTime(SBTlmPtr);
    ExpSecs = GetTime.Seconds + 1;
    CFE_SB_TimeStampMsg(SBTlmPtr);
    GetTime.Seconds = 0xffff;
    GetTime.Subseconds = 0xffff;
    GetTime = CFE_SB_GetMsgTime(SBTlmPtr);

    /* Verify CFE_SB_GetMsgTime returns the time value expected by
     * CFE_TIME_GetTime.  The stub for CFE_TIME_GetTime simply increments
     * the seconds cnt on each call
     */
    if (GetTime.Seconds != ExpSecs || GetTime.Subseconds != 0)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "GetTime.Sec=%lu, ExpSecs=%lu; GetTime.Subsec=%lu, "
                   "ExpSubsecs=0",
                   (unsigned long) GetTime.Seconds,
                   (unsigned long) ExpSecs,
                   (unsigned long) GetTime.Subseconds);
        UT_Text(cMsg);
        TestStat = CFE_FAIL;
        UT_DisplayPkt(SBTlmPtr, sizeof(SB_UT_Test_Tlm_t));
    }

    REPORT();
} /* end Test_CFE_SB_TimeStampMsg */

/*
** Test setting and getting the opcode field of message
*/
void Test_CFE_SB_SetGetCmdCode(void)
{
    SB_UT_Test_Cmd_t       SBCmd;
    CFE_SB_MsgPtr_t        SBCmdPtr = (CFE_SB_MsgPtr_t) &SBCmd;
    SB_UT_Test_Tlm_t       SBTlm;
    CFE_SB_MsgPtr_t        SBTlmPtr = (CFE_SB_MsgPtr_t) &SBTlm;
    SB_UT_TstPktWoSecHdr_t SBNoSecHdrPkt;
    CFE_SB_MsgPtr_t        SBNoSecHdrPktPtr = (CFE_SB_MsgPtr_t) &SBNoSecHdrPkt;
    int32                  RtnFromSet, ExpRtnFrmSet;
    uint16                 CmdCodeSet, CmdCodeReturned;
    uint8                  ActualCmdCodeField;
    int16                  ExpCmdCode;
    CCSDS_PriHdr_t         *PktPtr;
    CFE_SB_MsgId_t         msgId;

    START();

    /* Loop through all cmd code values(plus a few invalid) for cmd
     * pkts w/sec hdr
     */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    /* Set MsgId to all f's */
    memset(SBCmdPtr, 0xff, sizeof(SBCmd));

    msgId = SB_UT_CMD_MID;

    CFE_SB_InitMsg(SBCmdPtr, msgId, sizeof(SB_UT_Test_Cmd_t), true);

    PktPtr = (CCSDS_PriHdr_t*)SBCmdPtr;

    CCSDS_WR_SHDR(*PktPtr,1);

    for (CmdCodeSet = 0; CmdCodeSet < 0x100; CmdCodeSet++)
    {
        RtnFromSet = CFE_SB_SetCmdCode(SBCmdPtr, CmdCodeSet);

        ExpRtnFrmSet = CFE_SUCCESS;
        CmdCodeReturned = CFE_SB_GetCmdCode(SBCmdPtr);
        ActualCmdCodeField = UT_GetActualCmdCodeField(SBCmdPtr);

        /* GSFC CmdCode is the 7 LSBs of the 1st byte of cmd sec hdr */
        ExpCmdCode = CmdCodeSet & 0x007f;

        if (CmdCodeReturned != ExpCmdCode ||
            ActualCmdCodeField != ExpCmdCode ||
            RtnFromSet != ExpRtnFrmSet)
        {
            snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                     "CCRet=%d, CCSet=%d, ActCC=%d, ExpCC=%d, "
                       "RtnFrmSet=0x%lx, ExpRtFrmSet=0x%lx",
                     CmdCodeReturned, CmdCodeSet, ActualCmdCodeField,
                     ExpCmdCode, (unsigned long) RtnFromSet,
                     (unsigned long) ExpRtnFrmSet);
            UT_Text(cMsg);
            UT_DisplayPkt(SBCmdPtr, sizeof(SBCmd));
            TestStat = CFE_FAIL;
            break;
        }
    }

    REPORT();

    /* Loop through all cmd code values (plus a few invalid) for cmd
     * pkts wo/sec hdr
     */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    /* Set MsgId to all f's */
    memset(SBNoSecHdrPktPtr, 0xff, sizeof(SB_UT_TstPktWoSecHdr_t));

    /* Set MsgId */
    CFE_SB_SetMsgId(SBNoSecHdrPktPtr, SB_UT_TLM_MID);

    PktPtr = (CCSDS_PriHdr_t*)SBNoSecHdrPktPtr;

    CCSDS_WR_SHDR(*PktPtr,0);

    for (CmdCodeSet = 0; CmdCodeSet < 0x100; CmdCodeSet++)
    {
        RtnFromSet = CFE_SB_SetCmdCode(SBNoSecHdrPktPtr, CmdCodeSet);
        ExpRtnFrmSet = CFE_SB_WRONG_MSG_TYPE;
        CmdCodeReturned = CFE_SB_GetCmdCode(SBNoSecHdrPktPtr);
        ActualCmdCodeField = UT_GetActualCmdCodeField(SBNoSecHdrPktPtr);

        /* GSFC CmdCode is the 7 LSBs of the 1st byte of cmd Sec hdr */
        ExpCmdCode = CmdCodeSet & 0x007f;

        if (RtnFromSet != ExpRtnFrmSet)
        {
            snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                     "CCRet=%d, CCSet=%d, ActCC=%d, ExpCC=%d, "
                       "RtnFrmSet=0x%lx, ExpRtFrmSet=0x%lx",
                     CmdCodeReturned, CmdCodeSet, ActualCmdCodeField,
                     ExpCmdCode, (unsigned long) RtnFromSet,
                     (unsigned long) ExpRtnFrmSet);
            UT_Text(cMsg);
            UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SBNoSecHdrPkt));
            TestStat = CFE_FAIL;
            break;
        }
    }

    REPORT();

    /* Loop through all cmd code values (plus a few invalid) for tlm
     * pkts w/sec hdr
     */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    /* Set MsgId to all f's */
    memset(SBTlmPtr, 0xff, sizeof(SBTlm));

    /* Set MsgId to 0x0805 */
    CFE_SB_SetMsgId(SBTlmPtr, SB_UT_TLM_MID);

    PktPtr = (CCSDS_PriHdr_t*)SBTlmPtr;

    CCSDS_WR_SHDR(*PktPtr,1);

    for (CmdCodeSet = 0; CmdCodeSet < 0x100; CmdCodeSet++)
    {
        RtnFromSet = CFE_SB_SetCmdCode(SBTlmPtr, CmdCodeSet);
        ExpRtnFrmSet = CFE_SB_WRONG_MSG_TYPE;
        CmdCodeReturned = CFE_SB_GetCmdCode(SBTlmPtr);
        ActualCmdCodeField = UT_GetActualCmdCodeField(SBTlmPtr);

        /* GSFC CmdCode is the 7 LSBs of the 1st byte of cmd Sec hdr */
        ExpCmdCode = CmdCodeSet & 0x007f;

        if (RtnFromSet != ExpRtnFrmSet)
        {
            snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                     "CCRet=%d, CCSet=%d, ActCC=%d, ExpCC=%d, "
                       "RtnFrmSet=0x%lx, ExpRtFrmSet=0x%lx",
                     CmdCodeReturned, CmdCodeSet, ActualCmdCodeField,
                     ExpCmdCode, (unsigned long) RtnFromSet,
                     (unsigned long) ExpRtnFrmSet);
            UT_Text(cMsg);
            UT_DisplayPkt(SBTlmPtr, sizeof(SBTlm));
            TestStat = CFE_FAIL;
            break;
        }
    }

    REPORT();

    /* Loop through all cmd code values (plus a few invalid) for tlm
     * pkts wo/sec hdr
     */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    /* Set pkt to all f's */
    memset(SBNoSecHdrPktPtr, 0xff, sizeof(SBNoSecHdrPkt));

    /* Set MsgId to 0x0005 */
    CFE_SB_SetMsgId(SBNoSecHdrPktPtr, SB_UT_TLM_MID);

    PktPtr = (CCSDS_PriHdr_t*)SBNoSecHdrPktPtr;

    CCSDS_WR_SHDR(*PktPtr,0);

    for (CmdCodeSet = 0; CmdCodeSet < 0x100; CmdCodeSet++)
    {
        RtnFromSet = CFE_SB_SetCmdCode(SBNoSecHdrPktPtr, CmdCodeSet);
        ExpRtnFrmSet = CFE_SB_WRONG_MSG_TYPE;
        CmdCodeReturned = CFE_SB_GetCmdCode(SBNoSecHdrPktPtr);
        ActualCmdCodeField = UT_GetActualCmdCodeField(SBNoSecHdrPktPtr);

        /* GSFC CmdCode is the 7 LSBs of the 1st byte of cmd sec hdr */
        ExpCmdCode = CmdCodeSet & 0x007f;

        if (RtnFromSet != ExpRtnFrmSet)
        {
            snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                     "CCRet=%d, CCSet=%d, ActCC=%d, ExpCC=%d, "
                       "RtnFrmSet=0x%lx, ExpRtFrmSet=0x%lx",
                     CmdCodeReturned, CmdCodeSet, ActualCmdCodeField,
                     ExpCmdCode, (unsigned long) RtnFromSet,
                     (unsigned long) ExpRtnFrmSet);
            UT_Text(cMsg);
            UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SBNoSecHdrPkt));
            TestStat = CFE_FAIL;
            break;
        }
    }

    REPORT();
} /* end Test_CFE_SB_SetGetCmdCode */

/*
** Test generating, setting, getting, and validating a checksum field
** for a message
*/
void Test_CFE_SB_ChecksumUtils(void)
{
    SB_UT_Test_Cmd_t       SBCmd;
    CFE_SB_MsgPtr_t        SBCmdPtr = (CFE_SB_MsgPtr_t) &SBCmd;
    SB_UT_Test_Tlm_t       SBTlm;
    CFE_SB_MsgPtr_t        SBTlmPtr = (CFE_SB_MsgPtr_t) &SBTlm;
    SB_UT_TstPktWoSecHdr_t SBNoSecHdrPkt;
    CFE_SB_MsgPtr_t        SBNoSecHdrPktPtr = (CFE_SB_MsgPtr_t) &SBNoSecHdrPkt;
    uint16                 RtnFrmGet, ExpRtnFrmGet;
    bool                RtnFrmValidate, ExpRtnFrmVal;

    START();

    /* Begin test for cmd pkts w/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    /* Initialize pkt, setting data to zero */
    CFE_SB_InitMsg(SBCmdPtr, 0x1805, sizeof(SBCmd), true);

    CCSDS_WR_SID( (*((CCSDS_PriHdr_t*) SBCmdPtr)), 0x1805 );

    /* Set checksum field */

    CFE_SB_GenerateChecksum(SBCmdPtr);

    RtnFrmGet = CFE_SB_GetChecksum(SBCmdPtr);
#ifndef MESSAGE_FORMAT_IS_CCSDS_VER_2
    ExpRtnFrmGet = 0x2f;
#else
    ExpRtnFrmGet = 0x65;
#endif

    /* Validation expected to return true */
    RtnFrmValidate = CFE_SB_ValidateChecksum(SBCmdPtr);
    ExpRtnFrmVal = true;

    if (RtnFrmGet != ExpRtnFrmGet || RtnFrmValidate != ExpRtnFrmVal)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "Unexpected results in 1st check, RtnFrmValidate=%d, "
                   "ExpRtnFrmVal=%d, RtnFrmGet=0x%x, ExpRtnFrmGet=0x%x",
                 RtnFrmValidate, ExpRtnFrmVal, RtnFrmGet, ExpRtnFrmGet);
        UT_Text(cMsg);
        UT_DisplayPkt(SBCmdPtr, sizeof(SBCmd));
        TestStat = CFE_FAIL;
    }

    /* Change 1 byte in pkt and verify checksum is no longer valid.
     * Increment MsgId by 1 to 0x1806.  Validation expected to
     * return false
     */
    CCSDS_WR_SID( (*((CCSDS_PriHdr_t*) SBCmdPtr)), 0x1806 );

    RtnFrmValidate = CFE_SB_ValidateChecksum(SBCmdPtr);
    ExpRtnFrmVal = false;

    if (TestStat == CFE_FAIL || RtnFrmValidate != ExpRtnFrmVal)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "Second validate...RtnFrmValidate=%d, ExpRtnFrmVal=%d",
                 RtnFrmValidate, ExpRtnFrmVal);
        UT_Text(cMsg);
        UT_DisplayPkt(SBCmdPtr, sizeof(SBCmd));
        TestStat = CFE_FAIL;
    }

    REPORT();

    /* Begin test for cmd pkts wo/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    /* Initialize pkt, setting data to zero */
    CFE_SB_InitMsg(SBNoSecHdrPktPtr, 0x1005,
                   sizeof(SBNoSecHdrPkt), true);


    CCSDS_WR_SHDR( SBNoSecHdrPktPtr->Hdr, 0 );

    /* Set checksum field */
    CFE_SB_GenerateChecksum(SBNoSecHdrPktPtr);
    RtnFrmGet = CFE_SB_GetChecksum(SBNoSecHdrPktPtr);
    ExpRtnFrmGet = 0;

    /* Validation expected to return false */
    RtnFrmValidate = CFE_SB_ValidateChecksum(SBNoSecHdrPktPtr);
    ExpRtnFrmVal = false;

    if (RtnFrmGet != ExpRtnFrmGet || RtnFrmValidate != ExpRtnFrmVal)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "Unexpected results in 1st check, RtnFrmValidate=%d, "
                   "ExpRtnFrmVal=%d, RtnFrmGet=0x%x, ExpRtnFrmGet=0x%x",
                 RtnFrmValidate, ExpRtnFrmVal, RtnFrmGet, ExpRtnFrmGet);
        UT_Text(cMsg);
        UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SBNoSecHdrPkt));
        TestStat = CFE_FAIL;
    }

    /* Change 1 byte in pkt and verify checksum is no longer valid.
     * Increment MsgId by 1 to 0x1006.  Validation expected to
     * return false
     */
    CFE_SB_SetMsgId(SBNoSecHdrPktPtr, SB_UT_TLM_MID);

    CCSDS_WR_SHDR( SBCmdPtr->Hdr, 0 );
    RtnFrmValidate = CFE_SB_ValidateChecksum(SBNoSecHdrPktPtr);
    ExpRtnFrmVal = false;

    if (TestStat == CFE_FAIL || RtnFrmValidate != ExpRtnFrmVal)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "Second validate...RtnFrmValidate=%d, ExpRtnFrmVal=%d",
                 RtnFrmValidate, ExpRtnFrmVal);
        UT_Text(cMsg);
        UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SBNoSecHdrPkt));
        TestStat = CFE_FAIL;
    }

    REPORT();

    /* Begin test for tlm pkts w/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    /* Initialize pkt, setting data to zero */
    CFE_SB_InitMsg(SBTlmPtr, 0x0805, sizeof(SBTlm), true);

    /* Set checksum field */
    CFE_SB_GenerateChecksum(SBTlmPtr);
    RtnFrmGet = CFE_SB_GetChecksum(SBTlmPtr);
    ExpRtnFrmGet = 0;

    /* Validation expected to return false */
    RtnFrmValidate = CFE_SB_ValidateChecksum(SBTlmPtr);
    ExpRtnFrmVal = false;

    if (RtnFrmGet != ExpRtnFrmGet || RtnFrmValidate != ExpRtnFrmVal)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "Unexpected results in 1st check, RtnFrmValidate=%d, "
                   "ExpRtnFrmVal=%d, RtnFrmGet=0x%x, ExpRtnFrmGet=0x%x",
                 RtnFrmValidate, ExpRtnFrmVal, RtnFrmGet, ExpRtnFrmGet);
        UT_Text(cMsg);
        UT_DisplayPkt(SBTlmPtr, sizeof(SBTlm));
        TestStat = CFE_FAIL;
    }

    /* Change 1 byte in pkt and verify checksum is no longer valid.
     * Increment MsgId by 1 to 0x0806.  Validation expected to return false
     */
    CFE_SB_SetMsgId(SBTlmPtr, 0x1806);
    RtnFrmValidate = CFE_SB_ValidateChecksum(SBTlmPtr);
    ExpRtnFrmVal = false;

    if (TestStat == CFE_FAIL || RtnFrmValidate != ExpRtnFrmVal)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "Second validate...RtnFrmValidate=%d, ExpRtnFrmVal=%d",
                 RtnFrmValidate, ExpRtnFrmVal);
        UT_Text(cMsg);
        UT_DisplayPkt(SBTlmPtr, sizeof(SBTlm));
        TestStat = CFE_FAIL;
    }

    REPORT();

    /* Begin test for tlm pkts wo/sec hdr */
    SB_ResetUnitTest();
    TestStat = CFE_PASS;

    /* Initialize pkt, setting data to zero */
    CFE_SB_InitMsg(SBNoSecHdrPktPtr, 0x0005,
                   sizeof(SBNoSecHdrPkt), true);

    /* Setting checksum field */
    CFE_SB_GenerateChecksum(SBNoSecHdrPktPtr);
    RtnFrmGet = CFE_SB_GetChecksum(SBNoSecHdrPktPtr);
    ExpRtnFrmGet = 0;

    /* Validation expected to return false */
    RtnFrmValidate = CFE_SB_ValidateChecksum(SBNoSecHdrPktPtr);
    ExpRtnFrmVal = false;

    if (RtnFrmGet != ExpRtnFrmGet || RtnFrmValidate != ExpRtnFrmVal)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "Unexpected results in 1st check, RtnFrmValidate=%d, "
                   "ExpRtnFrmVal=%d, RtnFrmGet=0x%x, ExpRtnFrmGet=0x%x",
                 RtnFrmValidate, ExpRtnFrmVal, RtnFrmGet, ExpRtnFrmGet);
        UT_Text(cMsg);
        UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SBNoSecHdrPkt));
        TestStat = CFE_FAIL;
    }

    /* Change 1 byte in pkt and verify checksum is no longer valid.
     * Increment MsgId by 1 to 0x0006.  Validation expected to
     * return false
     */
    CFE_SB_SetMsgId(SBNoSecHdrPktPtr, 0x0006);
    RtnFrmValidate = CFE_SB_ValidateChecksum(SBNoSecHdrPktPtr);
    ExpRtnFrmVal = false;

    if (TestStat == CFE_FAIL || RtnFrmValidate != ExpRtnFrmVal)
    {
        snprintf(cMsg, UT_MAX_MESSAGE_LENGTH,
                 "Second validate...RtnFrmValidate=%d, ExpRtnFrmVal=%d",
                 RtnFrmValidate, ExpRtnFrmVal);
        UT_Text(cMsg);
        UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SBNoSecHdrPkt));
        TestStat = CFE_FAIL;
    }

    REPORT();
} /* end Test_CFE_SB_ChecksumUtils */

/*
** Test validating a msg id
*/
void Test_CFE_SB_ValidateMsgId(void)
{
    CFE_SB_MsgId_t        MsgId;

    START();

    SB_ResetUnitTest();

    /* Validate Msg Id */
    MsgId = CFE_PLATFORM_SB_HIGHEST_VALID_MSGID;
    CFE_SB_ValidateMsgId(MsgId);

    REPORT();

    START();

    /* Test for invalid msg id */
    MsgId = CFE_PLATFORM_SB_HIGHEST_VALID_MSGID + 1;
    CFE_SB_ValidateMsgId(MsgId);

    REPORT();
}

/*
** Function for calling SB special test cases functions
*/
void Test_SB_SpecialCases(void)
{
    STARTBLOCK();

    Test_OS_MutSem_ErrLogic();
    Test_ReqToSendEvent_ErrLogic();
    Test_PutDestBlk_ErrLogic();
    Test_CFE_SB_GetPipeIdx();
    Test_CFE_SB_Buffers();
    Test_CFE_SB_BadPipeInfo();
    Test_SB_SendMsgPaths();
    Test_RcvMsg_UnsubResubPath();
    Test_MessageString();
    Test_SB_IdxPushPop();

    ENDBLOCK();
} /* end Test_SB_SpecialCases */

/*
** Test msg key idx push pop
*/
void Test_SB_IdxPushPop()
{
    int32           i;
    CFE_SB_MsgRouteIdx_t Idx;

    START();

    SB_ResetUnitTest();

    CFE_SB_InitIdxStack();

    for (i = 0; i < CFE_PLATFORM_SB_MAX_MSG_IDS; i++)
    {
        /* Subscribe to maximum number of messages */
        Idx = CFE_SB_RouteIdxPop_Unsync();
        ASSERT_EQ(CFE_SB_RouteIdxToValue(Idx), i);
    }


    Idx = CFE_SB_RouteIdxPop_Unsync();
    ASSERT_EQ(CFE_SB_RouteIdxToValue(Idx), CFE_SB_RouteIdxToValue(CFE_SB_INVALID_ROUTE_IDX));

    /*
     *  This sub-unit test is dependent upon the previous 
     *  Maybe do a
     * 
    SB_ResetUnitTest();

    CFE_SB_InitIdxStack();
     * again here 
     */
    for (i = 0; i < CFE_PLATFORM_SB_MAX_MSG_IDS; i++)
    {
        /* Un-subscribe from all messages */
        CFE_SB_RouteIdxPush_Unsync(CFE_SB_ValueToRouteIdx(i));
    }

    CFE_SB_RouteIdxPush_Unsync(CFE_SB_ValueToRouteIdx(i));

    REPORT();

} /* end Test_SB_IdxPushPop */

/*
** Test pipe creation with semaphore take and give failures
*/
void Test_OS_MutSem_ErrLogic(void)
{
    CFE_SB_PipeId_t PipeId;
    CFE_SB_MsgId_t  MsgId = SB_UT_CMD_MID;
    uint16          PipeDepth = 50;

    START();

    SB_ResetUnitTest();
    UT_SetDeferredRetcode(UT_KEY(OS_MutSemTake), 1, CFE_OS_SEM_FAILURE);
    UT_SetDeferredRetcode(UT_KEY(OS_MutSemGive), 2, CFE_OS_SEM_FAILURE);
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));

    ASSERT(CFE_SB_Subscribe(MsgId, PipeId));

    EVTCNT(3);

    EVTSENT(CFE_SB_PIPE_ADDED_EID);
    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_OS_MutSemTake_ErrLogic */

/*
** Test successful recursive event prevention
*/
void Test_ReqToSendEvent_ErrLogic(void)
{
    uint32 TaskId = 13;
    uint32 Bit = 5;

    START();

    SB_ResetUnitTest();

    /* Clear task bits, then call function, which should set the bit for
     * the specified task
     */
    CFE_SB.StopRecurseFlags[TaskId] = 0x0000;
    ASSERT_EQ(CFE_SB_RequestToSendEvent(TaskId, Bit), CFE_SB_GRANTED);

    /* Call the function a second time; the result should indicate that the
     * bit is already set
     */
    ASSERT_EQ(CFE_SB_RequestToSendEvent(TaskId, Bit), CFE_SB_DENIED);

    EVTCNT(0);

    REPORT();
} /* end Test_ReqToSendEvent_ErrLogic */

/*
** Test getting a destination descriptor to the SB memory pool using a null
** destination pointer
*/
void Test_PutDestBlk_ErrLogic(void)
{
    START();

    SB_ResetUnitTest();
    ASSERT_EQ(CFE_SB_PutDestinationBlk(NULL), CFE_SB_BAD_ARGUMENT);

    EVTCNT(0);

    REPORT();
} /* end Test_PutDestBlk_ErrLogic */

/*
** Test internal function to get the pipe table index for the given pipe ID
*/
void Test_CFE_SB_GetPipeIdx(void)
{
    START();

    SB_ResetUnitTest();
    CFE_SB.PipeTbl[0].PipeId = 0;
    CFE_SB.PipeTbl[0].InUse = CFE_SB_NOT_IN_USE;
    ASSERT_EQ(CFE_SB_GetPipeIdx(0), CFE_SB_INVALID_PIPE);

    EVTCNT(0);

    REPORT();
} /* end Test_CFE_SB_GetPipeIdx */

/*
** Test functions that involve a buffer in the SB buffer pool
*/
void Test_CFE_SB_Buffers(void)
{
    int32 ExpRtn;

    CFE_SB_BufferD_t *bd;

    START();

    SB_ResetUnitTest();
    CFE_SB.StatTlmMsg.Payload.MemInUse = 0;
    CFE_SB.StatTlmMsg.Payload.PeakMemInUse = sizeof(CFE_SB_BufferD_t) * 4;
    bd = CFE_SB_GetBufferFromPool(0, 0);

    ASSERT_EQ(CFE_SB.StatTlmMsg.Payload.PeakMemInUse, sizeof(CFE_SB_BufferD_t) * 4);

    EVTCNT(0);

    REPORT();

    START();

    ExpRtn = CFE_SB.StatTlmMsg.Payload.SBBuffersInUse;
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_PutPoolBuf), 1, -1);
    CFE_SB_ReturnBufferToPool(bd);
    ASSERT_EQ(CFE_SB.StatTlmMsg.Payload.SBBuffersInUse, ExpRtn);

    EVTCNT(0);

    REPORT();

    START();

    bd->UseCount = 0;
    CFE_SB_DecrBufUseCnt(bd);
    ASSERT_EQ(bd->UseCount, 0);

    EVTCNT(0);

    REPORT();

    START();

    UT_SetDeferredRetcode(UT_KEY(CFE_ES_PutPoolBuf), 1, -1);
    CFE_SB.StatTlmMsg.Payload.MemInUse = 0;
    CFE_SB_PutDestinationBlk((CFE_SB_DestinationD_t *) bd);

    ASSERT_EQ(CFE_SB.StatTlmMsg.Payload.MemInUse, 0);

    EVTCNT(0);

    REPORT();
} /* end Test_CFE_SB_Buffers */

/*
** Test internal function to get the pipe table index for the given pipe ID
*/
void Test_CFE_SB_BadPipeInfo(void)
{
    CFE_SB_PipeId_t PipeId;
    uint16          PipeDepth = 10;
    CFE_SB_Qos_t    CFE_SB_Default_Qos;

    START();

    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe1"));

    /* Set the pipe ID to an erroneous value and attempt to delete the pipe */
    CFE_SB.PipeTbl[0].PipeId = 1;
    CFE_SB.PipeTbl[0].InUse = 1;
    ASSERT_EQ(CFE_SB_DeletePipeFull(0, 0), CFE_SB_BAD_ARGUMENT);

    EVTCNT(2);

    REPORT();

    START();

    /* Reset the pipe ID and delete the pipe */
    CFE_SB.PipeTbl[0].PipeId = 0;

    ASSERT_EQ(CFE_SB_SubscribeFull(0 ,0, CFE_SB_Default_Qos,
                                  CFE_PLATFORM_SB_DEFAULT_MSG_LIMIT, 2), CFE_SB_BAD_ARGUMENT);

    EVTCNT(4);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();

} /* end Test_CFE_SB_BadPipeInfo */
/*
** Test send housekeeping information command
*/

void Test_SB_SendMsgPaths(void)
{
    CFE_SB_CmdHdr_t  NoParamCmd;
    CFE_SB_MsgId_t   MsgId;
    CFE_SB_PipeId_t  PipeId;
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t) &TlmPkt;
    int32            PipeDepth = 2;

    START();

    /* Test inhibiting sending a "no subscriptions for a message ID" message */
    SB_ResetUnitTest();
    CFE_SB_InitMsg(&NoParamCmd, CFE_SB_SEND_HK_MID,
                   sizeof(NoParamCmd), true);
    CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t) &NoParamCmd;
    CFE_SB.StopRecurseFlags[1] |= CFE_BIT(CFE_SB_SEND_NO_SUBS_EID_BIT);
    CFE_SB_ProcessCmdPipePkt();

    ASSERT_TRUE(!UT_EventIsInHistory(CFE_SB_SEND_NO_SUBS_EID));

    CFE_SB.HKTlmMsg.Payload.MsgSendErrorCounter = 0;
    CFE_SB.StopRecurseFlags[1] |= CFE_BIT(CFE_SB_GET_BUF_ERR_EID_BIT);
    MsgId = CFE_SB_GetMsgId((CFE_SB_MsgPtr_t) &CFE_SB.HKTlmMsg);
    CFE_SB.MsgMap[CFE_SB_MsgKeyToValue(CFE_SB_ConvertMsgIdtoMsgKey(MsgId))] = CFE_SB_INVALID_ROUTE_IDX;
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_GetPoolBuf), 1, CFE_ES_ERR_MEM_BLOCK_SIZE);
    CFE_SB_ProcessCmdPipePkt();
    ASSERT_EQ(CFE_SB.HKTlmMsg.Payload.MsgSendErrorCounter, 0);

    ASSERT_TRUE(!UT_EventIsInHistory(CFE_SB_GET_BUF_ERR_EID));

    EVTCNT(0);

    CFE_SB.StopRecurseFlags[1] = 0;

    /* Create a message ID with the command bit set and disable reporting */
    MsgId = SB_UT_CMD_MID;
    SB_ResetUnitTest();
    CFE_SB.SenderReporting = 0;
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));
    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), true);
    ASSERT(CFE_SB_SendMsg(TlmPktPtr));

    EVTCNT(3);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();

    START();

    /* Test inhibiting sending a "message ID limit error" message */
    SB_ResetUnitTest();
    MsgId = SB_UT_TLM_MID;
    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), false);
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "MsgLimTestPipe"));

    /* Set maximum allowed messages on the pipe at one time to 1 */
    SETUP(CFE_SB_SubscribeEx(MsgId, PipeId, CFE_SB_Default_Qos, 1));

    /* First send should pass */
    ASSERT(CFE_SB_SendMsg(TlmPktPtr));

    CFE_SB.StopRecurseFlags[1] |= CFE_BIT(CFE_SB_MSGID_LIM_ERR_EID_BIT);
    ASSERT(CFE_SB_SendMsg(TlmPktPtr));
    CFE_SB.StopRecurseFlags[1] = 0;

    ASSERT_TRUE(!UT_EventIsInHistory(CFE_SB_MSGID_LIM_ERR_EID));

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();

    START();

    /* Test inhibiting sending a "pipe full" message */
    SB_ResetUnitTest();
    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), true);
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "PipeFullTestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));

    /* This send should pass */
    ASSERT(CFE_SB_SendMsg(TlmPktPtr));

    /* Tell the QueuePut stub to return OS_QUEUE_FULL on its next call */
    UT_SetDeferredRetcode(UT_KEY(OS_QueuePut), 1, OS_QUEUE_FULL);
    CFE_SB.StopRecurseFlags[1] |= CFE_BIT(CFE_SB_Q_FULL_ERR_EID_BIT);
    ASSERT(CFE_SB_SendMsg(TlmPktPtr));
    CFE_SB.StopRecurseFlags[1] = 0;

    ASSERT_TRUE(!UT_EventIsInHistory(CFE_SB_Q_FULL_ERR_EID_BIT));

    EVTCNT(3);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();

    START();

    /* Test inhibiting sending a "pipe write error" message */
    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "TestPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));
    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), false);
    UT_SetDeferredRetcode(UT_KEY(OS_QueuePut), 1, OS_ERROR);
    CFE_SB.StopRecurseFlags[1] |= CFE_BIT(CFE_SB_Q_WR_ERR_EID_BIT);
    ASSERT(CFE_SB_SendMsg(TlmPktPtr));
    CFE_SB.StopRecurseFlags[1] = 0;
    ASSERT(CFE_SB_SendMsg(TlmPktPtr));

    EVTCNT(3);

    ASSERT_TRUE(!UT_EventIsInHistory(CFE_SB_Q_WR_ERR_EID));

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();

    START();

    /* Setup Test skipping sending to a pipe when the pipe option is set to ignore */
    SB_ResetUnitTest();
    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), true);
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "SkipPipe"));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));
    SETUP(CFE_SB_SetPipeOpts(PipeId, CFE_SB_PIPEOPTS_IGNOREMINE));

    /* Test skipping this pipe and the send should pass */
    ASSERT(CFE_SB_SendMsg(TlmPktPtr));

    TEARDOWN(CFE_SB_SetPipeOpts(PipeId, 0));
    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_SB_SendMsgPaths */

/*
** Test receiving a message response to a unsubscribing to message, then
** resubscribing to it while it's in the pipe
*/
void Test_RcvMsg_UnsubResubPath(void)
{
    CFE_SB_MsgPtr_t  PtrToMsg;
    CFE_SB_MsgId_t   MsgId = SB_UT_TLM_MID;
    CFE_SB_PipeId_t  PipeId;
    SB_UT_Test_Tlm_t TlmPkt;
    CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t) &TlmPkt;
    uint32           PipeDepth = 10;

    START();
    SB_ResetUnitTest();
    SETUP(CFE_SB_CreatePipe(&PipeId, PipeDepth, "RcvMsgTestPipe"));
    CFE_SB_InitMsg(&TlmPkt, MsgId, sizeof(TlmPkt), true);
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));
    SETUP(CFE_SB_SendMsg(TlmPktPtr));

    SETUP(CFE_SB_Unsubscribe(MsgId, PipeId));
    SETUP(CFE_SB_Subscribe(MsgId, PipeId));
    ASSERT(CFE_SB_RcvMsg(&PtrToMsg, PipeId, CFE_SB_PEND_FOREVER));

    ASSERT_TRUE(PtrToMsg != NULL);

    EVTCNT(6);

    EVTSENT(CFE_SB_SUBSCRIPTION_RCVD_EID);

    TEARDOWN(CFE_SB_DeletePipe(PipeId));

    REPORT();
} /* end Test_RcvMsg_UnsubResubPath */

/*
** Test the paths through the MessageStringSet and MessageStringGet functions
*/
void Test_MessageString(void)
{
	const char *SrcString = "abcdefg";
	char DestString[20];
	char *DestStringPtr = DestString;
	const char *DefString = "default";

    SB_ResetUnitTest();

    /* Test setting string where the destination size > source string size */
	CFE_SB_MessageStringSet(DestStringPtr, SrcString, sizeof(DestString),
			                strlen(SrcString));
    UT_Report(__FILE__, __LINE__,
              strcmp(DestString, SrcString) == 0, "CFE_SB_MessageStringSet",
              "Destination size > source string size");

	/* Test setting string where the source string is empty */
	CFE_SB_MessageStringSet(DestStringPtr, "", sizeof(DestString),
			                strlen(SrcString));
    UT_Report(__FILE__, __LINE__,
    		  strcmp(DestString, SrcString) != 0, "CFE_SB_MessageStringSet",
              "Empty source string");

	/* Test setting string where the destination size < source string size */
	CFE_SB_MessageStringSet(DestStringPtr, SrcString, strlen(SrcString) - 1,
			                strlen(SrcString));
    UT_Report(__FILE__, __LINE__,
    		  strncmp(DestString, SrcString, strlen(SrcString) - 1) == 0,
    		  "CFE_SB_MessageStringSet",
              "Destination size < source string size");

	/* Test getting string where the destination size > source string size */
	CFE_SB_MessageStringGet(DestStringPtr, SrcString, DefString,
			                sizeof(DestString), strlen(SrcString));
    UT_Report(__FILE__, __LINE__,
    		  strcmp(DestString, SrcString) == 0, "CFE_SB_MessageStringGet",
              "Destination size > source string size");

	/* Test getting string where the destination size is zero */
    DestString[0] = '\0';
	CFE_SB_MessageStringGet(DestStringPtr, SrcString, DefString, 0,
			                strlen(SrcString));
    UT_Report(__FILE__, __LINE__,
            strcmp(DestString, SrcString) != 0, "CFE_SB_MessageStringGet",
              "Destination size = 0");

	/* Test getting string where the default string is NULL */
	CFE_SB_MessageStringGet(DestStringPtr, SrcString, NULL,
			                sizeof(DestString), 0);
    UT_Report(__FILE__, __LINE__,
    		  strcmp(DefString, SrcString) != 0, "CFE_SB_MessageStringGet",
              "Default string = NULL");

	/* Test getting string where the source string size is zero */
	CFE_SB_MessageStringGet(DestStringPtr, SrcString, DefString,
			                sizeof(DestString), 0);
    UT_Report(__FILE__, __LINE__,
    		strcmp(DestString, SrcString) != 0, "CFE_SB_MessageStringGet",
              "Source string size = 0");

	/* Test getting string where the destination size < source string size */
    DestString[0] = '\0';
	CFE_SB_MessageStringGet(DestStringPtr, SrcString, DefString,
			                strlen(SrcString) - 1, strlen(SrcString));
    UT_Report(__FILE__, __LINE__,
    		  strncmp(DestString, SrcString, strlen(DestString)) == 0,
    		  "CFE_SB_MessageStringGet",
              "Destination size < source string size");
} /* end Test_MessageString */
