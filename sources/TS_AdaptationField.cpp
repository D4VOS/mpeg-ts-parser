#include "../headers/TS_AdaptationField.h"
#include "../headers/TS_TransportStream.h"

TS_AdaptationField::TS_AdaptationField() {}

uint8_t TS_AdaptationField::getAdaptationFieldLength() const { return adaptation_field_length.to_ulong(); }

void TS_AdaptationField::Reset() {
    adaptation_field_length.reset();
    random_access_indicator.reset();
    elementary_stream_priority_indicator.reset();
    PCR_flag.reset();
    OPCR_flag.reset();
    splicing_point_flag.reset();
    transport_private_data_flag.reset();
    adaptation_field_extension_flag.reset();
    PCR_base.reset();
    PCR_reserved.reset();
    PCR_extension.reset();
    OPCR_base.reset();
    OPCR_reserved.reset();
    OPCR_extension.reset();
    splice_countdown.reset();
    transport_private_data_length.reset();
    private_data_byte = nullptr;
    adaptation_field_extension_length.reset();
    ltw_flag.reset();
    piecewise_rate_flag.reset();
    seamless_splice_flag.reset();
    reserved_extension_1.reset();
    ltw_valid_flag.reset();
    ltw_offset.reset();
    reserved_extension_2.reset();
    piecewise_rate.reset();
    splice_type.reset();
    DTS_next_AU_32_30.reset();
    marker_bit_30.reset();
    DTS_next_AU_29_15.reset();
    marker_bit_15.reset();
    DTS_next_AU_14_0.reset();
    marker_bit.reset();
    stuffing_count = 0;
}

void TS_AdaptationField::Parse(const uint8_t* input) {
    //this->Reset();
    adaptation_field_length = input[4];

    std::stringstream ss(xTS::getBitStream(input, 5, adaptation_field_length.to_ulong()));
    int used_bytes = 1;

    if(adaptation_field_length.to_ulong() > 0){
        ss >> discontinuity_indicator;
        ss >> random_access_indicator;
        ss >> elementary_stream_priority_indicator;
        ss >> PCR_flag;
        ss >> OPCR_flag;
        ss >> splicing_point_flag;
        ss >> transport_private_data_flag;
        ss >> adaptation_field_extension_flag;

        if(PCR_flag == '1'){
            used_bytes += 6;
            ss >> PCR_base;
            ss >> PCR_reserved;
            ss >> PCR_extension;
        }
        if(OPCR_flag == '1'){
            used_bytes += 6;
            ss >> OPCR_base;
            ss >> OPCR_reserved;
            ss >> OPCR_extension;
        }
        if(splicing_point_flag == '1'){
            used_bytes++;
            ss >> splice_countdown;
        }
        if(transport_private_data_flag == '1'){
            ss >> transport_private_data_length;
            private_data_byte = new uint8_t[transport_private_data_length.to_ulong()];
            ss >> private_data_byte;
            used_bytes += transport_private_data_length.to_ulong();
        }
        if(adaptation_field_extension_flag == '1'){
            ss >> adaptation_field_extension_length;
            ss >> ltw_flag;
            ss >> piecewise_rate_flag;
            ss >> seamless_splice_flag;
            ss >> reserved_extension_1;
            used_bytes += adaptation_field_extension_length.to_ulong();
            if(ltw_flag == '1'){
                used_bytes += 2;
                ss >> ltw_valid_flag;
                ss >> ltw_offset;
            }
            if(piecewise_rate_flag == '1'){
                used_bytes += 3;
                ss >> reserved_extension_2;
                ss >> piecewise_rate;
            }
            if(seamless_splice_flag == '1'){
                used_bytes += 5;
                ss >> splice_type;
                ss >> DTS_next_AU_32_30;
                ss >> marker_bit_30;
                ss >> DTS_next_AU_29_15;
                ss >> marker_bit_15;
                ss >> DTS_next_AU_14_0;
                ss >> marker_bit;
            }
        }
        stuffing_count = adaptation_field_length.to_ulong() - used_bytes;
    }
    ss.str(std::string());
    ss.clear();
}

void TS_AdaptationField::Print() const {
    //AF: L= 47 DC=0 RA=0 SP=0 PR=0 OR=0 SP=0 TP=0 EX=0 Stuffing=46
    std::cout << " AF:"  <<
                  " L="  << adaptation_field_length.to_ulong() <<
                  " DC=" << discontinuity_indicator.to_ulong() <<
                  " RA=" << random_access_indicator.to_ulong() <<
                  " SPI="<< elementary_stream_priority_indicator.to_ulong() <<
                  " PR=" << PCR_flag.to_ulong() <<
                  " OR=" << OPCR_flag.to_ulong() <<
                  " SPF="<< splicing_point_flag.to_ulong() <<
                  " TP=" << transport_private_data_flag.to_ulong() <<
                  " EX=" << adaptation_field_extension_flag.to_ulong();

    if (PCR_flag == '1')
    {
        int PCR_val = (PCR_base.to_ulong() * 300) + PCR_extension.to_ulong();
        double time = (double(PCR_val)/double(xTS::ExtendedClockFrequency_Hz));
        std::cout << " PCR=" << PCR_val << " (Time=" << time << "s)";
    }

    if (OPCR_flag == '1')
    {
        int OPCR_val = (OPCR_base.to_ulong() * 300) + OPCR_extension.to_ulong();
        double time = (double(OPCR_val)/double(xTS::ExtendedClockFrequency_Hz));
        std::cout << " OPCR=" << OPCR_val << " (Time=" << time << "s)";
    }
    std::cout <<" Stuffing=" << stuffing_count;
}


