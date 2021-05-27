#ifndef TRANSPORT_STREAM_PARSER_REWORK_PES_PACKETHEADER_H
#define TRANSPORT_STREAM_PARSER_REWORK_PES_PACKETHEADER_H

#include "TS_Common.h"

class PES_PacketHeader {
public:
    enum eStreamId : uint8_t {
        eStreamId_program_stream_map = 0xBC,
        eStreamId_padding_stream = 0xBE,
        eStreamId_private_stream_2 = 0xBF,
        eStreamId_ECM = 0xF0,
        eStreamId_EMM = 0xF1,
        eStreamId_program_stream_directory = 0xFF,
        eStreamId_DSMCC_stream = 0xF2,
        eStreamId_ITUT_H222_1_type_E = 0xF8,
    };

protected:
    //PES packet header
    std::bitset<24> packet_start_code_prefix;
    std::bitset<8> stream_id;
    std::bitset<16> PES_packet_length;
    //PTS & DTS parsing
    std::bitset<2> PES_scrambling_control;
    std::bitset<1> PES_priority;
    std::bitset<1> data_alignment_indicator;
    std::bitset<1> copyright;
    std::bitset<1> original_or_copy;
    std::bitset<2> PTS_DTS_flags;
    std::bitset<1> ESCR_flag;
    std::bitset<1> ES_rate_flag;
    std::bitset<1> DSM_trick_mode_flag;
    std::bitset<1> additional_copy_info_flag;
    std::bitset<1> PES_CRC_flag;
    std::bitset<1> PES_extension_flag;
    std::bitset<8> PES_header_data_length;

    std::bitset<33> PTS;
    std::bitset<33> DTS;

    bool is_PTS;
    bool is_DTS;

    uint8_t PESH_DataLength;
    uint8_t headerLength;
public:
    void Reset();

    void Parse(const uint8_t *);

    void Print() const;

public:
//PES packet header
    uint32_t getPacketStartCodePrefix() const { return packet_start_code_prefix.to_ulong(); }

    uint8_t getStreamId() const { return stream_id.to_ulong(); }

    uint16_t getPacketLength() const { return PES_packet_length.to_ulong(); }

    uint8_t getHeaderLen() const { return headerLength; }

    uint8_t getHeaderDataLength() const { return 3 + PES_header_data_length.to_ulong(); }

    void wipe(std::basic_stringstream<char, std::char_traits<char>, std::allocator<char> > *);

    std::bitset<33> connectBitsets(std::bitset<3>, std::bitset<15>, std::bitset<15>);
};

#endif //TRANSPORT_STREAM_PARSER_REWORK_PES_PACKETHEADER_H
