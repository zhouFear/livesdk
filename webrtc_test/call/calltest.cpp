#include "calltest.h"
#include "call/call.h"
#include "modules/audio_mixer/audio_mixer_impl.h"
#include "test/vcm_capturer.h"
#include "modules/audio_device/audio_device_impl.h"
#include "api/audio_codecs/audio_encoder_factory_template.h"
#include "api/audio_codecs/audio_decoder_factory_template.h"
#include "api/audio_codecs/opus/audio_encoder_opus.h"
#include "api/audio_codecs/opus/audio_decoder_opus.h"
// #include ""

namespace {
	enum classPayloadTypes {
		kPayloadTypeRtx = 98,
		kPayloadTypeIlbc = 102,
		kPayloadTypeOpus = 111,
		kPayloadTypeH264 = 122,
		kPayloadTypeVP8 = 120,
		kPayloadTypeVP9 = 101,
	};


	const int CALL_MIN_BPS = 60 * 1000;
	const int CALL_START_BPS = 300 * 1000;
	const int CALL_MAX_BPS = 800 * 1000;

	const int USED_AUDIO_DEVICE_ID = 0;
	const int USED_VIDEO_DEVICE_ID = 0;
	
	const int USED_VIDEO_WIDTH = 640;
	const int USED_VIDEO_HEIGHT = 480;
	const int USED_VIDEO_FPS = 30;
	const int USED_MAX_VIDEO_QP = 48;

	const int SEND_SSRC = 11111111;
	const int RECV_SSRC	= 22222222;
	const int RTX_SSRC	= 33333333;

	const int AUDIO_MIN_BPS = 6 * 1000;
	const int AUDIO_MAX_BPS = 32 * 1000;
	const int kTransportSequenceNumberExtensionId = 8;

	const char* AV_SYNC_GROUP = "av_sync";
}

calltest::calltest():
	m_local_video_render_ptr(webrtc::test::VideoRenderer::Create("localvideorender", USED_VIDEO_WIDTH, USED_VIDEO_HEIGHT))
	,m_remote_video_render_ptr(webrtc::test::VideoRenderer::Create("remotevideorender", USED_VIDEO_WIDTH, USED_VIDEO_HEIGHT)) {
	
}

calltest::~calltest() {

}


void calltest::CreateCall()
{
	webrtc::AudioState::Config audioStateConfig;
	audioStateConfig.audio_device_module = m_adv_ptr;
	audioStateConfig.audio_mixer = webrtc::AudioMixerImpl::Create();
	audioStateConfig.audio_processing = webrtc::AudioProcessingBuilder().Create();

	webrtc::BitrateConstraints call_bitrate_config;
	call_bitrate_config.min_bitrate_bps = CALL_MIN_BPS;
	call_bitrate_config.start_bitrate_bps = CALL_START_BPS;
	call_bitrate_config.max_bitrate_bps = CALL_MAX_BPS;

	m_event_log_ptr = new webrtc::RtcEventLogNull();
	webrtc::CallConfig callConfig(m_event_log_ptr);
	callConfig.audio_state = webrtc::AudioState::Create(audioStateConfig);
	callConfig.bitrate_config = call_bitrate_config;
	m_adv_ptr->RegisterAudioCallback(callConfig.audio_state->audio_transport());

	m_call_ptr.reset(webrtc::Call::Create(callConfig));
	m_call_ptr->SignalChannelNetworkState(webrtc::MediaType::AUDIO, webrtc::kNetworkUp);
	m_call_ptr->SignalChannelNetworkState(webrtc::MediaType::VIDEO, webrtc::kNetworkUp);

	m_audioloop_transport_ptr.reset(new MediaLoopTransport(m_call_ptr, webrtc::MediaType::AUDIO));
	m_videoloop_transport_ptr.reset(new MediaLoopTransport(m_call_ptr, webrtc::MediaType::VIDEO));
}

void calltest::AudioDeviceModuleSetup()
{
	bool stereo_playout = false;
	bool stereo_record = false;

	if (!m_adv_ptr)
		m_adv_ptr = webrtc::AudioDeviceModule::Create(webrtc::AudioDeviceModule::kPlatformDefaultAudio, nullptr);
	m_adv_ptr->Init();
	m_adv_ptr->SetPlayoutDevice(USED_AUDIO_DEVICE_ID);
	m_adv_ptr->InitSpeaker();
	m_adv_ptr->SetRecordingDevice(USED_AUDIO_DEVICE_ID);
	m_adv_ptr->InitMicrophone();

	m_adv_ptr->StereoPlayoutIsAvailable(&stereo_playout);
	m_adv_ptr->SetStereoPlayout(stereo_playout);
	m_adv_ptr->StereoRecordingIsAvailable(&stereo_record);
	m_adv_ptr->SetStereoRecording(stereo_record);
	m_adv_ptr->InitPlayout();
	m_adv_ptr->InitRecording();
}

void calltest::StartCapture()
{
	m_video_capture_ptr.reset(webrtc::test::VcmCapturer::Create(USED_VIDEO_WIDTH, USED_VIDEO_HEIGHT, USED_VIDEO_FPS, USED_VIDEO_DEVICE_ID));
	// m_video_capture_ptr->Start();
}

