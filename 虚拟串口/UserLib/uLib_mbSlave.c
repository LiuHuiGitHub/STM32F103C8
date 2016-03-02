#include "typedef.h"
#include "uLib_mbSlave.h"
#include "uLib_mbSlave_Itf.h"
#include "uLib_crc16rtu.h"
#include "string.h"

/*
Modbus Format

host cmd format
********************************************************************
fun		  | addr | fun | regh | regl | datah | datal | crc8l | crc8h |
********************************************************************
size	  |   1  |  1  |   1  |   1  |   1   |   1   |   1   |   1   |
********************************************************************

***************************************************************************************
fun		  | addr | fun | regh | regl | datah | datal | size | data0...n | crc8l | crc8h |
***************************************************************************************
size	  |   1  |  1  |   1  |   1  |   1   |   1   |   1  |     n     |   1   |   1   |
***************************************************************************************

slave data format
************************************************************
fun		  | addr | fun | dataLen | data0...n | crc8l | crc8h |
************************************************************
size	  |   1  |  1  |    1    |     n     |   1   |   1   |
************************************************************
*/
/* Typedef */
typedef enum
{
    MB_NO_ERR                   = 0x00,
    MB_ERR_FUNCTION             = 0x01,
    MB_ERR_ADDR                 = 0x02,
    MB_ERR_VALUE                = 0x03,
    MB_ERR_SLAVE_FAULT          = 0x04,
    MB_ERR_ACK                  = 0x05,
    MB_ERR_BUSY                 = 0x06,
    MB_ERR_MEMORY_PARITY        = 0x08,
    MB_ERR_GATEWAY_PATH         = 0x0A,
    MB_ERR_GATEWAY_RSP_FAULT    = 0x0B,
} MB_ERRPR;

typedef enum
{
    MB_FUN_READ_COILS               = 0x01,
    MB_FUN_READ_INPUT_COILS         = 0x02,
    MB_FUN_READ_HOLDING_REGISTERS   = 0x03,
    MB_FUN_READ_INPUT_REGISTERS     = 0x04,

    MB_FUN_WRITE_SIGNAL_COILS       = 0x05,
    MB_FUN_WRITE_SIGNAL_REGISTERS   = 0x06,
    MB_FUN_WRITE_MULTIPLE_COILS     = 0x0F,
    MB_FUN_WRITE_MULTIPLE_REGISTERS = 0x10,
	MB_FUN_WRITE_MASK_REGISTERS		= 0x16,
} MB_FUNCTION;

/* Config */
#define MB_REQ_ADU_MAX_LEN          256
#define MB_RSP_ADU_MAX_LEN          256

/* define */
#define COIL_OFF                0
#define COIL_ON                 1

#define MB_REQ_ADDR             *(mb_req_adu+0)
#define MB_REQ_FUN              *(mb_req_pdu+0)
#define MB_REQ_START_ADDR       ((UINT16)*(mb_req_pdu+1)<<8|*(mb_req_pdu+2))
#define MB_REQ_VALUE            ((UINT16)*(mb_req_pdu+3)<<8|*(mb_req_pdu+4))
#define MB_REQ_SIZE             *(mb_req_pdu+5)
#define MB_REQ_DATA_ADDR		(mb_req_pdu+6)
#define MB_REQ_AND_MASK			((UINT16)*(mb_req_pdu+3)<<8|*(mb_req_pdu+4))
#define MB_REQ_OR_MASK			((UINT16)*(mb_req_pdu+5)<<8|*(mb_req_pdu+6))

