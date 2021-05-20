#include "headers/tsCommon.h"
#include "headers/tsTransportStream.h"
#include "headers/tsPacketHeader.h"
#include "headers/tsAdaptationField.h"

int main() {
    FILE *TSFile, *Output;
    TSFile = fopen("resources/example_new.ts", "rb");
    Output = fopen("output.txt", "w");

    uint8_t buffer[xTS::TS_PacketLength];

    tsPacketHeader packetHeader;
    tsAdaptationField adaptationField;

    while(!feof(TSFile)){
        fread(buffer, 1, xTS::TS_PacketLength, TSFile);
        ///---------------------- HEADER PARSING ----------------------///
        packetHeader.Parse(buffer);
        ///-----------------------------------------------------------///
        if(packetHeader.getSyncByte() != 0x47) break;   //desync

        ///----------------- ADAPTATION FIELD PARSING ----------------///
        if(packetHeader.hasAdaptationField()){adaptationField.Parse(buffer);}
        ///-----------------------------------------------------------///
    }
    fclose(Output);
    fclose(TSFile);
    return 0;
}
