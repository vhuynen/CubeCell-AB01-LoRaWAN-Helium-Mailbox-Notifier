function Decoder(bytes, port) {

	//decode message type form payload
	var data= bytes[0];
	//output decoded message depending on the data
	switch(data){
		case 0x01: 
			return {
				msg: "You have got mail !\nBattery Voltage: " + ((bytes[1]  << 8 ) + bytes[2])  /  1000  + " Volts\nBattery Level: " + ((((((bytes[1]  << 8 ) + bytes[2])  /  1000) -3.7)/(4.2-3.7))*100).toPrecision(4) +"%\nHave a nice day !",
		};
		case 0x02: 
			return {
				msg: "You have got maybe a parcel !\nBattery Voltage: " + ((bytes[1]  << 8 ) + bytes[2])  /  1000  + " Volts\nBattery Level: " + ((((((bytes[1]  << 8 ) + bytes[2])  /  1000) -3.7)/(4.2-3.7))*100).toPrecision(4) +"%\nHave a nice day !",
		};
		case 0x03: 
			return {
				msg: "[ERROR] Doors have been opened too long when you have turned on the MCU. Reboot the MCU to reinitialize it !",
		};		
		case 0x04: 
			return {
				msg: "[ERROR] Doors have been opened too long when you have fetched your mail. Reboot the MCU to re-initialize it !",
		};	
		case 0x05: 
			return {
				msg: "[ERROR] Doors have been opened too long by the postman. Reboot the MCU to reinitialize it !",
		};
		case 0x06: 
			return {
				msg: "You have joined the people's network !\nBattery Voltage: " + ((bytes[1]  << 8 ) + bytes[2])  /  1000  + " Volts\nBattery Level: " + ((((((bytes[1]  << 8 ) + bytes[2])  /  1000) -3.7)/(4.2-3.7))*100).toPrecision(4) +"%\nHave a nice day !",

		};				
		default:
			return {msg:"unknown"};
	}
}