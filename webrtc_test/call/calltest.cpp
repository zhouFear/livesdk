#include "calltest.h"
#include "call/call.h"
#include "modules/audio_mixer/audio_mixer_impl.h"
#include "test/vcm_capturer.h"
#include "modules/audio_device/audio_device_impl.h"

namespace {
	const int CALL_MIN_BPS = 60 * 1000;
	const int CALL_START_BPS = 300 * 1000;
	const int CALL_MAX_BPS = 800 * 1000;

	const int USED_AUDIO_DEVICE_ID = 0;
	const int USED_VIDEO_DEVICE_ID = 0;
	
	const int USED_VIDEO_WIDTH = 640;
	const int USED_VIDEO_HEIGHT = 480;
	const int USED_VIDEO_FPS = 30;
	const int USED_MAX_VIDEO_QP = 48;

}

calltest::calltest() {
	
}

calltest::~calltest() {

}


void calltest::CreateCall()
{
	webrtc::AudioState::Config audioStateConfig;
	audioStateConfig.audio_device_module = m_adv;
	audioStateConfig.audio_mixer = webrtc::AudioMixerImpl::Create();
	audioStateConfig.audio_processing = webrtc::AudioProcessingBuilder().Create();

	webrtc::BitrateConstraints call_bitrate_config;
	call_bitrate_config.min_bitrate_bps = CALL_MIN_BPS;
	call_bitrate_config.start_bitrate_bps = CALL_START_BPS;
	call_bitrate_config.max_bitrate_bps = CALL_MAX_BPS;

	m_event_log = new webrtc::RtcEventLogNull();
	webrtc::CallConfig callConfig(m_event_log);
	callConfig.audio_state = webrtc::AudioState::Create(audioStateConfig);
	callConfig.bitrate_config = call_bitrate_config;
	m_adv->RegisterAudioCallback(callConfig.audio_state->audio_transport());

	m_call_ptr.reset(webrtc::Call::Create(callConfig));
	m_call_ptr->SignalChannelNetworkState(webrtc::MediaType::AUDIO, webrtc::kNetworkUp);
	m_call_ptr->SignalChannelNetworkState(webrtc::MediaType::VIDEO, webrtc::kNetworkUp);
}

void calltest::AudioDeviceModuleSetup()
{
	bool stereo_playout = false;
	bool stereo_record = false;

	if (!m_adv)
		m_adv = webrtc::AudioDeviceModule::Create(webrtc::AudioDeviceModule::kPlatformDefaultAudio, nullptr);
	m_adv->Init();
	m_adv->SetPlayoutDevice(USED_AUDIO_DEVICE_ID);
	m_adv->InitSpeaker();
	m_adv->SetRecordingDevice(USED_AUDIO_DEVICE_ID);
	m_adv->InitMicrophone();

	m_adv->StereoPlayoutIsAvailable(&stereo_playout);
	m_adv->SetStereoPlayout(stereo_playout);
	m_adv->StereoRecordingIsAvailable(&stereo_record);
	m_adv->SetStereoRecording(stereo_record);
	m_adv->InitPlayout();
	m_adv->InitRecording();
}

void calltest::StartCapture()
{
	m_video_capture_ptr.reset(webrtc::test::VcmCapturer::Create(USED_VIDEO_WIDTH, USED_VIDEO_HEIGHT, USED_VIDEO_FPS, USED_VIDEO_DEVICE_ID));
	m_video_capture_ptr->Start();
}
