#include "CoreserviceManager.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <set>

#include "Events.h"

#include "chimera_exec.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::shared_ptr<CoreServiceManager> coreServiceMnager;

std::shared_ptr<CoreServiceManager> InitializeManager()
{
	if (!coreServiceMnager)
		coreServiceMnager = std::make_shared<CoreServiceManager>();
	return coreServiceMnager;
}

std::shared_ptr<CoreServiceManager> Manager()
{
	return coreServiceMnager;
}

void DestroyManager()
{
	coreServiceMnager.reset();
}

CoreServiceManager::CoreServiceManager()
{
	chimeraRtMgr_init(false);
	RegisterEvents();
}

void CoreServiceManager::ListAllDevices()
{
	chimeraRtMgr_getAllDevices();
	std::cout << "Get All Devices executed" << std::endl;
	for (auto& device : devices)
	{
		device.second->PrintBasciData();
	}
	std::cout << std::endl;
}

void CoreServiceManager::ConnectByIp(const std::string& ip)
{
	std::string response = chimeraRtMgr_ConnectDeviceByIP(ip.c_str()).get();
	auto js = json::parse(response);
	std::cout << std::setw(4) << js << "\n\n";
}

void CoreServiceManager::ManageDevice(const std::string& id)
{
	auto dev = devices.find(id);
	if (dev != devices.end())
	{
		dev->second->ManageDevice();
		managedDev = dev->second.get();
	}
}

void CoreServiceManager::UnManageDevice(const std::string& id)
{
	auto dev = devices.find(id);
	int res = chimeraRtMgr_UnManageDevice(id.c_str());
	std::cout << "Unmanage result: " << res << std::endl;
	if (dev->second.get() == managedDev)
		managedDev = nullptr;
}

void CoreServiceManager::SetManagedEdit()
{
	managedDev->SetManagedEdit();
}

void CoreServiceManager::SetSubjectImage(const std::string& fileName)
{
	json j = {
		{"value", fileName},
		{"bank", 0},
		{"type", "FILE"}
	};
	std::string subject = j.dump();
	chimeraRtMgr_SetImageSource(subject.c_str());
}

void CoreServiceManager::RunJob(const std::string& jsonFileName)
{
	std::ifstream t(jsonFileName);
	std::stringstream buffer;
	buffer << t.rdbuf();

	chimeraRtMgr_RunJob(buffer.str().c_str());
}

void CoreServiceManager::Capture()
{
	managedDev->Capture();
}

CoreServiceManager::~CoreServiceManager()
{
	chimeraRtMgr_destroy();
}

//events
void CoreServiceManager::RegisterEvents()
{
	chimeraRtMgr_register(device_event, event_type::evt_device);
	chimeraRtMgr_register(capture_event, event_type::evt_image);
	chimeraRtMgr_register(job_event, event_type::evt_job);
}

void CoreServiceManager::DeviceEvent(const std::string& content)
{
	auto js = json::parse(content);
	auto jsdevices = js["devices"];

	std::set<std::string> to_remove;
	std::set<std::string> present;
	for (auto it = jsdevices.begin(); it != jsdevices.end(); ++it)
	{
		auto jsdevice = it.value();
		std::string id = jsdevice["id"];
		present.insert(id);
		if (auto dev = devices.find(id); dev == devices.end()) {
			auto device = std::make_unique<Device>(jsdevice);
			std::cout << "New device added: " << device->id() << "   " << device->ip() << std::endl;
			devices[id] = std::move(device);
		}
		else
		{
			dev->second->SetDescription(jsdevice);
		}
	}

	for(auto& dev : devices)
	{ 
		if (!present.contains(dev.second->id()))
			to_remove.insert(dev.second->id());
	}

	for (auto& id : to_remove)
	{
		std::cout << "Removing: " << id << std::endl;
		devices.erase(id);
	}

}

void CoreServiceManager::RegisterCallback(CallbackType type, EventCallback::CALLBACK_T callback)
{
	//capture_callback
}

void CoreServiceManager::CaptureEvent(const std::string& content)
{
	callbacks.Execute(content);
}