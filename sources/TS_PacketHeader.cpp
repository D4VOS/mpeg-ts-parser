#include "../headers/TS_PacketHeader.h"
#include "../headers/TS_TransportStream.h"

void TS_PacketHeader::Parse(const uint8_t *input) {
    std::stringstream ss(xTS::getBitStream(input, 0, xTS::TS_HeaderLength));    //wyciaganie bitstream naglowka

    ss >> sync_byte;
    ss >> transport_error_indicator;
    ss >> payload_unit_start_indicator;
    ss >> transport_priority;
    ss >> PID;
    ss >> transport_scrambling_control;
    ss >> adaptation_field_control;
    ss >> continuity_counter;
}

void TS_PacketHeader::Reset() {
    sync_byte.reset();
    transport_error_indicator.reset();
    payload_unit_start_indicator.reset();
    transport_priority.reset();
    PID.reset();
    transport_scrambling_control.reset();
    adaptation_field_control.reset();
    continuity_counter.reset();
}

void TS_PacketHeader::Print() const {
    std::cout << "TS:"
                 " SB=" << sync_byte.to_ulong() <<
              " E=" << transport_error_indicator.to_ulong() <<
              " S=" << payload_unit_start_indicator.to_ulong() <<
              " P=" << transport_priority.to_ulong() <<
              " PID=" << PID.to_ulong() <<
              " TSC=" << transport_scrambling_control.to_ulong() <<
              " AF=" << adaptation_field_control.to_ulong() <<
              " CC=" << continuity_counter.to_ulong();
}

uint8_t TS_PacketHeader::getSyncByte() const { return sync_byte.to_ulong(); }

uint8_t TS_PacketHeader::getTransportErrorIndicator() const { return transport_error_indicator.to_ulong(); }

uint8_t TS_PacketHeader::getPayloadUnitStartIndicator() const { return payload_unit_start_indicator.to_ulong(); }

uint8_t TS_PacketHeader::getTransportPriority() const { return transport_priority.to_ulong(); }

uint16_t TS_PacketHeader::getPID() const { return PID.to_ulong(); }

uint8_t TS_PacketHeader::getTransportScramblingControl() const { return transport_scrambling_control.to_ulong(); }

uint8_t TS_PacketHeader::getAdaptationFieldControl() const { return adaptation_field_control.to_ulong(); }

uint8_t TS_PacketHeader::getContinuityCounter() const { return continuity_counter.to_ulong(); }
