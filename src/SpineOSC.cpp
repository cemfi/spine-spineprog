// SpineOSC.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SpineOSC.h"
#include "hidapi.h"
#include <stdio.h>
#include "udp.hh"
#include "oscpkt.hh"
using namespace oscpkt;


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);



/// spine code
bool shouldRun;
hid_device *handle = NULL;

void openHID()
{
	handle = hid_open(0x2341, 0x8036, NULL);
	if (!handle) {
		return;
	}
	hid_set_nonblocking(handle, 1);
}

void sendOSC(float value,int connector, int channel)
{
	static UdpSocket *sock = NULL;
	if (!sock || !sock->isOk()) {
		const int PORT_NUM = 9876;
		sock = new UdpSocket;
		sock->connectTo("localhost", PORT_NUM);
	}
	if (sock->isOk()) {
		Message msg("/spine");
		msg.pushInt32(connector);
		msg.pushInt32(channel);
		msg.pushFloat(value);
		PacketWriter pw;
		//pw.startBundle().startBundle().addMessage(msg).endBundle().endBundle();
		pw.addMessage(msg);
		sock->sendPacket(pw.packetData(), pw.packetSize());
	}
}


DWORD run(LPVOID lpdwThreadParam)
{
	shouldRun = true;

	double last_value[12];
	int message_id[12];

	while (shouldRun) {
		int numMessages = 0;
		int spineID = 0;

		if (!handle) {
			openHID();
			if (!handle) {
				//[NSThread sleepForTimeInterval : 0.1];
				Sleep(100);
			}
		}
		if (handle) {
			//hid_write(handle, "hello", 6);
			//NSLog(@"open");
			unsigned char buf[256];
			memset(buf, 0, sizeof(buf));
			buf[0] = 0x3;
			int res = hid_read(handle, buf, sizeof(buf));
			if (res == 0) {
				// waiting
			}
			else if (res < 0) {
				// unable to read
				hid_close(handle);
				handle = NULL;
				//[NSThread sleepForTimeInterval:0.1];
			}
			else {
				// OK
				//NSLog(@"%d, %d", res, buf[0]);
				if ((res == 62) && (buf[0] == 3)) {
					union {
						float fvalue;
						unsigned char bvalue[4];
					} fbConversion;

					numMessages = buf[1] >> 4;
					spineID = buf[1] & 0xF;

					int i;
					for (i = 0; i < numMessages; i++) {
						// third byte: 5 bits value id (1F - no compund), 3 bits slot-ID
						message_id[i] = buf[2 + 5 * i];
						fbConversion.bvalue[0] = buf[3 + 5 * i];
						fbConversion.bvalue[1] = buf[4 + 5 * i];
						fbConversion.bvalue[2] = buf[5 + 5 * i];
						fbConversion.bvalue[3] = buf[6 + 5 * i];
						last_value[i] = fbConversion.fvalue;
					}
				}
			}
		}
		int i;
		for (i = 0; i < numMessages; i++) {
			float value = last_value[i];
			int connector = message_id[i] & 0x7;
			int channel = (message_id[i] >> 3) & 0x1F;

			/*dispatch_async(dispatch_get_main_queue(), ^{

				[self.delegate receivedValue:value connector : connector channel : channel spineID : spineID];
			});*/
			wchar_t str[256];
			swprintf(str, 256, L"%d, %d, %f\n", connector, channel, value);
			sendOSC(value, connector, channel);
			OutputDebugString(str);
		}
	}
	return 0;
}

/// end spine code

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SPINEOSC, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPINEOSC));
	OutputDebugString(L"starting spine");

	DWORD dwThreadId;
	CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)&run,
		NULL,
		0,
		&dwThreadId);

	OutputDebugString(L"spine stopped");

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPINEOSC));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = 0;//MAKEINTRESOURCE(IDC_SPINEOSC);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | \
	   WS_CAPTION | \
	   WS_SYSMENU | \
	   WS_MINIMIZEBOX,
	   CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	wchar_t *text = NULL;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		text = L"Sending /spine OSC messages to port 9876";
		TextOut(hdc, 50, 40, text, wcslen(text));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
