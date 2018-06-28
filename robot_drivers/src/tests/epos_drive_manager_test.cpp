#include "epos_drive_manager_test.h"

EposDriveManagerTest::EposDriveManagerTest()
{
	_epos_device = EposDriveManager();
}

EposDriveManagerTest::~EposDriveManagerTest()
{
}


bool EposDriveManagerTest::initialize(const std::string device_name, const std::string port_name)
{
	return _epos_device.initialize(device_name, port_name);
}

bool EposDriveManagerTest::terminate() const
{
	return _epos_device.terminate();
}


bool EposDriveManagerTest::test_rpm() const
{
}

bool EposDriveManagerTest::test_current() const
{
}

bool EposDriveManagerTest::test_position() const
{
}


bool EposDriveManagerTest::test_brakes() const
{
}
