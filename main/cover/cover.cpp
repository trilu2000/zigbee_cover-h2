#include "cover.h"

#include "esp_log.h"

#define TAG "COVER"


void Cover::init(const CoverConfig &config)
{
    config_ = config;

    ESP_LOGI(TAG, "Initializing cover");
 
    ESP_LOGI(TAG, "UART=%d RX=%d TX=%d RSTN=%d",
             config_.uart, config_.rx_pin, config_.tx_pin, config_.rstn_pin);

    // setup uart
    uart_config_t uart_config = {};
    uart_config.baud_rate = 115200;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity    = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;

    ESP_ERROR_CHECK( uart_param_config( config_.uart, &uart_config ) );
    ESP_ERROR_CHECK( uart_set_pin( config_.uart, config_.tx_pin, config_.rx_pin, UART_PIN_NO_CHANGE,  UART_PIN_NO_CHANGE ));
    ESP_ERROR_CHECK( uart_driver_install( config_.uart, 1024, 1024, 0, nullptr, 0 ) );

    // setup rstn
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << config_.rstn_pin);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;

    ESP_ERROR_CHECK( gpio_config( &io_conf ) );

    // initial check of rstn state
	rstn_initialize_state_ = gpio_get_level(config_.rstn_pin);
	last_rstn_state_ = rstn_initialize_state_;
	ESP_LOGE(TAG, "RSTN startup state: %s", last_rstn_state_ ? "HIGH" : "LOW");

	if (last_rstn_state_) 
	{
		// rstn is high after complete device startup - esp32 and rl78 reboot
		// after a three minutes delay rstn will toogle to low and high again
		// rstn is high after an esp32 reset
		// we send a hello_ind in any case and start the operation poll after some time
		start_uart_comms();
	}
}


void Cover::poll() 
{
    // check rstn_pin
    check_rstn_state();

    // read uart port
    uint8_t byte;
    while (uart_read_bytes(config_.uart, &byte, 1, 0) == 1)
    {
        rx_buffer_.push_back(byte);
    }

    // check if there was something received
    while (FrameView frame = extract_frame(rx_buffer_))
    {
        //dump_frame(frame, FrameDir::RX);
        process_frame(frame);

        rx_buffer_.erase(
            rx_buffer_.begin(),
            rx_buffer_.begin() + frame.length
        );
    }

    // some timed polls
    const uint64_t now = esp_timer_get_time();
    operation_poll_.poll(now, this);
    position_poll_.poll(now, this);
    position_timeout_.poll(now, this);

    //check_communication_timeout();
}

void Cover::set_operation(CoverCommand command)
{
    ESP_LOGD(TAG, "Set operation %s", cover_command_name(command));

	// set value (correct checksum not needed)
	FUN_MSG_COMMAND_OPERATION[26] = static_cast<uint8_t>(command);
	send_frame(FUN_MSG_COMMAND_OPERATION);
}

void Cover::set_position(uint8_t position)
{
    ESP_LOGD(TAG, "Set position %d", position);
	// set value (correct checksum not needed)
	FUN_MSG_COMMAND_POSITION[29] = position;
	send_frame(FUN_MSG_COMMAND_POSITION);
}

void Cover::send_frame(uint8_t *data, uint8_t len)
{
	//FrameView frame{data, len};
    ESP_LOGI(TAG, "tx %s", format_hex_pretty( data, len ));
    uart_write_bytes( config_.uart, data, len );    
}

void Cover::led_orange()
{
	send_frame(SYS_RSSI_GET_REQ);
}
void Cover::led_green()
{
    send_frame(DEV_MGNT_REGISTER_DEVICE_IND);
}
void Cover::led_red()
{
    send_frame(DEV_MGNT_DEREGISTER_DEVICE_CFM);
}



/* -----------------------------------------------------------------------------------------------------------
 * RX analysis and processing
 */