#define MB_RSP_ADDR				*(mb_rsp+0)
#define MB_RSP_FUN				*(mb_rsp_pdu)
#define MB_RSP_READ_SIZE		*(mb_rsp_pdu+1)
#define MB_RSP_READ_DATA_ADDR	(mb_rsp_pdu+2)
#define MB_RSP_WRITE_ADDRH		*(mb_rsp_pdu+1)
#define MB_RSP_WRITE_ADDRL		*(mb_rsp_pdu+2)
#define MB_RSP_WRITE_DATAH		*(mb_rsp_pdu+3)
#define MB_RSP_WRITE_DATAL		*(mb_rsp_pdu+4)
#define MB_RSP_WRITE_AND_MASKH	*(mb_rsp_pdu+3)
#define MB_RSP_WRITE_AND_MASKL	*(mb_rsp_pdu+4)
#define MB_RSP_WRITE_OR_MASKH	*(mb_rsp_pdu+5)
#define MB_RSP_WRITE_OR_MASKL	*(mb_rsp_pdu+6)
#define MB_RSP_CRC16H			*(mb_rsp_pdu+mb_rsp_pdu_len+1)
#define MB_RSP_CRC16L			*(mb_rsp_pdu+mb_rsp_pdu_len)
#define MB_RSP_PDU_LEN			(mb_rsp_pdu_len)
#define MB_RSP_ADU_LEN			(MB_RSP_PDU_LEN+3)
#define MB_RSP_ERROR_STATE		*(mb_rsp_pdu+1)

#define MB_EXCEP_RSP_PDU_LEN	2
#define MB_EXCEP_RSP_CRC16H		*(mb_rsp_pdu+MB_EXCEP_RSP_PDU_LEN+1)
#define MB_EXCEP_RSP_CRC16L		*(mb_rsp_pdu+MB_EXCEP_RSP_PDU_LEN)
#define MB_EXCEP_RSP_ADU_LEN	(MB_EXCEP_RSP_PDU_LEN+3)

volatile UINT8 SlaveAddr = 0x01;

/*Receive*/
static UINT8 mb_req[MB_REQ_ADU_MAX_LEN] = {0};
static const UINT8 *const mb_req_adu = mb_req;
static const UINT8 *const mb_req_pdu = mb_req+1;
static UINT8 mb_req_adu_len = 0;
/*Transmit*/
static UINT8 mb_rsp[MB_RSP_ADU_MAX_LEN] = {0};
static UINT8 *const mb_rsp_pdu = mb_rsp+1;
static UINT8 mb_rsp_pdu_len = 0;


#define MB_COILS_NUMBER          0x64
UINT8 Coils[MB_COILS_NUMBER] = {0,1,0,1,1,1,0,0,1,1,0,1,0};
MB_ERRPR FUN_ReadSignalCoils(UINT16 addr, UINT16 value)
{
	UINT8 i;
	MB_RSP_READ_SIZE = ((UINT8)value+7)/8;
	mb_rsp_pdu_len = 2+MB_RSP_READ_SIZE;
	for(i=0; i<value; i++)
	{
		if(Coils[i] != COIL_OFF)
		{
			*(MB_RSP_READ_DATA_ADDR+i/8) |= 1<<(i%8);
		}
		else
		{
			*(MB_RSP_READ_DATA_ADDR+i/8) &= ~(1<<(i%8));
		}
	}
    return MB_NO_ERR;
}
MB_ERRPR FUN_WriteSignalCoils(UINT16 addr, UINT16 value)
{
	MB_RSP_WRITE_ADDRH = (UINT8)(addr>>8);
	MB_RSP_WRITE_ADDRL = (UINT8)(addr);
	MB_RSP_WRITE_DATAH = (UINT8)(value>>8);
	MB_RSP_WRITE_DATAL = (UINT8)(value);
	mb_rsp_pdu_len = 5;
	if(value == 0xFF00)
	{
		Coils[addr] = COIL_ON;
	}
	else
	{
		Coils[addr] = COIL_OFF;
	}
    return MB_NO_ERR;
}
MB_ERRPR FUN_WriteMultipleCoils(UINT16 addr, UINT8 *dat, UINT8 len)
{
	UINT8 i;
	MB_RSP_WRITE_ADDRH = (UINT8)(addr>>8);
	MB_RSP_WRITE_ADDRL = (UINT8)(addr);
	MB_RSP_WRITE_DATAH = (UINT8)(len>>8);
	MB_RSP_WRITE_DATAL = (UINT8)(len);
	mb_rsp_pdu_len = 5;
	for(i=0; i<len; i++)
	{
		if(dat[i/8] & (1<<(i%8)))
		{
			Coils[addr+i] = COIL_ON;
		}
		else
		{
			Coils[addr+i] = COIL_OFF;
		}
	}
    return MB_NO_ERR;
}


