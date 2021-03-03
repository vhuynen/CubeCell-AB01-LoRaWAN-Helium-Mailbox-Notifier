# HelTec-AB01-LoRaWAN-Helium-Mailbox-Notifier

It's an IoT DIY project which notifies you when you have received a parcel or a letter in your letterbox. It's the same projet that [ESP32-CAM-Mailbox-Notifier](https://github.com/vhuynen/ESP32-CAM-Mailbox-Notifier) but designed for long range distance.

This project uses an **HelTec AB01** LoRaWAN module to send data over an LoRaWAN Gateway when WiFi is out of range. I use an [Helium Gateway](https://www.helium.com/) from the **People's Network** in order to forward packets over Helium Network which send data to [AWS IoT Core](https://aws.amazon.com/iot-core/) (MQTT) or [Pipedream](https://pipedream.com/) (HTTP) and more. LoRaWAN is a bidirectional protocol useful when you need to send few data over long distance (several miles) from low-power and low-bandwidth IoT.

The compromise over this kind of architecture is that you can't send heavy payload as you can do over WiFi and finally the data must be treated over third-part system.  

