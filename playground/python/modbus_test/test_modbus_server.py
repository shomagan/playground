"""
Pytest suite for the user Modbus TCP server at 192.168.7.2:502.

Register map (from regs_description_user):
  FC04 @0  U16  mdb_addr           = 0x0003
  FC04 @1  U8x4 interface_ip       = {192, 168, 7, 2}
  FC03 @0  U16x16 mdb_holding_regs = 0x0001 .. 0x0010
  FC02 @0  U16  digital_input_0    = 0x1828
  FC02 @16 U8   digital_input_1    = 0x55
  FC02 @24 U8   digital_input_2    = 0xAA
  FC01 @0  U16  digital_output     = 0x0010

Unit ID defaults to 3 (mdb_addr). Holding register elements accept values 0..1000;
values above 1000 must return Modbus exception 0x03 (Illegal Data Value).
"""

import pytest

# Expected initial values from firmware defaults.
EXPECTED_MDB_ADDR = 0x0003
EXPECTED_INTERFACE_IP = (192, 168, 7, 2)
EXPECTED_HOLDING_REGS = list(range(1, 17))
EXPECTED_DIGITAL_INPUT_0 = 0x1828
EXPECTED_DIGITAL_INPUT_1 = 0x55
EXPECTED_DIGITAL_INPUT_2 = 0xAA
EXPECTED_DIGITAL_OUTPUT = 0x0010
HOLDING_REG_MAX = 1000


def _call(client, method, address, count=1, value=None, device_id=3):
    kwargs = {"device_id": device_id}
    if method in ("read_coils", "read_discrete_inputs", "read_holding_registers", "read_input_registers"):
        return getattr(client, method)(address, count=count, **kwargs)
    if method == "write_coil":
        return client.write_coil(address, value, **kwargs)
    if method == "write_register":
        return client.write_register(address, value, **kwargs)
    raise ValueError(f"unknown method: {method}")


def _require_ok(response, context):
    assert not response.isError(), f"{context}: {response}"
    return response


def _expect_modbus_error(client, method, address, device_id=3, value=None, count=1):
    response = _call(client, method, address, count=count, value=value, device_id=device_id)
    assert response.isError(), f"expected Modbus error for {method}@{address}, got {response}"


def _discrete_lsb(value: int) -> bool:
    """Firmware exposes user discrete inputs via the low bit of each 2-byte slot."""
    return bool(value & 0x01)


def _pack_ip_registers(ip: tuple[int, int, int, int]) -> list[int]:
    return [(ip[0] << 8) | ip[1], (ip[2] << 8) | ip[3]]


class TestConnection:
    def test_server_reachable(self, modbus_client):
        assert modbus_client.connected


class TestInputRegisters:
    def test_modbus_address(self, modbus_client, modbus_unit_id):
        response = _require_ok(
            _call(modbus_client, "read_input_registers", 0, 1, device_id=modbus_unit_id),
            "read mdb_addr",
        )
        assert response.registers[0] == EXPECTED_MDB_ADDR

    def test_interface_ip(self, modbus_client, modbus_unit_id):
        response = _require_ok(
            _call(modbus_client, "read_input_registers", 1, 2, device_id=modbus_unit_id),
            "read interface_ip",
        )
        assert response.registers == _pack_ip_registers(EXPECTED_INTERFACE_IP)


