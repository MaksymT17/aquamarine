#include "SilberService.h"
#include "Message.h"

static Configuration default_conf{75, 10, 1, 50, 5, 10.0};
namespace am::service
{
    SilberService::SilberService(const std::string &shMemName) : m_server(std::make_unique<ServerProcCommunicator>(shMemName)),
                                                                 m_amApi(std::make_unique<am::AmApi>(default_conf)),
                                                                 m_isRunning(false)

    {
    }

    void SilberService::start()
    {
        m_isRunning = true;
        while (m_isRunning)
        {
            Message *message = m_server->receive();
            printf("received %zu message\n", message->type);
            if (!m_connections.isRequestValid(message))
            {
                std::cout << "received UNEXPECTED_REQUEST req\n"
                          << message->type << std::endl;
                Message response{message->id, MessageType::UNEXPECTED_REQUEST};
                m_server->send(&response);
                continue;
            }
            if (message->type == MessageType::HANDSHAKE)
            {
                std::cout << "received HANDSHAKE req\n";
                Message msg{message->id, MessageType::HANDSHAKE_OK};
                auto iter = m_connections.processActionUpdate(message);
                m_server->send(&msg);
            }
            else if (message->type == MessageType::SET_CONFIG)
            {
                MessageSetConfig *messageConf = static_cast<MessageSetConfig *>(message);
                std::cout << "received SET_CONFIG req px:" << messageConf->configuration.MinPixelsForObject << std::endl;
                Message msg{message->id, MessageType::SET_CONFIG_OK};
                auto iter = m_connections.processActionUpdate(message);
                // Configuration newConf{messageConf->configuration.AffinityThreshold, messageConf->configuration.MinPixelsForObject, messageConf->configuration.PixelStep, messageConf->configuration.CalculationTimeLimit, messageConf->configuration.IdleTimeout, messageConf->configuration.ThreadsMultiplier};

                m_amApi->setConfiguration(messageConf->configuration);
                std::cout << "received SET_CONFIG OK \n";
                m_server->send(&msg);
                std::cout << "received SET_CONFIG OK sent\n";
            }
            else if (message->type == MessageType::COMPARE_REQUEST)
            {
                std::cout << "received COMPARE_REQUEST req\n";
                auto iter = m_connections.processActionUpdate(message);
                m_amApi->setConfiguration(iter->configuration);

                MessageCompareRequest *messageCompare = static_cast<MessageCompareRequest *>(message);
                if (messageCompare)
                {
                    printf("compare_ %s %s _\n", messageCompare->base, messageCompare->to_compare);
                    auto iter = m_connections.processActionUpdate(message);
                    m_amApi->setConfiguration(iter->configuration); // set configuration for this client
                    printf("conf pixs %zu\n", iter->configuration.MinPixelsForObject);
                    am::analyze::algorithm::DescObjects result;
                    try
                    {
                        result = m_amApi->compare(messageCompare->base, messageCompare->to_compare);
                    }
                    catch (const am::common::exceptions::AmException exc)
                    {
                        std::cout << "Exception has been caught: " << exc.what() << ::std::endl;
                        Message failed{messageCompare->id, MessageType::COMPARE_FAIL};
                        m_server->send(&failed);
                        continue;
                    }
                    std::cout << "received after compare\n";
                    MessageCompareResult compare_result;
                    compare_result.id = messageCompare->id;

                    compare_result.payload_bytes = result.size() * sizeof(Rect);
                    Rect *rects = static_cast<Rect *>(compare_result.payload);
                    size_t id = 0;
                    for (auto &rectangle : result)
                    {
                        if (id == 100) // limit by 100 objects
                        {
                            compare_result.payload_bytes = 100 * sizeof(Rect);
                            break;
                        }
                        printf("_ %zd %zd %zd %zd _\n", rectangle.getLeft(), rectangle.getRight(), rectangle.getMaxHeight(), rectangle.getMinHeight());
                        rects[id++] = {rectangle.getLeft(), rectangle.getRight(), rectangle.getMaxHeight(), rectangle.getMinHeight()};
                        printf("_send %zd %zd %zd %zd _\n", rects[id - 1].l, rects[id - 1].r, rects[id - 1].t, rects[id - 1].b);
                    }
                    compare_result.type = MessageType::COMPARE_RESULT;
                    m_server->send(&compare_result);
                }
            }
            else if (message->type == MessageType::DISCONNECT)
            {
                std::cout << "received DISCONNECT req\n";
                m_isRunning = false;
                Message msg{message->id, MessageType::DISCONNECT};
                auto iter = m_connections.processActionUpdate(message);

                m_server->send(&msg);
            }
        }
    };

    void SilberService::stop()
    {
        m_isRunning = false;
    }
}