void calltest::CreateAudioSendStream()
{
	webrtc::AudioSendStream::Config audio_send_config(m_audioloop_transport_ptr.get());
	audio_send_config.send_codec_spec = webrtc::AudioSendStream::Config::SendCodecSpec(kPayloadTypeOpus, { "OPUS", 48000, 2,{{"usedtx", "0"},{"stereo", "1"}} });
	audio_send_config.encoder_factory = webrtc::CreateAudioEncoderFactory<webrtc::AudioEncoderOpus>();
	audio_send_config.rtp.ssrc = SEND_SSRC + 1;
	audio_send_config.rtp.extensions.clear();
	if (m_bSend_side_bwe)
	{
		audio_send_config.min_bitrate_bps = AUDIO_MIN_BPS;
		audio_send_config.max_bitrate_bps = AUDIO_MAX_BPS;

		audio_send_config.rtp.extensions.push_back(webrtc::RtpExtension(webrtc::RtpExtension::kTransportSequenceNumberUri, kTransportSequenceNumberExtensionId));
	}
	m_audiosendstream_ptr.reset(m_call_ptr->CreateAudioSendStream(audio_send_config));
}

void calltest::CreateAudioReciveStream()
{
	webrtc::AudioReceiveStream::Config audio_rev_config;
	audio_rev_config.rtcp_send_transport = m_audioloop_transport_ptr.get();
	audio_rev_config.decoder_factory = webrtc::CreateAudioDecoderFactory<webrtc::AudioDecoderOpus>();
	audio_rev_config.sync_group = AV_SYNC_GROUP;
	audio_rev_config.decoder_map = { {kPayloadTypeOpus, {"OPUS", 48000, 2}} };
	audio_rev_config.rtp.remote_ssrc = SEND_SSRC + 1;
	audio_rev_config.rtp.local_ssrc = RECV_SSRC + 1;
	audio_rev_config.rtp.transport_cc = m_bSend_side_bwe;
	audio_rev_config.rtp.extensions.clear();

	if (m_bSend_side_bwe) {
		audio_rev_config.rtp.extensions.push_back(webrtc::RtpExtension(webrtc::RtpExtension::kTransportSequenceNumberUri, kTransportSequenceNumberExtensionId));
	}
	m_audiorecivestream_ptr = m_call_ptr->CreateAudioReceiveStream(audio_rev_config);

}

void calltest::CreateVideoSendStream()
{
	webrtc::VideoSendStream::Config video_send_config(m_videoloop_transport_ptr.get());

	video_send_config.rtp.ssrcs.push_back(SEND_SSRC);
	video_send_config.rtp.payload_name = "VP8";
	video_send_config.rtp.payload_type = kPayloadTypeVP8;
	// video_send_config.pre_encode_callback = m_local_video_render_ptr.get();
	video_send_config.encoder_settings.encoder_factory = [this]() {
		std::unique_ptr<FakeEncoder> fake_encoder;
		if (video_encoder_configs_[0].codec_type == kVideoCodecVP8) {
			fake_encoder = std::make_unique<FakeVp8Encoder>(clock_);
		}
		else {
			fake_encoder = std::make_unique<FakeEncoder>(clock_);
		}
		fake_encoder->SetMaxBitrate(fake_encoder_max_bitrate_);
		return fake_encoder;
	};
	video_send_config.rtp.nack.rtp_history_ms = 1000;
	video_send_config.rtp.rtx.ssrcs.push_back(RTX_SSRC);
	video_send_config.rtp.rtx.payload_type = kPayloadTypeRtx;

	video_send_config.rtp.extensions.clear();
	if (g_Send_side_bwe) {
		video_send_config.rtp.extensions.push_back(webrtc::RtpExtension(webrtc::RtpExtension::kTransportSequenceNumberUri, kTransportSequenceNumberExtensionId));
	}
	else {
		video_send_config.rtp.extensions.push_back(webrtc::RtpExtension(webrtc::RtpExtension::kAbsSendTimeUri, kAbsSendTimeExtensionId));
	}
	video_send_config.rtp.extensions.push_back(webrtc::RtpExtension(webrtc::RtpExtension::kVideoContentTypeUri, kVideoContentTypeExtensionId));
	video_send_config.rtp.extensions.push_back(webrtc::RtpExtension(webrtc::RtpExtension::kVideoTimingUri, kVideoTimingExtensionId));
	webrtc::VideoEncoderConfig encoder_config;

	encoder_config.codec_type = webrtc::kVideoCodecVP8;
	encoder_config.number_of_streams = 1;
	encoder_config.min_transmit_bitrate_bps = VIDEO_MIN_BPS;
	encoder_config.max_bitrate_bps = VIDEO_MAX_BPS;
	encoder_config.content_type = webrtc::VideoEncoderConfig::ContentType::kRealtimeVideo;

	webrtc::VideoCodecVP8 vp8_settings = webrtc::VideoEncoder::GetDefaultVp8Settings();
	encoder_config.encoder_specific_settings = new rtc::RefCountedObject<webrtc::VideoEncoderConfig::Vp8EncoderSpecificSettings>(vp8_settings);
	encoder_config.video_stream_factory = new rtc::RefCountedObject<cricket::EncoderStreamFactory>("VP8", USED_MAX_VIDEO_QP, USED_VIDEO_FPS, false, false);


	g_VideoSendStream = g_Call->CreateVideoSendStream(std::move(video_send_config), std::move(encoder_config));
	g_VideoSendStream->SetSource(g_Video_capturers.get(), webrtc::VideoSendStream::DegradationPreference::kBalanced);
}

void calltest::CreateVideoReciveStream()
{

}
