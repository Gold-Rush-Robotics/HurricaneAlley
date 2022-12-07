//
// Created by Philip Smith on 12/6/22.
//
#include "encoder.h"
#include <iostream>
#include <cstdint>

constexpr int NUM_ENCODERS = 3; //Change to match the number of encoders
constexpr int ENCODER_FIRST = 0;
constexpr int ENCODER_LAST = NUM_ENCODERS;

constexpr int sizeof_int32 = 4;
constexpr int sizeof_int16 = 2;

constexpr int sizeof_cnt = sizeof_int32;
constexpr int sizeof_vel = sizeof_int16;

constexpr int CNT_PAYLOAD_LEN = sizeof_cnt*NUM_ENCODERS;
constexpr int VEL_PAYLOAD_LEN = sizeof_vel*NUM_ENCODERS;
constexpr int ENC_DATA_PAYLOAD_LEN = CNT_PAYLOAD_LEN + VEL_PAYLOAD_LEN;
constexpr int FW_PAYLOAD_LEN = 3;

constexpr int SUPPORTED_FW_VERSION_MAJ = 2;

constexpr uint8_t OCTOQUAD_CHIP_ID = 0x51;
constexpr int OCTOQUAD_I2C_ADDR = 0x30;

constexpr int OCTOQUAD_REG_CHIP_ID = 0;
constexpr int OCTOQUAD_REG_FW_MAJ = 1;
constexpr uint8_t OCTOQUAD_REG_ENC0 = 0x0C;
constexpr uint8_t OCTOQUAD_REG_VEL0 = 0x2C;

constexpr int I2C_BUS_NUM = 1;

//---------------------------------------------------------------------
//                              Helper Functions
//---------------------------------------------------------------------

int32_t bytesToInt32(const char* theBytes) {
    if (theBytes.size() != sizeof_int32) {
        std::cerr << "Err, bytesToInt32" << std::endl;
        std::exit();
    }

    int32_t result = 0;
    std::memcpy(&result, theBytes.data(), sizeof_int32);
    return result;
}

std::int16_t bytesToInt16(const char* theBytes)
{
    if (theBytes.size() != sizeof_int16)
    {
        std::cerr << "Err, bytesToInt16\n";
        exit();
    }

    std::int16_t result = 0;
    std::memcpy(&result, theBytes.data(), sizeof_int16);
    return result;
}

int[] parseCountData(const char* theBytes){
    if (theBytes.size() != CNT_PAYLOAD_LEN)
    {
        std::cerr << "Err, len(theBytes)\n";
        exit();
    }

    int[] counts = new int[NUM_ENCODERS];
    for(int i = ENCODER_FIRST; i < ENCODER_LAST; i++){
        auto start = theBytes.begin() + (i * sizeof_cnt);
        auto end = theBytes.begin() + (1+i) * sizeof_cnt;
        std::vector<unsigned char> data;
        std::copy(start, end, data.begin());
        counts[i] = bytesToInt32(data);
    }
}

int[] parseVelocityData()



