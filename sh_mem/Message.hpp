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
    UNEXPECTED_REQUEST
};

struct Message
{
    size_t id;
    MessageType type;
};

struct AmConfiguration
{
    size_t AffinityThreshold;
    size_t MinPixelsForObject;
    uint8_t PixelStep;
    double CalculationTimeLimit;
    size_t IdleTimeout;
    double ThreadsMultiplier;
};

struct MessageSetConfig : public Message
{
    AmConfiguration configuration;
};

struct MessageCompareRequest : public Message
{
    char base[200];
    char to_compare[200];
};

struct Rect
{
    size_t l, r, t, b;
};

struct MessageCompareResult : public Message
{
    Rect payload[100];
    size_t payload_bytes;
};