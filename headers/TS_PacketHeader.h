#ifndef TRANSPORT_STREAM_PARSER_REWORK_TS_PACKETHEADER_H
#define TRANSPORT_STREAM_PARSER_REWORK_TS_PACKETHEADER_H

#include "TS_Common.h"

class TS_PacketHeader {
public:
    enum class ePID : uint16_t {
        PAT = 0x0000,
        CAT = 0x0001,
        TSDT = 0x0002,
        IPMT = 0x0003,
        NIT = 0x0010,
        SDT = 0x0011,
        NuLL = 0x1FFF,
    };

protected:
    std::bitset<8> sync_byte;
    std::bitset<1> transport_error_indicator;
    std::bitset<1> payload_unit_start_indicator;
    std::bitset<1> transport_priority;
    std::bitset<13> PID;
    std::bitset<2> transport_scrambling_control;
    std::bitset<2> adaptation_field_control;
    std::bitset<4> continuity_counter;

public: ///Setters and print
    void Reset();

    void Parse(const uint8_t *);

    void Print() const;

public: ///Getters
    uint8_t getSyncByte() const;

    uint8_t getPayloadUnitStartIndicator() const;

    uint16_t getPID() const;

    uint8_t getContinuityCounter() const;

public:
    bool hasAdaptationField() const {
        return (adaptation_field_control.to_ulong() == 2 || adaptation_field_control.to_ulong() == 3);
    }
};

#endif //TRANSPORT_STREAM_PARSER_REWORK_TS_PACKETHEADER_H

