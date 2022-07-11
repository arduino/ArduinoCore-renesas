/* generated configuration header file - do not edit */
#ifndef NX_USER_H_
#define NX_USER_H_
#ifdef __cplusplus
extern "C" {
#endif

/* NXD Source Common Configuration */

#ifdef NX_DISABLE_ERROR_CHECKING
#define NX_SMTP_DISABLE_ERROR_CHECKING  /* SMTP does not use the common error checking macro */
#endif

#if (+0)
#define NX_PHYSICAL_HEADER ()
#endif
#if (+0)
#define NX_PHYSICAL_TRAILER ()
#endif

#if (+0)
#define NX_ARP_EXPIRATION_RATE ()
#endif
#if (+0)
#define NX_ARP_UPDATE_RATE ()
#endif
#if (+0)
#define NX_TCP_ACK_TIMER_RATE ()
#endif
#if (+0)
#define NX_TCP_FAST_TIMER_RATE ()
#endif
#if (+0)
#define NX_TCP_TRANSMIT_TIMER_RATE ()
#endif
#if (+0)
#define NX_TCP_KEEPALIVE_INITIAL ()
#endif
#if (+0)
#define NX_TCP_KEEPALIVE_RETRY ()
#endif
#if (+0)
#define NX_ARP_MAXIMUM_RETRIES ()
#endif
#if (+0)
#define NX_ARP_MAX_QUEUE_DEPTH ()
#endif

#if (+0)
#define NX_MAX_MULTICAST_GROUPS ()
#endif
#if (+0)
#define NX_MAX_LISTEN_REQUESTS ()
#endif

#if (+0)
#define NX_TCP_ACK_EVERY_N_PACKETS ()
#endif

/* Automatically define NX_TCP_ACK_EVERY_N_PACKETS to 1 if NX_TCP_IMMEDIATE_ACK is defined.
 This is needed for backward compatibility. */
#if (defined(NX_TCP_IMMEDIATE_ACK) && !defined(NX_TCP_ACK_EVERY_N_PACKETS))
#define NX_TCP_ACK_EVERY_N_PACKETS 1
#endif

#if (+0)
#define NX_TCP_MAXIMUM_RETRIES ()
#endif
#if (+0)
#define NX_TCP_MAXIMUM_TX_QUEUE ()
#endif
#if (+0)
#define NX_TCP_RETRY_SHIFT ()
#endif
#if (+0)
#define NX_TCP_KEEPALIVE_RETRIES ()
#endif

#if (+0)
#define NX_MAX_PHYSICAL_INTERFACES ()
#endif

#define NX_ENABLE_EXTENDED_NOTIFY_SUPPORT

#if (+0)
#define NX_PACKET_HEADER_PAD
#define NX_PACKET_HEADER_PAD_SIZE ()
#endif
#if (+0)
#define NX_TCP_MSS_CHECKING_ENABLED
#define NX_TCP_MSS_MINIMUM ()
#endif

#if (+0)
#define NX_ARP_DEFEND_INTERVAL ()
#endif
#if (+0)
#define NX_TCP_MAX_OUT_OF_ORDER_PACKETS ()
#endif

// IPv6 Options

#if (+0)
#define NX_PATH_MTU_INCREASE_WAIT_INTERVAL 
#endif
#if (+0)
#define NX_IPV6_DEFAULT_ROUTER_TABLE_SIZE 
#endif
#if (+0)
#define NX_IPV6_DESTINATION_TABLE_SIZE 
#endif
#if (+0)
#define NX_IP_MAX_REASSEMBLY_TIME 
#endif
#if (+0)
#define NX_IPV4_MAX_REASSEMBLY_TIME 
#endif
#if (+0)
#define NX_IPV6_MAX_REASSEMBLY_TIME 
#endif

#if (+0)
#define NX_IPV6_PREFIX_LIST_TABLE_SIZE 
#endif

#if (+0)
#define NX_MAX_IPV6_ADDRESSES 
#endif
// Neighbor Cache Configuration Options
#if (+0)
#define NX_DELAY_FIRST_PROBE_TIME 
#endif

#if (+0)
#define NX_IPV6_DAD_TRANSMITS 
#endif
#define NX_DISABLE_IPV6_PURGE_UNUSED_CACHE_ENTRIES
#if (+0)
#define NX_IPV6_NEIGHBOR_CACHE_SIZE 
#endif
#if (+0)
#define NX_MAX_MULTICAST_SOLICIT 
#endif
#if (+0)
#define NX_MAX_UNICAST_SOLICIT 
#endif
#if (+0)
#define NX_ND_MAX_QUEUE_DEPTH 
#endif
#if (+0)
#define NX_REACHABLE_TIME 
#endif
#if (+0)
#define NX_RETRANS_TIMER 
#endif
// Miscellaneous ICMPv6 Configuration Options

#if (+0)
#define NX_ICMPV6_MAX_RTR_SOLICITATIONS    
#endif
#if (+0)
#define NX_ICMPV6_RTR_SOLICITATION_INTERVAL      
#endif

#if (+0)
#define NX_PACKET_ALIGNMENT      
#endif

int rand(void);
#define NX_RAND rand

void srand(unsigned int);
#define NX_SRAND srand

/* Azure IoT Configuration */
#define NX_AZURE_DISABLE_IOT_SECURITY_MODULE /* The Azure IoT Security Module redefines a reserved symbol and cannot be supported at this time. */

/* DHCP Common Configuration. */
#define NX_DHCP_TYPE_OF_SERVICE                    (NX_IP_NORMAL)
#define NX_DHCP_FRAGMENT_OPTION                    (NX_DONT_FRAGMENT)
#define NX_DHCP_TIME_TO_LIVE                       (128)
#define NX_DHCP_QUEUE_DEPTH                        (5)

/* DHCP IPv4 Client Configuration. */
/* Required for projects that need the packet pool to be placed in nonsecure memory. */
#define NX_DHCP_CLIENT_USER_CREATE_PACKET_POOL

#ifdef NX_DHCP_CLIENT_SEND_ARP_PROBE
#define NX_DHCP_CLIENT_SEND_ARP_PROBE
#define NX_DHCP_ARP_PROBE_WAIT                       (1 * NX_IP_PERIODIC_RATE)
#define NX_DHCP_ARP_PROBE_MIN                        (1 * NX_IP_PERIODIC_RATE)
#define NX_DHCP_ARP_PROBE_MAX                        (2 * NX_IP_PERIODIC_RATE)
#define NX_DHCP_ARP_PROBE_NUM                        2
#endif
#define NX_DHCP_THREAD_PRIORITY                      3
#define NX_DHCP_THREAD_STACK_SIZE                    4096
#define NX_DHCP_TIME_INTERVAL                        (1 * NX_IP_PERIODIC_RATE)
#define NX_DHCP_MAX_RETRANS_TIMEOUT                  (64 * NX_IP_PERIODIC_RATE)
#define NX_DHCP_MIN_RENEW_TIMEOUT                    (60 * NX_IP_PERIODIC_RATE)
#define NX_DHCP_MIN_RETRANS_TIMEOUT                  (4 * NX_IP_PERIODIC_RATE)
#define NX_DHCP_OPTIONS_BUFFER_SIZE                  312
#define NX_DHCP_CLIENT_MAX_RECORDS                   1
#define NX_DHCP_RESTART_WAIT                         (10 * NX_IP_PERIODIC_RATE)

/* DHCP IPv4 Server Configuration. */
#define NX_DHCP_SERVER_THREAD_PRIORITY             (2)
#define NX_DHCP_PACKET_ALLOCATE_TIMEOUT            (2 * NX_IP_PERIODIC_RATE)
#define NX_DHCP_FAST_PERIODIC_TIME_INTERVAL        (10)
#define NX_DHCP_CLIENT_SESSION_TIMEOUT             (10 * NX_DHCP_FAST_PERIODIC_TIME_INTERVAL)
#define NX_DHCP_DEFAULT_LEASE_TIME                 (0xFFFFFFFF)
#define NX_DHCP_SLOW_PERIODIC_TIME_INTERVAL        (1000)
#define NX_DHCP_CLIENT_OPTIONS_MAX                 (12)
#define NX_DHCP_OPTIONAL_SERVER_OPTION_LIST        "1 3 6"
#define NX_DHCP_OPTIONAL_SERVER_OPTION_SIZE        (3)
#define NX_DHCP_SERVER_HOSTNAME_MAX                (32)
#define NX_DHCP_CLIENT_HOSTNAME_MAX                (32)
#define NX_DHCP_IP_ADDRESS_MAX_LIST_SIZE           20
#define NX_DHCP_CLIENT_RECORD_TABLE_SIZE           50
#define NX_BOOT_BUFFER_SIZE                        548

/* DHCP Common IPv6 Configuration. */
#define NX_DHCPV6_TYPE_OF_SERVICE                    (NX_IP_NORMAL)
#define NX_DHCPV6_TIME_TO_LIVE                       (128)
#define NX_DHCPV6_QUEUE_DEPTH                        (5)
#define NX_DHCPV6_PACKET_TIME_OUT                    (3* NX_IP_PERIODIC_RATE)
#define NX_DHCPV6_SESSION_TIMER_INTERVAL             (3)

/* DHCP IPv6 Client Configuration. */
#define NX_DHCPV6_THREAD_PRIORITY                      (3)
#define NX_DHCPV6_MUTEX_WAIT                           (TX_WAIT_FOREVER)
#define NX_DHCPV6_IP_LIFETIME_TIMER_INTERVAL           (1  * NX_IP_PERIODIC_RATE)
#define NX_DHCPV6_MAX_IA_ADDRESS                       (1)
#define NX_DHCPV6_NUM_DNS_SERVERS                      (2)
#define NX_DHCPV6_NUM_TIME_SERVERS                     (1)
#define NX_DHCPV6_DOMAIN_NAME_BUFFER_SIZE              (32)
#define NX_DHCPV6_TIME_ZONE_BUFFER_SIZE                (16)
#define NX_DHCPV6_MAX_MESSAGE_SIZE                     (100)

/* DHCP IPv6 Server Configuration. */
#define NX_DHCPV6_SERVER_THREAD_PRIORITY               (2)
#define NX_DHCPV6_IP_LEASE_TIMER_INTERVAL              (60)
#define NX_DHCPV6_PACKET_WAIT_OPTION                   (1)
#define NX_DHCPV6_PREFERENCE_VALUE                     (0)
#define NX_DHCPV6_MAX_OPTION_REQUEST_OPTIONS           (6)
#define NX_DHCPV6_DEFAULT_T1_TIME                      (2000)
#define NX_DHCPV6_DEFAULT_T2_TIME                      (3000)
#define NX_DHCPV6_STATUS_MESSAGE_MAX                   (100)
#define NX_DHCPV6_MAX_LEASES                           (100)
#define NX_DHCPV6_MAX_CLIENTS                          (120)
#define NX_DHCPV6_FRAGMENT_OPTION                      (NX_DONT_FRAGMENT)
#define NX_DHCPV6_DEFAULT_PREFERRED_TIME               (2 * NX_DHCPV6_DEFAULT_T1_TIME)
#define NX_DHCPV6_DEFAULT_VALID_TIME                   (2 * NX_DHCPV6_DEFAULT_PREFERRED_TIME)
#define NX_DHCPV6_SERVER_DUID_VENDOR_ASSIGNED_ID       "abcdeffghijklmnopqrstuvwxyz"
#define NX_DHCPV6_SERVER_DUID_VENDOR_PRIVATE_ID        (0x12345678)
#define NX_DHCPV6_SERVER_DUID_VENDOR_ASSIGNED_LENGTH   (48)
#define NX_DHCPV6_STATUS_MESSAGE_SUCCESS               "IA OPTION GRANTED"
#define NX_DHCPV6_STATUS_MESSAGE_UNSPECIFIED           "IA OPTION NOT GRANTED-FAILURE UNSPECIFIED"
#define NX_DHCPV6_STATUS_MESSAGE_NO_ADDRS_AVAILABLE    "IA OPTION NOT GRANTED-NO ADDRESSES AVAILABLE"
#define NX_DHCPV6_STATUS_MESSAGE_NO_BINDING            "IA OPTION NOT GRANTED-INVALID CLIENT REQUEST"
#define NX_DHCPV6_STATUS_MESSAGE_NOT_ON_LINK           "IA OPTION NOT GRANTED-CLIENT NOT ON LINK"
#define NX_DHCPV6_STATUS_MESSAGE_USE_MULTICAST         "IA OPTION NOT GRANTED-CLIENT MUST USE MULTICAST"
#define NX_DHCPV6_SESSION_TIMEOUT                      (20)

/* HTTP Common Configuration. */

#define NX_HTTP_FRAGMENT_OPTION                     NX_DONT_FRAGMENT
#define NX_HTTP_TIME_TO_LIVE                        128
#define NX_HTTP_TYPE_OF_SERVICE                     NX_IP_NORMAL
#define NX_HTTP_MAX_RESOURCE                        40
#define NX_HTTP_MAX_NAME                            20
#define NX_HTTP_MAX_PASSWORD                        20

/* HTTP Client Configuration. */
#define NX_HTTP_CLIENT_MIN_PACKET_SIZE              600
#define NX_HTTP_CLIENT_TIMEOUT                      (10 * NX_IP_PERIODIC_RATE)

/* HTTP Server Configuration. */

#define NX_HTTP_SERVER_PRIORITY                     16
#define NX_HTTP_SERVER_WINDOW_SIZE                  2048
#define NX_HTTP_SERVER_TIMEOUT                      (10 * NX_IP_PERIODIC_RATE)
#define NX_HTTP_SERVER_TIMEOUT_ACCEPT               (10 * NX_IP_PERIODIC_RATE)
#define NX_HTTP_SERVER_TIMEOUT_DISCONNECT           (10 * NX_IP_PERIODIC_RATE)
#define NX_HTTP_SERVER_TIMEOUT_RECEIVE              (10 * NX_IP_PERIODIC_RATE)
#define NX_HTTP_SERVER_TIMEOUT_SEND                 (10 * NX_IP_PERIODIC_RATE)
#define NX_HTTP_MAX_HEADER_FIELD                    256
#define NX_HTTP_SERVER_MAX_PENDING                  5
#define NX_HTTP_MAX_NAME                            20
#define NX_HTTP_MAX_PASSWORD                        20
#define NX_HTTP_SERVER_MIN_PACKET_SIZE              600
#define NX_HTTP_SERVER_THREAD_TIME_SLICE            2
#define NX_HTTP_SERVER_TRANSMIT_QUEUE_DEPTH         20
#define NX_HTTP_SERVER_RETRY_SECONDS                2
#define NX_HTTP_SERVER_RETRY_MAX                    10
#define NX_HTTP_SERVER_RETRY_SHIFT                  1

/* DNS Client Configuration. */
#define NX_DNS_CLIENT_USER_CREATE_PACKET_POOL

#define NX_DNS_PACKET_PAYLOAD_UNALIGNED                         (16 + sizeof(NX_IPV4_HEADER) + sizeof(NX_UDP_HEADER) + NX_DNS_MESSAGE_MAX)
#define NX_DNS_TYPE_OF_SERVICE                                  NX_IP_NORMAL
#define NX_DNS_FRAGMENT_OPTION                                  NX_DONT_FRAGMENT
#define NX_DNS_TIME_TO_LIVE                                     128
#define NX_DNS_MAX_SERVERS                                      5
#define NX_DNS_MESSAGE_MAX                                      512
#define NX_DNS_MAX_RETRIES                                      3
#define NX_DNS_MAX_RETRANS_TIMEOUT                              (64 * NX_IP_PERIODIC_RATE)
#define NX_DNS_PACKET_ALLOCATE_TIMEOUT                          (1 * NX_IP_PERIODIC_RATE)

/* Telnet Common Configuration. */
#define NX_TELNET_TOS                                (NX_IP_NORMAL)
#define NX_TELNET_FRAGMENT_OPTION                    (NX_DONT_FRAGMENT)
#define NX_TELNET_SERVER_PORT                        (23)
#define NX_TELNET_TIME_TO_LIVE                       (128)

/* AutoIP Configuration. */
#define NX_AUTO_IP_PROBE_WAIT                    1
#define NX_AUTO_IP_PROBE_NUM                     3
#define NX_AUTO_IP_PROBE_MIN                     1
#define NX_AUTO_IP_PROBE_MAX                     2
#define NX_AUTO_IP_MAX_CONFLICTS                 10
#define NX_AUTO_IP_RATE_LIMIT_INTERVAL           60
#define NX_AUTO_IP_ANNOUNCE_WAIT                 2
#define NX_AUTO_IP_ANNOUNCE_NUM                  2
#define NX_AUTO_IP_ANNOUNCE_INTERVAL             2
#define NX_AUTO_IP_DEFEND_INTERVAL               10

/* Web HTTP Common Configuration. */

#define NX_WEB_HTTP_FRAGMENT_OPTION                     NX_DONT_FRAGMENT
#define NX_WEB_HTTP_TIME_TO_LIVE                        128
#define NX_WEB_HTTP_TYPE_OF_SERVICE                     NX_IP_NORMAL
#define NX_WEB_HTTP_MAX_NAME                            20
#define NX_WEB_HTTP_MAX_PASSWORD                        20

/* Web HTTP Client Configuration. */
#if 0
#ifndef NX_WEB_HTTPS_ENABLE
#define NX_WEB_HTTPS_ENABLE
#endif
#endif
#define NX_WEB_HTTP_CLIENT_MIN_PACKET_SIZE              300

/* Web HTTP Server Configuration. */

#define NX_WEB_HTTP_SERVER_PRIORITY                     16
#define NX_WEB_HTTP_SERVER_WINDOW_SIZE                  2048
#define NX_WEB_HTTP_SERVER_TIMEOUT                      (10 * NX_IP_PERIODIC_RATE)
#define NX_WEB_HTTP_SERVER_TIMEOUT_ACCEPT               (10 * NX_IP_PERIODIC_RATE)
#define NX_WEB_HTTP_SERVER_TIMEOUT_DISCONNECT           (10 * NX_IP_PERIODIC_RATE)
#define NX_WEB_HTTP_SERVER_TIMEOUT_RECEIVE              (10 * NX_IP_PERIODIC_RATE)
#define NX_WEB_HTTP_SERVER_TIMEOUT_SEND                 (10 * NX_IP_PERIODIC_RATE)
#define NX_WEB_HTTP_MAX_HEADER_FIELD                    256
#define NX_WEB_HTTP_SERVER_MAX_PENDING                  4
#define NX_WEB_HTTP_MAX_RESOURCE                        40
#define NX_WEB_HTTP_SERVER_SESSION_MAX                  2
#define NX_WEB_HTTP_SERVER_MIN_PACKET_SIZE              600
#define NX_WEB_HTTP_SERVER_THREAD_TIME_SLICE            2
#define NX_WEB_HTTP_SERVER_TRANSMIT_QUEUE_DEPTH         20
#define NX_WEB_HTTP_SERVER_RETRY_SECONDS                2
#define NX_WEB_HTTP_SERVER_RETRY_MAX                    10
#define NX_WEB_HTTP_SERVER_RETRY_SHIFT                  1

/* SMTP Client Configuration */
#define NX_SMTP_CLIENT_TCP_WINDOW_SIZE              (1460)
#define NX_SMTP_CLIENT_PACKET_TIMEOUT               (2 * NX_IP_PERIODIC_RATE)
#define NX_SMTP_CLIENT_CONNECTION_TIMEOUT           (10 * NX_IP_PERIODIC_RATE)
#define NX_SMTP_CLIENT_DISCONNECT_TIMEOUT           (5 * NX_IP_PERIODIC_RATE)
#define NX_SMTP_GREETING_TIMEOUT                    (10 * NX_IP_PERIODIC_RATE)
#define NX_SMTP_ENVELOPE_TIMEOUT                    (10 * NX_IP_PERIODIC_RATE)
#define NX_SMTP_MESSAGE_TIMEOUT                     (30 * NX_IP_PERIODIC_RATE)
#define NX_SMTP_CLIENT_SEND_TIMEOUT                 (5 * NX_IP_PERIODIC_RATE)
#define NX_SMTP_SERVER_CHALLENGE_MAX_STRING         (200)
#define NX_SMTP_CLIENT_MAX_PASSWORD                 (20)
#define NX_SMTP_CLIENT_MAX_USERNAME                 (40)

/* Telnet Server Configuration */

#define NX_TELNET_SERVER_PRIORITY                           (16)
#define NX_TELNET_MAX_CLIENTS                               (4)
#define NX_TELNET_SERVER_WINDOW_SIZE                        (2048)
#define NX_TELNET_SERVER_TIMEOUT                            (10 * NX_IP_PERIODIC_RATE)
#define NX_TELNET_ACTIVITY_TIMEOUT                          (600)
#define NX_TELNET_TIMEOUT_PERIOD                            (60)
#define NX_TELNET_SERVER_PACKET_PAYLOAD                     (300)
#define NX_TELNET_SERVER_PACKET_POOL_SIZE                   (2048)

/* SNTP Client Configuration */
#define NX_SNTP_CLIENT_THREAD_STACK_SIZE           (2048)
#define NX_SNTP_CLIENT_THREAD_TIME_SLICE           (TX_NO_TIME_SLICE)
#define NX_SNTP_CLIENT_THREAD_PRIORITY             (2)
#define NX_SNTP_CLIENT_PREEMPTION_THRESHOLD         NX_SNTP_CLIENT_THREAD_PRIORITY
#define NX_SNTP_CLIENT_UDP_SOCKET_NAME             "SNTP Client socket"
#define NX_SNTP_CLIENT_UDP_PORT                    (123)
#define NX_SNTP_SERVER_UDP_PORT                    (123)
#define NX_SNTP_CLIENT_TIME_TO_LIVE                (128)
#define NX_SNTP_CLIENT_MAX_QUEUE_DEPTH             (5)
#define NX_SNTP_CLIENT_PACKET_TIMEOUT              (1 * NX_IP_PERIODIC_RATE)
#define NX_SNTP_CLIENT_NTP_VERSION                 (3)
#define NX_SNTP_CLIENT_MIN_NTP_VERSION             (3)
#define NX_SNTP_CLIENT_MIN_SERVER_STRATUM          (2)
#define NX_SNTP_CLIENT_MIN_TIME_ADJUSTMENT         (10)
#define NX_SNTP_CLIENT_MAX_TIME_ADJUSTMENT         (180000)
#define NX_SNTP_CLIENT_IGNORE_MAX_ADJUST_STARTUP   (NX_TRUE)
#define NX_SNTP_CLIENT_MAX_TIME_LAPSE              (7200)
#define NX_SNTP_UPDATE_TIMEOUT_INTERVAL            (1)
#define NX_SNTP_CLIENT_UNICAST_POLL_INTERVAL       (3600)
#define NX_SNTP_CLIENT_EXP_BACKOFF_RATE            (2)
#define NX_SNTP_CLIENT_RTT_REQUIRED                (NX_FALSE)
#define NX_SNTP_CLIENT_MAX_ROOT_DISPERSION         (50000)
#define NX_SNTP_CLIENT_INVALID_UPDATE_LIMIT        (3)
#define NX_SNTP_CLIENT_RANDOMIZE_ON_STARTUP        (NX_FALSE)
#define NX_SNTP_CLIENT_SLEEP_INTERVAL              (1)
#define NX_SNTP_CURRENT_YEAR                       (2016)

/* TFTP Common Configuration */
#define NX_TFTP_ERROR_STRING_MAX                    (64)
#define NX_TFTP_TIME_TO_LIVE                        128
#define NX_TFTP_FRAGMENT_OPTION                     NX_DONT_FRAGMENT
#define NX_TFTP_TYPE_OF_SERVICE                     (NX_IP_NORMAL)

/* TFTP Client Configuration */
#define NX_TFTP_SOURCE_PORT                         NX_ANY_PORT

/* TFTP Server Configuration */

#ifdef NX_TFTP_SERVER_RETRANSMIT_ENABLE
#define NX_TFTP_SERVER_TIMEOUT_PERIOD                       (20)
#define NX_TFTP_SERVER_RETRANSMIT_TIMEOUT                   (200)
#define NX_TFTP_SERVER_MAX_RETRIES                          (5)
#else
#define NX_TFTP_MAX_CLIENT_RETRANSMITS                      (2)
#endif
#define NX_TFTP_SERVER_PRIORITY                             (16)
#define NX_TFTP_MAX_CLIENTS                                 (10)
#define NX_TFTP_SERVER_TIME_SLICE                           (2)

/* MQTT Client Configuration */
#define NXD_MQTT_SOCKET_TIMEOUT (0xFFFFFFFF)
#define NXD_MQTT_KEEPALIVE_TIMER_RATE (1 * TX_TIMER_TICKS_PER_SECOND)
#define NXD_MQTT_PING_TIMEOUT_DELAY (1 * TX_TIMER_TICKS_PER_SECOND)

#if (0)
#ifndef NX_SECURE_ENABLE
#define NX_SECURE_ENABLE
#endif
#endif

/* NAT Configuration */
#define NX_NAT_MIN_ENTRY_COUNT              (3)
#define NX_NAT_ENTRY_RESPONSE_TIMEOUT       (240 * NX_IP_PERIODIC_RATE)
#define NX_NAT_START_TCP_PORT               (20000)
#define NX_NAT_END_TCP_PORT                 (30000)
#define NX_NAT_START_UDP_PORT               (20000)
#define NX_NAT_END_UDP_PORT                 (30000)
#define NX_NAT_START_ICMP_QUERY_ID          (20000)
#define NX_NAT_END_ICMP_QUERY_ID            (30000)

/* FTP Common Configuration */
#define NX_FTP_CONTROL_TOS                  NX_IP_NORMAL
#define NX_FTP_DATA_TOS                     NX_IP_NORMAL
#define NX_FTP_FRAGMENT_OPTION              NX_DONT_FRAGMENT
#define NX_FTP_TIME_TO_LIVE                 128
#define NX_FTP_TIMEOUT_PERIOD               60

/* FTP Client Configuration */
#define NXD_FTP_CLIENT_SOURCE_PORT NX_ANY_PORT

/* FTP Server Configuration */
#define  NX_FTP_SERVER_PRIORITY                                   16
#define  NX_FTP_SERVER_TIME_SLICE                                 2
#define  NX_FTP_MAX_CLIENTS                                       4
#define  NX_FTP_CONTROL_WINDOW_SIZE                               400
#define  NX_FTP_DATA_WINDOW_SIZE                                  2048
#define  NX_FTP_SERVER_TIMEOUT                                    (1 * NX_IP_PERIODIC_RATE)
#define  NX_FTP_USERNAME_SIZE                                     20
#define  NX_FTP_PASSWORD_SIZE                                     20
#define  NX_FTP_ACTIVITY_TIMEOUT                                  240
#define  NX_FTP_SERVER_RETRY_SECONDS                              2
#define  NX_FTP_SERVER_TRANSMIT_QUEUE_DEPTH                       20
#define  NX_FTP_SERVER_RETRY_MAX                                  10
#define  NX_FTP_SERVER_RETRY_SHIFT                                2

/* SNMP Agent Configuration */
#define NX_SNMP_AGENT_THREAD_STACK_SIZE           (4096)
#define NX_SNMP_AGENT_PRIORITY                    (16)
#define NX_SNMP_TYPE_OF_SERVICE                   NX_IP_NORMAL
#define NX_SNMP_FRAGMENT_OPTION                   NX_DONT_FRAGMENT
#define NX_SNMP_TIME_TO_LIVE                      (128)
#define NX_SNMP_AGENT_TIMEOUT                     (100)
#define NX_SNMP_MAX_OCTET_STRING                  (255)
#define NX_SNMP_MAX_CONTEXT_STRING                (32)
#define NX_SNMP_MAX_USER_NAME                     (64)
#define NX_SNMP_MAX_SECURITY_KEY                  (64)
#define NX_SNMP_PACKET_SIZE                       (560)
#define NX_SNMP_AGENT_PORT                        (161)
#define NX_SNMP_MANAGER_TRAP_PORT                 (162)
#define NX_SNMP_MAX_TRAP_KEY                      (64)

/* POP3 Client Configuration */
#define NXD_POP3_CLIENT_MAIL_BUFFER_SIZE            (2000)
#define NXD_POP3_CLIENT_PACKET_TIMEOUT              (1 * NX_IP_PERIODIC_RATE)
#define NXD_POP3_CLIENT_CONNECTION_TIMEOUT          (30 * NX_IP_PERIODIC_RATE)
#define NXD_POP3_CLIENT_DISCONNECT_TIMEOUT          (2 * NX_IP_PERIODIC_RATE)
#define NX_POP3_TCP_SOCKET_SEND_WAIT               (2 * NX_IP_PERIODIC_RATE)
#define NX_POP3_SERVER_REPLY_TIMEOUT               (10 * NX_IP_PERIODIC_RATE)
#define NXD_POP3_CLIENT_TCP_WINDOW_SIZE             (1460)
#define NX_POP3_MAX_USERNAME                       (40)
#define NX_POP3_MAX_PASSWORD                       (20)

#ifdef __cplusplus
}
#endif
#endif /* NX_USER_H_ */
