#include "ProcCommunicator.h"
#include <iostream>
#include <thread>
#include <chrono>
#include<mutex>

static const std::string shared_mem_name{"/sh_mem1"};

// Function to be executed in a separate thread
void backgroundTask()
{
    std::cout << "Background task started...\n";
    ProcCommunicator *slave = new ProcCommunicator(false, true, shared_mem_name);
    Message *res = nullptr;

    int counter = 0;

    while (counter < 200)
    {
        {
            Message msg{777, MessageType::HANDSHAKE_OK};
            Message* res = slave->receive();
            //std::cout << res.id << std::endl;
            msg.id = res->id;
            //std::cout << msg.id << std::endl;
            slave->send(&msg);
            //std::cout << res.id << " -> " << msg.id << "." << std::endl;
            counter++;
        }
    }

    delete slave;
    std::cout << "Background task completed.\n";
}

void backgroundTaskMasterMaster()
{
    std::cout << "Background Master task started...\n";
    ProcCommunicator master(true, true, shared_mem_name);
    Message msg_hand{1, MessageType::HANDSHAKE};

    int counter = 0;

    while (counter < 100)
    {

        master.send(&msg_hand);
        auto msg_resp = master.receive();
        std::cout << "m 1 =" << msg_resp->id << std::endl;
        counter++;
    }

    std::cout << "Background Master task completed.\n";
}

int main()
{
    std::cout << "Main thread starts...\n";
    std::vector<int> vec1{1, 2, 3, 4, 5};

    Message msg_hand{2, MessageType::HANDSHAKE};
    ProcCommunicator master(true, true, shared_mem_name);

    std::thread worker(backgroundTask);
    std::thread worker_master(backgroundTaskMasterMaster);

    // std::this_thread::sleep_for(std::chrono::seconds(5));
    int counter = 0;

    while (counter < 100)
    {
        master.send(&msg_hand);
        auto msg_resp = master.receive();
        //std::cout << "m 2 " << msg_resp->id << std::endl;
        counter++;
    }

    if (worker.joinable())
        worker.join();

    if (worker_master.joinable())
        worker_master.join();

    return 0;
}