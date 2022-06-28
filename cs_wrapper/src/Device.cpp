#pragma once
#include "Device.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include "chimera_exec.h"

Device::Device(const nlohmann::json& descr)
	: description( descr )
{
}

void Device::SetDescription(const nlohmann::json& newDescr)
{
	if (description != newDescr) {
		std::cout << "Description updated of device: " << ip() << std::endl;

		auto diff = nlohmann::json::diff(description, newDescr);
		for (int i = 0; i < diff.size(); ++i)
		{
			std::string key = diff[i]["path"];
			key = key.substr(1);
			auto old_value = description[key];
			diff[i]["OldValue"] = old_value;
		}
		std::cout << std::setw(2) << diff << std::endl;
		description = newDescr;
	}
}


void Device::ManageDevice()
{
	std::cout << "Managed Device: " << std::endl;
	std::string response = chimeraRtMgr_ManagedDevice(id().c_str()).get();
	auto manage_response = nlohmann::json::parse(response);
	std::cout << std::setw(2) << manage_response << std::endl;
	std::cout << std::endl;
}

void Device::SetManagedEdit()
{
	std::cout << "Set Managed Edit: " << std::endl;
	int res = chimeraRtMgr_SetManagedEdit();
	std::cout << "Result: " << res << std::endl;
	std::cout << std::endl;
}

void Device::Capture()
{
	std::cout << "Capture request: " << std::endl;

	std::ifstream t("C:\\Users\\af9036\\OneDrive - Zebra Technologies\\Desktop\\CSTools\\helpers\\capture_payloads\\capture_payload.json");
	std::stringstream buffer;
	buffer << t.rdbuf();

	chimeraRtMgr_Capture(1, nullptr, buffer.str().c_str());
}

void Device::PrintBasciData() const
{
	std::cout << "Device: " << id() << std::endl;
	std::cout << "Ip: " << ip() << std::endl;
	std::cout << "Status: " << description["connected"] << std::endl;
	std::cout << std::endl;
}