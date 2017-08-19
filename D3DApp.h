#pragma once
#include <Windows.h>
#include "GameTimer.h"

class D3DApp {
public:
	int Run();
	void CalculateFrameStats();
	void UpdateScene(float deltaTime);
	void DrawScene();
private:
	GameTimer mTimer;

	bool mAppPaused;
};