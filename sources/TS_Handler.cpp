#include "../headers/TS_Handler.h"


TS_Handler::TS_Handler(std::string source, std::string output) : source_path(source.c_str()),
                                                                 output_path(output.c_str()) {
    TSFile = fopen(source_path, "rb");
    assert(TSFile != NULL);

    currentPacket = 0;
}

TS_Handler::~TS_Handler() {
    fclose(TSFile);
    printf("\nDone. Packets listing in %s.\n", output_path);
}

void TS_Handler::Init() {
    printf("\nStarted parsing %s file.. ", source_path);


    while (!feof(TSFile)) {
        size_t read = fread(&buffer, 1, xTS::TS_PacketLength, TSFile);

        if (read != xTS::TS_PacketLength) { break; }

        packetHeader.Reset();
        packetHeader.Parse(buffer);
        adaptationField.Reset();

        if (packetHeader.getSyncByte() == 'G') {
            assembler = getAssembler(packetHeader.getPID());    //get specific PID assembler

            if (packetHeader.hasAdaptationField()) {
                adaptationField.Parse(buffer);
            }
            std::cout << std::setfill('0') << std::setw(10);
            std::cout << currentPacket << " ";
            packetHeader.Print();

            if (packetHeader.hasAdaptationField()) { adaptationField.Print(); }
            PES_Assembler::eResult Result = assembler->AbsorbPacket(buffer, &packetHeader, &adaptationField);
            switch (Result) {
                case PES_Assembler::eResult::StreamPackedLost : {
                    std::cout << " PcktLost SID=" << assembler->getStreamId() << " ";
                    break;
                }
                case PES_Assembler::eResult::AssemblingStarted : {
                    std::cout << " Started SID=" << assembler->getStreamId() << " ";
                    assembler->PrintPESH();
                    break;
                }
                case PES_Assembler::eResult::AssemblingContinue: {
                    std::cout << " Continue SID=" << assembler->getStreamId() << " ";
                    break;
                }
                case PES_Assembler::eResult::AssemblingFinished: {
                    std::cout << " Finished SID=" << assembler->getStreamId() << " ";
                    {
                        std::cout << "PES: PcktLen=" << assembler->getPacketLength();
                        std::cout << " HeadLen=" << assembler->getHeaderLength();
                        std::cout << " DataLen=" << assembler->getDataLength();
                        break;
                    }
                }
                default: {
                    break;
                }
            }
            std::cout << "\n";
            currentPacket++;
        }
    }

}

PES_Assembler *TS_Handler::getAssembler(uint16_t PID) {
    if (assemblers.find(PID) == assemblers.end()) {
        assemblers.insert(std::pair<uint16_t, PES_Assembler>{PID, PES_Assembler(uint8_t(PID))});
    }
    return &assemblers.at(PID);
}
