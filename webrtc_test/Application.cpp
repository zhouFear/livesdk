#include "Application.h"
#include "framework.h"
#include "Resource.h"
#include <exception>
#include "modules/video_capture/video_capture_factory.h"
#include "modules/video_capture/video_capture.h"
#include "common.h"
#include "api/video/i420_buffer.h"
#include "libyuv/include/libyuv.h"

#pragma comment(lib,"D3D11.lib")
#define WM_DRAWFRAME WM_USER + 2000


Application::Application(HINSTANCE inst, int nCmdShow): m_hInst(inst), m_deviceInfo(webrtc::VideoCaptureFactory::CreateDeviceInfo())
{
	LoadStringW(inst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(inst, IDC_WEBRTCTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass();
	// 执行应用程序初始化:
	if (!InitInstance(nCmdShow))
	{
		throw std::exception("application init error!");
	}
}

Application::~Application()
{

}

ATOM Application::MyRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Application::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_WEBRTCTEST));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WEBRTCTEST);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL Application::InitInstance(int nCmdShow)
{
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, this);

	if (!hWnd)
	{
		return FALSE;
	}
	m_hwnd = hWnd;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void Application::Create(HWND hWnd)
{
	int err = 0;
	HWND _h = CreateWindow(L"Button", L"开始", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		1100, 80, 200, 30, hWnd, (HMENU)IDB_START, m_hInst, NULL);
	err = GetLastError();
	_h = CreateWindow(L"Button", L"截图", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		1100, 120, 200, 30, hWnd, (HMENU)IDB_JIETU, m_hInst, NULL);
	err = GetLastError();
	m_comboxHwnd = CreateWindow(L"ComboBox", L"", WS_VISIBLE | WS_CHILD| CBS_DROPDOWN | CBS_DROPDOWNLIST | CBS_SIMPLE,
		1100, 10, 200, 50, hWnd, (HMENU)IDC_DEVICE_LIST, m_hInst, NULL);
	err = GetLastError();
	m_renderHwnd = CreateWindow(L"Static", L"", WS_VISIBLE | WS_CHILD, 10, 10, 1080, 650, hWnd, (HMENU)IDC_STATIC_IMAGE, m_hInst, NULL);
	err = GetLastError();
	_doConstruct();
}

int Application::Run()
{
	HACCEL hAccelTable = LoadAccelerators(m_hInst, MAKEINTRESOURCE(IDC_WEBRTCTEST));
	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

void Application::StartCapture(WCHAR* devName)
{
	string strUniqueDevName = m_deviceMap[devName];
	webrtc::VideoCaptureCapability videoCap;
	if (strUniqueDevName != "" && m_deviceInfo)
	{
		int numbers = m_deviceInfo->NumberOfCapabilities(strUniqueDevName.c_str());
		m_deviceInfo->GetCapability(strUniqueDevName.c_str(), 2, videoCap);
		m_videoCapMod = webrtc::VideoCaptureFactory::Create(strUniqueDevName.c_str());
		m_videoCapMod->RegisterCaptureDataCallback(this);
		m_videoCapMod->StartCapture(videoCap);
	}
}

HRESULT Application::InitD3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;  // BufferCount: number of buffers  1: 设置一个后缓存，再加上一个自动设置的前缓存，就是双缓存
	sd.BufferDesc.Width = 0; //系统自动设置合适的值
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;  //不处于全屏模式，所以不用管刷新率
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;  //程序自己决定缩放比率
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //对于大多数显示器，不用设置扫描线顺序
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // Use the surface or resource as an output render target.
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;   //不需要抗锯齿，一般默认该值
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE; //窗口化
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_pSwapChain,
		&m_pd3dDevice,
		NULL,
		&m_pImmediateContext);
	if (FAILED(hr))
	{
		return hr;
	}
	//gain access to texture subresource in swap chain (back buffer)
	ID3D11Resource* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, _uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)); // Get the back buffer  0: 后缓存的索引
	m_pd3dDevice->CreateRenderTargetView(
		pBackBuffer,
		nullptr,
		&pTarget  // pTarget is used to save RenderTarget view
	);
	pBackBuffer->Release();
}

