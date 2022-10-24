import { Module } from "./system/module";
import { Setup } from "./screens/setup";
import { UnitServos } from "./screens/unit-servos";
import { UnitTestList } from "./screens/unit-test-list";
import { UnitTestServo } from "./screens/unit-test-servo";
import { SetupUnitName } from "./screens/setup-unit";
import { SetupUnitWifi } from "./screens/setup-wifi";
import { Firmware } from "./screens/setup-firmware";

Module.register({
    'unit-update': Firmware,
    'unit-setup': Setup,
    'unit-setup-general': SetupUnitName,
    'unit-setup-wifi': SetupUnitWifi,
    'unit-servos': UnitServos,
    'unit-test-list': UnitTestList,
    'unit-test-servo': UnitTestServo,
});