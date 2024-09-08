#include "ProcCommunicator.h"
#include "ServerProcCommunicator.h"
#include "ClientProcCommunicator.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#ifndef _WIN32
#include <unistd.h>
#endif

static const std::string shared_mem_name{"/shmsh__47"};
static constexpr size_t MSG_COUNT = 10000;

void backgroundTask1()
{
    std::cout << "Background Master_1 starting...\n";
    ClientProcCommunicator master(shared_mem_name);
    Message request{1, MessageType::HANDSHAKE};
    Message response{666, MessageType::HANDSHAKE};

    int counter = 0;

    while (counter < MSG_COUNT)
    {
        master.sendRequestGetResponse(&request, response);
        std::cout << "m 1 =" << response.id << " " << response.type << std::endl;
        counter++;
    }

    std::cout << "Background Master_1 completed.\n";
}

void backgroundTask2()
{
    // std::cout << "Background Master_2 starting...\n";
    ClientProcCommunicator master(shared_mem_name);
    Message request{2, MessageType::HANDSHAKE};
    Message response{666, MessageType::HANDSHAKE};

    int counter = 0;

    while (counter < MSG_COUNT)
    {
        master.sendRequestGetResponse(&request, response);
        // std::cout << "m 2 =" << response.id << std::endl;
        counter++;
    }

    std::cout << "Background Master_2 completed.\n";
}

void backgroundTask3()
{
    std::cout << "Background Master_3 starting...\n";
    ClientProcCommunicator master(shared_mem_name);
    Message request{3, MessageType::HANDSHAKE};
    Message response{666, MessageType::HANDSHAKE};

    int counter = 0;

    while (counter < MSG_COUNT)
    {
        master.sendRequestGetResponse(&request, response);
        counter++;
    }

    std::cout << "Background Master_3 completed.\n";
}

void backgroundTask4()
{
    std::cout << "Background Master_4 starting...\n";
    ClientProcCommunicator master(shared_mem_name);
    Message request{4, MessageType::HANDSHAKE};
    Message response{666, MessageType::HANDSHAKE};

    int counter = 0;

    while (counter < MSG_COUNT)
    {
        master.sendRequestGetResponse(&request, response);
        // std::cout << "m 2 =" << response.id << std::endl;
        counter++;
    }

    std::cout << "Background Master_4 completed.\n";
}

void backgroundTask5()
{
    std::cout << "Background Master_5 starting...\n";
    ClientProcCommunicator master(shared_mem_name);
    Message request{5, MessageType::HANDSHAKE};
    Message response{666, MessageType::HANDSHAKE};

    int counter = 0;

    while (counter < MSG_COUNT)
    {
        master.sendRequestGetResponse(&request, response);
        // std::cout << "m 5 =" << response.id << std::endl;
        counter++;
    }

    std::cout << "Background Master_5 completed.\n";
}
int main()
{
    {
        // test: one to one
        std::cout << "Server starts...\n";
        ServerProcCommunicator *slave = new ServerProcCommunicator(shared_mem_name);
        Message *res = nullptr;

        int counter = 0;
        std::thread worker_master1(backgroundTask1);

        while (counter < MSG_COUNT * 1)
        {
            Message msg{777, MessageType::HANDSHAKE_OK};
            Message *res = slave->receive();
            // usleep(6000);
            msg.id = res->id;
            slave->send(&msg);

            counter++;
        }
        std::cout << "Server syncing...\n";

        if (worker_master1.joinable())
            worker_master1.join();

        delete slave;
        std::cout << "Server has finished work...\n";
    }

    {
        // test: one to many
        std::cout << "Server starts...\n";
        ServerProcCommunicator *slave = new ServerProcCommunicator(shared_mem_name);
        Message *res = nullptr;

        int counter = 0;
        std::thread worker_master1(backgroundTask1);
        std::thread worker_master2(backgroundTask2);
        std::thread worker_master3(backgroundTask3);
        std::thread worker_master4(backgroundTask4);
        std::thread worker_master5(backgroundTask5);

        while (counter < MSG_COUNT * 5)
        {
            // std::cout << "Server st1 "<<counter<<std::endl;
            Message msg{777, MessageType::HANDSHAKE_OK};
            Message *res = slave->receive();
            // usleep(6000);
            msg.id = res->id;
            slave->send(&msg);

            counter++;
            // std::cout << "Server st1 "<<counter<<std::endl;
        }
        std::cout << "Server syncing...\n";

        if (worker_master1.joinable())
            worker_master1.join();
        if (worker_master2.joinable())
            worker_master2.join();
        if (worker_master3.joinable())
            worker_master3.join();
        if (worker_master4.joinable())
            worker_master4.join();
        if (worker_master5.joinable())
            worker_master5.join();

        delete slave;
        std::cout << "Server has finished work...\n";
    }
    return 0;
}
