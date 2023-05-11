import turtle
import serial

turtle.mode('logo')
turtle.speed(10)
turtle.shape('turtle')

s = serial.Serial('COM4')


while True:
    res = s.read()
    print(bytes.hex(res))
    if bytes.hex(res) == 60:
        print("Straight")
    elif bytes.hex(res) == 20:
        print("Right")
    elif bytes.hex(res) == 40:
        print("Left")
    elif bytes.hex(res) == 00:
        print("Stop")
        break
    elif bytes.hex(res) == 80:
        print("180")

    if bytes.hex(res) == 60:
        turtle.forward(10)
    elif bytes.hex(res) == 20:
        turtle.right(90)
        turtle.forward(10)
    elif bytes.hex(res) == 40:
        turtle.left(90)
        turtle.forward(10)
    elif bytes.hex(res) == 00:
        break
    elif bytes.hex(res) == 80:
        turtle.right(180)
        turtle.forward(10)

"""
straight 	011 00000 = hex 60
right      	001 00000 = hex 20
left		010 00000 = hex 40
stop	 	000 00000 = hex 00
turn 180	100 00000 = hex 80
"""