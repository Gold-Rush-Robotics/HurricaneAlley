#include "encoder.h"
#include <iostream>
#include "octoquad.h"
#include <stdio.h>
#include <linux/joystick.h>
#include "../Utilities/I2C.h"

#define NUM_ENCODERS 8
#define ENCODER_IDX_MIN 0
#define ENCODER_IDX_MAX (NUM_ENCODERS - 1)
#define ENCODER_IDX_IN_RANGE(val) ((val >= ENCODER_IDX_MIN && val <= ENCODER_IDX_MAX))
#define OCTOQUAD_I2C_ADDR 0x30
#define OCTOQUAD_DRIVER_SUPPORTED_FW_VERSION_MAJ 2

static OctoQuadInterface INTERFACE_CHOICE = OCTOQUAD_INTERFACE_I2C;

/*Initialize Encoder Handler and print out setupvalues*/
void EncoderHandler::init()
{
    if (bcm2835_i2c_begin() != 1)
    {
        fprintf(stderr, "i2c failed to begin\n probably not run as root\n");
        exit(1);
    }
    bcm2835_i2c_setSlaveAddress(OCTOQUAD_I2C_ADDR);

    // Define the platform HAL implementation for the OctoQuad driver
    OctoQuadPlatformImpl platform = {
        .i2c_read_registers = &I2C::readRegisters,
        .i2c_write_registers = &I2C::writeRegisters};
    octoquad_init(INTERFACE_CHOICE, platform);

    // Check the CHIP_ID
    uint8_t chipId;
    if (!octoquad_get_chip_id(&chipId))
    {
        printf("ERROR reading chip ID");
    }

    if (chipId == OCTOQUAD_CHIP_ID)
    {
        printf("CHIP_ID reports 0x%X as expected\r\n", chipId);
    }
    else
    {
        printf("CHIP_ID check failed, got 0x%X expect 0x%X\r\n", chipId, OCTOQUAD_CHIP_ID);
    }

    // Read the firmware version
    OctoQuadFwVersion firmwareVersion;

    octoquad_get_fw_version(&firmwareVersion);

    // Print to console
    printf("OctoQuad Reports FW v%d.%d.%d\r\n", firmwareVersion.maj, firmwareVersion.min, firmwareVersion.eng);

    // Check if that FW version is compatible
    if (firmwareVersion.maj != OCTOQUAD_DRIVER_SUPPORTED_FW_VERSION_MAJ)
    {
        printf("Cannot continue: The connected OctoQuad is running a firmware with a different major version than this program expects (got %d, expect %d)\r\n", firmwareVersion.maj, OCTOQUAD_DRIVER_SUPPORTED_FW_VERSION_MAJ);
    }

    OctoQuadChannelBankMode channelBankMode = OCTOQUAD_CHANNEL_BANK_MODE_ALL_QUADRATURE;
    if (!octoquad_get_channel_bank_mode(&channelBankMode))
    {
        printf("Error getting channel bank mode");
    }
    printf("Channel Bank Mode = %x\r\n", channelBankMode);

    OctoQuadI2cRecoveryMode recoveryMode = OCTOQUAD_I2C_RECOVERY_MODE_NONE;
    if (!octoquad_get_i2c_recovery_mode(&recoveryMode))
    {
        printf("Error getting i2c recovery mode");
    }
    printf("I2C Recovery Mode = %d\r\n", recoveryMode);

    for (int i = ENCODER_IDX_MIN; i <= ENCODER_IDX_MAX; i++)
    {
        uint8_t intvl = 0;
        if (!octoquad_get_velocity_measurement_intvl(i, &intvl))
        {
            printf("error egging velocity measurement interval");
        }
        printf("Channel %d velocity sample interval = %d\r\n", i, intvl);
    }

    for (int i = ENCODER_IDX_MIN; i <= ENCODER_IDX_MAX; i++)
    {
        OctoQuadChannelPulseWidthParams params;
        if (!octoquad_get_channel_pulse_width_params(i, &params))
        {
            printf("Error getting channel pulse width params");
        }
        printf("Channel %d pulse min/max = %d/%d\r\n", i, params.min, params.max);
    }

    bool channelDirections[8];
    octoquad_get_all_channel_directions(channelDirections);

    for (int i = ENCODER_IDX_MIN; i <= ENCODER_IDX_MAX; i++)
    {
        printf("Channel %d reverse = %d\r\n", i, channelDirections[i]);
    }

    octoquad_reset_all_positions();
}

/*Reset All Encoder Positions*/
bool EncoderHandler::resetPositions()
{
    return octoquad_reset_all_positions();
}

/*! Get all the Values
    \return returns a pointer to an 16 slot array, with the first 8 having positions and the last 8 having velocities
*/
int32_t *EncoderHandler::getValues()
{
    std::cout << "inside encoder " << std::endl;
    int32_t counts[8];
    int16_t vels[8];
    if (!octoquad_read_all_positions(counts))
        std::cout << "error reading" << std::endl;
    if (!octoquad_read_all_velocities(vels))
        std::cout << "error reading" << std::endl;

    printf("C: %d %d %d \n", counts[0], counts[1], counts[2]);

    static int32_t value[16];
    for (int i = 0; i < 8; i++)
    {
        value[i] = counts[i];
        value[i + 8] = vels[i];
    }
    return value;
}

/*! gets the counts from a single encoder 0-7
    \return the count from encoder
*/
int32_t EncoderHandler::getPos(int encoder)
{
    
    if (!ENCODER_IDX_IN_RANGE(encoder))
    {
        std::cout << "Invalid Encoder ID:" << encoder << std::endl;
        exit(1);
    }
    int32_t values[1] = {0};
    std::cout << "inside encoder 1" << std::endl;
    octoquad_read_single_position(encoder, values);
    std::cout << "inside encoder 2" << std::endl;
    return values[0];
}

void EncoderHandler::doNothing(){
    std::cout << "do Nothing" << std::endl;
    OctoQuadFwVersion firmwareVersion;
    octoquad_get_fw_version(&firmwareVersion);
    printf("OctoQuad Reports FW v%d.%d.%d\r\n", firmwareVersion.maj, firmwareVersion.min, firmwareVersion.eng);
}

/*prints the encoder counts and velocities in a readable fashion*/
void EncoderHandler::printReadable()
{
    int32_t counts[8] = {0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF};
    int16_t vels[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    if (!octoquad_read_all_positions(counts))
    {
        printf("Error reading position");
    }
    if (!octoquad_read_all_velocities(vels))
    {
        printf("Error reading velocities");
    }
    printf("POS[%d,%d,%d,%d,%d,%d,%d,%d] VEL[%d,%d,%d,%d,%d,%d,%d,%d]\r\n",
           counts[0],
           counts[1],
           counts[2],
           counts[3],
           counts[4],
           counts[5],
           counts[6],
           counts[7],
           vels[0],
           vels[1],
           vels[2],
           vels[3],
           vels[4],
           vels[5],
           vels[6],
           vels[7]);
}
