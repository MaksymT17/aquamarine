#include "ProcCommunicator.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include "gtest/gtest.h"
static const std::string shared_mem_name{"/shmem_test"};

// Function to be executed in a separate thread
void backgroundTask()
{
    std::cout << "Background task starts.\n";
    Message msg_hand{2, MessageType::HANDSHAKE};
    ProcCommunicator master(true, true, shared_mem_name);
    int counter = 0;

    while (counter < 10)
    {
        //each client must receive its id
        master.send(&msg_hand);
        auto msg_resp = master.receive();
        EXPECT_EQ(msg_resp->id, 2);
        master.ackNotify();
        counter++;
    }

    std::cout << "Background task completed.\n";
}

void backgroundTaskMasterMaster()
{
    std::cout << "Background Master task started...\n";
    ProcCommunicator master(true, true, shared_mem_name);
    Message msg_hand{1, MessageType::HANDSHAKE};

    int counter = 0;

    while (counter < 10)
    {
        master.send(&msg_hand);
        auto msg_resp = master.receive();
        EXPECT_EQ(msg_resp->id, 1);
        master.ackNotify();
        counter++;
    }

    std::cout << "Background Master task completed.\n";
}
TEST(MessagingTest, TestMultiMasterMode)
{
    std::cout << "Server starts...\n";
    ProcCommunicator *slave = new ProcCommunicator(false, true, shared_mem_name);
    Message *res = nullptr;

    int counter = 0;
    std::thread worker(backgroundTask);
    std::thread worker_master(backgroundTaskMasterMaster);
    while (counter < 20)
    {
        Message msg{777, MessageType::HANDSHAKE_OK};
        Message *res = slave->receive();
        msg.id = res->id;
        slave->send(&msg);
        counter++;
    }

    delete slave;

    if (worker.joinable())
        worker.join();

    if (worker_master.joinable())
        worker_master.join();
}