FrameView Cover::extract_frame(std::vector<uint8_t> &buf)
{
	// we need at least a complete header
	if (buf.size() < 10) return {};

    // search sync 
    size_t sync_pos = find_sync(buf.data(), buf.size());

	// no sync found, but we keep the last byte as it could be the half of a sync
	if (sync_pos == buf.size())
	{
		buf.erase(buf.begin(), buf.end() - 1);
		return {};
	}

	// delete bytes upfront sync
	if (sync_pos > 0)
		buf.erase(buf.begin(), buf.begin() + sync_pos);

    // check after potential erease if we have a complete header
	if (buf.size() < 10)
		return {};

	// read length from header (Big Endian)
	uint16_t length = be16(&buf[2]);

	// check if length make sense
	if (length < 6 || length > 512)
	{
		ESP_LOGW(TAG, "Invalid length: %u", length);
		// length is not valid, delete first byte and start a search for next sync bytes
		buf.erase(buf.begin());
		return {};
	}

	// define the frame_size
	size_t frame_size = length + 4;

	// check if we really have the complete amount of bytes in the buffer
	if (buf.size() < frame_size)
		return {};

	// frame found, return it
	return { .data = buf.data(), .length = frame_size, };
}


void Cover::dump_frame(const FrameView &frame, FrameDir direction)
{
	//return;
	if (!frame) return;

	CmndFrame cmnd(frame.data);

	ESP_LOGD(TAG, "%s", DDEL);
	ESP_LOGI(TAG, "%s: %s", direction == FrameDir::RX ? "RX" : "TX", format_hex_pretty(frame.data, frame.length));

	//cmnd.dump();
	cmnd.min_dump();

	IERange ies(cmnd.payload(), cmnd.payload_length());

	for (auto ie : ies)
	{
		ie.dump();

        if (ie.type() == IEType::IE_FUN)
		{
			FunIE fun(ie);
			//fun.dump();
			fun.min_dump();
		}
	}
}


void Cover::process_frame(const FrameView &frame)
{
	CmndFrame cmnd(frame.data);
	IERange ies(cmnd.payload(), cmnd.payload_length());
    bool handled = false;

    // there are strings without IE, they are checked here
    // To catch DECT Button Events for further Zigbee handling 
    if ( cmnd.service() == SrvType::SYSTEM && cmnd.message() == MsgType::SYS_RSSI_GET_REQ ) 
	{	
	    ESP_LOGD(TAG, "received SYS_RSSI_GET_REQ");
        on_cover_button(CoverButton::SHORT);
        handled = true;
	}
    else if ( cmnd.service() == SrvType::DEVICE_MANAGEMENT && cmnd.message() == MsgType::DEV_MGNT_REGISTER_DEVICE_REQ ) 
	{
	    ESP_LOGD(TAG, "received DEV_MGNT_REGISTER_DEVICE_REQ");
        on_cover_button(CoverButton::LONG);
        handled = true;
	}
    else if ( cmnd.service() == SrvType::DEVICE_MANAGEMENT && cmnd.message() == MsgType::DEV_MGNT_DEREGISTER_DEVICE_REQ ) 
	{	
	    ESP_LOGD(TAG, "received DEV_MGNT_DEREGISTER_DEVICE_REQ");
        on_cover_button(CoverButton::LONG);
        handled = true;
    }

    for (auto ie : ies)
	{
		// boot sequenz:
		// -> GENERAL_HELLO_IND
		// <- GENERAL_GET_VERSION_REQ - IE_U8: 0x01 (1) (Software)
		// -> GENERAL_GET_VERSION_RES - IE_VERSION: "37.16.08"
		// <- PARAM_GET_REQ - IE_PARAMETER: type=0x00, param=0x01, dataLen=0
		// -> PARAM_GET_RES - IE_PARAMETER: type=0x00, param=0x01, dataLen=5, data: 03.7C.40.29.DD (5)
		// <- PARAM_GET_REQ - IE_PARAMETER: type=0x00, param=0x07, dataLen=0
		// -> PARAM_GET_RES - IE_PARAMETER: type=0x00, param=0x07, dataLen=14, data: 0D.33.37.2E.31.36.2E.30.38.23.31.2E.32.52 (14)
		// init done, we can start the get_fun_operation timer
				
		// GENERAL_GET_VERSION_REQ
		if ( cmnd.service() == SrvType::GENERAL && cmnd.message() == MsgType::GENERAL_GET_VERSION_REQ ) 
		{	
	        ESP_LOGD(TAG, "received GENERAL_GET_VERSION_REQ");
			send_get_version_res();
            handled = true;
		}
				
		// Service ID: 0x0203 (PARAMETERS), Message ID: 0x01 (PARAM_GET_REQ)
		// IE Type: 0x0B (IE_PARAMETER), IE_PARAMETER: type=0x00, param=0x01, dataLen=0
		else if ( cmnd.service() == SrvType::PARAMETERS && cmnd.message() == MsgType::PARAM_GET_REQ) 
		{
			uint8_t e_Param = ie.value()[1];
			if (e_Param == 0x01) 
			{
    	        ESP_LOGD(TAG, "received PARAM_GET_REQ 0x01");
				send_param_get_res_01();
                handled = true;
            }
			else if (e_Param == 0x07) 
			{
    	        ESP_LOGD(TAG, "received PARAM_GET_REQ 0x07");
				send_param_get_res_07();
                handled = true;
            }
		}
			
		// FUN Handling
		else if ( cmnd.service() == SrvType::FUN && cmnd.message() == MsgType::FUN_SEND_REQ) 
		{
			// prüfen von ifaceId und member, ggf. setzen von cover
			FunIE fun(ie);

			if ( fun.interface_id() == 0x0201 && fun.interface_member() == 0x01 && fun.payload_length() == 2 )
			{
				// SEND_FUN_GET_0201_01_Position, WAIT_FUN_0201_01_Position,
				process_position(fun.payload()[1]);
                handled = true;
			}
			else if ( fun.interface_id() == 0x0204 && fun.interface_member() == 0x01 && fun.payload_length() == 2 )
			{
				// SEND_FUN_GET_0204_01_Operation, WAIT_FUN_0204_01_Operation,
				process_operation(fun.payload()[1]);
                handled = true;
				// monitor the poll and set NORMAL state, check timeout in loop
//				last_rl78_response_ = esphome::millis();
//				set_diag_state(DiagState::NORMAL);
			}
        }
	}

    if (!handled)
    {
        dump_frame(frame, FrameDir::RX);
    }

}

