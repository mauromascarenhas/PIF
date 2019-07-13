#include "pifc.h"

int main(int argc, const char* argv[]) {

    if (argc < 2) return 1;
    std::ifstream file(argv[1]);
    if (file.is_open()){
        std::string line;
        while(getline(file, line)){
            //TODO: DO STUFF
        }
    }

    return 0;
}