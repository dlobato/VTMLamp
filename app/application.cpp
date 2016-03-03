#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <AppSettings.h>
#include <Adafruit_NeoPixel/Adafruit_NeoPixel.h>
#include <strip_programs.h>

// Which pin on the Esp8266 is connected to the NeoPixels?
#define PIN            4

// How many NeoPixels are attached to the Esp8266?
#define NUMPIXELS      2

#define WIFI_SSID "Vodafone-9EA315"
#define WIFI_PWD "10203040"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

HttpServer httpServer;
FTPServer ftpServer;

BssList networks;
String network, password;
Timer connectionTimer;

void onIndex(HttpRequest &request, HttpResponse &response)
{
	TemplateFileStream *tmpl = new TemplateFileStream("index.html");
	auto &vars = tmpl->variables();
	response.sendTemplate(tmpl); // will be automatically deleted
}

void onIpConfig(HttpRequest &request, HttpResponse &response)
{
	if (request.getRequestMethod() == RequestMethod::POST)
	{
		AppSettings.dhcp = request.getPostParameter("dhcp") == "1";
		AppSettings.ip = request.getPostParameter("ip");
		AppSettings.netmask = request.getPostParameter("netmask");
		AppSettings.gateway = request.getPostParameter("gateway");
		debugf("Updating IP settings: %d", AppSettings.ip.isNull());
		AppSettings.save();
	}

	TemplateFileStream *tmpl = new TemplateFileStream("settings.html");
	auto &vars = tmpl->variables();

	bool dhcp = WifiStation.isEnabledDHCP();
	vars["dhcpon"] = dhcp ? "checked='checked'" : "";
	vars["dhcpoff"] = !dhcp ? "checked='checked'" : "";

	if (!WifiStation.getIP().isNull())
	{
		vars["ip"] = WifiStation.getIP().toString();
		vars["netmask"] = WifiStation.getNetworkMask().toString();
		vars["gateway"] = WifiStation.getNetworkGateway().toString();
	}
	else
	{
		vars["ip"] = "192.168.1.77";
		vars["netmask"] = "255.255.255.0";
		vars["gateway"] = "192.168.1.1";
	}

	response.sendTemplate(tmpl); // will be automatically deleted
}

void onFile(HttpRequest &request, HttpResponse &response)
{
	String file = request.getPath();
	if (file[0] == '/')
		file = file.substring(1);

	if (file[0] == '.')
		response.forbidden();
	else
	{
		response.setCache(86400, true); // It's important to use cache for better performance.
		response.sendFile(file);
	}
}

void onAjaxNetworkList(HttpRequest &request, HttpResponse &response)
{
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();

	json["status"] = (bool)true;

	bool connected = WifiStation.isConnected();
	json["connected"] = connected;
	if (connected)
	{
		// Copy full string to JSON buffer memory
		json["network"]= WifiStation.getSSID();
	}

	JsonArray& netlist = json.createNestedArray("available");
	for (int i = 0; i < networks.count(); i++)
	{
		if (networks[i].hidden) continue;
		JsonObject &item = netlist.createNestedObject();
		item["id"] = (int)networks[i].getHashId();
		// Copy full string to JSON buffer memory
		item["title"] = networks[i].ssid;
		item["signal"] = networks[i].rssi;
		item["encryption"] = networks[i].getAuthorizationMethodName();
	}

	response.setAllowCrossDomainOrigin("*");
	response.sendJsonObject(stream);
}

void makeConnection()
{
	WifiStation.enable(true);
	WifiStation.config(network, password);

	AppSettings.ssid = network;
	AppSettings.password = password;
	AppSettings.save();

	network = ""; // task completed
}

void onAjaxConnect(HttpRequest &request, HttpResponse &response)
{
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();

	String curNet = request.getPostParameter("network");
	String curPass = request.getPostParameter("password");

	bool updating = curNet.length() > 0 && (WifiStation.getSSID() != curNet || WifiStation.getPassword() != curPass);
	bool connectingNow = WifiStation.getConnectionStatus() == eSCS_Connecting || network.length() > 0;

	if (updating && connectingNow)
	{
		debugf("wrong action: %s %s, (updating: %d, connectingNow: %d)", network.c_str(), password.c_str(), updating, connectingNow);
		json["status"] = (bool)false;
		json["connected"] = (bool)false;
	}
	else
	{
		json["status"] = (bool)true;
		if (updating)
		{
			network = curNet;
			password = curPass;
			debugf("CONNECT TO: %s %s", network.c_str(), password.c_str());
			json["connected"] = false;
			connectionTimer.initializeMs(1200, makeConnection).startOnce();
		}
		else
		{
			json["connected"] = WifiStation.isConnected();
			debugf("Network already selected. Current status: %s", WifiStation.getConnectionStatusName());
		}
	}

	if (!updating && !connectingNow && WifiStation.isConnectionFailed())
		json["error"] = WifiStation.getConnectionStatusName();

	response.setAllowCrossDomainOrigin("*");
	response.sendJsonObject(stream);
}

