#include "WIZnetInterface.h"
#include "mbed.h"

#define ECHO_SERVER_PORT 80

Serial pc(PTA2, PTA1);
SPI spi(D12, D11, D13);                // mosi, miso, sclk
WIZnetInterface eth(&spi, D10, D2); // spi, cs, reset

DigitalOut ledBlue(LED_BLUE, 1);
DigitalOut ledRed(LED_RED, 1);
DigitalOut ledGreen(LED_GREEN, 1);

void f_ethernet_init(void);

const char *IP_Addr = "169.254.175.210";
const char *IP_Subnet = "255.255.255.0";
const char *IP_Gateway = "169.254.175.1";
uint8_t mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
int ret, dummy;
int lv = 1;

char a[] = {
    "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"};
char b[] = {"<html>\r\n<body>\r\n"};
char c[] = {"<title>FRDM KL46Z</title>\r\n"};
char d[] = {"<h2>Web using Wiznet </h2>\r\n"};
char e[] = {"<br /><hr>\r\n"};
char f[] = {"<FONT FACE=\"Arial\" SIZE=\"2\" COLOR=\"FF0000\">"};

char paq_en[32];

void toggleRed() { ledRed = !ledRed; }

void toggleGreen() { ledGreen = !ledGreen; }

void toggleBlue() { ledBlue = !ledBlue; }

int main() {
  f_ethernet_init();

  TCPSocketServer server;
  server.bind(ECHO_SERVER_PORT);
  server.listen();
  pc.printf("\nWaiting for client...\n");

  TCPSocketConnection client;
  toggleBlue();
  while (lv) {

    lv = server.accept(client);
    if (!lv) {
      pc.printf("Client connected: %s\n", client.get_address());
      toggleBlue();
      toggleGreen();

    } else {
      toggleBlue();
      toggleRed();
      wait(10);
    }
  }

  client.send(a, sizeof(a));
  client.send(b, sizeof(b));
  client.send(c, sizeof(c));
  client.send(d, sizeof(d));
  client.send(e, sizeof(e));
  client.send(f, sizeof(f));

  while (1) {
    dummy++;
    sprintf(paq_en, "<FONT><br>Contador=%d</FONT>", dummy);
    client.send(paq_en, sizeof(paq_en));
    wait(1);
  }
}

void f_ethernet_init() {

  // mbed_mac_address((char *)mac);
  pc.printf("\tInitialize Ethernet...\n\r");
  wait(1);
  pc.printf("Expected MAC= %02X:%02X:%02X:%02X:%02X:%02X\n\r", mac[0], mac[1],
            mac[2], mac[3], mac[4], mac[5]);
  pc.printf("Before MAC= %s\n\r", eth.getMACAddress());
  ret = eth.init(mac, IP_Addr, IP_Subnet, IP_Gateway);
  if (!ret) {
    pc.printf("After MAC= %s\n\r", eth.getMACAddress());
  } else {
    pc.printf("Failed to set up ethernet...\n\r");
    toggleRed();
  }
  pc.printf("Board Connected.");
  wait(2);
  pc.printf(".");
  wait(2);
  pc.printf(".\n\r");
  wait(1);
  ret = eth.connect();
  if (!ret) {
    pc.printf("Board Connection Established!\n\n\r");
    wait(1);
    pc.printf("IP=%s\n\rMASK=%s\n\rGW=%s\n\r", eth.getIPAddress(),
              eth.getNetworkMask(), eth.getGateway());
  } else {
    pc.printf("Failed Connection\n\r");
    toggleRed();
  }
}