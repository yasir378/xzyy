#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>

int main() {
    // Create a raw socket
    int rawSocket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (rawSocket == -1) {
        std::cerr << "Failed to create raw socket." << std::endl;
        return 1;
    }

    // Set the interface name (e.g., wlan0)
    const char* interfaceName = "wlan0";

    // Set the source MAC address (your machine's MAC address)
    const unsigned char sourceMac[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

    // Set the target MAC address (access point's MAC address)
    const unsigned char targetMac[6] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

    // Set the target IP address (access point's IP address)
    const char* targetIp = "192.168.0.1";

    // Prepare the ARP packet
    unsigned char packet[42];

    // Ethernet header
    struct ethhdr* ethernetHeader = (struct ethhdr*)packet;
    memcpy(ethernetHeader->h_dest, targetMac, 6);
    memcpy(ethernetHeader->h_source, sourceMac, 6);
    ethernetHeader->h_proto = htons(ETH_P_ARP);

    // ARP header
    struct arphdr* arpHeader = (struct arphdr*)(packet + sizeof(struct ethhdr));
    arpHeader->ar_hrd = htons(ARPHRD_ETHER);
    arpHeader->ar_pro = htons(ETH_P_IP);
    arpHeader->ar_hln = 6;
    arpHeader->ar_pln = 4;
    arpHeader->ar_op = htons(ARPOP_REQUEST);

    // Source MAC address
    unsigned char* sourceMacAddress = (unsigned char*)(packet + sizeof(struct ethhdr) + sizeof(struct arphdr));
    memcpy(sourceMacAddress, sourceMac, 6);

    // Source IP address
    unsigned int* sourceIpAddress = (unsigned int*)(packet + sizeof(struct ethhdr) + sizeof(struct arphdr) + 6);
    *sourceIpAddress = inet_addr("192.168.0.2");

    // Target MAC address
    unsigned char* targetMacAddress = (unsigned char*)(packet + sizeof(struct ethhdr) + sizeof(struct arphdr) + 10);
    memset(targetMacAddress, 0, 6);

    // Target IP address
    unsigned int* targetIpAddress = (unsigned int*)(packet + sizeof(struct ethhdr) + sizeof(struct arphdr) + 16);
    *targetIpAddress = inet_addr(targetIp);

    // Set the destination address for sending the packet
    struct sockaddr_ll device;
    memset(&device, 0, sizeof(device));
    device.sll_family = AF_PACKET;
    device.sll_ifindex = if_nametoindex(interfaceName);
    device.sll_protocol = htons(ETH_P_ALL);
    if (sendto(rawSocket, packet, sizeof(packet), 0, (struct sockaddr*)&device, sizeof(device)) == -1) {
        std::cerr << "Failed to send ARP packet." << std::endl;
        return 1;
    }

    std::cout << "ARP packet sent successfully." << std::endl;

    // Close the raw socket
    close(rawSocket);

    return 0;
}
