
#include <string>
#include <fstream>
#include "raw_binary_archive.h"
#include "packets.h"

int main() {

    std::ofstream of("test.file");
    raw_binary_oarchive archive(of);
    base* b = new derived_two();
    archive << BOOST_SERIALIZATION_NVP(b);
}