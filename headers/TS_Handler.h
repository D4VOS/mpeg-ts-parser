#ifndef TRANSPORT_STREAM_PARSER_REWORK_TS_HANDLER_H
#define TRANSPORT_STREAM_PARSER_REWORK_TS_HANDLER_H
#include "TS_Common.h"
#include "TS_TransportStream.h"
#include "TS_PacketHeader.h"
#include "TS_AdaptationField.h"
#include "PES_Assembler.h"

class PES_Assembler;

class TS_Handler {
private:
    FILE *TSFile;
    uint8_t buffer[xTS::TS_PacketLength];
    int32_t currentPacket;

    const char * source_path;
    const char * output_path;

    PES_Assembler* assembler;
    TS_PacketHeader packetHeader;
    TS_AdaptationField adaptationField;
    std::unordered_map<uint16_t, PES_Assembler> assemblers;
public:
    TS_Handler(std::string, std::string);
    ~TS_Handler();
public:
    void Init();
    PES_Assembler* getAssembler(uint16_t);
};


#endif //TRANSPORT_STREAM_PARSER_REWORK_TS_HANDLER_H
