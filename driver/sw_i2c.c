#include <stdio.h>
// I2C implement
//
// SCL
// SDA
//

bool gpio_state = false;
#define SCL_PIN 
#define SDA_PIN



void set_SDA(void);
void clr_SDA(void);
void set_SCL(void);
void clr_SCL(void);

void i2c_start(void);
void i2c_stop(void);
void i2c_delay(void);


void i2c_write_byte(void);
void i2c_read_byte(void);
void i2c_write_mul_bytes(void);
void i2c_read_mul_bytes(void);

void i2c_delay(void) {
    __nop__();
    __nop__();
    __nop__();
    __nop__();
    __nop__();
    __nop__();
    __nop__();
    __nop__();
}

void i2c_start(void) {
    set_SDA();
    set_SCL();
    i2c_delay();
    clear_SDA();
    i2c_delay();
    clear_SCL();
    i2c_delay();
}

void i2c_stop(void) {
    clear_SDA();
    i2c_delay();
    set_SCL();
    i2c_delay();
    set_SDA();
    i2c_delay();
}




void __i2cSclHigh(void)
{
    unsigned short loop;

    IIC_SCL_HIGH;
    IIC_SCL_IN;
    loop = 0;
    while ((IIC_SCL_STATUS == LOW) && (--loop != 0))
    {
        _nop_();
        _nop_();
    }
    IIC_SCL_OUT;
}

/**************************************************************************//**
 * IIC delay: 6 NOPs.
 ******************************************************************************/
void __i2cDelay(void)
{
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();

}


/**************************************************************************//**
 * Create IIC start pulse.
 ******************************************************************************/
Bool  __i2cStart(void)
{
    UCHAR i;

    IIC_SCL_OUT;
    IIC_SDA_HIGH;
    __i2cSclHigh();

    IIC_SDA_IN;
    __i2cDelay();
    i = 0;

    while ((IIC_SDA_STATUS == LOW) && (i < 9)) //Gen 9 clock
    {
        IIC_SCL_LOW;
        __i2cDelay();
        __i2cSclHigh();
        i++;
    }
    IIC_SDA_OUT;
    IIC_SDA_LOW;
    __i2cDelay();
    IIC_SCL_LOW;
    return TRUE;
}


/**************************************************************************//**
 * Create IIC stop pulse.
 ******************************************************************************/
void __i2cStop(void)
{
    IIC_SDA_LOW;
    __i2cDelay();
    __i2cSclHigh();
    __i2cDelay();
    IIC_SDA_HIGH;
}


/**************************************************************************//**
 * Write one byte data through software IIC channel.
 * @param value Data to be sent.
 * @return ACK or NACK.
 ******************************************************************************/
Bool __i2cWriteByte(BYTE value)
{
    Bool result = ACK;

    UCHAR i = 0;
    code UCHAR Tbl[] = {BIT7, BIT6, BIT5, BIT4, BIT3, BIT2, BIT1, BIT0};

    for (i = 0 ; i < 8 ; i++)
    {
        __i2cDelay();
        IIC_SCL_LOW;
        __i2cDelay();
        if (value & Tbl[i])
        {
            IIC_SDA_HIGH;
        }
        else
        {
            IIC_SDA_LOW;
        }
        __i2cDelay();
        __i2cSclHigh();
    }
    __i2cDelay();
    IIC_SCL_LOW;
    //IICDelay();
    __i2cDelay();
    IIC_SDA_LOW; //Avoid pulse
    IIC_SDA_IN; //Release SDA bus
    IIC_SDA_HIGH;
    //9th clock
    __i2cDelay();
    __i2cSclHigh();
    //   __i2cDelay();
    //	_nop_();
    _nop_();
    _nop_();
    if (IIC_SDA_STATUS == LOW)
    {
        result = ACK;
    }
    else
    {
        i = 7;
        while (IIC_SDA_STATUS && i)
        {
            __i2cDelay();
            i--;
        }
        if (i == 0)
        {
            result = NACK;
        }
        else
        {
            result = ACK;
        }
    }
    IIC_SCL_LOW;
    __i2cDelay();
    IIC_SDA_OUT;
    IIC_SDA_LOW;
    __i2cDelay();
    return result;

}


