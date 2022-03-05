function Decoder(bytes, port, uplink_info) {

	//decode message type form payload
	var data = bytes[0];
	//output decoded message depending on the data
	switch (data) {
		case 0x03:
			return {
				msg: "Uplink n°" + uplink_info.fcnt + "\n[OK] You have got mail !\nBattery Voltage: "
					+ ((bytes[1] << 8) + bytes[2]) / 1000
					+ " Volts\nBattery Level: "
					+ ((((((bytes[1] << 8) + bytes[2]) / 1000) - 3.7) / (4.2 - 3.7)) * 100).toPrecision(4)
					+ "%\nHave a nice day !",
					code : 03
			};
		case 0x02:
			return {
				msg: "Uplink n°" + uplink_info.fcnt + "\n[OK] You have got maybe a package !\nBattery Voltage: "
					+ ((bytes[1] << 8) + bytes[2]) / 1000
					+ " Volts\nBattery Level: "
					+ ((((((bytes[1] << 8) + bytes[2]) / 1000) - 3.7) / (4.2 - 3.7)) * 100).toPrecision(4)
					+ "%\nHave a nice day !",
			};
		case 0x04:
			return {
				msg: "[ERROR] Doors have been opened for too long since you have powered the device on."
					+ "\nReboot the device to reinitialize it !",
			};
		case 0x05:
			return {
				msg: "[ERROR] Doors have been opened for too long since you have fetched your mail."
					+ "\nReboot the device to re-initialize it !",
			};
		case 0x06:
			return {
				msg: "[ERROR] Doors have been opened for too long by the postman."
					+ "\nReboot the device to reinitialize it !",
			};
		case 0x01:
			return {
				msg: "Uplink n°" + uplink_info.fcnt + "\nYou have joined the people's network !\nBattery Voltage: "
					+ ((bytes[1] << 8) + bytes[2]) / 1000
					+ " Volts\nBattery Level: "
					+ ((((((bytes[1] << 8) + bytes[2]) / 1000) - 3.7) / (4.2 - 3.7)) * 100).toPrecision(4)
					+ "%\nHave a nice day !",
			};
		default:
			return { msg: "\nunknown" };
	}
}