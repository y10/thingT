import { Module } from "./system/module";
import { Setup } from "./screens/setup";
import { SetupUnitName } from "./screens/setup-unit";
import { SetupUnitWifi } from "./screens/setup-wifi";
import { Firmware } from "./screens/setup-firmware";

Module.register({
    'unit-update': Firmware,
    'unit-setup': Setup,
    'unit-setup-general': SetupUnitName,
    'unit-setup-wifi': SetupUnitWifi,
});