size_t Cover::find_sync(const uint8_t *buf, size_t len)
{
	for (size_t i = 0; i + 1 < len; i++)
	{
		if (buf[i] == 0xDA && buf[i + 1] == 0xDA) return i;
	}
	return len;
}





/* -----------------------------------------------------------------------------------------------------------
 * TX messages
 */

void Cover::send_hello_ind()
{
	ESP_LOGD(TAG, "send GENERAL_HELLO_IND");
	send_frame(GENERAL_HELLO_IND);		
}

void Cover::send_get_version_res()
{
	ESP_LOGD(TAG, "send GENERAL_GET_VERSION_RES");
	send_frame(GENERAL_GET_VERSION_RES);		
}

void Cover::send_param_get_res_01()
{
	ESP_LOGD(TAG, "send PARAM_GET_RES_01");
	send_frame(PARAM_GET_RES_01);		
}

void Cover::send_param_get_res_07()
{
	ESP_LOGD(TAG, "send PARAM_GET_RES_07");
	send_frame(PARAM_GET_RES_07);		
}

void Cover::send_fun_get_position()
{
	send_frame(FUN_MSG_ATTRIBUTE_GET_POSTION);		
}

void Cover::send_fun_get_operation()
{
	send_frame(FUN_MSG_ATTRIBUTE_GET_OPERATION);		
}

template<size_t N>
void Cover::send_frame(const uint8_t (&data)[N])
{
	FrameView frame{data, N};

    uart_write_bytes( config_.uart, (const char *)frame.data, frame.length );
//	ESP_LOGD(TAG, "TX: %s", format_hex_pretty(frame.data, frame.length));
//	dump_frame(frame, FrameDir::TX);
}


/* -----------------------------------------------------------------------------------------------------------
 * rollotron functions
 */
void Cover::process_operation(uint8_t operation)
{
	// check if something has changed, otherwise return
	if (operation == operation_) return;
	operation_ = operation;

    CoverEvent event{};
    event.type = CoverEventType::OPERATION;
	
    switch (operation)
	{
		case 0x01:  // closing
            event.operation = CoverOperation::CLOSING;
            set_fun_position_poll();
			break;
		case 0x03:  // opening
            event.operation = CoverOperation::OPENING;
            set_fun_position_poll();
			break;
		default:   // everything else should be idle
            event.operation = CoverOperation::IDLE;
			stop_fun_position_poll();
			// as we do not know if we got the latest position, ask once more
			ESP_LOGD(TAG, "send get position");
            send_fun_get_position();
			break;
	}

	//ESP_LOGD(TAG, "Operation: %u", operation);
    on_cover_event(event);
}


