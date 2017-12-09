import serial
import random
import time

sd = serial.Serial("/dev/tty.usbmodem1411",115200)

t = 0
l = ["%03d"%(random.randint(0, 255)) for i in range(784)]
while (t < 5):
    print(t)
    # time.sleep(1)
    result = " ".join(l)
    result += " "
    # print("result: %s"%result)

    
    sd.write(bytearray(result, 'utf-8'))
    print("finished sending")
    # time.sleep(3)
    received = False
    while True:
        if received:
            break
        if sd.in_waiting:
            print(sd.in_waiting)
            result = sd.readline().decode("utf-8").strip()
            print("yo: %s"%result)

            received = True
    t+=1
