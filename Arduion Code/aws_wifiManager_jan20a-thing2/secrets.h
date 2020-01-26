#include <pgmspace.h>

#define SECRET
#define THINGNAME "ESP32Thing2"

//const char WIFI_SSID[] = "ATTisJR9MS";
//const char WIFI_PASSWORD[] = "ajgk9wmkadsv000";
const char AWS_IOT_ENDPOINT[] = "afa25mgzomlzi-ats.iot.us-east-2.amazonaws.com";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUF3BJEqhHN5CQaDXhvbQgxWpLUXkwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIwMDEyNjE2NTMz
M1oXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALpSiRLWlWPGeukkxilH
fdcpaoKYOzeRtaVPewHP8ediKmxAfji8sMvkf5mmEc48+h+aUKwAV3owWUGFXLtk
ejHVq1GJHLYC8w7muhLNIAuP+j2Ur7KAPPtj39fGlRiL03r5MMdrL1IGiOx5n8ea
3EyIStOMkfjpLjHTjCAnIhHSeU8YI4nz1fI+hyQLBdJ9zqMLWkvu720w8lmelwuG
8UcTWQk52RHx8YaApokyskWmF/mLswgWRSsdadnp8phEvLrcFYqUV31Zoz5drSzw
YyL4cv90NVfBnT+eK0vyVV8Bs0lsMic4XR5t4UmzPAIY/WGZSuK/Lujm1CCGFQ/r
nE8CAwEAAaNgMF4wHwYDVR0jBBgwFoAUau/ElADfl+y+8Z/vCeC7hEY6IpkwHQYD
VR0OBBYEFOhEiLD3oQTOKTzu/Jxgp83IV1XVMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQDMAZis3uFXGTPctNLA4yzxbpaO
IreqBsBvo0vyVILMP6Sih8v6C7Ifc6BMOnzZpYrmOTSSN4GlY5a9iC19bm0WWpMo
ILiSD2CBxRqYq22cYMRUwJ1vlXXVm//1BCMgTtLxfI1MIj6bgBXjAksH9m9JrMay
iS4QVbhasJw9cjhzlnfa8r2bCj4lnsplKwOvtfhLRbBEJXRRn7yWbaxnzPncEovX
GdEb8iO1bkhrF2W2plToRgTSmJvaetKhMWsvKZWtf6Vxh0eULqzG+d6455kUJb6b
tZj+78BTI/Bq+bxGxjeBlihNp5biRdpJ4tZx26Wh96sghBeSHS+U2qUqFSif
-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAulKJEtaVY8Z66STGKUd91ylqgpg7N5G1pU97Ac/x52IqbEB+
OLywy+R/maYRzjz6H5pQrABXejBZQYVcu2R6MdWrUYkctgLzDua6Es0gC4/6PZSv
soA8+2Pf18aVGIvTevkwx2svUgaI7Hmfx5rcTIhK04yR+OkuMdOMICciEdJ5Txgj
ifPV8j6HJAsF0n3OowtaS+7vbTDyWZ6XC4bxRxNZCTnZEfHxhoCmiTKyRaYX+Yuz
CBZFKx1p2enymES8utwVipRXfVmjPl2tLPBjIvhy/3Q1V8GdP54rS/JVXwGzSWwy
JzhdHm3hSbM8Ahj9YZlK4r8u6ObUIIYVD+ucTwIDAQABAoIBADZGor2FGuGEyPsv
hjYyKDtMSkWeuOlYHdT5ETkW7T3c/8SnCN3YQZwzhOQqV1d/tdEM7+Ea/GCQwv9u
LPQ9scbgmlZXHbD7rPgzSBMABAy6NUhJDuFtr9X/Yw6FP5t15/YTrxz4YUGAu9Q/
HcdUmkYFEd8MPN2Id7d2C4S2QZt092zev37mkHpSJl9ywd76pnaVjDTrx5O9RgRq
UzD3swBeXxOS8g7xTH77oypJt0+HcYRO0wbiJRPxi53wR2HIYmwPHoH2VS6fuvUe
CuLftDWSXK8jN9HONJUBb0pR+RCoQ33NClhVonRse5Rq2RFqyA97gYvnfi+NPHbJ
mf/dMWkCgYEA9EsoVqtsnzaRQEub6aMOL1iSjBwwzJnxKd3cZtluLBj3zpd/WPai
h2BbamxWdFud2QDU+sZkSLKUQRu1+oh+cxahlnxA+TdNqWrVGhUY/Tc9U3O/xJwW
e6ns5zVNMscy7JFLJMdqy9/C92qeI9fh1nlZ+kNuf+uJ/EPxOgf9XM0CgYEAw0A5
LAOawnzwYbye/bIarkeP2H35Q9vww7NPMaqz8eZVzIbcjU2XRSKXFELFPJHolev7
zQFGHvxQyqtMDE8fvUrLJmQ6kn2adP6GM3itNfpyZnmd5k6Rm5g2lC+7CoBL6U0n
m4Xb/EBih3zP6JI4ok+Zj2nPlufclN4fZ60zHYsCgYA2qyMJ0i7qqkFzC/iGHlEs
UzvoUt6wyrpnThjmgz3ebG/4MXr0I61Xcj7C4B+wxlfamg8PV5pVknEO6O6edDEn
/CaJEKFRpAijIx8xSQtgjx4QMNT3YJ6/adV0ID94X4+L1S/luSKzIQ32ZEujZcpK
PrneAX0W71TfuSygee0++QKBgQCVfVmIkmXuHHxrSGAueyukXdQLfD/Zrc6D94hd
zYWiYqk0Q9kfMtXrr9UeRRUAD/KUvq32QtBuLt6onUhHcFkrs9UgWsLhn9sfUjRr
uixb1Jm8TccQKv2etIatJHI/UwVL5a+6il1on98QPwZr2XAVDX7T575xmqovLHW9
CkXFiwKBgQDdbc2PmNn9IwvfjcdR5piMSR+SGtR9wQZ+fWAUTNOlS7xdtoLJg756
hlCJWOQAsmb9CWtFXBNQQTm6UjZ0ULKPkirEzVfIETmLHcGTzkEajH8Mm7C5Hz6h
O7A1fD5A//SSMzNAjR0zAhHm0w4KcSCy0LwGGX+tQOARU4s+6qkmMA==
-----END RSA PRIVATE KEY-----
)KEY";
