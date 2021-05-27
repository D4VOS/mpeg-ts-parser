#include "headers/TS_Common.h"
#include "headers/TS_TransportStream.h"
#include "headers/TS_PacketHeader.h"
#include "headers/TS_AdaptationField.h"
#include "headers/PES_Assembler.h"

void parsePID(PES_Assembler &current, TS_PacketHeader &header, TS_AdaptationField &af, int packetID, uint8_t *buffer) {
    if (header.hasAdaptationField()) {
        af.Parse(buffer);
    }
    std::cout << std::setfill('0') << std::setw(10);
    std::cout << packetID << " ";
    header.Print();

    if (header.hasAdaptationField()) { af.Print(); }
    PES_Assembler::eResult Result = current.AbsorbPacket(buffer, &header, &af);
    switch (Result) {
        case PES_Assembler::eResult::StreamPackedLost : {
            std::cout << " PcktLost ";
            break;
        }
        case PES_Assembler::eResult::AssemblingStarted : {
            std::cout << " Started ";
            current.PrintPESH();
            break;
        }
        case PES_Assembler::eResult::AssemblingContinue: {
            std::cout << " Continue ";
            break;
        }
        case PES_Assembler::eResult::AssemblingFinished: {
            std::cout << " Finished ";
            {
                std::cout << "PES: PcktLen=" << current.getPacketLength();
                std::cout << " HeadLen=" << current.getHeaderLength();
                std::cout << " DataLen=" << current.getDataLength();
                break;
            }
        }
        default: {
            break;
        }
    }
    std::cout << "\n";
}

int main(int argc, char *argv[], char *envp[]) {
    FILE *TSFile;
    TSFile = fopen("resources/example_new.ts", "rb");


    buffer_restore b(std::cout);                // std::cout -> output.txt
    std::ofstream file("outputs/listing.txt");   // comment these three line
    std::cout.rdbuf(file.rdbuf());                // to standard output


    if (TSFile == NULL) {
        std::cout << "Wrong file name\n";
        return EXIT_FAILURE;
    }

    uint8_t buffer[xTS::TS_PacketLength];

    TS_PacketHeader packetHeader;
    TS_AdaptationField adaptationField;
    PES_Assembler PES_Assembler_VOICE(136), PES_Assembler_VIDEO(174);

    printf("Started parsing example_new.ts file.. ");
    int32_t TS_PacketId = 0, currentPID = 0;
    while (!feof(TSFile)) {

        size_t NumRead = fread(buffer, 1, xTS::TS_PacketLength, TSFile);
        if (NumRead != xTS::TS_PacketLength) { break; }
        packetHeader.Reset();
        packetHeader.Parse(buffer);
        adaptationField.Reset();
        if (packetHeader.getSyncByte() == 'G') {
            currentPID = packetHeader.getPID();
            switch (currentPID) {
                case (136): {
                    parsePID(PES_Assembler_VOICE, packetHeader, adaptationField, TS_PacketId, buffer);
                    break;
                }
                case (174): {
                    parsePID(PES_Assembler_VIDEO, packetHeader, adaptationField, TS_PacketId, buffer);
                    break;
                }
                default:
                    break;
            }
            TS_PacketId++;
        }
    }
    printf("Done. Listing in output/listing.txt.");
    fclose(TSFile);
    return 0;
}