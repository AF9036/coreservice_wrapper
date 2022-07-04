#include "CommandLine.h"
#include <iostream>

#include <cli/cli.h>
#include <cli/clifilesession.h>

using namespace cli;
using namespace std;

void RunCommandLine(CoreServiceManager& manager)
{
	auto rootMenu = make_unique<Menu>("cli");

	rootMenu->Insert(
		"list_devices",
		[&](std::ostream& out) { 
			manager.ListAllDevices(); 
		},
		"List all available devices"
	);
	rootMenu->Insert(
		"connect",
		[&](std::ostream& out, const std::string& address) {
			manager.ConnectByIp(address);
		},
		"Connect to device by ip address"
	);
	rootMenu->Insert(
		"managed",
		[&](std::ostream& out, const std::string& id) {
			manager.ManageDevice(id);
		},
		"Managed device by its id"
	);
	rootMenu->Insert(
		"unmanage",
		[&](std::ostream& out, const std::string& id) {
			manager.UnManageDevice(id);
		},
		"unManage device by its id"
	);
	rootMenu->Insert(
		"managed_edit",
		[&](std::ostream& out) {
			manager.SetManagedEdit();
		},
		"Set managed edit"
	);
	rootMenu->Insert(
		"capture",
		[&](std::ostream& out) {
			manager.Capture();
		},
		"Capture single frame"
	);
	rootMenu->Insert(
		"subject_image",
		[&](std::ostream& out, const std::string& fileName) {
			manager.SetSubjectImage(fileName);
		},
		"Set Subject Image File"
	);
	rootMenu->Insert(
		"run_job",
		[&](std::ostream& out, const std::string& job_json_file) {
			manager.RunJob(job_json_file);
		},
		"Run Job"
	);
	rootMenu->Insert(
		"deploy",
		[&](std::ostream& out, const std::string& job_json_file) {
			manager.DeployJob(job_json_file);
		},
		"Run Job"
	);
	Cli cli(std::move(rootMenu));
	
	cli.ExitAction([](auto& out) { out << "Goodbye and thanks for all the fish.\n"; });
	
	CliFileSession input(cli);
	input.Start();
}