#include "xmlDict.hpp"
#include <iostream>

int main() {
    // Create parser and parse the XML file
    EnhancedXmlParser parser;
    auto xml = parser.parseFile("config.xml");
    
    if (!xml) {
        std::cerr << "Failed to parse XML file" << std::endl;
        return 1;
    }
    
    // 1. Simple string access
    std::cout << "1. Accessing guest interface: ";
    auto guest_interface = xml["config"]["host"]["network"]["interfaces"]["interface"][0]["@name"];
    if (guest_interface.isValid()) {
        std::cout << guest_interface.asString() << std::endl;
    } else {
        std::cout << "not found" << std::endl;
    }
    
    // 2. Accessing attributes with @ prefix
    std::cout << "2. Accessing guest name attribute: ";
    auto guest_name = xml["config"]["guest"][0]["@name"];
    if (guest_name.isValid()) {
        std::cout << guest_name.asString() << std::endl;
    } else {
        std::cout << "not found" << std::endl;
    }
    
   
    // 4. Accessing VLAN ID
    std::cout << "4. Accessing VLAN ID: ";
    auto vlan_id = xml["config"]["host"]["network"]["firewall"]["vlans"]["vlan"][0];
    if (vlan_id.isValid()) {
        std::cout << vlan_id.asString() << std::endl;
        vlan_id.print();
    } else {
        std::cout << "not found" << std::endl;
    }
    
    // 5. Accessing VLAN PCP
    std::cout << "5. Accessing VLAN PCP: ";
    auto vlan_pcp = xml["config"]["host"]["network"]["mapping"]["vlans"]["vlan"][0]["@pcp"];
    if (vlan_pcp.isValid()) {
        std::cout << vlan_pcp.asString() << std::endl;
    } else {
        std::cout << "not found" << std::endl;
    }
    
    // 7. Loop through multiple VLANs
    std::cout << "7. All VLANs in first interface:" << std::endl;
    for (int i = 0; i < 10; ++i) { // Try up to 10 VLANs
        auto vlan_name = xml["config"]["host"]["network"]["mapping"]["vlans"]["vlan"][i]["@id"];
        if (vlan_name.isValid()) {
            std::cout << "   VLAN " << i << ": " << vlan_name.asString() << std::endl;
        } else {
            break; // No more VLANs
        }
    }
    
    // 8. Convert to integer
    std::cout << "8. VLAN ID as integer: ";
    auto vlan_id_int = xml["config"]["host"]["network"]["mapping"]["vlans"]["vlan"][2]["@id"].asInt();
    std::cout << vlan_id_int << std::endl;
    
    // 9. Error handling example
    std::cout << "\n9. Non-existent path handling: ";
    auto non_existent = xml["config"]["nonexistent"]["path"][999]["@attr"];
    if (non_existent.isValid()) {
        std::cout << non_existent.asString() << std::endl;
    } else {
        std::cout << "safely returns empty/invalid" << std::endl;
    }
    
    return 0;
}
