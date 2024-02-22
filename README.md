# 2022-23-Wireless-DRS
ESP32 hosted Web server to control DRS wirelessly during wind tunnel testing

# Using Wireless DRS
Connect to the ESP32 over WIFI  
:warning: input `our ip` to your browesr

# Docs/Debug

<details>
  <summary>Advanced Documentation</summary>
    <blockquote>
      <p>
        <a href="https://randomnerdtutorials.com/esp32-web-server-arduino-ide/">ESP32 Web Server Tutorial</a>
      </p>
      <details><summary>Finding The IP Address</summary>
        <blockquote>
          After uploading the code, open the Serial Monitor at a baud rate of 115200.<br>
          Press the ESP32 EN button (reset). The ESP32 connects to Wi-Fi, and outputs the ESP IP address on the Serial Monitor. Copy that IP address, because you need it to access the ESP32 web server.<br>
          <img src="https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2018/07/ESP-IP-address-1.png?w=739&quality=100&strip=all&ssl=1">
        </blockquote>
      </details>
      <details open><summary>Accessing the Control Site</summary>
        <blockquote>
          To access the web server, open your browser, paste the ESP32 IP address, and you’ll see the following page. Ex: 192.168.1.135.<br>
          <img src="https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2018/07/esp32-web-server-outputs.png?w=623&quality=100&strip=all&ssl=1">
        </blockquote>
      </details>
    </blockquote>
</details>
