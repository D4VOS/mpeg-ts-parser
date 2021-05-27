#include "../headers/PES_Assembler.h"
#include "../headers/TS_TransportStream.h"
#include <cstring>
#include <assert.h>

int SAVED = 0;

PES_Assembler::PES_Assembler(uint8_t pid) : PID(pid) {
    lastContinuityCounter = -1;     //start ind
    bufferSize = 0;
    started = false;
    dataOffset = xTS::PES_HeaderLength;
    uint8_t clear_buffer = 0;
    buffer = &clear_buffer;
    file = fopen(std::string("outputs/output_pid" + std::to_string(PID) + ".bin").c_str(), "wb");
}

PES_Assembler::~PES_Assembler() {}

void PES_Assembler::PrintPESH() const {
    PESH.Print();
}


PES_Assembler::eResult
PES_Assembler::AbsorbPacket(const uint8_t *TransportStreamPacket,
                            const TS_PacketHeader *PacketHeader,
                            const TS_AdaptationField *AdaptationField) {
    int CC = int(PacketHeader->getContinuityCounter()), start = 0, payload = 0;
    if (lastContinuityCounter != -1 && (CC - lastContinuityCounter != 1 && CC - lastContinuityCounter != -15)) {
        xBufferReset();
        return eResult::StreamPackedLost;
    }

    if (PacketHeader->getPayloadUnitStartIndicator() == 1) {
        if (started && PESH.getPacketLength() == 0) {
            started = false;
            fwrite(&buffer[this->getHeaderLength()], 1, this->getDataLength(), file);
            xBufferReset();
        }

        if (buffer != nullptr) xBufferReset();
        started = true;
        lastContinuityCounter = PacketHeader->getContinuityCounter();

        start = xTS::TS_HeaderLength + AdaptationField->getAdaptationFieldLength() + 1;
        payload =
                xTS::TS_PacketLength
                - xTS::TS_HeaderLength
                - (AdaptationField->getAdaptationFieldLength() + 1);
        if (AdaptationField->getAdaptationFieldLength() == 0) {
            start--;
            payload++;
        }

        PESH.Parse(&TransportStreamPacket[start]);

        xBufferAppend(&TransportStreamPacket[start], payload);
        return eResult::AssemblingStarted;
    }


    if (started) {
        lastContinuityCounter = PacketHeader->getContinuityCounter();

        if (PacketHeader->hasAdaptationField()) {
            start = xTS::TS_HeaderLength + AdaptationField->getAdaptationFieldLength() + 1;
            payload =
                    xTS::TS_PacketLength
                    - xTS::TS_HeaderLength
                    - (AdaptationField->getAdaptationFieldLength() + 1);
        } else {
            start = xTS::TS_HeaderLength;
            payload = xTS::TS_PacketLength - xTS::TS_HeaderLength;
        }

        xBufferAppend(&TransportStreamPacket[start], payload);

        if (PESH.getPacketLength() + 6 == bufferSize) {
            started = false;
            fwrite(&buffer[this->getHeaderLength()], 1, this->getDataLength(), file);
            return eResult::AssemblingFinished;
        }
        return eResult::AssemblingContinue;
    }
}

void PES_Assembler::xBufferReset() {
    buffer = nullptr;
    bufferSize = 0;
    dataOffset = 0;
}

void PES_Assembler::xBufferAppend(const uint8_t *input, int32_t size) {
    dataOffset += size;
    bufferSize += size;

    if (buffer == nullptr) {
        buffer = new uint8_t[bufferSize];
        std::memcpy(buffer, input, bufferSize);
        return;
    }

    uint8_t *temp_buffer = new uint8_t[bufferSize];

    std::memcpy(temp_buffer, buffer, bufferSize - size);
    std::memcpy(&temp_buffer[bufferSize - size], input, size);
    delete[] buffer;
    buffer = new uint8_t[bufferSize];
    std::memcpy(buffer, temp_buffer, bufferSize);
    delete[] temp_buffer;
}
