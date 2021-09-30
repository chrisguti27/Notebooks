import ssl
import random
import json
import time
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
from time import sleep
from datetime import date, datetime
from faker import Faker

CLIENT_NAME = "sensor-pruebas"
TOPIC = "iot-sensor/test"
fake = Faker()
device = fake.name()

BROKER_PATH = "a1nhczs7u29wa9-ats.iot.us-east-1.amazonaws.com"
ROOT_CA_PATH = './AmazonRootCA1.pem'

PRIVATE_KEY_PATH = './f2a35ec754-private.pem.key'
CERTIFICATE_PATH = './f2a35ec754-certificate.pem.crt'

# Create and Configure the IoT Client
IoTclient = AWSIoTMQTTClient(CLIENT_NAME)
IoTclient.configureEndpoint(BROKER_PATH, 8883)
IoTclient.configureCredentials(ROOT_CA_PATH, PRIVATE_KEY_PATH, CERTIFICATE_PATH)

# Allow the device to queue infinite messages
IoTclient.configureOfflinePublishQueueing(-1)
# Number of messages to send after a connection returns
IoTclient.configureDrainingFrequency(2)  # 2 requests/second
# How long to wait for a [dis]connection to complete (in seconds)
IoTclient.configureConnectDisconnectTimeout(10)
# How long to wait for publish/[un]subscribe (in seconds)
IoTclient.configureMQTTOperationTimeout(5) 

IoTclient.connect()
IoTclient.publish(TOPIC, "connected", 0)

# Create and Send Payloads to the IoT Topic
def create_payload():
	
	payload = json.dumps({
		"id" : int(time.time()),
		"envasadora" : device,
		"paquetes por minuto" : int(random.uniform(10, 90)),
		"temperatura sellos" : random.uniform(100 ,200),
		
	})
	print(payload)
	return payload

while True:
	IoTclient.publish(TOPIC, create_payload(), 0)
	time.sleep(5)

