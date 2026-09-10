import os

import pytest
from pymodbus.client import ModbusTcpClient


MODBUS_HOST = os.environ.get("MODBUS_HOST", "192.168.7.2")
MODBUS_PORT = int(os.environ.get("MODBUS_PORT", "502"))
MODBUS_UNIT_ID = int(os.environ.get("MODBUS_UNIT_ID", "3"))


@pytest.fixture(scope="session")
def modbus_host():
    return MODBUS_HOST


@pytest.fixture(scope="session")
def modbus_port():
    return MODBUS_PORT


@pytest.fixture(scope="session")
def modbus_unit_id():
    return MODBUS_UNIT_ID


@pytest.fixture(scope="session")
def modbus_client(modbus_host, modbus_port):
    client = ModbusTcpClient(modbus_host, port=modbus_port, timeout=3)
    if not client.connect():
        pytest.skip(f"Modbus server not reachable at {modbus_host}:{modbus_port}")
    yield client
    client.close()