#define MB_INPUT_COILS_NUMBER           0x64
UINT8 InputCoils[MB_INPUT_COILS_NUMBER] = {0};
MB_ERRPR FUN_ReadInputCoils(UINT16 addr, UINT16 value)
{
	UINT8 i;
	MB_RSP_READ_SIZE = ((UINT8)value+7)/8;
	mb_rsp_pdu_len = 2+MB_RSP_READ_SIZE;
	for(i=0; i<value; i++)
	{
		if(InputCoils[i] != COIL_OFF)
		{
			*(MB_RSP_READ_DATA_ADDR+i/8) |= 1<<(i%8);
		}
		else
		{
			*(MB_RSP_READ_DATA_ADDR+i/8) &= ~(1<<(i%8));
		}
	}
    return MB_NO_ERR;
}

#define MB_HOLDING_REGISTERS_NUMBER       0x7D
UINT16 HoldingRegisters[MB_HOLDING_REGISTERS_NUMBER] = {0x12,0x34,0x56,0x78,0x90,0x9,0x87,0x65,0x43,0x21,0x13,0x14,0x15,0x16,0x17};
MB_ERRPR FUN_ReadHoldingRegisters(UINT16 addr, UINT16 value)
{
	UINT8 i;
	MB_RSP_READ_SIZE = value*2;
	mb_rsp_pdu_len = 2+MB_RSP_READ_SIZE;
	for(i=0; i<value; i++)
	{
		*(UINT8*)(MB_RSP_READ_DATA_ADDR+2*i) = (UINT8)(HoldingRegisters[addr+i]>>8);
		*(UINT8*)(MB_RSP_READ_DATA_ADDR+2*i+1) = (UINT8)HoldingRegisters[addr+i];
	}
    return MB_NO_ERR;
}
MB_ERRPR FUN_WriteHoldingSignalRegisters(UINT16 addr, UINT16 value)
{
	MB_RSP_WRITE_ADDRH = (UINT8)(addr>>8);
	MB_RSP_WRITE_ADDRL = (UINT8)(addr);
	MB_RSP_WRITE_DATAH = (UINT8)(value>>8);
	MB_RSP_WRITE_DATAL = (UINT8)(value);
	HoldingRegisters[addr] = value;
	mb_rsp_pdu_len = 5;
    return MB_NO_ERR;
}
MB_ERRPR FUN_WriteMultipleHoldingRegisters(UINT16 addr, UINT8 *dat, UINT8 len)
{
	UINT8 i;
	MB_RSP_WRITE_ADDRH = (UINT8)(addr>>8);
	MB_RSP_WRITE_ADDRL = (UINT8)(addr);
	MB_RSP_WRITE_DATAH = (UINT8)(len>>8);
	MB_RSP_WRITE_DATAL = (UINT8)(len);
	mb_rsp_pdu_len = 5;
	for(i=0; i<len; i+=2)
	{
		HoldingRegisters[addr++] = (UINT16)dat[i]<<8 | dat[i+1];
	}
    return MB_NO_ERR;
}
MB_ERRPR FUN_WriteMaskRegisters(UINT16 addr, UINT16 andMadk, UINT16 orMask)
{
	MB_RSP_WRITE_ADDRH = (UINT8)(addr>>8);
	MB_RSP_WRITE_ADDRL = (UINT8)(addr);
	MB_RSP_WRITE_AND_MASKH = (UINT8)(andMadk>>8);
	MB_RSP_WRITE_AND_MASKL = (UINT8)(andMadk);
	MB_RSP_WRITE_OR_MASKH = (UINT8)(orMask>>8);
	MB_RSP_WRITE_OR_MASKL = (UINT8)(orMask);
	mb_rsp_pdu_len = 7;
	HoldingRegisters[addr] = (HoldingRegisters[addr]&andMadk) | (~andMadk&orMask);
    return MB_NO_ERR;
}

#define MB_INPUT_REGISTERS_NUMBER       0x7D
UINT16 InputRegisters[MB_INPUT_REGISTERS_NUMBER] = {12,1,2,3,4,5,6,7,8,9};
MB_ERRPR FUN_ReadInputRegisters(UINT16 addr, UINT16 value)
{
	UINT8 i;
	MB_RSP_READ_SIZE = value*2;
	mb_rsp_pdu_len = 2+MB_RSP_READ_SIZE;
	for(i=0; i<value; i++)
	{
		*(UINT8*)(MB_RSP_READ_DATA_ADDR+2*i) = (UINT8)(InputRegisters[addr+i]>>8);
		*(UINT8*)(MB_RSP_READ_DATA_ADDR+2*i+1) = (UINT8)InputRegisters[addr+i];
	}
    return MB_NO_ERR;
}

