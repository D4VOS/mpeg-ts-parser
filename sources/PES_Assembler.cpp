#include "../headers/PES_Assembler.h"
#include "../headers/TS_TransportStream.h"

int SAVED = 0;

PES_Assembler::PES_Assembler(uint8_t pid) : PID(pid) {
    lastContinuityCounter = -1;     //start ind
    bufferSize = 0;
    started = false;
    dataOffset = xTS::PES_HeaderLength;
    uint8_t clear_buffer = 0;
    buffer = &clear_buffer;

    switch (PID) {
        case 136:
            file_extension = ".mp2";
            break;
        case 174:
            file_extension = ".264";
            break;
        default:
            file_extension = ".bin";
            break;
    }

    std::string file_name = std::string("outputs/PID_" + std::to_string(PID) + file_extension);
    file = fopen(file_name.c_str(), "wb");
}

PES_Assembler::~PES_Assembler() {
    if (started) {
        fwrite(&buffer[this->getHeaderLength()], this->getDataLength(), 1, file);
    }
}

void PES_Assembler::PrintPESH() const {
    PESH.Print();
}


PES_Assembler::eResult
PES_Assembler::AbsorbPacket(const uint8_t *TransportStreamPacket,
                            const TS_PacketHeader *PacketHeader,
                            const TS_AdaptationField *AdaptationField) {
    int start, payload;
    if (continuityCheck(PacketHeader->getContinuityCounter())) {
        xBufferReset();
        return eResult::StreamPackedLost;
    }

    if (PacketHeader->getPayloadUnitStartIndicator() == 1) {
        if (started && PESH.getPacketLength() == 0) {
            started = false;
            fwrite(&buffer[this->getHeaderLength()], this->getDataLength(), 1, file);
            xBufferReset();
        }

        if (buffer != nullptr) xBufferReset();

        lastContinuityCounter = PacketHeader->getContinuityCounter();
        started = true;

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
            payload = xTS::TS_PacketLength - xTS::TS_HeaderLength;
            start = xTS::TS_HeaderLength;
        }

        xBufferAppend(&TransportStreamPacket[start], payload);

        if (PESH.getPacketLength() + xTS::PES_HeaderLength == bufferSize) {
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

    delete[] buffer;
    buffer = new uint8_t[bufferSize];
    std::memcpy(buffer, temp_buffer, bufferSize);
    delete[] temp_buffer;
}

bool PES_Assembler::continuityCheck(uint8_t CC) {
    return (lastContinuityCounter != -1 && (CC - lastContinuityCounter != 1 && CC - lastContinuityCounter != -15));
}
