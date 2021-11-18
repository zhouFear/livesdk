#ifndef call_h__
#define call_h__
#include "call/call.h"
#include "audioloop.h"
#include "test/test_video_capturer.h"
#include "test/video_renderer.h"
#include "api/video_codecs/video_encoder_config.h"
#include "api/test/video/function_video_decoder_factory.h"
#include "api/test/video/function_video_encoder_factory.h"

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
	void CreateAudioSendStream();
	void CreateAudioReciveStream();
	void CreateVideoSendStream();
	void CreateVideoReciveStream();

	void CreateVideoSendConfig(webrtc::VideoSendStream::Config* video_config,
		size_t num_video_streams,
		size_t num_used_ssrcs,
		webrtc::Transport* send_transport);
protected:
	void RegisterRtpExtension(const webrtc::RtpExtension& extension);
private:
	void _AddRtpExtensionByUri(const std::string& uri,
		std::vector<webrtc::RtpExtension>* extensions) const;
	absl::optional<webrtc::RtpExtension> _GetRtpExtensionByUri(
		const std::string& uri) const;
private:
	rtc::scoped_refptr<webrtc::AudioDeviceModule> m_adv_ptr = nullptr;
	webrtc::RtcEventLog* m_event_log_ptr = nullptr;
	std::shared_ptr<webrtc::Call> m_call_ptr = nullptr;
	std::unique_ptr<webrtc::test::TestVideoCapturer> m_video_capture_ptr = nullptr;

	std::unique_ptr<webrtc::test::VideoRenderer> m_local_video_render_ptr;
	std::unique_ptr<webrtc::test::VideoRenderer> m_remote_video_render_ptr;
	
	std::unique_ptr<MediaLoopTransport> m_audioloop_transport_ptr = nullptr;
	std::unique_ptr<MediaLoopTransport> m_videoloop_transport_ptr = nullptr;

	std::unique_ptr<webrtc::AudioSendStream> m_audiosendstream_ptr = nullptr;
	std::unique_ptr<webrtc::AudioReceiveStream> m_audiorecivestream_ptr = nullptr;
	std::unique_ptr<webrtc::VideoSendStream> m_videosendstream_ptr = nullptr;
	std::unique_ptr<webrtc::VideoReceiveStream> m_videorecivestream_ptr = nullptr;
	bool m_bSend_side_bwe = false;

	std::vector<webrtc::VideoEncoderConfig> m_video_encoder_configs;
	std::vector<webrtc::RtpExtension> m_rtp_extensions;
	webrtc::test::FunctionVideoEncoderFactory m_fake_encoder_factory;
	std::unique_ptr<webrtc::VideoBitrateAllocatorFactory> m_bitrate_allocator_factory;
	int m_fake_encoder_max_bitrate = -1;
	webrtc::Clock* const m_clock;
	
};
#endif // call_h__
