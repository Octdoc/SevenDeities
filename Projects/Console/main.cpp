#include <iostream>
#include <chrono>
#include <thread>

int main()
{
	auto prevTime = std::chrono::steady_clock::now();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	auto currentTime = std::chrono::steady_clock::now();
	std::cout << std::chrono::duration<float>(currentTime - prevTime).count() << std::endl;
	prevTime = currentTime;
	std::this_thread::sleep_for(std::chrono::milliseconds(16));
	currentTime = std::chrono::steady_clock::now();
	std::cout << std::chrono::duration<float>(currentTime - prevTime).count() << std::endl;
	prevTime = currentTime;
	std::this_thread::sleep_for(std::chrono::milliseconds(32));
	currentTime = std::chrono::steady_clock::now();
	std::cout << std::chrono::duration<float>(currentTime - prevTime).count() << std::endl;
	prevTime = currentTime;
	std::this_thread::sleep_for(std::chrono::milliseconds(64));
	currentTime = std::chrono::steady_clock::now();
	std::cout << std::chrono::duration<float>(currentTime - prevTime).count() << std::endl;
	prevTime = currentTime;

	system("pause");
	return 0;
}