void uLib_mbSlaveInit(UINT8 addr)
{
    SlaveAddr = addr;
	uLib_mbSlaveItfInit();
    uLib_mbSlaveItfReceiveCtrl(TRUE);
}

void uLib_mbReadReqADU(UINT8 *dat, UINT16 len)
{
    if(len <= MB_REQ_ADU_MAX_LEN)
    {
        mb_req_adu_len = len;
        memcpy(mb_req, dat, mb_req_adu_len);
    }
}
void uLib_mbRspPDU(void)
{
	UINT16 crc;
	crc = uLib_crc16rtu(mb_rsp, mb_rsp_pdu_len+1);
	MB_RSP_CRC16H = (UINT8)(crc>>8);
	MB_RSP_CRC16L = (UINT8)(crc);
	uLib_mbSlaveItfTransmitData(mb_rsp, MB_RSP_ADU_LEN);
}

void uLib_mbExcepRspPDU(void)
{
	UINT16 crc;
	crc = uLib_crc16rtu(mb_rsp, 4);
	MB_EXCEP_RSP_CRC16H = (UINT8)(crc>>8);
	MB_EXCEP_RSP_CRC16L = (UINT8)(crc);
	uLib_mbSlaveItfTransmitData(mb_rsp, MB_RSP_ADU_LEN);
}

