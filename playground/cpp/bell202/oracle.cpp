#include <iostream>
#include "bell202response.hpp"
#include "bell202data.h"
int main() {
    // Create parser and parse the XML file
    struct bell202_data data[PACKET_NUMBER];
    bell202_get_data(data);
    Bell202Modulation bell202(data);
    bell202.make_response();
    return 0;
}
