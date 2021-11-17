#ifndef call_h__
#define call_h__
#include "call/call.h"
#include "audioloop.h"
#include "test/test_video_capturer.h"

class webrtc::AudioDeviceModule;
class webrtc::RtcEventLog;
class webrtc::Call;
class calltest
{
public:
	calltest();
	~calltest();

	void CreateCall();
	void AudioDeviceModuleSetup();
	void StartCapture();
private:
	rtc::scoped_refptr<webrtc::AudioDeviceModule> m_adv = nullptr;
	webrtc::RtcEventLog* m_event_log = nullptr;
	std::unique_ptr<webrtc::Call> m_call_ptr = nullptr;
	std::unique_ptr<AudioLoopTransport> m_audio_loop_ptr = nullptr;
	std::unique_ptr<webrtc::test::TestVideoCapturer> m_video_capture_ptr = nullptr;


};
#endif // call_h__
