
#----------------------------------------------------------------
#                      IMPORTS
#----------------------------------------------------------------

import smbus
import time
import sys

#----------------------------------------------------------------
#                      CONSTANTS
#----------------------------------------------------------------

NUM_ENCODERS = 8
ENCODER_FIRST = 0
ENCODER_LAST = NUM_ENCODERS

sizeof_int16 = 2
sizeof_int32 = 4

sizeof_vel = sizeof_int16
sizeof_cnt = sizeof_int32

CNT_PAYLOAD_LEN = sizeof_cnt*NUM_ENCODERS
VEL_PAYLOAD_LEN = sizeof_vel*NUM_ENCODERS
ENC_DATA_PAYLOAD_LEN = CNT_PAYLOAD_LEN + VEL_PAYLOAD_LEN
FW_PAYLOAD_LEN = 3

SUPPORTED_FW_VERSION_MAJ = 2

OCTOQUAD_CHIP_ID = 0x51
OCTOQUAD_I2C_ADDR = 0x30

OCTOQUAD_REG_CHIP_ID = 0
OCTOQUAD_REG_FW_MAJ = 1
OCTOQUAD_REG_ENC0 = 0x0C
OCTOQUAD_REG_VEL0 = 0x2C

OCTOQUAD_REG_COMMAND = 0x04

OCTOQUAD_CMD_RESET_EVERYTHING = 20

I2C_BUS_NUM = 1

#----------------------------------------------------------------
#                      HELPER FUNCTIONS
#----------------------------------------------------------------


def bytesToInt32(theBytes):
    if (len(theBytes) != sizeof_int32):
        print ("Err, bytesToInt32")
        exit()

    return int.from_bytes(theBytes, byteorder='little', signed=True)


def bytesToInt16(theBytes):
    if (len(theBytes) != sizeof_int16):
        print ("Err, bytesToInt16")
        exit()

    return int.from_bytes(theBytes, byteorder='little', signed=True)

def parseCountData(theBytes):
    if (len(theBytes) != CNT_PAYLOAD_LEN):
        print ("Err, len(theBytes)")
        exit()

    counts = []

    for i in range(ENCODER_FIRST,ENCODER_LAST):
        counts.append(bytesToInt32(theBytes[i*sizeof_cnt:(1+i) * sizeof_cnt]))

    return counts

def parseVelocityData(theBytes):
    if (len(theBytes) != VEL_PAYLOAD_LEN):
        print ("Err, len(theBytes)")
        exit()

    counts = []

    for i in range(ENCODER_FIRST,ENCODER_LAST):
        counts.append(bytesToInt16(theBytes[i*sizeof_vel:(1+i) * sizeof_vel]))

    return counts

def readFwVersion():
    fw = i2c.read_i2c_block_data(OCTOQUAD_I2C_ADDR, OCTOQUAD_REG_FW_MAJ, FW_PAYLOAD_LEN)
    return fw
    
def readChipID():
    return i2c.read_byte_data(OCTOQUAD_I2C_ADDR, OCTOQUAD_REG_CHIP_ID)    

def readCounts():
    theBytes = i2c.read_i2c_block_data(OCTOQUAD_I2C_ADDR, OCTOQUAD_REG_ENC0, CNT_PAYLOAD_LEN)
    return parseCountData(theBytes)

def readVelocities():
    theBytes = i2c.read_i2c_block_data(OCTOQUAD_I2C_ADDR, OCTOQUAD_REG_VEL0, VEL_PAYLOAD_LEN)
    return parseVelocityData(theBytes)

def reset_all_positions():
    i2c.write_i2c_block_data(OCTOQUAD_I2C_ADDR, OCTOQUAD_REG_COMMAND, [OCTOQUAD_CMD_RESET_EVERYTHING])


#----------------------------------------------------------------
#                      MAIN
#----------------------------------------------------------------

i2c = smbus.SMBus(I2C_BUS_NUM)

class Encoder:
    counts: "list[int]" = [0, 0, 0, 0, 0, 0, 0, 0]
    vels: "list[int]" = [0, 0, 0, 0, 0, 0, 0, 0]
    def __init__(self) -> None:

        # Verify CHIP_ID
        print("Reading CHIP_ID")
        # Do a throwaway read; for some reason the first byte read sometimes reports 128 (Pi specific issue?)
        readChipID()
        reportedChipID = readChipID()
        if (reportedChipID == OCTOQUAD_CHIP_ID):
            print("CHIP_ID reports 0x%X as expected" % OCTOQUAD_CHIP_ID)
        else:
            print("CHIP_ID check failed, got 0x%X, expect 0x%X" % (reportedChipID, OCTOQUAD_CHIP_ID))
            sys.exit()

        # Report FW version to console
        fw = readFwVersion()
        print ("OctoQuad reports FW v%d.%d.%d" % (fw[0], fw[1], fw[2]))

        # Check if we are compatible with that vesion
        if (fw[0] != SUPPORTED_FW_VERSION_MAJ):
            print("Cannot continue: The connected OctoQuad is running a firmware with a different major version than this program expects (expect %d)" % SUPPORTED_FW_VERSION_MAJ)
            sys.exit()

    def update(self) -> None:
        self.counts = readCounts()
        self.vels = readVelocities()
    
    def reset(self) -> None:
        self.counts = [0, 0, 0, 0, 0, 0, 0, 0]
        self.vels = [0, 0, 0, 0, 0, 0, 0, 0]
        reset_all_positions()
    
    def getCounts(self) -> "list[int]":
        return self.counts
    def getVelo(self) -> "list[int]":
        return self.vels


if __name__ == "__main__":
    octo = Encoder()
    while True:
        octo.update()
        print(octo.getCounts())