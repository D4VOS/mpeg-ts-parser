#ifndef TRANSPORT_STREAM_PARSER_REWORK_PES_ASSEMBLER_H
#define TRANSPORT_STREAM_PARSER_REWORK_PES_ASSEMBLER_H

#include "TS_Common.h"
#include "PES_PacketHeader.h"
#include "TS_PacketHeader.h"
#include "TS_AdaptationField.h"


class PES_Assembler {
public:
    enum class eResult : int32_t {
        UnexpectedPID = 1,
        StreamPackedLost,
        AssemblingStarted,
        AssemblingContinue,
        AssemblingFinished,
    };
protected:
//setup
    int32_t PID;
//buffer
    uint8_t *buffer;
    uint32_t bufferSize;
    uint32_t dataOffset;

//operation
    int8_t lastContinuityCounter;
    bool started;
    PES_PacketHeader PESH;
    FILE *file;
    std::string file_extension;
public:
    PES_Assembler(uint8_t);

    ~PES_Assembler();

public:
    eResult AbsorbPacket(const uint8_t *TransportStreamPacket, const TS_PacketHeader *PacketHeader,
                         const TS_AdaptationField *AdaptationField);

    void PrintPESH() const;

    int getStreamId() const { return (int)(PESH.getStreamId()); }

    int32_t getPacketLength() const { return dataOffset; }

    int32_t getHeaderLength() const { return PESH.getHeaderDataLength() + 6; }

    int32_t getDataLength() const { return dataOffset - this->getHeaderLength(); }

    bool continuityCheck(uint8_t);

protected:
    void xBufferReset();

    void xBufferAppend(const uint8_t *Data, int32_t Size);
};

#endif //TRANSPORT_STREAM_PARSER_REWORK_PES_ASSEMBLER_H
