
#include "lwipClient.h"

class WiFiClient : public lwipClient {
   public:
   WiFiClient() {}
   WiFiClient(struct tcp_struct *tcpClient) : lwipClient(tcpClient) {}
};

