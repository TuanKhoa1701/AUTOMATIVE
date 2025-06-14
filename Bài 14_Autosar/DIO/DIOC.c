#include "DIO.h"
Dio_LevelType DIO_ReadChannel(Dio_ChannelType ChannelId)
{
    GPIO_TypeDef *GPIO_Port;
    uint8_t GIPO_Pin;

    GPIO_Port = GPIO_GetPort(ChannelId);
    if(GPIO_Port == NULL)
    {
        Det_ReportError(DIO_MODULE_ID, 0, DIO_READCHANNEL_ID, DIO_E_PARAM_INVALID_CHANNEL);
        return STD_LOW; // Return a default value or handle error appropriately
    }
    GIPO_Pin = GPIO_GetPin(ChannelId);
    if(Read_InputDataBit(GPIO_Port, GIPO_Pin) == STD_HIGH)
    {
        return STD_HIGH;
    }
    else
    {
        return STD_LOW;
    }
}
void DIO_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    GPIO_TypeDef *GPIO_Port;
    uint8_t GIPO_Pin;

    GPIO_Port = GPIO_GetPort(ChannelId);
    if(GPIO_Port == NULL)
    {
        Det_ReportError(DIO_MODULE_ID, 0, DIO_WRITECHANNEL_ID, DIO_E_PARAM_INVALID_CHANNEL);
        return; // Handle error appropriately
    }
    GIPO_Pin = GPIO_GetPin(ChannelId);
    
    if(Level == STD_HIGH)
    {
        Set_OutputDataBit(GPIO_Port, GIPO_Pin);
    }
    else
    {
        Clear_OutputDataBit(GPIO_Port, GIPO_Pin);
    }
}
Dio_PortLevelType DIO_ReadPort(Dio_PortType PortId)
{
    GPIO_TypeDef *GPIO_Port;

    GPIO_Port = GPIO_GetPort(PortId);
    if(GPIO_Port == NULL)
    {
        return 0; // Return a default value or handle error appropriately
    }
    
    return (Dio_PortLevelType)Read_InputData(GPIO_Port);
}
void DIO_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
{
    GPIO_TypeDef *GPIO_Port;

    GPIO_Port = GPIO_GetPort(PortId);
    if(GPIO_Port == NULL)
    {
        Det_ReportError(DIO_MODULE_ID, 0, DIO_WRITEPORT_ID, DIO_E_PARAM_INVALID_PORT);
        return; // Handle error appropriately
    }
    
    GPIO_Write(GPIO_Port, Level);
}
Dio_PortLevelType DIO_ReadChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr)
{
    GPIO_TypeDef *GPIO_Port;
    uint16_t mask;
    uint8_t offset;

    if(ChannelGroupIdPtr == NULL)
    {
        return 0; // Return a default value or handle error appropriately
    }

    GPIO_Port = GPIO_GetPort(ChannelGroupIdPtr->port);
    if(GPIO_Port == NULL)
    {
        return 0; // Return a default value or handle error appropriately
    }
    
    mask = ChannelGroupIdPtr->mask;
    offset = ChannelGroupIdPtr->offset;

    return (Dio_PortLevelType)((Read_InputData(GPIO_Port) & mask) >> offset);
}

void DIO_WriteChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level)
{
    GPIO_TypeDef *GPIO_Port;
    uint16_t mask;
    uint8_t offset;

    if(ChannelGroupIdPtr == NULL)
    {
        return; // Handle error appropriately
    }

    GPIO_Port = GPIO_GetPort(ChannelGroupIdPtr->port);
    if(GPIO_Port == NULL)
    {
        return; // Handle error appropriately
    }
    
    mask = ChannelGroupIdPtr->mask;
    offset = ChannelGroupIdPtr->offset;

    Dio_PortLevelType currentLevel = Read_InputData(GPIO_Port) & ~mask;
    currentLevel |= (Level << offset) & mask;

    GPIO_Write(GPIO_Port, currentLevel);
}
void DIO_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    if(versioninfo == NULL)
    {
        return; // Handle error appropriately
    }

    versioninfo->vendorID = DIO_VENDOR_ID;
    versioninfo->moduleID = DIO_MODULE_ID;
    versioninfo->major = DIO_SW_MAJOR_VERSION;
    versioninfo->minor = DIO_SW_MINOR_VERSION;
    versioninfo->patch = DIO_SW_PATCH_VERSION;
}
Dio_LevelType DIO_FlipChannel(Dio_ChannelType ChannelId)
{
    GPIO_TypeDef *GPIO_Port;
    uint8_t GIPO_Pin;

    GPIO_Port = GPIO_GetPort(ChannelId);
    if(GPIO_Port == NULL)
    {
        return STD_LOW; // Return a default value or handle error appropriately
    }
    GIPO_Pin = GPIO_GetPin(ChannelId);

    if(Read_InputDataBit(GPIO_Port, GIPO_Pin) == STD_HIGH)
    {
        DIO_WriteChannel(ChannelId, STD_LOW);
        return STD_LOW;
    }
    else
    {
        DIO_WriteChannel(ChannelId, STD_HIGH);
        return STD_HIGH;
    }
}
void DIO_MaskedWritePort(Dio_PortType PortId, Dio_PortLevelType Level, Dio_PortLevelType Mask)
{
    GPIO_TypeDef *GPIO_Port;

    GPIO_Port = GPIO_GetPort(PortId);
    if(GPIO_Port == NULL)
    {
        return; // Handle error appropriately
    }
    
    Dio_PortLevelType currentLevel = Read_InputData(GPIO_Port) & ~Mask;
    currentLevel |= (Level & Mask);

    GPIO_Write(GPIO_Port, currentLevel);
}