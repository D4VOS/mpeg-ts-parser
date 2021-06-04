#include "../headers/PES_PacketHeader.h"
#include "../headers/TS_TransportStream.h"

void PES_PacketHeader::Parse(const uint8_t *input) {

    std::stringstream ss(xTS::getBinaryRepresentation(input, 0, xTS::PES_HeaderLength));

    ss >> packet_start_code_prefix;
    ss >> stream_id;
    ss >> PES_packet_length;

    headerLength = 6;
    uint8_t ID = this->getStreamId();
    //std::cout << "|&|" << PES_packet_length.to_ulong() << "|*|";
    if (ID != eStreamId_program_stream_map &&
        ID != eStreamId_padding_stream &&
        ID != eStreamId_private_stream_2 &&
        ID != eStreamId_ECM &&
        ID != eStreamId_EMM &&
        ID != eStreamId_program_stream_directory &&
        ID != eStreamId_DSMCC_stream &&
        ID != eStreamId_ITUT_H222_1_type_E) {
        this->wipe(&ss);
        ss << xTS::getBinaryRepresentation(input, xTS::PES_HeaderLength, 3); // grab flags

        ss.ignore(2);   //'10'
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
        //std::cout << "|||" << PES_header_data_length.to_ulong() << "|||";
        this->wipe(&ss);
        ss << xTS::getBinaryRepresentation(input, xTS::PES_HeaderLength + 3, PES_header_data_length.to_ulong()); // grab flags
        headerLength += 3 + PES_header_data_length.to_ulong();
        if (PTS_DTS_flags.to_ulong() == 2) {    //'10'
            ss.ignore(4);
            std::bitset<3> PTS_32_30;
            ss >> PTS_32_30;
            ss.ignore(1);
            std::bitset<15> PTS_29_15;
            ss >> PTS_29_15;
            ss.ignore(1);
            std::bitset<15> PTS_14_0;
            ss >> PTS_14_0;
            ss.ignore(1);
            PTS = connectBitsets(PTS_32_30, PTS_29_15, PTS_14_0);
            is_PTS = true;
        }
        if (PTS_DTS_flags.to_ulong() == 3) {    //'11'
            ss.ignore(4);
            std::bitset<3> PTS_32_30;
            ss >> PTS_32_30;
            ss.ignore(1);
            std::bitset<15> PTS_29_15;
            ss >> PTS_29_15;
            ss.ignore(1);
            std::bitset<15> PTS_14_0;
            ss >> PTS_14_0;
            ss.ignore(1);


            ss.ignore(4);
            std::bitset<3> DTS_32_30;
            ss >> DTS_32_30;
            ss.ignore(1);
            std::bitset<15> DTS_29_15;
            ss >> DTS_29_15;
            ss.ignore(1);
            std::bitset<15> DTS_14_0;
            ss >> DTS_14_0;
            ss.ignore(1);

            PTS = connectBitsets(PTS_32_30, PTS_29_15, PTS_14_0);
            is_PTS = true;
            DTS = connectBitsets(DTS_32_30, DTS_29_15, DTS_14_0);
            is_DTS = true;
        }
        this->wipe(&ss);
    }


}

void PES_PacketHeader::wipe(std::basic_stringstream<char, std::char_traits<char>, std::allocator<char> > *stream) {
    stream->str(std::string());
    stream->clear();
}

std::bitset<33> PES_PacketHeader::connectBitsets(std::bitset<3> first, std::bitset<15> second, std::bitset<15> third) {
    std::string value = (first.to_string() + second.to_string() + third.to_string());
    //::cout << "\n|" << third.to_string() << "|\n";
    return std::bitset<33>(value);
}

void PES_PacketHeader::Print() const {
    std::cout << "PES: " <<
              " PSCP=" << packet_start_code_prefix.to_ulong() <<
              " SID=" << stream_id.to_ulong() <<
              " L=" << PES_packet_length.to_ulong();

    if (is_PTS) {
        std::cout << " PTS=" << PTS.to_ulong() <<
                  " (Time=" << double(PTS.to_ulong()) / double(xTS::BaseClockFrequency_Hz) << "s.)";
    }

    if (is_DTS) {
        std::cout << " DTS=" << DTS.to_ulong() <<
                  " (Time=" << double(DTS.to_ulong()) / double(xTS::BaseClockFrequency_Hz) << "s.)";
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
