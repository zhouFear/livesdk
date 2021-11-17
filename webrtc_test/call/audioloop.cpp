#include "audioloop.h"
// #include ""

AudioLoopTransport::AudioLoopTransport(std::shared_ptr<webrtc::Call> call_ptr): m_call_ptr(call_ptr)
{

}

bool AudioLoopTransport::SendRtp(const uint8_t* packet, size_t length, const webrtc::PacketOptions& options)
{
	int64_t send_time = webrtc::Clock::GetRealTimeClock()->TimeInMicroseconds();
	rtc::SentPacket sent_packet(options.packet_id, send_time);
	m_call_ptr->OnSentPacket(sent_packet);
	m_call_ptr->Receiver()->DeliverPacket(webrtc::MediaType::AUDIO, rtc::CopyOnWriteBuffer(packet, length), -1);
	return true;
}

bool AudioLoopTransport::SendRtcp(const uint8_t* packet, size_t length)
{
	int64_t send_time = webrtc::Clock::GetRealTimeClock()->TimeInMicroseconds();
	m_call_ptr->Receiver()->DeliverPacket(webrtc::MediaType::AUDIO, rtc::CopyOnWriteBuffer(packet, length), -1);
	return true;
}

