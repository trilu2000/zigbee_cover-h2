#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "esp_timer.h"


struct FrameView
{
    const uint8_t *data = nullptr;
    size_t length = 0;
    explicit operator bool() const { return data != nullptr && length > 0; }
};

constexpr uint8_t hi_byte(uint16_t value)     { return static_cast<uint8_t>(value >> 8); }
constexpr uint8_t lo_byte(uint16_t value)     { return static_cast<uint8_t>(value & 0xFF);	}

inline uint16_t be16(uint16_t value)   { return (value >> 8) | (value << 8); }
inline uint16_t be16(const uint8_t *p) { return (static_cast<uint16_t>(p[0]) << 8) | p[1]; }




inline const char *format_hex_pretty(const uint8_t *data, size_t len)
{
    static char buffer[512];

    char *p = buffer;
    size_t remaining = sizeof(buffer);

    for (size_t i = 0; i < len && remaining > 3; i++)
    {
        int written = snprintf(p, remaining, "%02X", data[i]);

        p += written;
        remaining -= written;

        if (i + 1 < len && remaining > 1)
        {
            *p++ = '.';
            *p = '\0';
            remaining--;
        }
    }

    // Länge anhängen
    if (remaining > 1)
    {
        snprintf(p, remaining, " (%u)", (unsigned)len);
    }
    return buffer;
}

inline const char *format_hex_pretty(const FrameView &frame)
{
    return format_hex_pretty(frame.data, frame.length);
}

template <size_t N>
inline const char *format_hex_pretty(const uint8_t (&data)[N])
{
    return format_hex_pretty(data, N);
}



template <auto Callback>
class TimedAction
{
public:
    void start_once(uint32_t delay_ms)
    {
        interval_us_ = static_cast<uint64_t>(delay_ms) * 1000ULL;
        last_ = esp_timer_get_time();
        periodic_ = false;
        active_ = true;
    }

    void start_periodic(uint32_t interval_ms)
    {
        interval_us_ = static_cast<uint64_t>(interval_ms) * 1000ULL;
        last_ = esp_timer_get_time();
        periodic_ = true;
        active_ = true;
    }

    void stop()
    {
        active_ = false;
    }

    template <typename Owner>
    void poll(uint64_t now, Owner *owner)
    {
        if (!active_)
            return;

        if (now - last_ < interval_us_)
            return;

        if (periodic_)
        {
            last_ = now;
        }
        else
        {
            active_ = false;
        }

        (owner->*Callback)();
    }

    bool active() const
    {
        return active_;
    }

private:
    uint64_t interval_us_ = 0;
    uint64_t last_ = 0;

    bool active_ = false;
    bool periodic_ = false;
};