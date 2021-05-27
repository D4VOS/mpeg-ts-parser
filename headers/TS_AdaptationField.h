#ifndef TRANSPORT_STREAM_PARSER_REWORK_TS_ADAPTATIONFIELD_H
#define TRANSPORT_STREAM_PARSER_REWORK_TS_ADAPTATIONFIELD_H

#include "TS_Common.h"

class TS_AdaptationField {
protected:
    std::bitset<8> adaptation_field_length;
    std::bitset<1> discontinuity_indicator;
    std::bitset<1> random_access_indicator;
    std::bitset<1> elementary_stream_priority_indicator;
    std::bitset<1> PCR_flag;
    std::bitset<1> OPCR_flag;
    std::bitset<1> splicing_point_flag;
    std::bitset<1> transport_private_data_flag;
    std::bitset<1> adaptation_field_extension_flag;
    std::bitset<33> PCR_base;
    std::bitset<6> PCR_reserved;
    std::bitset<9> PCR_extension;
    std::bitset<33> OPCR_base;
    std::bitset<6> OPCR_reserved;
    std::bitset<9> OPCR_extension;
    std::bitset<8> splice_countdown;
    std::bitset<8> transport_private_data_length;
    uint8_t *private_data_byte;
    std::bitset<8> adaptation_field_extension_length;
    std::bitset<1> ltw_flag;
    std::bitset<1> piecewise_rate_flag;
    std::bitset<1> seamless_splice_flag;
    std::bitset<5> reserved_extension_1;
    std::bitset<1> ltw_valid_flag;
    std::bitset<15> ltw_offset;
    std::bitset<2> reserved_extension_2;
    std::bitset<22> piecewise_rate;
    std::bitset<4> splice_type;
    std::bitset<3> DTS_next_AU_32_30;
    std::bitset<1> marker_bit_30;
    std::bitset<15> DTS_next_AU_29_15;
    std::bitset<1> marker_bit_15;
    std::bitset<15> DTS_next_AU_14_0;
    std::bitset<1> marker_bit;
    uint16_t stuffing_count;
public:
    TS_AdaptationField();

public:
    void Reset();

    void Parse(const uint8_t *);

    void Print() const;

public: ///Getters
    uint8_t getAdaptationFieldLength() const;
};


#endif //TRANSPORT_STREAM_PARSER_REWORK_TS_ADAPTATIONFIELD_H
