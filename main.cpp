#include "headers/TS_Handler.h"

int main(int argc, char *argv[], char *envp[]) {
    std::string source_path = "resources/example_new.ts",
                output_path = "outputs/listing.txt";

    buffer_restore b(std::cout);                    // std::cout -> output_path
    std::ofstream file(output_path);           // comment these three lines
    std::cout.rdbuf(file.rdbuf());                // to set up standard output

    TS_Handler tsparser = TS_Handler(source_path, output_path);     //setting up transport stream handler
    tsparser.Init();                                                   //parser initialization

    return 0;
}