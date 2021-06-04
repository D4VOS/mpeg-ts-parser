#include "headers/TS_Handler.h"

int main(int argc, char *argv[], char *envp[]) {
    std::string tsfile_path = "resources/example_new.ts",
                outputfile_path = "outputs/listing.txt";

    buffer_restore b(std::cout);                    // std::cout -> outputfile_path
    std::ofstream file(outputfile_path);           // comment these three lines
    std::cout.rdbuf(file.rdbuf());                // to set up standard output

    TS_Handler tsparser = TS_Handler(tsfile_path, outputfile_path);     //setting up transport stream handler
    tsparser.Init();                                                   //parser initialization

    return 0;
}