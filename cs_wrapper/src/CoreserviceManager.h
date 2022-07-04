#pragma once

#include "Device.h"

#include <memory>
#include <mutex>
#include <string>
#include <map>
#include <vector>
#include "EventCallback.h"

class CoreServiceManager
{
public:
	CoreServiceManager();
	~CoreServiceManager();

	void ListAllDevices();
	
	void ConnectByIp(const std::string& ip);
	void ManageDevice(const std::string& id);
	void UnManageDevice(const std::string& id);

	void SetSubjectImage(const std::string& fileName);
	void RunJob(const std::string& jsonFileName);
	void DeployJob(const std::string& jsonFileName);

	//events on managed device
	void SetManagedEdit();
	void Capture();

	//events:
	void RegisterCallback(CallbackType, EventCallback::CALLBACK_T f);

	void DeviceEvent(const std::string& content);
	void CaptureEvent(const std::string& content);
private:
	void RegisterEvents();

	//serial number -> device assignment
	std::mutex mtx;
	std::map<std::string, std::shared_ptr<Device>> devices;
	EventCallback callbacks;
	Device* managedDev;
};


std::shared_ptr<CoreServiceManager> InitializeManager();
std::shared_ptr<CoreServiceManager> Manager();
void DestroyManager();