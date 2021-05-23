#include "../headers/TS_TransportStream.h"

std::string xTS::getBitStream(const uint8_t *input, int start, int count) {
    std::string byte_stream = "", stream = "";
    for (int byte = start, byte_val = 0; byte < count + start; byte++) {
        byte_stream.clear();
        byte_val = int(input[byte]);
        for (int bit = 0; bit < CHAR_BIT; bit++, byte_val >>= 1) {
            if (byte_val % 2) byte_stream.push_back('1');
            else              byte_stream.push_back('0');
        }
        std::reverse(byte_stream.begin(), byte_stream.end());
        stream.append(byte_stream);
    }
    return stream;
}
