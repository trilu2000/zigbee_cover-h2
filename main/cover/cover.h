#pragma once

#include <stdint.h>
#include <stddef.h>
#include <vector>

#include "esp_log.h"
#include "esp_timer.h"

#include "driver/gpio.h"
#include "driver/uart.h"

#include "types.h"
#include "../helpers.h"


struct CoverConfig
{
    gpio_num_t rx_pin;
    gpio_num_t tx_pin;
    gpio_num_t rstn_pin;
    uart_port_t uart;
};




class Cover
{
public:

    void init(const CoverConfig &config);
    void poll();

    void set_operation(CoverCommand command);
    void set_position(uint8_t position);

    void send_frame(uint8_t *data, uint8_t len);

    void led_orange();
    void led_green();
    void led_red();

private:
    // variables declaration
    std::vector<uint8_t> rx_buffer_;
    CoverConfig config_;

    uint8_t position_ = 255;
    uint8_t operation_ = 255;

	bool rstn_initialize_state_{false};
	bool last_rstn_state_{false};


    // rx processing
    FrameView extract_frame(std::vector<uint8_t> &buf);
    void dump_frame(const FrameView &frame, FrameDir direction);
    void process_frame(const FrameView &frame);
    size_t find_sync(const uint8_t *buf, size_t len);


    // tx functions
    void send_hello_ind();
    void send_get_version_res();
    void send_param_get_res_01();
    void send_param_get_res_07();
    void send_fun_get_position();
    void send_fun_get_operation();
    template<size_t N>
    void send_frame(const uint8_t (&data)[N]);


    // rollotron functions
    void process_operation(uint8_t operation);
    void process_position(uint8_t position);

    void check_rstn_state();
    void start_uart_comms() ;

    void set_fun_operation_poll();
    void stop_fun_operation_poll();
    void set_fun_position_poll();
    void stop_fun_position_poll();


    // poll timer decarations
    TimedAction<&Cover::send_fun_get_operation> operation_poll_;
    TimedAction<&Cover::send_fun_get_position> position_poll_;
    TimedAction<&Cover::send_hello_ind> hello_timeout_;
    TimedAction<&Cover::send_fun_get_position> position_timeout_;


    // supporting classes
    class CmndFrame
    {
    public:

        explicit CmndFrame(const uint8_t *data);

        uint16_t length() const;
        uint8_t cookie() const;
        uint8_t unit() const;
        uint16_t service() const;
        uint8_t message() const;
        uint8_t checksum() const;

        const uint8_t *payload() const;
        size_t payload_length() const;

        void dump() const;
        void min_dump() const;

    private:

        #pragma pack(push,1)
        struct CmndHeader
        {
            uint8_t sync1;
            uint8_t sync2;
            uint16_t length;
            uint8_t cookie;
            uint8_t unit;
            uint16_t service;
            uint8_t message;
            uint8_t checksum;
        };
        #pragma pack(pop)

        const CmndHeader *hdr_;
    };


    class IE
    {
    public:

        explicit IE(const uint8_t *data);

        uint8_t type() const;
        uint16_t length() const;

        const uint8_t *value() const;
        size_t total_size() const;

        void dump() const;

    private:

        #pragma pack(push,1)
        struct IeHeader
        {
            uint8_t type;
            uint16_t length;
        };
        #pragma pack(pop)

        const IeHeader *hdr_;
    };


    class IERange
    {
    public:

        IERange(const uint8_t *data, size_t length);

        class Iterator
        {
        public:

            Iterator(const uint8_t *ptr);
            IE operator*() const;
            bool operator!=(const Iterator &other) const;
            Iterator& operator++();

        private:

            const uint8_t *ptr_;
        };

        Iterator begin() const;
        Iterator end() const;

    private:

        const uint8_t *data_;
        size_t length_;
    };


    class FunIE
    {
    public:

        explicit FunIE(const IE &ie);
        uint8_t extended_mode() const;

        uint16_t src_device() const;
        uint8_t src_unit() const;

        uint8_t address_type() const;

        uint16_t dst_device() const;
        uint8_t dst_unit() const;

        uint8_t app_reference() const;
        uint8_t message_type() const;

        uint8_t interface_type() const;
        uint16_t interface_id() const;
        uint8_t interface_member() const;

        uint16_t data_length() const;

        size_t payload_length() const;
        const uint8_t *payload() const;

        void dump() const;
        void min_dump() const;

    private:

        #pragma pack(push,1)
        struct FunHeader
        {
            uint8_t extended_mode;
            uint16_t src_device;
            uint8_t src_unit;
            uint8_t address_type;
            uint16_t dst_device;
            uint8_t dst_unit;
            uint8_t app_reference;
            uint8_t message_type;
            uint8_t interface_type;
            uint16_t interface_id;
            uint8_t interface_member;
            uint16_t data_length;
        };
        #pragma pack(pop)

        const FunHeader *hdr_;
        uint16_t length_;
    };

};

enum class CoverEventType
{
    OPERATION,
    POSITION
};

enum class CoverButton
{
    SHORT,
    LONG
};

enum class CoverOperation
{
    IDLE,
    CLOSING,
    OPENING
};

struct CoverEvent
{
    CoverEventType type;

    union
    {
        CoverOperation operation;
        uint8_t position;
    };
};

extern "C" void on_cover_event(const CoverEvent &event);
extern "C" void on_cover_button(const CoverButton button);



/*class CMNDBuilder
{
public:
	CMNDBuilder(uint16_t service, uint8_t message, uint8_t unit = 0, uint8_t cookie = 0)
	{
		frame_.push_back(0xDA);
		frame_.push_back(0xDA);
		
		// length placeholder
		frame_.push_back(0);
		frame_.push_back(0);

        frame_.push_back(0);       // cookie
		frame_.push_back(unit);    // unit

        frame_.push_back(service >> 8);
		frame_.push_back(service & 0xff);

		frame_.push_back(message);

		frame_.push_back(0);       // checksum placeholder
	}

	void add_ie(IEBuilder &ie)
	{
		auto data = ie.build();
		frame_.insert(frame_.end(), data.begin(), data.end());
	}

	void add_u8_ie(uint8_t type, uint8_t value)
	{
		IEBuilder ie(type);
		ie.add_u8(value);
		add_ie(ie);
	}

	void add_bytes(const uint8_t *data, size_t len)
	{
		frame_.insert(frame_.end(), data, data + len);
	}

	void add_u8(uint8_t value)
	{
		frame_.push_back(value);
	}

	void add_u16(uint16_t value)
	{
		frame_.push_back(value >> 8);
		frame_.push_back(value & 0xFF);
	}

	void add_u32(uint32_t value)
	{
		frame_.push_back((value >> 24) & 0xFF);
		frame_.push_back((value >> 16) & 0xFF);
		frame_.push_back((value >> 8) & 0xFF);
		frame_.push_back(value & 0xFF);
	}

	std::vector<uint8_t> build()
	{
		uint16_t len = frame_.size() - 4;
		frame_[2] = len >> 8;
		frame_[3] = len & 0xff;
		update_checksum();
			
		return frame_;
	}

private:
	std::vector<uint8_t> frame_;

    void update_checksum()
	{
		uint8_t checksum = 0;
		// Summe ab LEN-Feld bis vor Checksum
		for (size_t i = 2; i < frame_.size(); i++)
		{
			if (i != 9) checksum += frame_[i];
		}
		frame_[9] = checksum;
	}
};*/