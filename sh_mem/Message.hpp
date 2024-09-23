#pragma once

#include <stddef.h>
#include <vector>

enum MessageType : size_t
{
    HANDSHAKE = 0,
    HANDSHAKE_OK,
    HANDSHAKE_FAIL,
    SET_CONFIG,
    SET_CONFIG_OK,
    SET_CONFIG_FAIL,
    COMPARE_REQUEST,
    COMPARE_RESULT,
    COMPARE_FAIL,
    DISCONNECT,
    DISCONNECT_OK,
    DISCONNECT_FAIL,
    UNEXPECTED_REQUEST // default value, user must specify type of message
};

struct Configuration
{
    size_t AffinityThreshold;
    size_t MinPixelsForObject;
    uint8_t PixelStep;
    double CalculationTimeLimit;
    size_t IdleTimeout;
    double ThreadsMultiplier;
};

struct Message
{
    Message(const size_t aId = 0, const MessageType aType = MessageType::UNEXPECTED_REQUEST)
        : id(aId),
          type(aType),
          size(sizeof(Message)) {}
    size_t id;
    MessageType type;
    size_t size;
};

struct MessageSetConfig : public Message
{
    MessageSetConfig() : Message()
    {
        size = sizeof(MessageSetConfig);
    }
    Configuration configuration;
};

struct MessageCompareRequest : public Message
{
    MessageCompareRequest() : Message()
    {
        size = sizeof(MessageCompareRequest);
    }
    char base[200];
    char to_compare[200];
};

struct Rect
{
    size_t l, r, t, b;
};

struct MessageCompareResult : public Message
{
    MessageCompareResult() : Message()
    {
        size = sizeof(MessageCompareResult);
    }
    Rect payload[100];
    size_t payload_bytes;
};