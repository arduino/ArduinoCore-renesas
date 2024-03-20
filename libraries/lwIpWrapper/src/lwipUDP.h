#ifndef ARDUINO_LWIP_UDP_H
#define ARDUINO_LWIP_UDP_H

#include <Udp.h>
#include <functional>

#include "CNetIf.h"

#define UDP_TX_PACKET_MAX_SIZE 24

/* UDP structure */
struct udp_struct {
    struct udp_pcb* pcb; /* pointer on the current udp_pcb */
    struct pbuf* p;
    uint16_t pbuf_offset;
    ip_addr_t ip; // the remote IP address from which the packet was received
    u16_t port; // the remote port from which the packet was received
    std::function<void()> onDataArrival;
};

class lwipUDP : public UDP {
private:
    uint16_t _port; // local port to listen on
    IPAddress _remoteIP; // remote IP address for the incoming packet whilst it's being processed
    uint16_t _remotePort; // remote port for the incoming packet whilst it's being processed
    IPAddress _sendtoIP; // the remote IP address set by beginPacket
    uint16_t _sendtoPort; // the remote port set by beginPacket

    struct pbuf* _data; // pbuf for data to send
    struct udp_struct _udp; // udp settings

protected:
    uint16_t _remaining; // remaining bytes of incoming packet yet to be processed

public:
    lwipUDP(); // Constructor
    virtual uint8_t begin(uint16_t) = 0;
    virtual uint8_t begin(IPAddress, uint16_t, bool multicast = false); // initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
    virtual uint8_t beginMulticast(IPAddress, uint16_t); // initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
    virtual void stop(); // Finish with the UDP socket

    // Sending UDP packets

    // Start building up a packet to send to the remote host specific in ip and port
    // Returns 1 if successful, 0 if there was a problem with the supplied IP address or port
    virtual int beginPacket(IPAddress ip, uint16_t port);
    // Start building up a packet to send to the remote host specific in host and port
    // Returns 1 if successful, 0 if there was a problem resolving the hostname or port
    virtual int beginPacket(const char* host, uint16_t port);
    // Finish off this packet and send it
    // Returns 1 if the packet was sent successfully, 0 if there was an error
    virtual int endPacket();
    // Write a single byte into the packet
    virtual size_t write(uint8_t);
    // Write size bytes from buffer into the packet
    virtual size_t write(const uint8_t* buffer, size_t size);

    using Print::write;

    // Start processing the next available incoming packet
    // Returns the size of the packet in bytes, or 0 if no packets are available
    virtual int parsePacket();
    // Number of bytes remaining in the current packet
    virtual int available();
    // Read a single byte from the current packet
    virtual int read();
    // Read up to len bytes from the current packet and place them into buffer
    // Returns the number of bytes read, or 0 if none are available
    virtual int read(unsigned char* buffer, size_t len);
    // Read up to len characters from the current packet and place them into buffer
    // Returns the number of characters read, or 0 if none are available
    virtual int read(char* buffer, size_t len)
    {
        return read((unsigned char*)buffer, len);
    };
    // Return the next byte from the current packet without moving on to the next byte
    virtual int peek();
    virtual void flush(); // Finish reading the current packet

    // Return the IP address of the host who sent the current incoming packet
    virtual IPAddress remoteIP()
    {
        return _remoteIP;
    };
    // Return the port of the host who sent the current incoming packet
    virtual uint16_t remotePort()
    {
        return _remotePort;
    };
    virtual void onDataArrival(std::function<void()> onDataArrival_fn);
};

#endif
