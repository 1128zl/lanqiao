/*
  ����˵��: CT117E-M4Ƕ��ʽ������GPIOģ��I2C������������
  �������: MDK-ARM HAL��
  Ӳ������: CT117E-M4Ƕ��ʽ������
  ��    ��: 2020-3-1
*/

#include "i2c_hal.h"

#define DELAY_TIME	20

/**
  * @brief SDA������ģʽ����
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
  * @brief SDA�����ģʽ����
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
  * @brief SDA�����һ��λ
  * @param val ���������
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
  * @brief SCL�����һ��λ
  * @param val ���������
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
  * @brief SDA����һλ
  * @param None
  * @retval GPIO����һλ
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
  * @brief I2C�Ķ�����ʱ
  * @param None
  * @retval None
  */
static void delay1(unsigned int n)
{
    uint32_t i;
    for ( i = 0; i < n; ++i);
}

/**
  * @brief I2C��ʼ�ź�
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
  * @brief I2C�����ź�
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
  * @brief I2C�ȴ�ȷ���ź�
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
  * @brief I2C����ȷ���ź�
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
  * @brief I2C���ͷ�ȷ���ź�
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
  * @brief I2C����һ���ֽ�
  * @param cSendByte ��Ҫ���͵��ֽ�
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
  * @brief I2C����һ���ֽ�
  * @param None
  * @retval ���յ����ֽ�
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

/*********************************eeprom���ֽڶ�ȡ**********************************/
//��ȡ
unsigned char eeprom_read(unsigned char address)
{
	unsigned char dat;
	I2CStart();
	I2CSendByte(0xa0);	//�����豸��ַ
	I2CWaitAck();				//�ȴ�
	I2CSendByte(address);	//���Ͷ�ȡ�ĵ�ַ
	I2CWaitAck();
	I2CStop();
	
	//��֪�˵�ַ֮��Ϳ�ʼ��ȡ����
	//���� 1��		д��	0
	I2CStart();
	I2CSendByte(0xa1);	//���Ͷ�ȡ��������
	I2CWaitAck();
	
	//����
	dat=I2CReceiveByte();
	I2CSendNotAck();	//����Ӧ��
	I2CStop();
	return dat;

}

//д����
void eeprom_write(unsigned char value,unsigned char address)
{
	I2CStart();
	I2CSendByte(0xa0);//�����豸��ַ
	I2CWaitAck();
	
	I2CSendByte(address);//����д���ַ
	I2CSendAck();				//����Ӧ��
	I2CSendByte(value);
	I2CWaitAck();
	I2CStop();
}
/*******************************END**********************************/


/************************eeprom���ֽڶ�ȡд��**************************/


//д�뺯��
/*******************************************************************************
* ������  : Eeprom_Write_Bytes
* ����    : eeprom���ֽ�д��
* ����    : uint8_t addr	--д��ĵ�ַ		uint8_t* arry--Ҫд�����Ϣ����	uint8_t n--д����ֽ���
* ����ֵ  : ��
*******************************************************************************/
void Eeprom_Write_Bytes(uint8_t addr,uint8_t* arry ,uint8_t n)
{
	I2CStart();	//����
	I2CSendByte(0xa0);	//�����豸��ַ
	I2CWaitAck();				//�ȴ�
	I2CSendByte(addr);	//����Ƭ�ڵ�ַ
	I2CWaitAck();				//�ȴ�
	
	
	while(n--)
	{
		I2CSendByte(*arry++);
		I2CWaitAck();
	}
	
	
	I2CStop();
}

/*******************************************************************************
* ������  : Eeprom_Read_Bytes
* ����    : eeprom���ֽڶ�ȡ
* ����    : uint8_t addr	--��ȡ�ĵ�ַ		uint8_t* arry--��ȡ������Ϣ���������	uint8_t n--��ȡ���ֽ���
* ����ֵ  : ��
*******************************************************************************/
void Eeprom_Read_Bytes(uint8_t addr,uint8_t* arry ,uint8_t n)
{
	I2CStart();
	I2CSendByte(0xa0);	//ȷ��д�����
	I2CWaitAck();				//�ȴ�
	I2CSendByte(addr);	//���Ͷ�ȡ�ĵ�ַ
	I2CWaitAck();
	I2CStop();
	
	
	I2CStart();
	I2CSendByte(0xa1);	//ȷ����ȡ����
	I2CWaitAck();				//�ȴ�
	while(n--)
	{
		*arry++=I2CReceiveByte();
		//�ж��Ƿ���������Ҫ��ȡ
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












