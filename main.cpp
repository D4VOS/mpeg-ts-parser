#include "headers/TS_Common.h"
#include "headers/TS_TransportStream.h"
#include "headers/TS_PacketHeader.h"
#include "headers/TS_AdaptationField.h"
#include "headers/PES_Assembler.h"


int main(int argc, char *argv[], char *envp[]) {
    FILE *TSFile, *Output;
    TSFile = fopen("resources/example_new.ts", "rb");
    if(TSFile == NULL) { printf("wrong file name\n"); return EXIT_FAILURE; }

    Output = fopen("output.txt", "w");

    uint8_t buffer[xTS::TS_PacketLength];

    TS_PacketHeader packetHeader;
    TS_AdaptationField adaptationField;
    PES_Assembler PES_Assembler;

    int32_t TS_PacketId = 0;
    while(!feof(TSFile))
    {
        size_t NumRead = fread(buffer, 1, xTS::TS_PacketLength, TSFile);
        if(NumRead != xTS::TS_PacketLength) { break; }
        packetHeader.Reset();
        packetHeader.Parse(buffer);
        adaptationField.Reset();
        if(packetHeader.getSyncByte() == 'G' && packetHeader.getPID() == 136)
        {
            if(packetHeader.hasAdaptationField())
            {
                adaptationField.Parse(buffer);
            }
            printf("\n%010d ", TS_PacketId);
            packetHeader.Print();

            if(packetHeader.hasAdaptationField()) { adaptationField.Print(); }
            PES_Assembler::eResult Result = PES_Assembler.AbsorbPacket(buffer, &packetHeader, &adaptationField);

            switch(Result)
            {
                case PES_Assembler::eResult::StreamPackedLost : printf("PcktLost "); break;
                case PES_Assembler::eResult::AssemblingStarted : printf("Started "); PES_Assembler.PrintPESH(); break;
                case PES_Assembler::eResult::AssemblingContinue: printf("Continue "); break;
                case PES_Assembler::eResult::AssemblingFinished: printf("Finished "); printf("PES: Len=%d", PES_Assembler.getNumPacketBytes()); break;
                default: break;
            }
            printf("\n");
        }
        TS_PacketId++;
    }

    fclose(Output);
    fclose(TSFile);
    return 0;
}