/*
  程序说明: CT117E-M4嵌入式竞赛板GPIO模拟I2C总线驱动程序
  软件环境: MDK-ARM HAL库
  硬件环境: CT117E-M4嵌入式竞赛板
  日    期: 2020-3-1
*/

#include "i2c_hal.h"

#define DELAY_TIME	20

/**
  * @brief SDA线输入模式配置
  * @param None
  * @retval None
  */
void SDA_Input_Mode()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief SDA线输出模式配置
  * @param None
  * @retval None
  */
void SDA_Output_Mode()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief SDA线输出一个位
  * @param val 输出的数据
  * @retval None
  */
void SDA_Output( uint16_t val )
{
    if ( val )
    {
        GPIOB->BSRR |= GPIO_PIN_7;
    }
    else
    {
        GPIOB->BRR |= GPIO_PIN_7;
    }
}

/**
  * @brief SCL线输出一个位
  * @param val 输出的数据
  * @retval None
  */
void SCL_Output( uint16_t val )
{
    if ( val )
    {
        GPIOB->BSRR |= GPIO_PIN_6;
    }
    else
    {
        GPIOB->BRR |= GPIO_PIN_6;
    }
}

/**
  * @brief SDA输入一位
  * @param None
  * @retval GPIO读入一位
  */
uint8_t SDA_Input(void)
{
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET){
		return 1;
	}else{
		return 0;
	}
}


/**
  * @brief I2C的短暂延时
  * @param None
  * @retval None
  */
static void delay1(unsigned int n)
{
    uint32_t i;
    for ( i = 0; i < n; ++i);
}

/**
  * @brief I2C起始信号
  * @param None
  * @retval None
  */
void I2CStart(void)
{
    SDA_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SDA_Output(0);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);
}

/**
  * @brief I2C结束信号
  * @param None
  * @retval None
  */
void I2CStop(void)
{
    SCL_Output(0);
    delay1(DELAY_TIME);
    SDA_Output(0);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SDA_Output(1);
    delay1(DELAY_TIME);

}

/**
  * @brief I2C等待确认信号
  * @param None
  * @retval None
  */
unsigned char I2CWaitAck(void)
{
    unsigned short cErrTime = 5;
    SDA_Input_Mode();
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    while(SDA_Input())
    {
        cErrTime--;
        delay1(DELAY_TIME);
        if (0 == cErrTime)
        {
            SDA_Output_Mode();
            I2CStop();
            return ERROR;
        }
    }
    SDA_Output_Mode();
    SCL_Output(0);
    delay1(DELAY_TIME);
    return SUCCESS;
}

/**
  * @brief I2C发送确认信号
  * @param None
  * @retval None
  */
void I2CSendAck(void)
{
    SDA_Output(0);
    delay1(DELAY_TIME);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);

}

/**
  * @brief I2C发送非确认信号
  * @param None
  * @retval None
  */
void I2CSendNotAck(void)
{
    SDA_Output(1);
    delay1(DELAY_TIME);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    SCL_Output(0);
    delay1(DELAY_TIME);

}

/**
  * @brief I2C发送一个字节
  * @param cSendByte 需要发送的字节
  * @retval None
  */
void I2CSendByte(unsigned char cSendByte)
{
    unsigned char  i = 8;
    while (i--)
    {
        SCL_Output(0);
        delay1(DELAY_TIME);
        SDA_Output(cSendByte & 0x80);
        delay1(DELAY_TIME);
        cSendByte += cSendByte;
        delay1(DELAY_TIME);
        SCL_Output(1);
        delay1(DELAY_TIME);
    }
    SCL_Output(0);
    delay1(DELAY_TIME);
}

/**
  * @brief I2C接收一个字节
  * @param None
  * @retval 接收到的字节
  */
unsigned char I2CReceiveByte(void)
{
    unsigned char i = 8;
    unsigned char cR_Byte = 0;
    SDA_Input_Mode();
    while (i--)
    {
        cR_Byte += cR_Byte;
        SCL_Output(0);
        delay1(DELAY_TIME);
        delay1(DELAY_TIME);
        SCL_Output(1);
        delay1(DELAY_TIME);
        cR_Byte |=  SDA_Input();
    }
    SCL_Output(0);
    delay1(DELAY_TIME);
    SDA_Output_Mode();
    return cR_Byte;
}

//
void I2CInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = GPIO_PIN_7 | GPIO_PIN_6;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*********************************eeprom单字节读取**********************************/
//读取
unsigned char eeprom_read(unsigned char address)
{
	unsigned char dat;
	I2CStart();
	I2CSendByte(0xa0);	//发送设备地址
	I2CWaitAck();				//等待
	I2CSendByte(address);	//发送读取的地址
	I2CWaitAck();
	I2CStop();
	
	//告知了地址之后就开始读取数据
	//读是 1，		写是	0
	I2CStart();
	I2CSendByte(0xa1);	//发送读取数据命令
	I2CWaitAck();
	
	//接收
	dat=I2CReceiveByte();
	I2CSendNotAck();	//发送应答
	I2CStop();
	return dat;

}

//写函数
void eeprom_write(unsigned char value,unsigned char address)
{
	I2CStart();
	I2CSendByte(0xa0);//发送设备地址
	I2CWaitAck();
	
	I2CSendByte(address);//发送写入地址
	I2CSendAck();				//发送应答
	I2CSendByte(value);
	I2CWaitAck();
	I2CStop();
}
/*******************************END**********************************/


/************************eeprom多字节读取写入**************************/


//写入函数
/*******************************************************************************
* 函数名  : Eeprom_Write_Bytes
* 功能    : eeprom多字节写入
* 输入    : uint8_t addr	--写入的地址		uint8_t* arry--要写入的信息数组	uint8_t n--写入的字节数
* 返回值  : 无
*******************************************************************************/
void Eeprom_Write_Bytes(uint8_t addr,uint8_t* arry ,uint8_t n)
{
	I2CStart();	//开启
	I2CSendByte(0xa0);	//发送设备地址
	I2CWaitAck();				//等待
	I2CSendByte(addr);	//发送片内地址
	I2CWaitAck();				//等待
	
	
	while(n--)
	{
		I2CSendByte(*arry++);
		I2CWaitAck();
	}
	
	
	I2CStop();
}

/*******************************************************************************
* 函数名  : Eeprom_Read_Bytes
* 功能    : eeprom多字节读取
* 输入    : uint8_t addr	--读取的地址		uint8_t* arry--读取到的信息保存的数组	uint8_t n--读取的字节数
* 返回值  : 无
*******************************************************************************/
void Eeprom_Read_Bytes(uint8_t addr,uint8_t* arry ,uint8_t n)
{
	I2CStart();
	I2CSendByte(0xa0);	//确定写入操作
	I2CWaitAck();				//等待
	I2CSendByte(addr);	//发送读取的地址
	I2CWaitAck();
	I2CStop();
	
	
	I2CStart();
	I2CSendByte(0xa1);	//确定读取操作
	I2CWaitAck();				//等待
	while(n--)
	{
		*arry++=I2CReceiveByte();
		//判断是否还有数据需要读取
		if(n)	
		{
			I2CSendAck();
		}
		else
		{
		
			I2CSendNotAck();
		}
	}
	I2CStop();
}












