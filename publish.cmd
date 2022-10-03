xcopy /s .\.bin\arduino.ino.bin \\nuc\sites\ota\thnigT.bin* /Y
rem curl -F "firmware=@./.bin/arduino.ino.bin" thingT.local/esp/update