UCHAR __i2cReadByte(UCHAR ack)
{

    UCHAR i, result = 0;

    ack &= BIT0;
    IIC_SDA_IN;
    IIC_SDA_HIGH;
    for (i = 0; i < 8; i++)
    {
        result <<= 1;
        __i2cSclHigh();
        __i2cDelay();
        if (IIC_SDA_STATUS == LOW)
        {
            result &= 0xFE; //SDA = 0
        }
        else
        {
            result |= 0x01; //SDA = 1
        }
        IIC_SCL_LOW;
        __i2cDelay();
    }
    IIC_SDA_OUT;
    if (ack == ACK)
    {
        IIC_SDA_LOW;
    }
    else
    {
        IIC_SDA_HIGH;
    }
    __i2cDelay();
    __i2cSclHigh();
    __i2cDelay();
    IIC_SCL_LOW;
    __i2cDelay();
    return result;

}

UCHAR DrvI2C_Write(USHRT reg_addr, UINT uCount, UCHAR *pbyBuffer)
{
    ULONG k = 0, i = 0;
    UCHAR Ret = RET_ERROR;

    for (i = 0; i < 8; i++)
    {
        __i2cStart();
        if (__i2cWriteByte(ucSlave) == NACK)
        {
            __i2cStop();
            continue;
        }

        if(ucAddrLen == 1) 
        {
            if (__i2cWriteByte(reg_addr&0xFF) == NACK)
            {
                __i2cStop();
                continue;
            }
        }
        else if(ucAddrLen == 2)
        {
            if (__i2cWriteByte(reg_addr&0xFF) == NACK)
            {
                __i2cStop();
                continue;
            }
            if (__i2cWriteByte((reg_addr>>8)&0xFF) == NACK)
            {
                __i2cStop();
                continue;
            }
        }
        
        for ( k = 0; k < uCount; k++ )
        {
            if (__i2cWriteByte(pbyBuffer[k]) == NACK)
            {
                __i2cStop();
                break;
            }
        }
        if ( k != uCount )
        {
            continue;
        }
        __i2cStop();
        break;
    }
    if ( k == uCount )
    {
        Ret = RET_OK; // return OK
    }
    return Ret;

}

UCHAR DrvI2C_Read(USHRT reg_addr, UCHAR *rData, UCHAR length)
{
    UCHAR i, j;
    UCHAR ack;
    UCHAR Ret = RET_ERROR;

    for (i = 0, j = 0; i < 8; i++)
    {
        __i2cStart();

        if (__i2cWriteByte(ucSlave) == NACK)
        {
            __i2cStop();
            continue;
        }
        if(ucAddrLen == 1) 
        {
            if (__i2cWriteByte(reg_addr&0xFF) == NACK)
            {
                __i2cStop();
                continue;
            }
        }
        else if(ucAddrLen == 2)
        {
            if (__i2cWriteByte(reg_addr&0xFF) == NACK)
            {
                __i2cStop();
                continue;
            }
            if (__i2cWriteByte((reg_addr>>8)&0xFF) == NACK)
            {
                __i2cStop();
                continue;
            }
        }
        __i2cStart();

        if (__i2cWriteByte(ucSlave | I2C_READ) == NACK)
        {
            __i2cStop();
            continue;
        }

        for ( j = 0; j < length; j++ )
        {
            if (j == (length - 1))
            {
                ack = NACK;
            }
            else
            {
                ack = ACK;
            }
            rData[j] = __i2cReadByte(ack);
        }
        __i2cStop();
        break;
    }
    if ( j != 0 )
    {
        Ret = RET_OK;
    }

    return Ret;
}
