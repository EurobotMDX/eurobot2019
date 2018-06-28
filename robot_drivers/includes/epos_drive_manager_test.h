#pragma once

#include "epos_drive_manager.h"

class EposDriveManagerTest
{
public:
	EposDriveManagerTest();
	~EposDriveManagerTest();

	bool initialize(const std::string device_name, const std::string port_name);
	bool terminate() const;

	bool test_all() const;

	bool test_rpm() const;
	bool test_current() const;
	bool test_position() const;

	bool test_brakes() const;

private:
	EposDriveManager _epos_device;
};
