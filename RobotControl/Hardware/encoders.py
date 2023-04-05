
#----------------------------------------------------------------
#                      IMPORTS
#----------------------------------------------------------------

import spidev
import time
import sys
from microcontroller import Pin
import digitalio

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

SECONDS_PER_US = 1 / 1000000

SPI_BUS = 0 # We only have one bus
SPI_CS = 1 # Not actually used bc we control manually using a GPIO
SPI_MODE = 3
SPI_BAUD = 1000000
SS_PIN = 5

OCTOQUAD_FLAG_WRITE = 0x57
OCTOQUAD_FLAG_WRITE_STICKY = 0x53
OCTOQUAD_FLAG_READ = 0x52

SUPPORTED_FW_VERSION_MAJ = 2
OCTOQUAD_CHIP_ID = 0x51

OCTOQUAD_REG_CHIP_ID = 0
OCTOQUAD_REG_FW_MAJ = 1
OCTOQUAD_REG_ENC0 = 0x0C

#----------------------------------------------------------------
#                      SPI XFER FUNTCIONS
#----------------------------------------------------------------
def sleepMicroseconds(us):
    time.sleep(us * SECONDS_PER_US)

def slaveSelectAssert():
    digitalio.DigitalInOut(Pin(SS_PIN)).value = False
    sleepMicroseconds(50)

def slaveSelectUnassert():
    digitalio.DigitalInOut(Pin(SS_PIN)).value = True
    sleepMicroseconds(50)

def octoquad_xfer(tx):
    slaveSelectAssert()
    rx = spi.xfer(tx)
    slaveSelectUnassert()
    return rx

def writeRegisters(reg, data):
    data.insert(0, OCTOQUAD_FLAG_WRITE)
    data.insert(1, reg)
    octoquad_xfer(data)

def readRegisters(num):
    tx = bytearray(num)
    tx.insert(0, OCTOQUAD_FLAG_READ)
    rx = octoquad_xfer(tx)
    return rx[1:len(rx)]

def setRegisterAddress(reg, sticky):
    tx = bytearray(2)
    if sticky == True:
        tx[0] = OCTOQUAD_FLAG_WRITE_STICKY
    else:
        tx[0] = OCTOQUAD_FLAG_WRITE
    tx[1] = reg
    octoquad_xfer(tx)

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

def parseEncoderDataBlock(theBytes, enc, vel):
    if (len(theBytes) != (CNT_PAYLOAD_LEN + VEL_PAYLOAD_LEN)):
        print ("Err, len(theBytes)")
        exit()

    enc.clear()
    vel.clear()

    enc.extend(parseCountData(theBytes[0:CNT_PAYLOAD_LEN]))
    vel.extend(parseVelocityData(theBytes[CNT_PAYLOAD_LEN:CNT_PAYLOAD_LEN+VEL_PAYLOAD_LEN]))

def updateEncoderDataSticky(enc, vel):
    parseEncoderDataBlock(readRegisters(ENC_DATA_PAYLOAD_LEN), enc, vel)
    
def readFwVersion():
    setRegisterAddress(OCTOQUAD_REG_FW_MAJ, False)
    fw = readRegisters(FW_PAYLOAD_LEN)
    return fw
    
def readChipID():
    setRegisterAddress(OCTOQUAD_REG_CHIP_ID, False)
    return readRegisters(1)[0]

#----------------------------------------------------------------
#                      MAIN
#----------------------------------------------------------------

spi = spidev.SpiDev(SPI_BUS, SPI_CS)

class Encoder:
    counts: list[int]
    vels: list[int]
    def __init__(self) -> None:
        # Setup slave select pin
        digitalio.DigitalInOut(Pin(SS_PIN)).switch_to_output(True)

        # We're not transmitting just yet
        slaveSelectUnassert()

        # Setup SPI peripheral
        
        spi.max_speed_hz = SPI_BAUD
        spi.mode = SPI_MODE

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

        # Set sticky register for reads of encoder data
        setRegisterAddress(OCTOQUAD_REG_ENC0, True)

    def update(self) -> None:
        updateEncoderDataSticky(self.counts, self.vels)
    
    def getCounts(self) -> list[int]:
        return self.counts
    def getVelo(self) -> list[int]:
        return self.vels
