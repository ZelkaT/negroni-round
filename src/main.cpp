#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

#ifdef USE_ROUND_LCD
#include "display.h"
#endif

// Configuration
const char* ssid = "Negroni";  // WiFi network name
const byte DNS_PORT = 53;
const int HTTP_PORT = 80;

// Create server instances
DNSServer dnsServer;
WebServer webServer(HTTP_PORT);

// HTML content - your Negroni poster
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Negroni Cocktail Poster</title>
    <link href="https://fonts.googleapis.com/css2?family=DM+Serif+Display:ital@0;1&family=Archivo:wght@300;400;500;600&display=swap" rel="stylesheet">
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
            background: #f5f0eb;
            padding: 40px 20px;
            font-family: 'Archivo', sans-serif;
        }

        .poster-container {
            position: relative;
            width: 100%;
            max-width: 600px;
            aspect-ratio: 0.707;
            background: #f9f6f2;
            box-shadow: 
                0 2px 4px rgba(0,0,0,0.05),
                0 8px 16px rgba(0,0,0,0.08),
                inset 0 0 0 1px rgba(139, 115, 85, 0.1);
            padding: 60px 50px;
            display: flex;
            flex-direction: column;
            animation: fadeIn 0.8s ease-out;
        }

        @keyframes fadeIn {
            from {
                opacity: 0;
                transform: translateY(20px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }

        .poster-title {
            font-family: 'DM Serif Display', serif;
            font-size: clamp(3rem, 8vw, 5.5rem);
            font-weight: 400;
            color: #1a1614;
            letter-spacing: -0.02em;
            margin-bottom: clamp(40px, 6vw, 80px);
            line-height: 0.95;
            animation: slideDown 0.6s ease-out 0.2s both;
        }

        @keyframes slideDown {
            from {
                opacity: 0;
                transform: translateY(-30px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }

        .geometric-illustration {
            flex: 1;
            display: flex;
            justify-content: center;
            align-items: center;
            margin-bottom: clamp(40px, 5vw, 60px);
            position: relative;
        }

        .illustration-frame {
            position: relative;
            width: 100%;
            max-width: 380px;
            aspect-ratio: 1;
            background: #e8e2db;
            padding: 30px;
            animation: scaleIn 0.7s ease-out 0.4s both;
        }

        @keyframes scaleIn {
            from {
                opacity: 0;
                transform: scale(0.9);
            }
            to {
                opacity: 1;
                transform: scale(1);
            }
        }

        .shapes-container {
            position: relative;
            width: 100%;
            height: 100%;
        }

        .shape {
            position: absolute;
            transition: transform 0.3s ease;
        }

        .shape-1 {
            width: 100%;
            height: 45%;
            background: #c8434e;
            bottom: 0;
            left: 0;
            animation: slideUp 0.6s ease-out 0.6s both;
        }

        .shape-2 {
            width: 68.75%;
            height: 68.75%;
            border-radius: 50%;
            background: #f4ebe3;
            bottom: 0;
            right: 15%;
            animation: fadeInScale 0.6s ease-out 0.8s both;
        }

        @keyframes slideUp {
            from {
                opacity: 0;
                transform: translateY(30px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }

        @keyframes slideLeft {
            from {
                opacity: 0;
                transform: translateX(30px);
            }
            to {
                opacity: 1;
                transform: translateX(0);
            }
        }

        @keyframes fadeInScale {
            from {
                opacity: 0;
                transform: scale(0.8);
            }
            to {
                opacity: 1;
                transform: scale(1);
            }
        }

        .poster-footer {
            display: grid;
            grid-template-columns: 1fr auto auto;
            gap: 30px;
            padding-top: 20px;
            border-top: 1px solid #2a2420;
            font-size: clamp(0.7rem, 1.5vw, 0.85rem);
            color: #2a2420;
            animation: fadeIn 0.6s ease-out 1s both;
        }

        .footer-section {
            display: flex;
            flex-direction: column;
            gap: 4px;
        }

        .footer-label {
            font-weight: 600;
            letter-spacing: 0.05em;
            font-size: 0.7em;
            opacity: 0.7;
        }

        .footer-content {
            font-weight: 300;
            line-height: 1.5;
        }

        .poster-number {
            font-family: 'DM Serif Display', serif;
            font-size: clamp(1.8rem, 4vw, 2.5rem);
            font-weight: 400;
            align-self: flex-end;
            font-style: italic;
            letter-spacing: -0.02em;
        }

        .poster-container:hover .shape {
            transform: scale(1.02);
        }

        .footer-label a:hover {
            opacity: 1;
        }

        @media (max-width: 600px) {
            .poster-container {
                padding: 40px 30px;
            }

            .poster-footer {
                grid-template-columns: 1fr;
                gap: 20px;
            }

            .poster-number {
                align-self: flex-start;
                margin-top: 10px;
            }
        }
    </style>
</head>
<body>
    <div class="poster-container">
        <h1 class="poster-title">Negroni</h1>
        
        <div class="geometric-illustration">
            <div class="illustration-frame">
                <div class="shapes-container">
                    <div class="shape shape-1"></div>
                    <div class="shape shape-2"></div>
                </div>
            </div>
        </div>

        <div class="poster-footer">
            <div class="footer-section">
                <div class="footer-label"><a href="https://github.com/ZelkaT/negroni/" target="_blank" style="color: #2a2420; text-decoration: none; transition: opacity 0.2s;">https://github.com/ZelkaT/negroni/</a></div>
            </div>
            
            <div class="footer-section">
                <div class="footer-label">Recipe</div>
                <div class="footer-content">
                    1⅓ ounces Gin<br>
                    1⅓ ounces Campari<br>
                    1⅓ ounces Sweet Vermouth
                </div>
            </div>

            <div class="footer-section">
                <div class="footer-label">Garnish</div>
                <div class="footer-content">
                    1 orange twist
                </div>
            </div>

            <div class="poster-number">/93</div>
        </div>
    </div>
</body>
</html>
)rawliteral";

// Handle all requests by serving the HTML page
void handleRoot() {
  webServer.send(200, "text/html", index_html);
}

// Redirect all other requests to root (captive portal behavior)
void handleNotFound() {
  webServer.send(200, "text/html", index_html);
}

void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\nStarting Negroni Captive Portal...");

#ifdef USE_ROUND_LCD
  displayInit();  // draw the poster on the round screen
#endif
  
  // Set up Access Point (no password)
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  
  // Print AP IP address
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  Serial.print("SSID: ");
  Serial.println(ssid);
  
  // Start DNS server to redirect all domains to ESP32 IP
  dnsServer.start(DNS_PORT, "*", IP);
  
  // Set up web server routes
  webServer.on("/", handleRoot);
  webServer.on("/generate_204", handleRoot);  // Android captive portal check
  webServer.on("/fwlink", handleRoot);         // Microsoft captive portal check
  webServer.onNotFound(handleNotFound);        // Catch all other requests
  
  // Start web server
  webServer.begin();
  
  Serial.println("Captive portal started!");
  Serial.println("Connect to WiFi and you'll be redirected automatically.");
}

void loop() {
  // Process DNS requests
  dnsServer.processNextRequest();
  
  // Handle web server requests
  webServer.handleClient();
}
