#ifndef TRANSPORT_STREAM_PARSER_REWORK_PES_ASSEMBLER_H
#define TRANSPORT_STREAM_PARSER_REWORK_PES_ASSEMBLER_H
#include "TS_Common.h"
#include "PES_PacketHeader.h"
#include "TS_PacketHeader.h"
#include "TS_AdaptationField.h"

class PES_Assembler
{
public:
    enum class eResult : int32_t
    {
        UnexpectedPID = 1,
        StreamPackedLost ,
        AssemblingStarted ,
        AssemblingContinue,
        AssemblingFinished,
    };
protected:
    int PES_offset;
//setup
    int32_t PID;
//buffer
    uint8_t* buffer;
    uint32_t bufferSize;
    uint32_t dataOffset;
//operation
    int8_t lastContinuityCounter;
    bool started;
    PES_PacketHeader PESH;
public:
    PES_Assembler ();
    ~PES_Assembler();

public:
    void Init (int32_t PID);
    eResult AbsorbPacket(const uint8_t* TransportStreamPacket, const TS_PacketHeader* PacketHeader, const TS_AdaptationField* AdaptationField);
    void PrintPESH () const;
    uint8_t* getPacket ();
    int32_t getNumPacketBytes() const;
protected:
    void xBufferReset ();
    void xBufferAppend(const uint8_t* Data, int32_t Size);
};

#endif //TRANSPORT_STREAM_PARSER_REWORK_PES_ASSEMBLER_H
