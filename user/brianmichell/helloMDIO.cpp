#include <iostream>
#include "mdio/mdio.h"

int main() {
    std::cout << "Hello, MDIO!" << std::endl;

    std::string path = "/home/brian_michell_tgs_com/source/mdio-cpp/build/mdio/zarrs/acceptance";

    mdio::Future<mdio::Dataset> dsRes = mdio::Dataset::Open(path, mdio::constants::kOpen);
    if (!dsRes.status().ok()) {
        std::cerr << "Failed to open dataset: " << dsRes.status() << std::endl;
        return 1;
    }

    mdio::Dataset ds = dsRes.value();
    std::cout << ds << std::endl;

    return 0;
}