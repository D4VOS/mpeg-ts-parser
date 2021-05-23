#include "../headers/PES_PacketHeader.h"
#include "../headers/TS_TransportStream.h"

void PES_PacketHeader::Parse(const uint8_t *input) {
    std::stringstream ss(xTS::getBitStream(input, 0, xTS::PES_HeaderLength));

    ss >> packet_start_code_prefix;
    ss >> stream_id;
    ss >> PES_packet_length;
    uint8_t current_byte = 6;
    uint8_t ID = this->getStreamId();
    std::cout << ID << eStreamId_program_stream_map;
    if (ID != eStreamId_program_stream_map &&
        ID != eStreamId_padding_stream &&
        ID != eStreamId_private_stream_2 &&
        ID != eStreamId_ECM &&
        ID != eStreamId_EMM &&
        ID != eStreamId_program_stream_directory &&
        ID != eStreamId_DSMCC_stream &&
        ID != eStreamId_ITUT_H222_1_type_E) {
        ss.str(std::string());
        ss.clear();
        ss << xTS::getBitStream(input, current_byte, 3); // grab flags

        std::bitset<2> marker;  //dummy
        ss >> marker; //'10'
        ss >> PES_scrambling_control;
        ss >> PES_priority;
        ss >> data_alignment_indicator;
        ss >> copyright;
        ss >> original_or_copy;
        ss >> PTS_DTS_flags;
        ss >> ESCR_flag;
        ss >> ES_rate_flag;
        ss >> DSM_trick_mode_flag;
        ss >> additional_copy_info_flag;
        ss >> PES_CRC_flag;
        ss >> PES_extension_flag;
        ss >> PES_header_data_length;

        if (PTS_DTS_flags.to_ulong() == 2) {
            ss.ignore(4);
            std::bitset<3> PTS_32_30;
            ss.ignore(1);
            std::bitset<15> PTS_29_15;
            ss.ignore(1);
            std::bitset<15> PTS_14_0;
            ss.ignore(1);

            PTS = connectBitsets(PTS_32_30, PTS_29_15, PTS_14_0);
            is_PTS = true;
        }
        if (PTS_DTS_flags.to_ulong() == 3) {
            ss.ignore(4);
            std::bitset<3> PTS_32_30;
            ss.ignore(1);
            std::bitset<15> PTS_29_15;
            ss.ignore(1);
            std::bitset<15> PTS_14_0;
            ss.ignore(1);

            ss.ignore(4);
            std::bitset<3> DTS_32_30;
            ss.ignore(1);
            std::bitset<15> DTS_29_15;
            ss.ignore(1);
            std::bitset<15> DTS_14_0;
            ss.ignore(1);

            PTS = connectBitsets(PTS_32_30, PTS_29_15, PTS_14_0);
            is_PTS = true;
            DTS = connectBitsets(DTS_32_30, DTS_29_15, DTS_14_0);
            is_DTS = true;
        }
    }


}

std::bitset<33> PES_PacketHeader::connectBitsets(std::bitset<3> first, std::bitset<15> second, std::bitset<15> third) {
    return std::bitset<33>(std::string(first.to_string() + second.to_string() + third.to_string()));
}

void PES_PacketHeader::Print() const {
    std::cout << "PES: " <<
              " PSCP=" << packet_start_code_prefix.to_ulong() <<
              " SID=" << stream_id.to_ulong() <<
              " L=" << PES_packet_length.to_ulong();

    if (is_PTS) {
        std::cout << " PTS=" << PTS.to_ulong() <<
                  "(Time=" << float(PTS.to_ulong() / xTS::BaseClockFrequency_Hz) << "s.)";
    }

    if (is_DTS) {
        std::cout << " DTS=" << DTS.to_ulong() <<
                  "(Time=" << float(DTS.to_ulong() / xTS::BaseClockFrequency_Hz) << "s.)";
    }
}

void PES_PacketHeader::Reset() {
    packet_start_code_prefix.reset();
    stream_id.reset();
    PES_packet_length.reset();
    PES_scrambling_control.reset();
    PES_priority.reset();
    data_alignment_indicator.reset();
    copyright.reset();
    original_or_copy.reset();
    PTS_DTS_flags.reset();
    ESCR_flag.reset();
    ES_rate_flag.reset();
    DSM_trick_mode_flag.reset();
    additional_copy_info_flag.reset();
    PES_CRC_flag.reset();
    PES_extension_flag.reset();
    PES_header_data_length.reset();
    PTS.reset();
    DTS.reset();
    is_DTS = false;
    is_PTS = false;
}
