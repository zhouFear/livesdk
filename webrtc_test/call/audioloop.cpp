#include "audioloop.h"
// #include ""

MediaLoopTransport::MediaLoopTransport(std::shared_ptr<webrtc::Call> call_ptr, webrtc::MediaType type): m_call_ptr(call_ptr), m_type(type)
{

}

bool MediaLoopTransport::SendRtp(const uint8_t* packet, size_t length, const webrtc::PacketOptions& options)
{
	int64_t send_time = webrtc::Clock::GetRealTimeClock()->TimeInMicroseconds();
	rtc::SentPacket sent_packet(options.packet_id, send_time);
	m_call_ptr->OnSentPacket(sent_packet);
	m_call_ptr->Receiver()->DeliverPacket(m_type, rtc::CopyOnWriteBuffer(packet, length), -1);
	return true;
}

bool MediaLoopTransport::SendRtcp(const uint8_t* packet, size_t length)
{
	int64_t send_time = webrtc::Clock::GetRealTimeClock()->TimeInMicroseconds();
	m_call_ptr->Receiver()->DeliverPacket(m_type, rtc::CopyOnWriteBuffer(packet, length), -1);
	return true;
}

