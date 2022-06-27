#pragma once

#include "CoreserviceManager.h"
#include "chimera_exec.h"
#include <memory>
#include <fstream>

void device_event(std::shared_ptr<event_t> evt)
{
	auto manager = Manager();
	if (!manager) return;

	manager->DeviceEvent(evt->octets);
}

void capture_event(std::shared_ptr<event_t> evt)
{
	auto manager = Manager();
	if (!manager) return;

	manager->CaptureEvent(evt->octets);
	std::cout << "Capture response " << std::endl;
	std::cout << evt->octets << std::endl;
}

void job_event(std::shared_ptr<event_t> evt)
{
	auto manager = Manager();
	if (!manager) return;

	//manager->JobEvent(evt->octets);

	std::cout << "Job event response" << std::endl;
	nlohmann::json data = nlohmann::json::parse(evt->octets);
	std::cout << std::setw(4) << data << std::endl;

	std::ofstream out("output.txt");
	out << std::setw(4) << data;
	out.close();
}