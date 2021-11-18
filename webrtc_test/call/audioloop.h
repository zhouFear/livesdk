#ifndef audioloop_h__
#define audioloop_h__
#include "call/call.h"
#include "api/call/transport.h"

class MediaLoopTransport: public webrtc::Transport
{
public:
	MediaLoopTransport(std::shared_ptr<webrtc::Call> call_ptr, webrtc::MediaType type);
	bool SendRtp(const uint8_t* packet, size_t length, const webrtc::PacketOptions& options) override;
	bool SendRtcp(const uint8_t* packet, size_t length) override;
protected:
private:
	std::shared_ptr<webrtc::Call> m_call_ptr;
	webrtc::MediaType m_type;
};


#endif // audioloop_h__
