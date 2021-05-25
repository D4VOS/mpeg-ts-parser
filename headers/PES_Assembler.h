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
    int PES_offset;
//setup
    int32_t PID;
//buffer
    uint8_t *buffer;
    uint32_t bufferSize;
    uint32_t dataOffset;
    uint32_t PES_A_Size;
    uint32_t headerLen;
    uint32_t dataLen;

//operation
    int8_t lastContinuityCounter;
    bool started;
    PES_PacketHeader PESH;
    FILE* file;
public:
    PES_Assembler(uint8_t);

    ~PES_Assembler();

public:
    void    Init(int32_t PID, std::string file_extension);
    eResult AbsorbPacket(const uint8_t* TransportStreamPacket, const TS_PacketHeader* PacketHeader, const TS_AdaptationField* AdaptationField);

    void PrintPESH() const;

    uint8_t *getPacket() { return &buffer[this->getHeaderLength()]; }
    FILE* getFile(){return file;}
    int32_t getPacketLength() const { return dataOffset; }

    int32_t getHeaderLength() const { return PESH.getHeaderDataLength() + 6; }

    int32_t getDataLength() const { return dataOffset - this->getHeaderLength(); }

protected:
    void xBufferReset();

    void xBufferAppend(const uint8_t *Data, int32_t Size);
};

#endif //TRANSPORT_STREAM_PARSER_REWORK_PES_ASSEMBLER_H
