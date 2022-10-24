import { Module } from "./system/module";

import { Index } from "./screens/index";
import { UnitControl } from "./screens/unit";
import { UnitMenu } from "./screens/unit-menu";
import { Gestures } from "./screens/gestures";

import { MenuToggle } from "./controls/menu-toggle";
import { MenuBottom } from "./controls/menu-bottom";
import { Checkbox } from "./controls/checkbox";
import { Snackbar } from "./controls/snackbar";
import { Slider } from "./controls/slider";
import { Spinner } from "./controls/spinner";
import { Outlet } from "./controls/outlet";

import "./system/touch";
import "./system/key";

Module.register({
    'sketch-outlet': Outlet.forRoot({
        './': {
            'unit': 'unit-control',
            'menu': 'unit-menu'
        },
        './js/setup.js': {
            'test': 'unit-test-list',
            'servos': 'unit-servos',
            'update': 'unit-update',
            'setup': 'unit-setup',
        }
    }),
    'sketch-slider': Slider,
    'sketch-spinner': Spinner,
    'sketch-checkbox': Checkbox,
    'sketch-snackbar': Snackbar,
    'sketch-menu-toggle': MenuToggle,
    'sketch-menu-bottom': MenuBottom,
    'unit-control': UnitControl,
    'unit-gestures': Gestures,
    'unit-menu': UnitMenu,
    'unit-outlet': Index,
})