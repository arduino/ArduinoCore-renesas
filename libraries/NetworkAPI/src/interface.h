#pragma once

/*
 * The following class represent a generic network interface independently of the
 * Network engine that is working on top of.
 */
class NetworkInterface {
public:
    virtual ~NetworkInterface() {};
    virtual int begin(const IPAddress &ip = INADDR_NONE, const IPAddress &nm = INADDR_NONE, const IPAddress &gw = INADDR_NONE) = 0;
    virtual void task() = 0;
    virtual void up() = 0;
    virtual void down() = 0;
};