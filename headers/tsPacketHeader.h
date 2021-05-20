#ifndef TRANSPORT_STREAM_PARSER_REWORK_TSPACKETHEADER_H
#define TRANSPORT_STREAM_PARSER_REWORK_TSPACKETHEADER_H

#include "tsCommon.h"

class tsPacketHeader {
public:
    enum class ePID : uint16_t {
        PAT = 0x0000,
        CAT = 0x0001,
        TSDT = 0x0002,
        IPMT = 0x0003,
        NIT = 0x0010, //DVB specific PID
        SDT = 0x0011, //DVB specific PID
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

    void Parse(const uint8_t *); //int32_t
    void Print() const;

public: ///Getters
    uint8_t getSyncByte() const;

    uint8_t getTransportErrorIndicator() const;

    uint8_t getPayloadUnitStartIndicator() const;

    uint8_t getTransportPriority() const;

    uint16_t getPID() const;

    uint8_t getTransportScramblingControl() const;

    uint8_t getAdaptationFieldControl() const;

    uint8_t getContinuityCounter() const;

public:
    bool hasAdaptationField() const {
        return (adaptation_field_control.to_ulong() == 2 || adaptation_field_control.to_ulong() == 3);
    }

    bool hasPayload() const {
        return (adaptation_field_control.to_ulong() == 1 || adaptation_field_control.to_ulong() == 3);
    }
};

#endif //TRANSPORT_STREAM_PARSER_REWORK_TSPACKETHEADER_H

