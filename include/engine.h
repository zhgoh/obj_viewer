#pragma once

struct GLFWwindow;

class Engine {
public:
	Engine(int width, int height);
	~Engine();
	
	void Run();

private:
	void Init();
	void InitCallbacks();
	void ImGuiFrame();
	void Render();

	GLFWwindow *mWindow;
	int mWidth, mHeight;
};