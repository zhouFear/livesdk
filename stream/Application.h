#pragma once
#include <Windows.h>
#include "modules/video_capture/video_capture.h"
#include <map>
#include <xstring>
#include <mutex>
#include <d3d11.h>

using namespace std;

#define MAX_LOADSTRING 100
class Application : public rtc::VideoSinkInterface<webrtc::VideoFrame>
{
public:
	Application(HINSTANCE inst, int nCmdShow);
	~Application();

	ATOM  MyRegisterClass();
	BOOL  InitInstance(int);
	void  Create(HWND hWnd);
	int  Run();
	void StartCapture(WCHAR* devName);
	
	HRESULT InitD3D(HWND hwnd);
	void CleanUp();
	void Render(int w, int h);

	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void SaveBitmapToFile();
	
	// rtc::VideoSinkInterface<webrtc::VideoFrame>
	virtual void OnFrame(const webrtc::VideoFrame& frame);
private:
	void _doConstruct();
private: 
	HINSTANCE m_hInst;
	HWND      m_hwnd;

	HWND      m_comboxHwnd;
	HWND      m_renderHwnd;

	WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
	WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
	map<wstring, string> m_deviceMap;

	std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> m_deviceInfo;
	std::mutex m_imge_mutex;
	std::shared_ptr<uint8_t> m_image_data;
	int frame_width;
	int frame_height;

	ID3D11Device* m_pd3dDevice = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;

	// std::chrono::steady_clock::time_point last;
	rtc::scoped_refptr<webrtc::VideoCaptureModule> m_videoCapMod;
};