void Cover::process_position(uint8_t position)
{
	// check if something has changed, otherwise return
    if (position == position_) return;
    position_ = position;

    CoverEvent event{};
    event.type = CoverEventType::POSITION;
    // 255: cover closed, 0: cover open
    event.position = position;

	//ESP_LOGD(TAG, "Position: %u", position);
    on_cover_event(event);
}


void Cover::check_rstn_state()
{
	bool state = gpio_get_level(config_.rstn_pin);
	if (state == last_rstn_state_) return;
	last_rstn_state_ = state;

	ESP_LOGD(TAG, "RSTN pin raised %s", state ? "HIGH" : "LOW");

	if (state) 
	{
		start_uart_comms();
	}
	else
	{
		stop_fun_operation_poll();
		stop_fun_position_poll();
	}
}


void Cover::start_uart_comms() 
{
	ESP_LOGD(TAG, "start uart communication");
    // we send an hello_ind and start the operation_poll with a small delay
	send_hello_ind();

    position_timeout_.start_once(500);
  	set_fun_operation_poll();
}


void Cover::set_fun_operation_poll()
{
	ESP_LOGD(TAG, "start operation poll");
    operation_poll_.start_periodic(1000);
}

void Cover::stop_fun_operation_poll()
{
	ESP_LOGD(TAG, "stop operation poll");
    operation_poll_.stop();
}

void Cover::set_fun_position_poll()
{
	ESP_LOGD(TAG, "start position poll");
    position_poll_.start_periodic(2000);
}

void Cover::stop_fun_position_poll()
{
	ESP_LOGD(TAG, "stop postion poll");
    position_poll_.stop();
}






/* -----------------------------------------------------------------------------------------------------------
 * CMND FRAME
 */

Cover::CmndFrame::CmndFrame(const uint8_t *data) : hdr_(reinterpret_cast<const CmndHeader *>(data))
{}

uint16_t Cover::CmndFrame::length() const
{
    return be16(hdr_->length);
}

uint8_t Cover::CmndFrame::cookie() const
{
    return hdr_->cookie;
}

uint8_t Cover::CmndFrame::unit() const
{
    return hdr_->unit;
}

uint16_t Cover::CmndFrame::service() const
{
    return be16(hdr_->service);
}

uint8_t Cover::CmndFrame::message() const
{
    return hdr_->message;
}

uint8_t Cover::CmndFrame::checksum() const
{
    return hdr_->checksum;
}

const uint8_t *Cover::CmndFrame::payload() const
{
    return reinterpret_cast<const uint8_t *>(hdr_) + sizeof(CmndHeader);
}

size_t Cover::CmndFrame::payload_length() const
{
    return length() - 6;
}

void Cover::CmndFrame::dump() const
{
    ESP_LOGV(TAG,"%s",SDEL);

    ESP_LOGV(TAG,"Service : 0x%04X (%s)",
        service(), get_service_name(service()));

    ESP_LOGV(TAG,"Message : 0x%02X (%s)",
        message(), get_msg_name(service(),message()));

    ESP_LOGV(TAG,"Payload : %s",
        format_hex_pretty(payload(),payload_length()));
}

void Cover::CmndFrame::min_dump() const
{
    ESP_LOGV(TAG, "Service=0x%04X Message=0x%02X",
        service(), message());
}



/* -----------------------------------------------------------------------------------------------------------
 * IE
 */

Cover::IE::IE(const uint8_t *data) : hdr_(reinterpret_cast<const IeHeader *>(data))
{}

uint8_t Cover::IE::type() const
{
    return hdr_->type;
}

uint16_t Cover::IE::length() const
{
    return be16(hdr_->length);
}

const uint8_t *Cover::IE::value() const
{
    return reinterpret_cast<const uint8_t *>(hdr_)+sizeof(IeHeader );
}

size_t Cover::IE::total_size() const
{
    return sizeof(IeHeader )+length();
}

