#include "visualizer.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <thread>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <character_type.pb.h>

#pragma comment(lib, "Ws2_32.lib")

// клиент упаковывает и передает данные
void init_client(struct sockaddr* local_addr, int local_addrlen) {
    int udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    connect(udp_socket, local_addr, local_addrlen);

    CCharacterSystem::CCharacterType type{ L"123", L"456", {{ {{{1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16}}, {17,18,19,20}, {21,22,23,24}}, 1, 2, 3, 4, 5 }} };

    CCharacterSystem CS;

    int size = 2048;
    char buffer[2048];
    void* buffer_ptr = static_cast<void*>(buffer);
    CS.serialize(type, buffer_ptr, size);

    int count = send(udp_socket, buffer, size, 0);

    closesocket(udp_socket);
}

// сервер принимает и распаковывает данные
void init_server(struct sockaddr* local_addr, int local_addrlen) {
    int udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    bind(udp_socket, local_addr, local_addrlen);

    int count, size = 2048;
    char buffer[2048];
    void* buffer_ptr = static_cast<void*>(buffer);

    count = recv(udp_socket, buffer, size, 0);

    CCharacterSystem CS;

    CCharacterSystem::CCharacterType CT;

    CS.deserialize(CT, buffer_ptr, count);

    assert(CT.m_data.size() == 1);
    assert(CT.m_strTypeName == L"123");
    assert(CT.m_strCharacterName == L"456");

    auto data = CT.m_data[0];
    assert(data.m_nAnimationMode == 1);
    assert(data.m_nCurrentAnimation == 2);
    assert(data.m_nSavedAnimation == 3);
    assert(data.m_nCountFrame == 4);
    assert(data.m_nSavedFrame == 5);


    std::cout << "Сериализация прошла успешно!";

    closesocket(udp_socket);
}

int main() {

    setlocale(LC_ALL, "ru");

    WSADATA wsa_data;

    WSAStartup(MAKEWORD(2, 2), &wsa_data);

    struct in_addr local_ip;
    inet_pton(AF_INET, "127.0.0.1", &local_ip);
    unsigned short local_port = 8888;

    struct sockaddr_in local_addr_in;

    memset(&local_addr_in, 0, sizeof(struct sockaddr_in));
    local_addr_in.sin_addr = local_ip;
    local_addr_in.sin_port = htons(local_port);
    local_addr_in.sin_family = AF_INET;
    struct sockaddr* local_addr = (struct sockaddr*)&local_addr_in;
    int local_addrlen = sizeof(struct sockaddr);


    std::thread th1(init_server, local_addr, local_addrlen);
    std::thread th2(init_client, local_addr, local_addrlen);
    
    th2.join();
    th1.join();

    WSACleanup();
}