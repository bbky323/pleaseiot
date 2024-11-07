/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * Additions Copyright 2016 Espressif Systems (Shanghai) PTE LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

/**
 * @file aws_iot_certifcates.c
 * @brief File to store the AWS certificates in the form of arrays
 */

#ifdef __cplusplus
extern "C" {
#endif

const char aws_root_ca_pem[] = {"-----BEGIN CERTIFICATE-----\n\
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n\
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n\
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n\
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n\
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n\
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n\
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n\
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n\
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n\
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n\
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n\
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n\
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n\
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n\
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n\
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n\
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n\
rqXRfboQnoZsG4q5WTP468SQvvG5\n\
-----END CERTIFICATE-----\n"};

const char certificate_pem_crt[] = {"-----BEGIN CERTIFICATE-----\n\
MIIDWTCCAkGgAwIBAgIUae9+S3uV/nPBKKFQlo+v1NvC7ZkwDQYJKoZIhvcNAQEL\n\
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n\
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI0MTAzMTA2NTEz\n\
N1oXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n\
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMXarTesNw3N5zSRL/Th\n\
dpo5WxyV4UEB3jBYBix8VhRJKdq2zWFW6rskep9u4iByHqUyDpULbgcbtphJHaih\n\
oqto24UE1XQnqJeuZzdA88iKJoeqh6AI73sKeIUpaae8569FLigO0FqzD6ql07cB\n\
qPJam1OWzuGvqOpE+f817Xa9QObJaPu9b2xsj/lYn0NdOx/zFmEHVqK2FBm2Bbul\n\
AZ/74BSBWGE8GqRuVxldNEAy9QCwQeOqhHYV1oEcxbs5zsJHEma/USa9/7TnRBoP\n\
qXNxTPpV2I33YrJ0yB8dSg9Kpw/J5q1rW7bnt0c7ad/cPmcWO6KvGRZWVIVb36BF\n\
X80CAwEAAaNgMF4wHwYDVR0jBBgwFoAUZWi6ovbmgSQw6WYw/lTH8ZEoWaswHQYD\n\
VR0OBBYEFHzamu2jk6877rERpxXFo+9BTNidMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n\
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQDBtGO6K/tsGkeDm3ui5o7xpbvc\n\
gxGsjjkZGfZdbymOOkqp/n4gc7mNvHqUbBUEH56d1tceLzDz4N3twfYlOKhmPxsN\n\
xVl+gtHVK576D4jrwLt/61uZ7AmyWWJqGsjusRMW1s9UJBQtJ9r1BYkyt1QtbtvH\n\
diq1m5HJIt2P0fHd9GuOZ0H+4weTt7+J14JMnqX16rnWcdiaP8Q/wn03aAvcJHvK\n\
9rKLR+8v+/yD/dTp3F/ZGFmY7pxWTV1LkaHpYG5gF9I+pBc0ijOu4pxVQLvMimkW\n\
THpsu0fJOabHcfMnR01NbTKi+20Sak+UQnlVFH/T4MdfmF/PpD/T4RLcSYR5\n\
-----END CERTIFICATE-----\n"};

const char private_pem_key[] = {"-----BEGIN RSA PRIVATE KEY-----\n\
MIIEowIBAAKCAQEAxdqtN6w3Dc3nNJEv9OF2mjlbHJXhQQHeMFgGLHxWFEkp2rbN\n\
YVbquyR6n27iIHIepTIOlQtuBxu2mEkdqKGiq2jbhQTVdCeol65nN0DzyIomh6qH\n\
oAjvewp4hSlpp7znr0UuKA7QWrMPqqXTtwGo8lqbU5bO4a+o6kT5/zXtdr1A5slo\n\
+71vbGyP+VifQ107H/MWYQdWorYUGbYFu6UBn/vgFIFYYTwapG5XGV00QDL1ALBB\n\
46qEdhXWgRzFuznOwkcSZr9RJr3/tOdEGg+pc3FM+lXYjfdisnTIHx1KD0qnD8nm\n\
rWtbtue3Rztp39w+ZxY7oq8ZFlZUhVvfoEVfzQIDAQABAoIBAGij9/q6zT8yx/Yo\n\
V2m3GMgGT6a1qjV8bGu7EkDS3Z/K9iTji4nQs0KYz3Wk6lloocaSy92N9voxcQfn\n\
m5UFL4hiY79dSBsvol3ecl5prsVAM5KK4UIFl8rGRel/do6tPQeYmgm7k/x0OXPy\n\
Uh+29AiBXSb/WplIExkbs/CDL/0cuh5j2g1kO6u7/9niiBjyJIACDbqg9n2J3qfF\n\
sb/S+gEozLOfH1GZMaa5nJEGprEaluTU4qMNdCmIynp7N4DTwKdiINA+0KyzkzUf\n\
vHITvUWMXuP9u/ERpJMREjAepWxu6FIxKG64GsGoRHXMmnbQcOEpNw4rLPVyFYcI\n\
3MZqbIECgYEA8aWRIAKe7E40nHCdD61SDZS5r5esvMjm3Rl8drieMUmhlhJ0HLy0\n\
fwyRaEkDWnOUhfk6fz7hWutpf6RAWnVRr9FerJ0xOUiLU/Y+9e8wlsHLA1Pygy0n\n\
WwyF4N7I2RefK7CyuWGWtz6MassBLvG3DqqMxseOeN2qsbi2NGuWhW0CgYEA0Zs1\n\
fWzpeWBaiOflap3VfLBv8p/qEdg2IKHB8P7mZC2mn2KQQ+q4hbHhnubl9kBUmuPz\n\
Wk/gsSF2Zopzxt1FFw1O0p8+kheVfIlrvMWDraOVC8Lv3xaaWjOgHet0BOAXhCM/\n\
P2/LrhDrAuCueyljPK3rcUzw9tZ0Fl1PdJmwp+ECgYB88ZyuLmy5xl4RnXC4UGei\n\
ugmgrw4TttUX8TuXrzGsTmQMUpJS4Yd6QadVgpsHQ8fB1fHXJU4kUqCD7QuROiP4\n\
gosvOT/gXJAd467+PqSi7qTnnZpBFTHKNBltAgRbq4kTcBOZIsppCSBVf42nFjgO\n\
j0Sm6zvRZtwBdH5RcGetqQKBgQDOUYWEUTk7hTF4gzKaEIQ8uPtoiin/qKAW3MIn\n\
MvCa+SiCyKUShS9eQirAm60OCGJ/GBPI+MlF8Zc29d9ihxjjyPCmd6NCcD5vmGDZ\n\
Ll4gA93I9vff0sLaOOjJqqLHzcC4C23NRoDpdLOvYHO+vY12BAe1vbOOdKdOAEke\n\
j82IYQKBgCNkD3ZSTYpzasVmiDQEY/PVzLC8nfh1ZUV9KoDZuDsbkfz9HulhpVul\n\
FNBVGJZWozUzA4RWamOCTxcSHfqc/21WTOU+ZL12QkVZtaPgjcVAb3LzkczSgbjv\n\
CftkLQVf/OgZELXSiEXYkHWV2LoIJgz5QvsWqnbaXUVUAvlLgo3z\n\
-----END RSA PRIVATE KEY-----\n"};


#ifdef __cplusplus
}
#endif