MB_ERRPR uLib_mbMessage(void)
{
    UINT16 u16_StartAddr = MB_REQ_START_ADDR;
    UINT16 u16_Value = MB_REQ_VALUE;
    MB_ERRPR errorState = MB_NO_ERR;
    switch(MB_REQ_FUN)
    {
        case MB_FUN_READ_COILS:
        {
            /*校验输出线圈数*/
            if(u16_Value < 0x0001
				|| u16_Value > MB_COILS_NUMBER
              )
            {
                errorState =  MB_ERR_VALUE;
                break;
            }
            /*校验起始地址*/
            if(u16_StartAddr + u16_Value > MB_COILS_NUMBER)
            {
                errorState =  MB_ERR_ADDR;
                break;
            }
            errorState =  FUN_ReadSignalCoils(u16_StartAddr, u16_Value);
            break;
        }

        case MB_FUN_READ_INPUT_COILS:
        {
            /*校验输入线圈数*/
            if(u16_Value < 0x0001
				|| u16_Value > MB_INPUT_COILS_NUMBER
              )
            {
                errorState =  MB_ERR_VALUE;
                break;
            }
            /*校验起始地址*/
            if(u16_StartAddr + u16_Value > MB_INPUT_COILS_NUMBER)
            {
                errorState =  MB_ERR_ADDR;
                break;
            }
            errorState =  FUN_ReadInputCoils(u16_StartAddr, u16_Value);
            break;
        }

        case MB_FUN_READ_HOLDING_REGISTERS:
        {
            /*校验保持寄存器数*/
            if(u16_Value < 0x0001
				|| u16_Value > MB_HOLDING_REGISTERS_NUMBER
              )
            {
                errorState =  MB_ERR_VALUE;
                break;
            }
            /*校验起始地址*/
            if(u16_StartAddr + u16_Value > MB_HOLDING_REGISTERS_NUMBER)
            {
                errorState =  MB_ERR_ADDR;
                break;
            }
            errorState =  FUN_ReadHoldingRegisters(u16_StartAddr, u16_Value);
            break;
        }

        case MB_FUN_READ_INPUT_REGISTERS:
        {
            /*校验输入寄存器数*/
            if(u16_Value < 0x0001
				|| u16_Value > MB_INPUT_REGISTERS_NUMBER
              )
            {
                errorState =  MB_ERR_VALUE;
                break;
            }
            /*校验起始地址*/
            if(u16_StartAddr + u16_Value > MB_INPUT_REGISTERS_NUMBER)
            {
                errorState =  MB_ERR_ADDR;
                break;
            }
            errorState =  FUN_ReadInputRegisters(u16_StartAddr, u16_Value);
            break;
        }

        case MB_FUN_WRITE_SIGNAL_COILS:
        {
            /*校验线圈值*/
            if(u16_Value != 0xFF00
				&& u16_Value != 0x00
              )
            {
                errorState =  MB_ERR_VALUE;
                break;
            }
            /*校验起始地址*/
            if(u16_StartAddr > MB_COILS_NUMBER)
            {
                errorState =  MB_ERR_ADDR;
                break;
            }
            errorState =  FUN_WriteSignalCoils(u16_StartAddr, u16_Value);
            break;
        }

        case MB_FUN_WRITE_SIGNAL_REGISTERS:
        {
            /*校验保持寄存器数*/
            if(u16_Value > 0xFFFF)
            {
                errorState =  MB_ERR_VALUE;
                break;
            }
            /*校验起始地址*/
            if(u16_StartAddr > MB_HOLDING_REGISTERS_NUMBER)
            {
                errorState =  MB_ERR_ADDR;
                break;
            }
            errorState =  FUN_WriteHoldingSignalRegisters(u16_StartAddr, u16_Value);
            break;
        }

        case MB_FUN_WRITE_MULTIPLE_COILS:
        {
            /*校验线圈数*/
            if(u16_Value > MB_COILS_NUMBER
                || u16_Value < 0x0001
                || MB_REQ_SIZE != ((UINT8)u16_Value+7)/8
              )
            {
                errorState =  MB_ERR_VALUE;
                break;
            }
            /*校验起始地址*/
            if(u16_StartAddr + u16_Value > MB_COILS_NUMBER)
            {
                errorState =  MB_ERR_ADDR;
                break;
            }
            errorState =  FUN_WriteMultipleCoils(u16_StartAddr, (UINT8*)MB_REQ_DATA_ADDR, u16_Value);
            break;
        }

        case MB_FUN_WRITE_MULTIPLE_REGISTERS:
        {
            /*校验寄存器数*/
            if(u16_Value > MB_HOLDING_REGISTERS_NUMBER
                || u16_Value < 0x0001
                || MB_REQ_SIZE != u16_Value * 2
              )
            {
                errorState =  MB_ERR_VALUE;
                break;
            }
            /*校验起始地址*/
            if(u16_StartAddr + u16_Value > MB_HOLDING_REGISTERS_NUMBER)
            {
                errorState =  MB_ERR_ADDR;
                break;
            }
            errorState =  FUN_WriteMultipleHoldingRegisters(u16_StartAddr, (UINT8*)MB_REQ_DATA_ADDR, MB_REQ_SIZE);
            break;
        }
			
		case MB_FUN_WRITE_MASK_REGISTERS:
		{
			/*校验起始地址*/
			if(u16_StartAddr > MB_HOLDING_REGISTERS_NUMBER)
			{
				errorState =  MB_ERR_ADDR;
				break;
			}			
			errorState =  FUN_WriteMaskRegisters(u16_StartAddr, MB_REQ_AND_MASK, MB_REQ_OR_MASK);
			break;
		}
			
        default:
            errorState =  MB_ERR_FUNCTION;
            break;
    }
    
	MB_RSP_ADDR = SlaveAddr;
    MB_RSP_FUN = MB_REQ_FUN;
    if(errorState)
    {
        MB_RSP_FUN |= 0x80;
    	MB_RSP_ERROR_STATE = errorState;
    }
    
    return errorState;
}

void uLib_mbSlaveHandler(void)
{
    MB_ERRPR errorState;
    
    uLib_mbSlaveItfHandler();
    
    if(mb_req_adu_len)
    {
        uLib_mbSlaveItfReceiveCtrl(FALSE);
        
        if(MB_REQ_ADDR == SlaveAddr
            && uLib_crc16rtu((UINT8*)mb_req_adu, mb_req_adu_len) == 0x00
          )
        {
            errorState = uLib_mbMessage();
            if(errorState == MB_NO_ERR)
            {
                uLib_mbRspPDU();
            }
            else
            {
                uLib_mbExcepRspPDU();
            }
        }
        uLib_mbSlaveItfReceiveCtrl(TRUE);
        mb_req_adu_len = 0;
    }
}