void Application::CleanUp()
{
	//释放Direct3D设备对象  
	if (m_pd3dDevice != NULL)
		m_pd3dDevice->Release();
	//释放Direct3D对象  
	if (m_pSwapChain != NULL)
		m_pSwapChain->Release();
	if (m_pImmediateContext != NULL)
		m_pImmediateContext->Release();
	if (pTarget != NULL)
		pTarget->Release();
}

void Application::Render(int w, int h)
{
	frame_width = w;
	frame_height = h;
	std::lock_guard<std::mutex> lock(m_imge_mutex);
	HDC hdc = GetDC(m_renderHwnd);
	RECT rect;
	GetWindowRect(m_renderHwnd, &rect);
	int MAX_WIDTH = rect.right;
	int MAX_HEIGHT = rect.bottom;
	int sz = w * 4 * h;
	HDC hdcTemp = CreateCompatibleDC(hdc);
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = w;
	bitmap.bmiHeader.biHeight = h;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = sz;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	BYTE* pDibs = NULL;
	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&(pDibs)), NULL, NULL);
	uint8_t* _data = m_image_data.get();
	for (int i = 0; i < sz; ++i)
	{
		pDibs[i] = _data[i];
	}
	HGLOBAL hOld = SelectObject(hdcTemp, hBitmap2);
	StretchBlt(hdc, MAX_WIDTH, MAX_HEIGHT, -MAX_WIDTH, -MAX_HEIGHT, hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, SRCCOPY );
	SelectObject(hdcTemp, hOld);
	ReleaseDC(m_renderHwnd, hdc);
	DeleteDC(hdcTemp);
	DeleteObject(hBitmap2);
}

LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPCREATESTRUCT pcs = NULL;
	Application* pThis = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			::DialogBox(pThis->m_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, Application::About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDB_START:
		{
			int ItemIndex = SendMessage(pThis->m_comboxHwnd, (UINT)CB_GETCURSEL,
				(WPARAM)0, (LPARAM)0);
			WCHAR  ListItem[256];
			(WCHAR)SendMessage(pThis->m_comboxHwnd, (UINT)CB_GETLBTEXT,
				(WPARAM)ItemIndex, (LPARAM)ListItem);
			pThis->StartCapture(ListItem);
		}
			break;
		case IDB_JIETU:
		{
			pThis->SaveBitmapToFile();
		}break;
		case CBN_SELCHANGE:
		{
			int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
				(WPARAM)0, (LPARAM)0);
			WCHAR  ListItem[256];
			(WCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
				(WPARAM)ItemIndex, (LPARAM)ListItem);
			pThis->StartCapture(ListItem);
		}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_CREATE:
		pcs = (LPCREATESTRUCT)lParam;
		pThis = reinterpret_cast<Application*>(pcs->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)pThis);
		pThis->Create(hWnd);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_DRAWFRAME:
	{
		pThis->Render((int)wParam, (int)lParam);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK Application::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void Application::OnFrame(const webrtc::VideoFrame& video_frame)
{
	rtc::scoped_refptr<webrtc::I420BufferInterface>buffer(
		video_frame.video_frame_buffer()->ToI420());
	if (video_frame.rotation() != webrtc::kVideoRotation_0)
	{
		buffer = webrtc::I420Buffer::Rotate(*buffer, video_frame.rotation());
	}
	std::lock_guard<std::mutex> lock(m_imge_mutex);
	if (frame_width != buffer->width() || frame_height != buffer->height())
	{
		m_image_data.reset(new uint8_t[buffer->width() * buffer->height() * 4]);
	}
	
	libyuv::I420ToARGB(buffer->DataY(), buffer->StrideY(), buffer->DataU(),
		buffer->StrideU(), buffer->DataV(), buffer->StrideV(),
		m_image_data.get(),
		buffer->width() * 4,
		buffer->width(), buffer->height());
	PostMessage(m_hwnd, WM_DRAWFRAME, buffer->width(), buffer->height());
}

void Application::SaveBitmapToFile()
{
	TCHAR UserProfile[MAX_PATH];
	memset(UserProfile, 0, sizeof(TCHAR) * MAX_PATH);
	GetEnvironmentVariable(_T("USERPROFILE"), UserProfile, MAX_PATH);
	_tcscat_s(UserProfile, MAX_PATH, _T("\\My Documents\\"));

	SYSTEMTIME timeNow;
	GetLocalTime(&timeNow);
	TCHAR FileName[MAX_PATH];
	memset(FileName, 0, sizeof(FileName));
	_sntprintf_s(FileName, MAX_PATH, MAX_PATH - 1, _T("%04d%02d%02d%02d%02d%02d.bmp"), timeNow.wYear, timeNow.wMonth, timeNow.wDay, timeNow.wHour, timeNow.wMinute, timeNow.wSecond);
	_tcscat_s(UserProfile, MAX_PATH, FileName);
	TCHAR*   lpFileName = UserProfile;

	HDC  hDC;               
	int  iBits;             
	WORD  wBitCount;

	DWORD dwPaletteSize = 0, dwBmBitsSize, dwDIBSize, dwWritten;
	BITMAP               Bitmap;                         //位图属性结构
	BITMAPFILEHEADER     bmfHdr;             //位图文件头结构
	BITMAPINFOHEADER     bi;               //位图信息头结构
	LPBITMAPINFOHEADER   lpbi;                 //指向位图信息头结构
	HANDLE                fh, hDib;   //定义文件，分配内存句柄
	HPALETTE            hPal, hOldPal = NULL;//调色板句柄

	// CDC   dc;
	HWND hwnd = m_renderHwnd;
	HDC   hdc = ::GetDC(hwnd);
	HDC hComDC = CreateCompatibleDC(hdc);
	RECT   r;
	GetClientRect(hwnd, &r);

	int cx = r.right - r.left;
	int cy = r.bottom - r.top;
	HBITMAP bm = CreateCompatibleBitmap(hdc, cx, cy);
	HGLOBAL hOld = SelectObject(hComDC, bm);
	BitBlt(hComDC, 0, 0, cx, cy, hdc, 0, 0, SRCCOPY);

	//计算位图文件每个像素所占字节数
	hDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else   if (iBits <= 4)
		wBitCount = 4;
	else   if (iBits <= 8)
		wBitCount = 8;
	else   if (iBits <= 24)
		wBitCount = 24;
	else   if (iBits <= 32)
		wBitCount = 32;
	//计算调色板大小
	if (wBitCount <= 8)
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);

	//设置位图信息头结构
	GetObject(bm, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
	//为位图内容分配内存
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	//   处理调色板
	hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = SelectPalette(hDC, hPal, FALSE);
		RealizePalette(hDC);
	}
	//   获取该调色板下新的像素值
	GetDIBits(hDC, bm, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);
	//恢复调色板
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
	}
	::ReleaseDC(NULL, hDC);
	//创建位图文件
	fh = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh != INVALID_HANDLE_VALUE)
	{
		//   设置位图文件头
		bmfHdr.bfType = 0x4D42;     //"BM"
		dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
		bmfHdr.bfSize = dwDIBSize;
		bmfHdr.bfReserved1 = 0;
		bmfHdr.bfReserved2 = 0;
		bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
		//   写入位图文件头
		WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
		//   写入位图文件其余内容
		WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	}
	//清除
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	SelectObject(hComDC, hOld);
	DeleteObject(bm);    
}

void Application::_doConstruct()
{
	if (m_deviceInfo)
	{
		for (int i = 0; i < m_deviceInfo->NumberOfDevices(); ++i)
		{
			char devName[MAX_PATH] = { 0 };
			char uniqueName[MAX_PATH] = { 0 };
			//保存所有的设备名称
			if (m_deviceInfo->GetDeviceName(i, devName, MAX_PATH, uniqueName, MAX_PATH) != -1)
			{
				WCHAR* wDevName = NULL;
				common::a2w(devName, MAX_PATH, &wDevName);
				SendMessage(m_comboxHwnd, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)wDevName);
				m_deviceMap[wDevName] = uniqueName;
				delete []wDevName;
			}
		}
	}
}
