#pragma once

#include <stdint.h> 
#include <unistd.h>
#include <stddef.h>
#include <string.h>

//#include "esp_rom_sys.h"
#include "esp_timer.h"
//#include "esp_vfs_dev.h"


struct Commands
{
    const char* command;
    uint8_t parameters;
    void (*callback)();
};

extern const Commands cmdTab[];

class InputParser
{
public:

    InputParser(uint8_t buffer_size) : buffer_size_(buffer_size), commands_(nullptr)  {}


    void init(const Commands *commands)
    {
        if (initialized_) return;

        // set table and initialized
        commands_ = commands;
        initialized_ = true;

        // create poll task
//        xTaskCreate(
//            task_entry,      // Task-Funktion
//            "cli_task",      // Name
//            4096,            // Stack-Größe
//            this,            // Parameter (Zeiger auf Klasse)
//            5,               // Priorität
//            nullptr          // Task Handle
//            );
    }


    void poll()
    {
        // read interface
        int c;
        while ((c = getchar()) != EOF)
        {
            input((char)c);
        }

        // echo input on console
        if (flush_pending_)
        {
            uint64_t now = esp_timer_get_time();
            if (now - last_input_time_ > 50000) // 50ms
            {
                fsync(2);
                flush_pending_ = false;
            }
        }
    }

    // read numbers
    template<typename T>
    InputParser& operator>>(T &value)
    {
        value = read_number<T>();
        return *this;
    }

    // read Hex-Array
    // parser >> data >> len;
    // rx DA.DA.00.0A.00.00.00.01
    template<size_t N>
    InputParser& operator>>(uint8_t (&data)[N])
    {
        last_hex_length_ = read_hex(data, N);
        return *this;
    }

    // number of Hex-Bytes
    // parser >> data >> len;
    InputParser& operator>>(uint8_t &value)
    {
        value = static_cast<uint8_t>(last_hex_length_);
        return *this;
    }


private:
    uint8_t buffer_size_;
    const Commands *commands_;
    char buffer_[200];
    uint8_t length_ = 0;
    uint8_t parameter_pos_ = 1;

    uint64_t last_input_time_ = 0;
    bool flush_pending_ = false;
    bool initialized_ = false;

    
    size_t last_hex_length_ = 0;

    // -- in case we want to create an own task --
    /*static void task_entry(void *arg)
    {
        InputParser *parser = static_cast<InputParser *>(arg);
        parser->task();

        vTaskDelete(nullptr);
    }

    void task()
    {
        while (true)
        {
            int c;
            while ((c = getchar()) != EOF)
            {
                input((char)c);
            }

            if (flush_pending_)
            {
                uint64_t now = esp_timer_get_time();

                if (now - last_input_time_ > 50000) // 50ms
                {
                    fsync(2);
                    flush_pending_ = false;
                }
            }
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }*/
    // ------------------------------------------------------

    void input(char c)
    {
        last_input_time_ = esp_timer_get_time();
        flush_pending_ = true;

        if (c == '\r' || c == '\n')
        {
            const char newline[] = "\r\n";
            write(2, newline, sizeof(newline)-1);

            execute();
            return;
        }

        if (c == 0x08)       // Backspace
        {
            if (length_) length_--;

            const char bs[] = "\b \b";
            write(2, bs, sizeof(bs)-1);

            return;
        }

        if (length_ < buffer_size_ - 1)
        {
            buffer_[length_++] = c;
            write(2, &c, 1);
        }
    }
    

    void execute()
    {
        if (length_ == 0 || commands_ == nullptr)
            return;

        const Commands *entry = commands_;

        while (entry->command != nullptr)
        {
            const uint8_t command_length = strlen(entry->command);

            // compare Command
            if (length_ >= command_length && strncmp(buffer_, entry->command, command_length) == 0)
            {
                // after the Command must come either an end or a whitespace
                if (length_ > command_length &&
                    buffer_[command_length] != ' ' &&
                    buffer_[command_length] != '\t')
                {
                    entry++;
                    continue;
                }

                parameter_pos_ = command_length;

                entry->callback();
                break;
            }

            entry++;
        }

        length_ = 0;
    }


    template<typename T>
    T read_number()
    {
        T value = 0;

        // Leerzeichen überspringen
        while (parameter_pos_ < length_)
        {
            char c = buffer_[parameter_pos_];

            if (c != ' ' && c != '\t') break;
            parameter_pos_++;
        }


        // Zahl lesen
        while (parameter_pos_ < length_)
        {
            char c = buffer_[parameter_pos_];

            if (c < '0' || c > '9') break;
            value = value * 10 + (c - '0');
            parameter_pos_++;
        }

        return value;
    }


    static int hex_value(char c)
    {
        if (c >= '0' && c <= '9')
            return c - '0';

        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;

        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;

        return -1;
    }


    size_t read_hex(uint8_t *data, size_t max_size)
    {
        size_t count = 0;

        // Leerzeichen vor dem Hex-String überspringen
        while (parameter_pos_ < length_)
        {
            char c = buffer_[parameter_pos_];

            if (c != ' ' && c != '\t')
                break;

            parameter_pos_++;
        }

        while (parameter_pos_ < length_)
        {
            // Platz im Zielarray prüfen
            if (count >= max_size)
                break;

            // Zwei Hex-Zeichen müssen vorhanden sein
            if (parameter_pos_ + 1 >= length_)
                break;

            int hi = hex_value(buffer_[parameter_pos_]);
            int lo = hex_value(buffer_[parameter_pos_ + 1]);

            if (hi < 0 || lo < 0)
                break;

            data[count++] =
                static_cast<uint8_t>((hi << 4) | lo);

            parameter_pos_ += 2;

            // Ende erreicht
            if (parameter_pos_ >= length_)
                break;

            // Nach jedem Byte muss ein Punkt folgen
            if (buffer_[parameter_pos_] != '.')
                break;

            parameter_pos_++;
        }

        return count;
    }
};


inline InputParser& get_input_parser()
{
    static InputParser parser(200);
    return parser;
}

inline InputParser &parser = get_input_parser();