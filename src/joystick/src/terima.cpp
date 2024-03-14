#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

float analog_l[2][2], analog_r[2][2];
int8_t button_kotak[2], button_lingkaran[2], button_silang[2], button_segitiga[2], button_r1[2], button_l1[2], button_up[2], button_down[2], button_left[2], button_right[2], button_r2[2], button_l2[2];


void sender(int socket, sockaddr_in& remote_addr) {
    for (;;) {
        std::string message;
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        sendto(socket, message.c_str(), message.size(), 0, (struct sockaddr*)&remote_addr, sizeof(remote_addr));
    }
}

void receiver(int socket) {
    for (;;) {
        char buffer[128];
        // /float dst;
        socklen_t len;
        struct sockaddr_in sender_addr;
        int received = recvfrom(socket, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&sender_addr, &len);


        // memcpy(&dst, buffer, strlen(buffer) + 1);
        // memcpy(&dst, buffer, strlen(buffer) + 1);
        memcpy(&analog_l[0][0], buffer, 4);
        memcpy(&analog_l[0][1], buffer + 4, 4);
        memcpy(&analog_r[0][0], buffer + 8, 4);
        memcpy(&analog_r[0][1], buffer + 12, 4);
        memcpy(&button_silang[0], buffer + 13, 1);
        memcpy(&button_lingkaran[0], buffer + 14, 1);
        memcpy(&button_segitiga[0], buffer + 15, 1);
        memcpy(&button_kotak[0], buffer + 16, 1);
        memcpy(&button_l1[0], buffer + 17, 1);
        memcpy(&button_r1[0], buffer + 18, 1);
        memcpy(&button_l2[0], buffer + 19, 1);
        memcpy(&button_r2[0], buffer + 20, 1);
        memcpy(&button_up[0], buffer + 21, 1);
        memcpy(&button_down[0], buffer + 22, 1);
        memcpy(&button_left[0], buffer + 23, 1);
        memcpy(&button_right[0], buffer + 24, 1);

        std::cout<< "Silang " << static_cast<int>(button_silang[0]) << " Lingkaran " << static_cast<int>(button_lingkaran[0]) << " Segitiga " << static_cast<int>(button_segitiga[0]) << " Kotak " << static_cast<int>(button_kotak[0]) << std::endl;
        std::cout<< "L1 " << static_cast<int>(button_l1[0]) << " R1 " << static_cast<int>(button_r1[0]) << " L2 " << static_cast<int>(button_l2[0]) << " R2 " << static_cast<int>(button_r2[0]) << std::endl;
        std::cout<< "Naik " << static_cast<int>(button_up[0]) << " Turun " << static_cast<int>(button_down[0]) << " Kiri " << static_cast<int>(button_left[0]) << " Kanan " << static_cast<int>(button_right[0]) << std::endl;
        std::cout<< "Analog L " << analog_l[0][0] << " " << analog_l[0][1] << std::endl;
        std::cout<< "Analog R " << analog_r[0][0] << " " << analog_r[0][1] << std::endl;

        // std::cout << "Silang " << buffer[13] << " Lingkaran " << buffer[14] << " Segitiga " << buffer[15] << " Kotak " << buffer[16] << std::endl;
        // std::cout << "L1 " << buffer[17] << " R1 " << buffer[18] << " L2 " << buffer[19] << " R2 " << buffer[20] << std::endl;
        // std::cout << "Naik " << buffer[21] << " Turun " << buffer[22] << " Kiri " << buffer[23] << " Kanan " << buffer[24] << std::endl;
        // std::cout << "Analog L " << buffer[0] << " " << buffer[4] << std::endl;
        // std::cout << "Analog R " << buffer[8] << " " << buffer[12] << std::endl;


        
        // for (int i = 0 ; i < 4; i++)
        

            //std::cout << "Received" << " : " <<dst << std::endl;
        
            // std::cout << "Received : " << dst << std::endl;


        // if (received > 0) {
        //     buffer[received] = 0;

        // }
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    struct sockaddr_in remote_addr;
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(12345);
    inet_pton(AF_INET, "192.168.0.172", &remote_addr.sin_addr);

    std::thread sender_thread(sender, sock, std::ref(remote_addr));
    std::thread receiver_thread(receiver, sock);

    sender_thread.join();
    receiver_thread.join();

    close(sock);

    return 0;
}