#include "../headers/PES_Assembler.h"
#include "../headers/TS_TransportStream.h"

PES_Assembler::PES_Assembler() {
    lastContinuityCounter = -1;     //start ind
}

PES_Assembler::~PES_Assembler() {

}

void PES_Assembler::PrintPESH() const {
    PESH.Print();
}

int32_t PES_Assembler::getNumPacketBytes() const {
    return dataOffset;
}

uint8_t *PES_Assembler::getPacket() {
    return buffer;
}

PES_Assembler::eResult
PES_Assembler::AbsorbPacket(const uint8_t *TransportStreamPacket,
                            const TS_PacketHeader *PacketHeader,
                            const TS_AdaptationField *AdaptationField) {
    if (PacketHeader->getPID() == PID) {
        if (PacketHeader->getPayloadUnitStartIndicator() == 1) {
            if (started) {
                started = false;
                //fwrite(buffer, bufferSize, 1, ofs); #todo saving to file
                printf(" PES: Finished with previous, Len=%4d ", bufferSize);
            }

            if (!started) {
                started = true;
                xBufferReset();
                PES_offset = PESH.Parse(TransportStreamPacket);
                if (PID != 174) bufferSize = PESH.getPacketLength() - PESH.getPesHeaderDataLength();
                xBufferAppend(TransportStreamPacket, PES_offset);
                return eResult::AssemblingStarted;
            }
        } else {
            if (PacketHeader->hasPayload()) {
                xBufferAppend(TransportStreamPacket, xTS::TS_HeaderLength + AdaptationField->getAdaptationFieldLength());
                lastContinuityCounter++;
            }

            return eResult::AssemblingContinue;
        }
    } else return eResult::UnexpectedPID;

    return eResult::AssemblingStarted;
};

void PES_Assembler::xBufferReset() {

}

void PES_Assembler::xBufferAppend(const uint8_t *Data, int32_t Size) {

}
