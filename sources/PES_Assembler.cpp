#include "../headers/PES_Assembler.h"
#include "../headers/TS_TransportStream.h"
#include <cstring>
#include <assert.h>

PES_Assembler::PES_Assembler(uint8_t pid) : PID(pid) {
    lastContinuityCounter = -1;     //start ind
    bufferSize = 0;
    started = false;
    dataOffset = xTS::PES_HeaderLength;
    buffer = NULL;
    if (pid == 136) { file = fopen("voice.mp2", "wb"); }    //voice file
    else if (pid == 174) file = fopen("vision.264", "wb");  //video file
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
        if (buffer != nullptr) xBufferReset();
        started = true;
        lastContinuityCounter = PacketHeader->getContinuityCounter();

        if (AdaptationField->getAdaptationFieldLength() == 0) {
            payload = 182 - AdaptationField->getAdaptationFieldLength();
            start = 6 + AdaptationField->getAdaptationFieldLength();
        }else{
            payload = 183 - AdaptationField->getAdaptationFieldLength();
            start = 5 + AdaptationField->getAdaptationFieldLength();
        }

        PESH.Parse(&TransportStreamPacket[start]);

        xBufferAppend(&TransportStreamPacket[start], payload);
        return eResult::AssemblingStarted;
    }


    if (started) {
        lastContinuityCounter = PacketHeader->getContinuityCounter();

        if (PacketHeader->hasAdaptationField())
        {
            start = 5 + AdaptationField->getAdaptationFieldLength();
            payload = 183 - AdaptationField->getAdaptationFieldLength();
        } else {
            start = 4;
            payload = 184;
        }

        xBufferAppend(&TransportStreamPacket[start], payload);

        if (PESH.getPacketLength() + 6 == bufferSize) {
            started = false;

            fwrite(&buffer[this->getHeaderLength()], this->getDataLength(), 1, file);
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

    buffer = new uint8_t[bufferSize];
    std::memcpy(buffer, temp_buffer, bufferSize);

}
