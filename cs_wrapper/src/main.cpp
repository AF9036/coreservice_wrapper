#include <cstdio>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <memory>
#include <string>
#include <iostream>

#include "chimera_exec.h"
#include "CoreserviceManager.h"
#include "CommandLine.h"

using namespace std::literals;

int main()
{
	auto manager = InitializeManager();
	RunCommandLine(*manager);
	manager.reset();
	DestroyManager();
	
	return 0;
}