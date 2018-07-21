Control Teufel's Decoderstation 5 remotely via Wifi/MQTT with slight modifications.

This tool listens for incoming IR signals to the Decoderstation and is also capable of fake-sending IR signals to the very same pin using a transistor circuit. It uses two additional lines to read the current status of the dispaly background light and the light behind the rotary encoder. Since reading from the rotary encoder didn't work properly, I'm using a I2C sniffer to listen for the current state of the Decoderstation's PT2258 Volume Controler IC to get the current volume level, to always get the precise value. The only value that can not be guaratneed to have the correct value is the input command.

## MQTT Topics

	dersimn/maintenance/DevilDecoder_12AB34/online      -> true
	dersimn/maintenance/DevilDecoder_12AB34/temperature -> 35.25
	dersimn/maintenance/DevilDecoder_12AB34/uptime      -> 2045
	dersimn/maintenance/DevilDecoder_12AB34/uptime/ms   -> 2040114

	dersimn/set/DevilDecoder_12AB34/hifi <- JSON

	dersimn/status/DevilDecoder_12AB34/hifi/display -> {"val":false}
	dersimn/status/DevilDecoder_12AB34/hifi/input   -> {"val":"Opt1"}
	dersimn/status/DevilDecoder_12AB34/hifi/power   -> {"val":true,"booting":false}
	dersimn/status/DevilDecoder_12AB34/hifi/volume  -> {"val":-28,"muted":false,"channels":{"FL":-28,"FR":-28,"C":-28,"SUB":-28,"RL":-28,"RR":-28}}
	dersimn/status/DevilDecoder_12AB34/ir           -> {"val":"807FE817","codec":"NEC","repeat":0}

## Things I tried first and didn't turn out to work:

- With the IRremoteESP8266 library it's not possible to use the same pin for listening and sending at the same time. You can load and un-load the library everytime you switch between reading and writing, but this causes the idle voltage to change on this certain pin. This voltage change will be interpreted by the Decoderstation as start of an IR signal and the station then waits for the rest of the signal (about 100ms). So basically you have to wait about 1000ms everytime you switch from IR listening to IR faking, which I think is too long. You can however use a transistor circuit.
- The Decoderstation "hickups" while listening for IR signals or the rotary encoder. I've noticed that earlier while using the remote: somethimes when changing the volume, the volume stops going up/down for a splitsecond, I thought this was because the remote is not strong enough to transmit every signal, but: the Decoderstation seems to poll the inputs and doesn't use interrupts, causing a few vol up/down comands to become lost on the way. This makes implementing "a microcontroler in the middle" impossible, because you can't be sure that the Decoderstation got every signal you sent to it.
- The first volume up/down signal is ignored: When pressing either button, the first time the volume is shown in the display, but won't change.

Result: It's impossible to "listen" for changes and build something like a process image, because even though our microcontroller gets evers IR impulse that was received, we can not guaratee that the Decoderstation received the same amount of pulses. For e.g. when sending 10 volume up and 10 volume down signals you don't end up at the same volume, the resulting volume differs everytime around maybe 1 or 3dB.
