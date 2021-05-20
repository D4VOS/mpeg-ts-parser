#include "../headers/tsPacketHeader.h"
#include "../headers/tsTransportStream.h"

void tsPacketHeader::Parse(const uint8_t *input) {
    this->Reset();
    std::stringstream bit_stream(xTS::getBitStream(input, 0, xTS::TS_HeaderLength));    //wyciaganie bitstream naglowka
    bit_stream >> sync_byte;
    bit_stream >> transport_error_indicator;
    bit_stream >> payload_unit_start_indicator;
    bit_stream >> transport_priority;
    bit_stream >> PID;
    bit_stream >> transport_scrambling_control;
    bit_stream >> adaptation_field_control;
    bit_stream >> continuity_counter;
    this->Print();
}

void tsPacketHeader::Reset() {
    sync_byte.reset();
    transport_error_indicator.reset();
    payload_unit_start_indicator.reset();
    transport_priority.reset();
    PID.reset();
    transport_scrambling_control.reset();
    adaptation_field_control.reset();
    continuity_counter.reset();
}

void tsPacketHeader::Print() const {
    std::cout << "\nTS:"
                  " SB=" << sync_byte.to_ulong() <<
                  " E=" << transport_error_indicator.to_ulong() <<
                  " S=" << payload_unit_start_indicator.to_ulong() <<
                  " P=" << transport_priority.to_ulong() <<
                  " PID=" << PID.to_ulong() <<
                  " TSC=" << transport_scrambling_control.to_ulong() <<
                  " AF=" << adaptation_field_control.to_ulong() <<
                  " CC=" << continuity_counter.to_ulong();
}

uint8_t tsPacketHeader::getSyncByte() const { return sync_byte.to_ulong(); }

uint8_t tsPacketHeader::getTransportErrorIndicator() const { return transport_error_indicator.to_ulong(); }

uint8_t tsPacketHeader::getPayloadUnitStartIndicator() const { return payload_unit_start_indicator.to_ulong(); }

uint8_t tsPacketHeader::getTransportPriority() const { return transport_priority.to_ulong(); }

uint16_t tsPacketHeader::getPID() const { return PID.to_ulong(); }

uint8_t tsPacketHeader::getTransportScramblingControl() const { return transport_scrambling_control.to_ulong(); }

uint8_t tsPacketHeader::getAdaptationFieldControl() const { return adaptation_field_control.to_ulong(); }

uint8_t tsPacketHeader::getContinuityCounter() const { return continuity_counter.to_ulong(); }
