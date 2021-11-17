#ifndef call_h__
#define call_h__
#include "call/call.h"
#include "audioloop.h"

class webrtc::AudioDeviceModule;
class webrtc::RtcEventLog;
class webrtc::Call;
class calltest
{
public:
	calltest();
	~calltest();

	void CreateCall();
	void AudioDeviceModule_Setup();
	void StartCapture();
private:
	rtc::scoped_refptr<webrtc::AudioDeviceModule> m_adv;
	webrtc::RtcEventLog* m_event_log;
	std::shared_ptr<webrtc::Call> m_call_ptr;
	std::shared_ptr<AudioLoopTransport> m_audioLoop_ptr;
};
#endif // call_h__
