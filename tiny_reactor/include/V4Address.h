#ifndef V4ADDRESS_H
#define V4ADDRESS_H

extern "C"{
#include <netinet/in.h>
#include <arpa/inet.h>
}

#include <cstring>
#include <cstdint>
#include <string>

class V4Address {
public:
    V4Address() = default;

    V4Address(const V4Address &other) = default;

    V4Address(uint32_t ip_net, uint16_t port_net) {
        ::memset(&addr_, 0, sizeof(addr_));
        addr_.sin_addr.s_addr = ip_net;
        addr_.sin_port = port_net;
        net_ready_ = 1;
    }

    V4Address(const std::string &ip_host, uint16_t port_host) {
        ip_host_ = ip_host;
        port_host_ = port_host;
        host_ready_ = 1;
    }

    inline int SetHostAddress(const std::string &ip_host, uint16_t port_host);

    inline void SetNetAddress(uint32_t ip_net, uint16_t port_net);

    inline void NetReady();

    [[nodiscard]] inline uint16_t GetPortNet();

    [[nodiscard]] inline uint16_t Port();

    [[nodiscard]] inline uint32_t GetIpNet();

    [[nodiscard]] inline std::string Ip();

    [[nodiscard]] inline sockaddr *PSockAddr();

    [[nodiscard]] inline socklen_t *PLen();

    [[nodiscard]] inline std::string ToString();

    [[nodiscard]] static inline uint32_t IpHost2Net(const std::string &ip);

    [[nodiscard]] static inline std::string IpNet2Host(uint32_t ip_net);

    [[nodiscard]] static inline uint16_t PortHost2Net(uint16_t port);

    [[nodiscard]] static inline uint16_t PortNet2Host(uint16_t port_net);

public:
    struct sockaddr_in addr_{};
    socklen_t len_ = sizeof(sockaddr_in);
    int host_ready_ = 0;
    int str_ready_ = 0;
    int net_ready_ = 0;

private:
    uint16_t port_host_ = 0;
    std::string ip_host_;
    std::string addr_str_;

};

/**
 * 设置网络字节序
 * @param ip_net    IP 网络字节序
 * @param port_net  PORT 网络字节序
 */
inline void V4Address::SetNetAddress(uint32_t ip_net, uint16_t port_net) {
    ::memset(&addr_, 0, sizeof(addr_));
    addr_.sin_addr.s_addr = ip_net;
    addr_.sin_port = port_net;
    net_ready_ = 1;
}

/**
 * 设置主机字节序
 * @param ip_host   IP 主机字节序
 * @param port_host PORT 主机字节序
 */
inline int
V4Address::SetHostAddress(const std::string &ip_host, uint16_t port_host) {
    ip_host_ = ip_host;
    port_host_ = port_host;
    host_ready_ = 1;

    if (!net_ready_) {
        ::memset(&addr_, 0, sizeof(addr_));
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons(port_host_);
        int ret = inet_pton(AF_INET, ip_host_.c_str(), &addr_.sin_addr.s_addr);
        if (ret <= 0) {
            return -1;
        }
        net_ready_ = 1;
    }

    return 1;
}

/**
 * 设置网络字节序已准备好
 */
inline void V4Address::NetReady() {
    net_ready_ = 1;
}

/**
 * 获取网络字节序的ip
 */
inline uint32_t V4Address::GetIpNet() {
    if (net_ready_) {
        return addr_.sin_addr.s_addr;
    } else if (host_ready_) {
        ::memset(&addr_, 0, sizeof(addr_));
        int ret = inet_pton(AF_INET, ip_host_.c_str(), &addr_.sin_addr.s_addr);
        if (ret <= 0) {
            return INADDR_NONE;
        }
        addr_.sin_port = htons(port_host_);
        net_ready_ = 1;
        return addr_.sin_addr.s_addr;
    } else {
        return INADDR_NONE;
    }
}

/**
 * 获取网络字节序的端口
 */