void onAjaxSetStripProgram(HttpRequest &request, HttpResponse &response)
{
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();

	uint8_t brightness = 0;
	uint8_t  r = 0;
	uint8_t  g = 0;
	uint8_t  b = 0;

	long tmp_param = request.getPostParameter("brightness").toInt();
	if (tmp_param >= 0 && tmp_param < 256)
	{
		brightness = tmp_param;
	}

	tmp_param = request.getPostParameter("r").toInt();
	if (tmp_param >= 0 && tmp_param < 256)
	{
		r = tmp_param;
	}

	tmp_param = request.getPostParameter("g").toInt();
	if (tmp_param >= 0 && tmp_param < 256)
	{
		g = tmp_param;
	}

	tmp_param = request.getPostParameter("b").toInt();
	if (tmp_param >= 0 && tmp_param < 256)
	{
		b = tmp_param;
	}

	debugf("setting pixels to: brightness=%d, r=%d, g=%d, b=%d", brightness, r, g, b);
	setColor(strip, brightness, r, g, b);

	json["status"] = (bool)true;
	json["brightness"] = brightness;
	json["r"] = r;
	json["g"] = g;
	json["b"] = b;

	response.setAllowCrossDomainOrigin("*");
	response.sendJsonObject(stream);
}

void startHttpServer()
{
	httpServer.listen(80);
	httpServer.addPath("/", onIndex);
	httpServer.addPath("/ipconfig", onIpConfig);
	httpServer.addPath("/ajax/get-networks", onAjaxNetworkList);
	httpServer.addPath("/ajax/connect", onAjaxConnect);
	httpServer.addPath("/ajax/set-strip-program", onAjaxSetStripProgram);
	httpServer.setDefaultHandler(onFile);
}

void startFtpServer()
{
	if (!fileExist("index.html"))
		fileSetContent("index.html", "<h3>Please connect to FTP and upload files from folder 'web/build' (details in code)</h3>");

	// Start FTP server
	ftpServer.listen(21);
	ftpServer.addUser("me", "123"); // FTP account
}


// Will be called when system initialization was completed
void startServers()
{
	startHttpServer();
	startFtpServer();
}

void networkScanCompleted(bool succeeded, BssList list)
{
	if (succeeded)
	{
		for (int i = 0; i < list.count(); i++)
			if (!list[i].hidden && list[i].ssid.length() > 0)
				networks.add(list[i]);
	}
	networks.sort([](const BssInfo& a, const BssInfo& b){ return b.rssi - a.rssi; } );
}

// Will be called when WiFi station was connected to AP
void connectOk()
{
	Serial.println("I'm CONNECTED");
}

// Will be called when WiFi station timeout was reached
void connectFail()
{
	Serial.println("I'm NOT CONNECTED. Need help :(");

	const String ap_ssid = "VTMLamp-" + WifiAccessPoint.getMAC();
	Serial.printf("%s\n", ap_ssid.c_str());

	// Start soft access point
	WifiAccessPoint.enable(true);
	WifiAccessPoint.config("VTMLamp-" + WifiAccessPoint.getMAC(), "", AUTH_OPEN);

	// use defaults and disable auto connect
	WifiStation.config(WIFI_SSID, WIFI_PWD, false);
}

void init()
{
	spiffs_mount(); // Mount file system, in order to work with files

	Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
	Serial.systemDebugOutput(true); // Enable debug output to serial
	AppSettings.load();

	WifiStation.enable(true);

	if (AppSettings.exist()){
		WifiStation.config(AppSettings.ssid, AppSettings.password);
		if (!AppSettings.dhcp && !AppSettings.ip.isNull())
			WifiStation.setIP(AppSettings.ip, AppSettings.netmask, AppSettings.gateway);
	}
	// Run our method when station was connected to AP (or not connected)
	WifiStation.waitConnection(connectOk, 20, connectFail); // We recommend 20+ seconds for connection timeout at start

	WifiStation.startScan(networkScanCompleted);

	//init neopixel strip
	strip.begin();  //init port
	setColor(strip, 0, 0, 0, 0);

	// Run WEB server on system ready
	System.onReady(startServers);
}