class TestHoldingRegisters:
    def test_read_defaults(self, modbus_client, modbus_unit_id):
        response = _require_ok(
            _call(modbus_client, "read_holding_registers", 0, 16, device_id=modbus_unit_id),
            "read holding regs",
        )
        assert response.registers == EXPECTED_HOLDING_REGS

    def test_write_valid_value(self, modbus_client, modbus_unit_id):
        address = 0
        new_value = 500
        _require_ok(
            _call(
                modbus_client,
                "write_register",
                address,
                value=new_value,
                device_id=modbus_unit_id,
            ),
            "write valid holding reg",
        )
        response = _require_ok(
            _call(modbus_client, "read_holding_registers", address, 1, device_id=modbus_unit_id),
            "read back holding reg",
        )
        assert response.registers[0] == new_value

        # Restore default so later runs stay deterministic.
        _require_ok(
            _call(
                modbus_client,
                "write_register",
                address,
                value=EXPECTED_HOLDING_REGS[address],
                device_id=modbus_unit_id,
            ),
            "restore holding reg",
        )

    def test_write_at_max_boundary(self, modbus_client, modbus_unit_id):
        address = 1
        _require_ok(
            _call(
                modbus_client,
                "write_register",
                address,
                value=HOLDING_REG_MAX,
                device_id=modbus_unit_id,
            ),
            "write max holding reg",
        )
        response = _require_ok(
            _call(modbus_client, "read_holding_registers", address, 1, device_id=modbus_unit_id),
            "read max holding reg",
        )
        assert response.registers[0] == HOLDING_REG_MAX

        _require_ok(
            _call(
                modbus_client,
                "write_register",
                address,
                value=EXPECTED_HOLDING_REGS[address],
                device_id=modbus_unit_id,
            ),
            "restore holding reg",
        )

    @pytest.mark.parametrize("bad_value", [1001, 5000, 0xFFFF])
    def test_write_over_max_returns_illegal_data_value(self, modbus_client, modbus_unit_id, bad_value):
        _expect_modbus_error(
            modbus_client,
            "write_register",
            address=2,
            value=bad_value,
            device_id=modbus_unit_id,
        )

    def test_illegal_address(self, modbus_client, modbus_unit_id):
        _expect_modbus_error(
            modbus_client,
            "read_holding_registers",
            address=100,
            count=1,
            device_id=modbus_unit_id,
        )


class TestDiscreteInputs:
    def test_digital_input_0(self, modbus_client, modbus_unit_id):
        response = _require_ok(
            _call(modbus_client, "read_discrete_inputs", 0, 1, device_id=modbus_unit_id),
            "read digital_input_0",
        )
        assert response.bits[0] == _discrete_lsb(EXPECTED_DIGITAL_INPUT_0)

    def test_digital_input_1(self, modbus_client, modbus_unit_id):
        response = _require_ok(
            _call(modbus_client, "read_discrete_inputs", 16, 1, device_id=modbus_unit_id),
            "read digital_input_1",
        )
        assert response.bits[0] == _discrete_lsb(EXPECTED_DIGITAL_INPUT_1)

    def test_digital_input_2(self, modbus_client, modbus_unit_id):
        response = _require_ok(
            _call(modbus_client, "read_discrete_inputs", 24, 1, device_id=modbus_unit_id),
            "read digital_input_2",
        )
        assert response.bits[0] == _discrete_lsb(EXPECTED_DIGITAL_INPUT_2)


class TestCoils:
    def test_read_digital_output(self, modbus_client, modbus_unit_id):
        response = _require_ok(
            _call(modbus_client, "read_coils", 0, 1, device_id=modbus_unit_id),
            "read digital_output",
        )
        assert response.bits[0] == _discrete_lsb(EXPECTED_DIGITAL_OUTPUT)

    def test_write_and_read_coil(self, modbus_client, modbus_unit_id):
        _require_ok(
            _call(modbus_client, "write_coil", 0, value=True, device_id=modbus_unit_id),
            "write coil on",
        )
        response = _require_ok(
            _call(modbus_client, "read_coils", 0, 1, device_id=modbus_unit_id),
            "read coil on",
        )
        assert response.bits[0] is True

        _require_ok(
            _call(modbus_client, "write_coil", 0, value=False, device_id=modbus_unit_id),
            "write coil off",
        )
        response = _require_ok(
            _call(modbus_client, "read_coils", 0, 1, device_id=modbus_unit_id),
            "read coil off",
        )
        assert response.bits[0] is False

        # Restore default state.
        default_on = _discrete_lsb(EXPECTED_DIGITAL_OUTPUT)
        _require_ok(
            _call(
                modbus_client,
                "write_coil",
                0,
                value=default_on,
                device_id=modbus_unit_id,
            ),
            "restore digital_output",
        )


class TestUnitId:
    def test_wrong_unit_id_is_rejected(self, modbus_client):
        response = _call(modbus_client, "read_holding_registers", 0, count=1, device_id=99)
        assert response.isError(), f"expected failure for wrong unit id, got {response}"