inline uint16_t V4Address::GetPortNet() {
    if (net_ready_) {
        return addr_.sin_port;
    } else if (host_ready_) {
        ::memset(&addr_, 0, sizeof(addr_));
        int ret = inet_pton(AF_INET, ip_host_.c_str(), &addr_.sin_addr.s_addr);
        if (ret <= 0) {
            return static_cast<uint16_t>(INADDR_NONE);
        }
        addr_.sin_port = htons(port_host_);
        net_ready_ = 1;
        return addr_.sin_port;
    } else {
        return static_cast<uint16_t>(INADDR_NONE);
    }
}

/**
 * 获取主机字节序的ip
 */
inline std::string V4Address::Ip() {
    if (host_ready_) {
        return ip_host_;
    } else if (net_ready_) {
        char ip[INET_ADDRSTRLEN] = {0};
        const char *ret = inet_ntop(AF_INET, &addr_.sin_addr.s_addr, ip,
                                    sizeof(ip));
        if (!ret) {
            return {};
        }
        ip_host_ = ip;
        port_host_ = ntohs(addr_.sin_port);
        host_ready_ = 1;
        return ip_host_;
    } else {
        return {};
    }
}

/**
 * 获取主机字节序的端口
 */
inline uint16_t V4Address::Port() {
    if (host_ready_) {
        return port_host_;
    } else if (net_ready_) {
        char ip[INET_ADDRSTRLEN] = {0};
        const char *ret = inet_ntop(AF_INET, &addr_.sin_addr.s_addr, ip,
                                    sizeof(ip));
        if (!ret) {
            return static_cast<uint16_t>(INADDR_NONE);
        }
        ip_host_ = ip;
        port_host_ = ntohs(addr_.sin_port);
        net_ready_ = 1;
        return port_host_;
    } else {
        return static_cast<uint16_t>(INADDR_NONE);
    }
}

/**
 * 获取字符串形式的地址
 */
inline std::string V4Address::ToString() {
    if (str_ready_) {
        return addr_str_;
    } else {
        addr_str_ = Ip() + ":" + std::to_string(Port());
        str_ready_ = 1;
        return addr_str_;
    }
}

/**
 * 将ip字符串转换为网络字节序
 * @param ip    字符串 IP
 * @return  IP 网络字节序
 */
inline uint32_t V4Address::IpHost2Net(const std::string &ip) {
    uint32_t ipNet;
    int ret = inet_pton(AF_INET, ip.c_str(), &ipNet);
    if (ret <= 0) {
        return INADDR_NONE;
    } else {
        return ipNet;
    }
}

/**
 * 将网络字节序转换为ip字符串
 * @param ip_net    IP 网络字节序
 * @return  字符串 IP
 */
inline std::string V4Address::IpNet2Host(uint32_t ip_net) {
    char ip[INET_ADDRSTRLEN] = {0};
    const char *ret = inet_ntop(AF_INET, &ip_net, ip, sizeof(ip));
    if (!ret) {
        return {};
    } else {
        return ip;
    }
}

/**
 * 将主机字节序的端口转换为网络字节序
 * @param port  主机字节序
 * @return  网络字节序
 */
inline uint16_t V4Address::PortHost2Net(uint16_t port) {
    return htons(port);
}

/**
 * 将网络字节序的端口转换为主机字节序
 * @param port_net  网络字节序
 * @return  主机字节序
 */
inline uint16_t V4Address::PortNet2Host(uint16_t port_net) {
    return ntohs(port_net);
}

/**
 * 获取 sockaddr 指针
 */
inline sockaddr *V4Address::PSockAddr() {
    if (net_ready_ || !host_ready_) {
        return reinterpret_cast<sockaddr *> (&addr_);
    } else {
        ::memset(&addr_, 0, sizeof(addr_));
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons(port_host_);
        int ret = inet_pton(AF_INET, ip_host_.c_str(), &addr_.sin_addr.s_addr);
        if (ret > 0) {
            return nullptr;
        } else {
            net_ready_ = 1;
            return reinterpret_cast<sockaddr *>(&addr_);
        }
    }
}

/**
 * 获取 sockaddr 长度指针
 */
inline socklen_t *V4Address::PLen() {
    return &len_;
}

#endif  // V4ADDRESS_H
