#pragma once
#include <string>
#include <nlohmann/json.hpp>

class Device
{
public:
	Device(const nlohmann::json& description);

	void SetDescription(const nlohmann::json& descr);

	std::string ip() const { 
		std::string address = description["ip"]; 
		return address;
	}

	std::string id() const {
		std::string address = description["id"];
		return address;
	}

	void PrintBasciData() const;
	void ManageDevice();
	void SetManagedEdit();
	void Capture();
private:
	nlohmann::json description;
};