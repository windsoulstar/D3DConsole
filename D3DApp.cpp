#include "stdafx.h"
#include "D3DApp.h"
#include <sstream>

float D3DApp::AspectRatio() const {
	return static_cast<float>(mClientWidth) / mClientHeight;
}

int D3DApp::Run() {
	MSG msg = { 0 };
	mTimer.Reset();

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			mTimer.Tick();
			if (!mAppPaused) {
				CalculateFrameStats();
				UpdateScene(mTimer.DeltaTime());
				DrawScene();
			}
			else
				Sleep(100);
		}
	}
	return (int)msg.wParam;
}

void D3DApp::CalculateFrameStats() {
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f) {
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << L""
			<< L"FPS:" << fps << L""
			<< L"Frame Time" << mspf << L"(ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}
