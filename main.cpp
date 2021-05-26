#include "headers/TS_Common.h"
#include "headers/TS_TransportStream.h"
#include "headers/TS_PacketHeader.h"
#include "headers/TS_AdaptationField.h"
#include "headers/PES_Assembler.h"


int main(int argc, char *argv[], char *envp[]) {
    FILE *TSFile, *Output;
    TSFile = fopen("example_new.ts", "rb");
    if(TSFile == NULL) { printf("wrong file name\n"); return EXIT_FAILURE; }

    Output = fopen("output.txt", "w");

    uint8_t buffer[xTS::TS_PacketLength];

    TS_PacketHeader packetHeader;
    TS_AdaptationField adaptationField;
    PES_Assembler PES_Assembler_VOICE(136), PES_Assembler_VIDEO(174);

    int32_t TS_PacketId = 0;
    while(!feof(TSFile))
    {
        size_t NumRead = fread(buffer, 1, xTS::TS_PacketLength, TSFile);
        if(NumRead != xTS::TS_PacketLength) { break; }
        packetHeader.Reset();
        packetHeader.Parse(buffer);
        adaptationField.Reset();
        if(packetHeader.getSyncByte() == 'G')
        {
            if(packetHeader.getPID() == 136) {
                if (packetHeader.hasAdaptationField()) {
                    adaptationField.Parse(buffer);
                }
                //printf("%010d ", TS_PacketId);
                //packetHeader.Print();

                //if(packetHeader.hasAdaptationField()) { adaptationField.Print(); }
                PES_Assembler::eResult Result = PES_Assembler_VOICE.AbsorbPacket(buffer, &packetHeader,
                                                                                 &adaptationField);

                //switch(Result)
                //{
                //    case PES_Assembler::eResult::StreamPackedLost : printf(" PcktLost "); break;
                //    case PES_Assembler::eResult::AssemblingStarted : printf(" Started "); PES_Assembler_VOICE.PrintPESH(); break;
                //    case PES_Assembler::eResult::AssemblingContinue: printf(" Continue "); break;
                //    case PES_Assembler::eResult::AssemblingFinished: printf(" Finished ");
                //        {
                //            PES: PcktLen=2894 HeadLen=14 DataLen=2880
                //            printf("PES: PcktLen=%d", PES_Assembler_VOICE.getPacketLength());
                //            printf(" HeadLen=%d", PES_Assembler_VOICE.getHeaderLength());
                //            printf(" DataLen=%d", PES_Assembler_VOICE.getDataLength());
                //            break;
                //        }
                //    default: break;
                //}
                //printf("\n");
            }
            else if(packetHeader.getPID() == 174) {
                if (packetHeader.hasAdaptationField()) {
                    adaptationField.Parse(buffer);
                }
                //printf("%010d ", TS_PacketId);
                //packetHeader.Print();

                //if(packetHeader.hasAdaptationField()) { adaptationField.Print(); }
                PES_Assembler::eResult Result = PES_Assembler_VIDEO.AbsorbPacket(buffer, &packetHeader,
                                                                                 &adaptationField);

                //switch(Result)
                //{
                //    case PES_Assembler::eResult::StreamPackedLost : printf(" PcktLost "); break;
                //    case PES_Assembler::eResult::AssemblingStarted : printf(" Started "); PES_Assembler_VIDEO.PrintPESH(); break;
                //    case PES_Assembler::eResult::AssemblingContinue: printf(" Continue "); break;
                //    case PES_Assembler::eResult::AssemblingFinished: printf(" Finished ");
                //        {
                //            PES: PcktLen=2894 HeadLen=14 DataLen=2880
                //            printf("PES: PcktLen=%d", PES_Assembler_VIDEO.getPacketLength());
                //            printf(" HeadLen=%d", PES_Assembler_VIDEO.getHeaderLength());
                //            printf(" DataLen=%d", PES_Assembler_VIDEO.getDataLength());
                //            break;
                //        }
                //    default: break;
                //}
                //printf("\n");
            }

            TS_PacketId++;
        }
    }

    fclose(Output);
    fclose(TSFile);
    return 0;
}