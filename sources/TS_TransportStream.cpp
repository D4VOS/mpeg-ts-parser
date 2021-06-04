#include "../headers/TS_TransportStream.h"

std::string xTS::getBinaryRepresentation(const uint8_t *input, int start, int count) {
    std::string stream = "";
    for (int current_byte = start; current_byte < count+start; current_byte++){
        std::string byte_repr = "";
        int byte_value = int(input[current_byte]);

        for(int current_bin = 0; current_bin < CHAR_BIT; current_bin++, byte_value >>= 1){
            if(byte_value % 2){
                byte_repr.append("1");
            }else{
                byte_repr.append("0");
            }
        }
        std::reverse(byte_repr.begin(), byte_repr.end());
        stream.append(byte_repr);
    }
    return stream;
}