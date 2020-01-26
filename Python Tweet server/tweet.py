# import time #
# import RPi.GPIO as GPIO
import requests
from twython import TwythonStreamer

# Search terms
TERMS = '#testingtweet'

# GPIO pin number of LED
#LED = 22

# Twitter application authentication
APP_KEY = 'PQFdvHWCilc2pMxw5sd2anZHw'
APP_SECRET = '8wSS2wK6pWRFuR9uZeghNhE5fwUVjKnyBQXN723PttB8xI0WnI'
OAUTH_TOKEN = '609715386-RR3AKCnTLlDMVbrsulpMzRgcDDD4FNZTfeKKRCxn'
OAUTH_TOKEN_SECRET = 'nBBRHsz2ZnHh4sABtQeOQ7FNWs5KIgXkH9ssIueii4HBJ'

# Setup callbacks from Twython Streamer class 
class BlinkyStreamer(TwythonStreamer):
        def on_success(self, data):
                if 'text' in data:
                        msg = (data['text'].encode('utf-8'))
                        msg2 = data['text']
                        print (msg)
                        payload = {'sensor':msg2}
                        requests.post('https://t9phi67w2b.execute-api.us-east-2.amazonaws.com/Prod/publish',json=payload)
#                        print
 #                       GPIO.output(LED, GPIO.HIGH)
 #                       time.sleep(0.5)
 #                       GPIO.output(LED, GPIO.LOW)

# Setup GPIO as output
#GPIO.setmode(GPIO.BOARD)
#GPIO.setup(LED, GPIO.OUT)
#GPIO.output(LED, GPIO.LOW)

# Create streamer
try:
        stream = BlinkyStreamer(APP_KEY, APP_SECRET, OAUTH_TOKEN, OAUTH_TOKEN_SECRET)
        stream.statuses.filter(track=TERMS)
except KeyboardInterrupt:
        print("Interrupt")
 #       GPIO.cleanup()
