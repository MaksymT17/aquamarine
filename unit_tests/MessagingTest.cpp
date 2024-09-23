#include "ServerProcCommunicator.h"
#include "ClientProcCommunicator.h"
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
    Message request{1, MessageType::HANDSHAKE};
    Message response{666, MessageType::HANDSHAKE};
    ClientProcCommunicator master(shared_mem_name);
    int counter = 0;

    while (counter < 10)
    {
        //each client must receive its id
        master.sendRequestGetResponse(&request, response);
        EXPECT_EQ(response.id, 1);
        EXPECT_EQ(response.type, MessageType::HANDSHAKE_OK);
        counter++;
    }

    std::cout << "Background task completed.\n";
}

void backgroundTaskMasterMaster()
{
    std::cout << "Background Master task started...\n";
    ClientProcCommunicator master( shared_mem_name);
    Message request{2, MessageType::HANDSHAKE};
    Message response{666, MessageType::HANDSHAKE};
    int counter = 0;

    while (counter < 10)
    {
        //each client must receive its id
        master.sendRequestGetResponse(&request, response);
        EXPECT_EQ(response.id, 2);
        EXPECT_EQ(response.type, MessageType::HANDSHAKE_OK);
        counter++;
    }

    std::cout << "Background Master task completed.\n";
}
TEST(MessagingTest, TestMultiMasterMode)
{
    std::cout << "Server starts...\n";
    ServerProcCommunicator *slave = new ServerProcCommunicator(shared_mem_name);
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