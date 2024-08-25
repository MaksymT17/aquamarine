#include "ProcCommunicator.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

static const std::string shared_mem_name{"/shmsh1"};
static constexpr size_t MSG_COUNT = 1000;

void backgroundTask1()
{
    std::cout << "Background Master_1 starting...\n";
    ProcCommunicator master(true, true, shared_mem_name);
    Message msg_hand{1, MessageType::HANDSHAKE};

    int counter = 0;

    while (counter < MSG_COUNT)
    {
        master.send(&msg_hand);
        auto msg_resp = master.receive();
        
        //std::cout << "m 1 =" << msg_resp->id << std::endl;
        master.ackNotify();
        counter++;
    }

    std::cout << "Background Master_1 completed.\n";
}

void backgroundTask2()
{
    std::cout << "Background Master_2 starting...\n";
    Message msg_hand{2, MessageType::HANDSHAKE};
    ProcCommunicator master(true, true, shared_mem_name);
    int counter = 0;

    while (counter < MSG_COUNT)
    {
        master.send(&msg_hand);
        auto msg_resp = master.receive();
        std::cout << "m 2 " << msg_resp->id << std::endl;
        master.ackNotify();
        counter++;
    }

    std::cout << "Background Master_2 completed.\n";
}

void backgroundTask3()
{
    std::cout << "Background Master_3 starting...\n";
    Message msg_hand{3, MessageType::HANDSHAKE};
    ProcCommunicator master(true, true, shared_mem_name);
    int counter = 0;

    while (counter < MSG_COUNT)
    {
        master.send(&msg_hand);
        auto msg_resp = master.receive();
        //std::cout << "m 3 " << msg_resp->id << std::endl;
        master.ackNotify();
        counter++;
    }

    std::cout << "Background Master_3 completed.\n";
}

void backgroundTask4()
{
    std::cout << "Background Master_4 starting...\n";
    Message msg_hand{4, MessageType::HANDSHAKE};
    ProcCommunicator master(true, true, shared_mem_name);
    int counter = 0;

    while (counter < MSG_COUNT)
    {
        master.send(&msg_hand);
        auto msg_resp = master.receive();
        //std::cout << "m 4 " << msg_resp->id << std::endl;
        master.ackNotify();
        counter++;
    }

    std::cout << "Background Master_4 completed.\n";
}

void backgroundTask5()
{
    std::cout << "Background Master_5 starting...\n";
    Message msg_hand{5, MessageType::HANDSHAKE};
    ProcCommunicator master(true, true, shared_mem_name);
    int counter = 0;

    while (counter < MSG_COUNT)
    {
        master.send(&msg_hand);
        auto msg_resp = master.receive();
        //std::cout << "m 5 " << msg_resp->id << std::endl;
        master.ackNotify();
        counter++;
    }

    std::cout << "Background Master_5 completed.\n";
}

int main()
{
    std::cout << "Server starts...\n";
    ProcCommunicator *slave = new ProcCommunicator(false, true, shared_mem_name);
    Message *res = nullptr;

    int counter = 0;
    std::thread worker_master1(backgroundTask1);
    std::thread worker_master2(backgroundTask2);
    std::thread worker_master3(backgroundTask3);
    std::thread worker_master4(backgroundTask4);
    std::thread worker_master5(backgroundTask5);

    while (counter < 5 * MSG_COUNT)
    {
        Message msg{777, MessageType::HANDSHAKE_OK};
        Message *res = slave->receive();
        msg.id = res->id;
        slave->send(&msg);
        counter++;
    }

    delete slave;

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

    std::cout << "Server has finished work...\n";
    return 0;
}