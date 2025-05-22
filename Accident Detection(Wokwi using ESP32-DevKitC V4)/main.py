# Reference: https://qiita.com/kotaproj/items/3b974425dfa2579afdbc

from machine import Pin
import network
import time
import urequests as requests
import urandom as random
import os

# AP
WIFI_SSID = "Wokwi-GUEST"
WIFI_PASS = ""

# discord webhook url
discord_id = "1293767438089060493"
discord_token = "K35zLOGAprFdegbyl_18kc3UtzUM4rJaANp9nrLlAGtZjpDgnUeeoxxqa5I8tMB8EWAq"
URL = "https://discord.com/api/webhooks/" + discord_id + "/" + discord_token

button_down = False

def randint(min, max):
  span = max - min + 1
  div = 0x3FFFFFFF // span
  offset = random.getrandbits(30) // div
  val = min + offset
  return val

def connect_wifi():
  wifi = network.WLAN(network.STA_IF)
  wifi.active(True)
  if not wifi.isconnected():
    print('connecting to network...')
    wifi.connect(WIFI_SSID, WIFI_PASS)
    while not wifi.isconnected():
      time.sleep(1)
  print('network config:', wifi.ifconfig())

  return

# Function to handle button press interrupt (optional)
def button_press(pin):
  global button_down
  led2_pin.value(not led2_pin.value())
  button_down = True

def sendMessage(payload):
  connect_wifi()
  print(URL)
  response = requests.post(URL,
                           headers={"Content-Type": "application/x-www-form-urlencoded"},
                           data=payload)
  response.close()
  print("Send success!")

# Start Function
if __name__ == '__main__':
  print(os.uname())
  print("Hello, " + os.uname().sysname + "!")

  # Define GPIO pins for LED and push button
  led1_pin = Pin(4, Pin.OUT)
  led2_pin = Pin(2, Pin.OUT)
  led1_pin.value(False)
  led2_pin.value(False)
  button_pin = Pin(35, Pin.IN, Pin.PULL_UP)  # Internal pull-up resistor

  # Attach interrupt to the push button pin
  button_pin.irq(trigger=Pin.IRQ_FALLING, handler=button_press)

  payload = """content=This message is sent by "{:s}".""".format(os.uname().sysname)
  connect_wifi() # Connecting to WiFi Router
  print(URL)
  response = requests.post(URL,
                           headers={"Content-Type": "application/x-www-form-urlencoded"},
                           data=payload)
  response.close()
  print("Send success!")

  while True:
    led1_pin.value(not led1_pin.value())
    if button_down == True:
      temp=randint(1,50)
      humi=randint(1,100)
      Temp="Temp: %2d\u00b0C" % (temp)
      Humi="Humi: %2d%%" % (humi)
      print(Temp)
      print(Humi)
      payload = """content={:s}, {:s}.""".format(Temp,Humi)
      connect_wifi()
      print(URL)
      response = requests.post(URL,
                               headers={"Content-Type": "application/x-www-form-urlencoded"},
                               data=payload)
      response.close()
      print("Send success!")
      button_down = False

    # Do other tasks while waiting for button press
    time.sleep_ms(100)
