/*
 * Copyright (c) 2022 DigitalChickenLabs
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "string.h"
#include "octoquad.h"
#include <stdio.h>

#define OCTOQUAD_FLAG_WRITE ((uint8_t)('W'))
#define OCTOQUAD_FLAG_WRITE_STICKY_REG ((uint8_t)('S'))
#define OCTOQUAD_FLAG_READ ((uint8_t)('R'))

#define OCTOQUAD_REG_CHIP_ID 0x00
#define OCTOQUAD_REG_FW_VERSION 0x01
#define OCTOQUAD_REG_COMMAND 0x04
#define OCTOQUAD_REG_COMMAND_DAT0 0x05

#define OCTOQUAD_REG_ENC0 0x0C
#define OCTOQUAD_REG_VEL0 0x2C

#define OCTOQUAD_CMD_SET_PARAM 1
#define OCTOQUAD_CMD_READ_PARAM 2
#define OCTOQUAD_CMD_WRITE_PARAMS_TO_FLASH 3

#define OCTOQUAD_CMD_RESET_EVERYTHING 20
#define OCTOQUAD_CMD_RESET_ENCODERS 21

#define OCTOQUAD_PARAM_ENCODER_DIRECTIONS 0
#define OCTOQUAD_PARAM_I2C_RECOVERY_MODE 1
#define OCTOQUAD_PARAM_CHANNEL_BANK_MODES 2
#define OCTOQUAD_PARAM_CHANNEL_VEL_INTVL 3
#define OCTOQUAD_PARAM_CHANNEL_PULSE_MINMAX 4

#define SPI_ASSERT_DELAY_US 50
#define FLASH_PROGRAM_DELAY_MS 100

static OctoQuadPlatformImpl platformImpl;
static bool isI2c = true;
static uint8_t txBuf[64];
static uint8_t rxBuf[64];

// ---------------------------------------------------------------------------------
//                               I2C
//----------------------------------------------------------------------------------

// Just little helper functions :)
static bool i2c_read_registers(const uint8_t reg, uint8_t n, uint8_t* const dst)
{
    return platformImpl.i2c_read_registers(OCTOQUAD_I2C_ADDR, reg, n, dst);
}
static bool i2c_write_registers(const uint8_t reg, const uint8_t* const src, uint8_t n)
{
    return platformImpl.i2c_write_registers(OCTOQUAD_I2C_ADDR, reg, src, n);
}
static bool i2c_read_register(const uint8_t reg, uint8_t* const dst)
{
    return platformImpl.i2c_read_registers(OCTOQUAD_I2C_ADDR, reg, 1, dst);
}
static bool i2c_write_register(const uint8_t reg, const uint8_t data)
{
    return platformImpl.i2c_write_registers(OCTOQUAD_I2C_ADDR, reg, &data, 1);
}

// ---------------------------------------------------------------------------------
//                               SPI
//----------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
//                               Delegating
//----------------------------------------------------------------------------------

static bool read_register(const uint8_t reg, uint8_t* dst)
{
    return i2c_read_register(reg, dst);
}

static bool read_registers(const uint8_t reg, const uint8_t n, uint8_t* const dst)
{
    return i2c_read_registers(reg, n, dst);
}

static bool write_register(const uint8_t reg, const uint8_t data)
{
    return i2c_write_register(reg, data);
}

static bool write_registers(const uint8_t reg, const uint8_t* const src, const uint8_t n)
{
    return i2c_write_registers(reg, src, n);
}

// ---------------------------------------------------------------------------------
//                               API
//----------------------------------------------------------------------------------

bool octoquad_init(OctoQuadInterface interface, OctoQuadPlatformImpl platform)
{
    if(interface == OCTOQUAD_INTERFACE_SPI || interface == OCTOQUAD_INTERFACE_I2C)
    {
        isI2c = interface == OCTOQUAD_INTERFACE_I2C;
        platformImpl = platform;

        return true;
    }

    return false;
}

bool octoquad_get_chip_id(uint8_t* const out)
{
    return read_register(OCTOQUAD_REG_CHIP_ID, out);
}

bool octoquad_get_fw_version(OctoQuadFwVersion * dst)
{
    return read_registers(OCTOQUAD_REG_FW_VERSION, sizeof(OctoQuadFwVersion), (uint8_t*) dst);
}

bool octoquad_read_single_position(uint8_t encoder, int32_t* out)
{
    if(!ENCODER_IDX_IN_RANGE(encoder))
    {
        return false;
    }

    const uint8_t target_reg = OCTOQUAD_REG_ENC0 + encoder * sizeof(int32_t);

    return read_registers(target_reg, sizeof(int32_t), (uint8_t*) out);
}

bool octoquad_read_single_velocity(uint8_t encoder, int16_t* out)
{
    if(!ENCODER_IDX_IN_RANGE(encoder))
    {
        return false;
    }

    const uint8_t target_reg = OCTOQUAD_REG_VEL0 + encoder * sizeof(int16_t);

    return read_registers(target_reg, sizeof(int16_t), (uint8_t*) out);
}

bool octoquad_read_all_positions(int32_t out[NUM_ENCODERS])
{    
    return read_registers(OCTOQUAD_REG_ENC0, NUM_ENCODERS * sizeof(int32_t), (uint8_t*) out);
}

bool octoquad_read_all_velocities(int16_t out[NUM_ENCODERS])
{
    return read_registers(OCTOQUAD_REG_VEL0, NUM_ENCODERS * sizeof(int16_t), (uint8_t*) out);
}

bool octoquad_read_all_encoder_data_struct(OctoQuadEncoderData* out)
{
    return read_registers(OCTOQUAD_REG_ENC0, sizeof(OctoQuadEncoderData), (uint8_t*)out);
}

bool octoquad_read_all_encoder_data(int32_t countsOut[NUM_ENCODERS], int16_t velsOut[NUM_ENCODERS])
{
    OctoQuadEncoderData data;
    bool ok = read_registers(OCTOQUAD_REG_ENC0, sizeof(OctoQuadEncoderData), (uint8_t*)&data);

    if(!ok)
    {
        return false;
    }

    memcpy(countsOut, (uint8_t*)&data, NUM_ENCODERS * sizeof(int32_t));
    memcpy(velsOut, (uint8_t*)&data, NUM_ENCODERS * sizeof(int16_t));

    return true;
}

bool octoquad_reset_single_position(uint8_t encoder)
{
    if(!ENCODER_IDX_IN_RANGE(encoder))
    {
        return false;
    }

    uint8_t outgoing[2];
    outgoing[0] = OCTOQUAD_CMD_RESET_ENCODERS;
    outgoing[1] = (uint8_t) (1 << encoder);

    return write_registers(OCTOQUAD_REG_COMMAND, outgoing, sizeof(outgoing));
}

bool octoquad_reset_all_positions()
{
    uint8_t outgoing[2];
    outgoing[0] = OCTOQUAD_CMD_RESET_ENCODERS;
    outgoing[1] = (uint8_t) 0xFF;

    return write_registers(OCTOQUAD_REG_COMMAND, outgoing, sizeof(outgoing));
}

bool octoquad_set_velocity_measurement_intvl(uint8_t encoder, uint8_t intvl)
{
    if(!ENCODER_IDX_IN_RANGE(encoder))
    {
        return false;
    }

    uint8_t outgoing[4];
    outgoing[0] = OCTOQUAD_CMD_SET_PARAM;
    outgoing[1] = OCTOQUAD_PARAM_CHANNEL_VEL_INTVL;
    outgoing[2] = encoder;
    outgoing[3] = intvl;

    return write_registers(OCTOQUAD_REG_COMMAND, outgoing, sizeof(outgoing));
}

bool octoquad_get_velocity_measurement_intvl(uint8_t encoder, uint8_t* out)
{
    if(!ENCODER_IDX_IN_RANGE(encoder))
    {
        return false;
    }

    uint8_t outgoing[3];
    outgoing[0] = OCTOQUAD_CMD_READ_PARAM;
    outgoing[1] = OCTOQUAD_PARAM_CHANNEL_VEL_INTVL;
    outgoing[2] = encoder;

    write_registers(OCTOQUAD_REG_COMMAND, outgoing, sizeof(outgoing));

    return read_registers(OCTOQUAD_REG_COMMAND_DAT0, 1, out);
}

bool octoquad_set_i2c_recovery_mode(OctoQuadI2cRecoveryMode mode)
{
    uint8_t outgoing[3];
    outgoing[0] = OCTOQUAD_CMD_SET_PARAM;
    outgoing[1] = OCTOQUAD_PARAM_I2C_RECOVERY_MODE;
    outgoing[2] = (uint8_t) mode;

    return write_registers(OCTOQUAD_REG_COMMAND, outgoing, sizeof(outgoing));
}

bool octoquad_get_i2c_recovery_mode(OctoQuadI2cRecoveryMode* mode)
{
    uint8_t outgoing[2];
    outgoing[0] = OCTOQUAD_CMD_READ_PARAM;
    outgoing[1] = OCTOQUAD_PARAM_I2C_RECOVERY_MODE;

    write_registers(OCTOQUAD_REG_COMMAND, outgoing, sizeof(outgoing));

    return read_registers(OCTOQUAD_REG_COMMAND_DAT0, 1, (uint8_t*) mode);
}

bool octoquad_set_channel_bank_mode(OctoQuadChannelBankMode mode)
{
    uint8_t outgoing[3];
    outgoing[0] = OCTOQUAD_CMD_SET_PARAM;
    outgoing[1] = OCTOQUAD_PARAM_CHANNEL_BANK_MODES;
    outgoing[2] = (uint8_t) mode;

    return write_registers(OCTOQUAD_REG_COMMAND, outgoing, sizeof(outgoing));
}

bool octoquad_get_channel_bank_mode(OctoQuadChannelBankMode* mode)
{
    uint8_t outgoing[2];
    outgoing[0] = OCTOQUAD_CMD_READ_PARAM;
    outgoing[1] = OCTOQUAD_PARAM_CHANNEL_BANK_MODES;

    write_registers(OCTOQUAD_REG_COMMAND, outgoing, sizeof(outgoing));

    return read_registers(OCTOQUAD_REG_COMMAND_DAT0, 1, (uint8_t*) mode);
}

bool octoquad_set_channel_pulse_width_params(uint8_t chan, OctoQuadChannelPulseWidthParams params)
{
    if(!ENCODER_IDX_IN_RANGE(chan))
    {
        return false;
    }

    uint8_t outgoing[3 + sizeof(uint16_t)*2];
    outgoing[0] = OCTOQUAD_CMD_SET_PARAM;
    outgoing[1] = OCTOQUAD_PARAM_CHANNEL_PULSE_MINMAX;
    outgoing[2] = chan;
    memcpy(outgoing+3, &params, sizeof(uint16_t)*2);

    return write_registers(OCTOQUAD_REG_COMMAND, outgoing, sizeof(outgoing));
}

bool octoquad_get_channel_pulse_width_params(uint8_t chan, OctoQuadChannelPulseWidthParams* out)
{
    if(!ENCODER_IDX_IN_RANGE(chan))
    {
        return false;
    }

    uint8_t outgoing[3];
    outgoing[0] = OCTOQUAD_CMD_READ_PARAM;
    outgoing[1] = OCTOQUAD_PARAM_CHANNEL_PULSE_MINMAX;
    outgoing[2] = chan;

    write_registers(OCTOQUAD_REG_COMMAND, outgoing, sizeof(outgoing));

    return read_registers(OCTOQUAD_REG_COMMAND_DAT0,  sizeof(uint16_t)*2, (uint8_t*) out);
}

bool octoquad_save_params_to_flash()
{
    bool ret = write_register(OCTOQUAD_REG_COMMAND, OCTOQUAD_CMD_WRITE_PARAMS_TO_FLASH);
    platformImpl.sleep_ms(FLASH_PROGRAM_DELAY_MS);
    return ret;
}

bool octoquad_reset_everything()
{
    return write_register(OCTOQUAD_REG_COMMAND, OCTOQUAD_CMD_RESET_EVERYTHING);
}

bool octoquad_set_all_channel_directions(const bool reverse[8])
{
    uint8_t directionRegisterData = 0;

    for(int i = ENCODER_IDX_MIN; i <= ENCODER_IDX_MAX; i++)
    {
        if(reverse[i])
        {
            directionRegisterData |= (1 << i);
        }
    }

    uint8_t outgoing[3];
    outgoing[0] = OCTOQUAD_CMD_SET_PARAM;
    outgoing[1] = OCTOQUAD_PARAM_ENCODER_DIRECTIONS;
    outgoing[2] = directionRegisterData;

    return write_registers(OCTOQUAD_REG_COMMAND, outgoing, sizeof(outgoing));
}

bool octoquad_get_all_channel_directions(bool reverseOut[8])
{
    uint8_t outgoing[2];
    outgoing[0] = OCTOQUAD_CMD_READ_PARAM;
    outgoing[1] = OCTOQUAD_PARAM_ENCODER_DIRECTIONS;

    write_registers(OCTOQUAD_REG_COMMAND, outgoing, sizeof(outgoing));

    uint8_t directions;

    if(read_registers(OCTOQUAD_REG_COMMAND_DAT0,  1, &directions))
    {
        for(int i = ENCODER_IDX_MIN; i <= ENCODER_IDX_MAX; i++)
        {
            reverseOut[i] = (directions & (1 << i)) != 0;
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool octoquad_set_single_channel_direction(uint8_t chan, bool reverse)
{
    if(!ENCODER_IDX_IN_RANGE(chan))
    {
        return false;
    }

    uint8_t outgoingRead[2];
    outgoingRead[0] = OCTOQUAD_CMD_READ_PARAM;
    outgoingRead[1] = OCTOQUAD_PARAM_ENCODER_DIRECTIONS;

    write_registers(OCTOQUAD_REG_COMMAND, outgoingRead, sizeof(outgoingRead));

    uint8_t directions;
    if(read_registers(OCTOQUAD_REG_COMMAND_DAT0,  1, &directions))
    {
        if(reverse)
        {
            directions |= (1 << chan);
        }
        else
        {
            directions &= ~(1 << chan);
        }

        uint8_t outgoingWrite[3];
        outgoingWrite[0] = OCTOQUAD_CMD_SET_PARAM;
        outgoingWrite[1] = OCTOQUAD_PARAM_ENCODER_DIRECTIONS;
        outgoingWrite[2] = directions;

        return write_registers(OCTOQUAD_REG_COMMAND, outgoingWrite, sizeof(outgoingWrite));
    }
    else
    {
        return false;
    }
}

bool octoquad_get_single_channel_direction(uint8_t chan, bool* reverseOut)
{
    if(!ENCODER_IDX_IN_RANGE(chan))
    {
        return false;
    }

    uint8_t outgoing[2];
    outgoing[0] = OCTOQUAD_CMD_READ_PARAM;
    outgoing[1] = OCTOQUAD_PARAM_ENCODER_DIRECTIONS;

    write_registers(OCTOQUAD_REG_COMMAND, outgoing, sizeof(outgoing));

    uint8_t directions;

    if(read_registers(OCTOQUAD_REG_COMMAND_DAT0,  1, &directions))
    {
        *reverseOut = (directions & (1 << chan)) != 0;

        return true;
    }
    else
    {
        return false;
    }
}