void Cover::IE::dump() const
{
    ESP_LOGV(TAG, "IE 0x%02X (%s) len=%u",
        type(), get_ie_name(type()), length());
}



/* -----------------------------------------------------------------------------------------------------------
 * IE RANGE Class
 */

Cover::IERange::IERange(const uint8_t *data,size_t length) : data_(data), length_(length)
{}

Cover::IERange::Iterator::Iterator(const uint8_t *ptr) : ptr_(ptr)
{}

Cover::IE Cover::IERange::Iterator::operator*() const
{
    return IE(ptr_);
}

bool Cover::IERange::Iterator::operator!=(const Iterator &other) const
{
    return ptr_ != other.ptr_;
}

Cover::IERange::Iterator &Cover::IERange::Iterator::operator++()
{
    ptr_ += IE(ptr_).total_size();
    return *this;
}

Cover::IERange::Iterator Cover::IERange::begin() const
{
    return Iterator(data_);
}

Cover::IERange::Iterator Cover::IERange::end() const
{
    return Iterator(data_+length_);
}




/* -----------------------------------------------------------------------------------------------------------
 * FUN IE Class
 */

Cover::FunIE::FunIE(const IE &ie)
{
    hdr_ = reinterpret_cast<const FunHeader *>(ie.value());
    length_ = ie.length();
}

uint8_t Cover::FunIE::extended_mode() const
{
    return hdr_->extended_mode;
}

uint16_t Cover::FunIE::src_device() const
{
    return be16(hdr_->src_device);
}

uint8_t Cover::FunIE::src_unit() const
{
    return hdr_->src_unit;
}

uint8_t Cover::FunIE::address_type() const
{
    return hdr_->address_type;
}

uint16_t Cover::FunIE::dst_device() const
{
    return be16(hdr_->dst_device);
}

uint8_t Cover::FunIE::dst_unit() const
{
    return hdr_->dst_unit;
}

uint8_t Cover::FunIE::app_reference() const
{
    return hdr_->app_reference;
}

uint8_t Cover::FunIE::message_type() const
{
    return hdr_->message_type;
}

uint8_t Cover::FunIE::interface_type() const
{
    return hdr_->interface_type;
}

uint16_t Cover::FunIE::interface_id() const
{
    return be16(hdr_->interface_id);
}

uint8_t Cover::FunIE::interface_member() const
{
    return hdr_->interface_member;
}

uint16_t Cover::FunIE::data_length() const
{
    return be16(hdr_->data_length);
}

size_t Cover::FunIE::payload_length() const
{
    return length_ - sizeof(FunHeader);
}

const uint8_t *Cover::FunIE::payload() const
{
    return reinterpret_cast<const uint8_t *>(hdr_) + sizeof(FunHeader);
}

void Cover::FunIE::dump() const
{
    ESP_LOGV(TAG, "%s", SDEL);

    ESP_LOGV(TAG, "Ext_Mode=%u SRC=0x%04X:%u -> DST=0x%04X:%u Addr_Type=%u App_Ref=%u",
        extended_mode(), src_device(), src_unit(), dst_device(), dst_unit(), address_type(), app_reference() );

    ESP_LOGV(TAG, "MSG_Type=0x%02X(%s) Iface_Type=%u Iface_ID=0x%04X Iface_Member=0x%02X Data_Len=%u",
        message_type(), get_fun_msg_type_name(message_type()), interface_type(), interface_id(), interface_member(), data_length() );

    if (data_length() > 0)
    {
        ESP_LOGV(TAG, "Raw IE : %s", format_hex_pretty(payload(), payload_length()) );
    }
}

void Cover::FunIE::min_dump() const
{
    ESP_LOGV(TAG, "MSG_Type: 0x%02X(%s), Iface: 0x%04X/0x%02X",
        message_type(), get_fun_msg_type_name(message_type()), interface_id(), interface_member() );

    if (data_length() > 0)
    {
        ESP_LOGV(TAG, "Raw IE : %s", format_hex_pretty(payload(), payload_length()) );
    }
}


extern "C" __attribute__((weak))
void on_cover_event(const CoverEvent &event)
{
    ESP_LOGI(TAG, "on_cover_event");
}

extern "C" __attribute__((weak))
void on_cover_button(const CoverButton button)
{
    ESP_LOGI(TAG, "on_cover_button");
}