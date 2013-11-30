#!/usr/bin/python
import serial
import time
import math
import sys
import struct
import pyglet
import threading

def hsv2rgb(h, s, v):
    h = float(h)
    s = float(s)
    v = float(v)
    h60 = h / 60.0
    h60f = math.floor(h60)
    hi = int(h60f) % 6
    f = h60 - h60f
    p = v * (1 - s)
    q = v * (1 - f * s)
    t = v * (1 - (1 - f) * s)
    r, g, b = 0, 0, 0
    if hi == 0: r, g, b = v, t, p
    elif hi == 1: r, g, b = q, v, p
    elif hi == 2: r, g, b = p, v, t
    elif hi == 3: r, g, b = p, q, v
    elif hi == 4: r, g, b = t, p, v
    elif hi == 5: r, g, b = v, p, q
    r, g, b = int(r * 255), int(g * 255), int(b * 255)
    return g, r, b

def writeLeds(ser,loc,data):
    dataLength = data.__len__() + 1

    out = bytearray();
    for b in struct.pack("!H",dataLength):
        out.append(b)
    out.append(loc)
    for c in data:
        out.append(c)
    ser.write(out);

def getImageCoordinates(sprite,x,y):
    dx = x - sprite.x
    dy = y - sprite.y

    if (dy > sprite.height): dy = sprite.height;
    if (dx > sprite.width): dx = sprite.width;
    if (dx < 0): dx = 0;
    if (dy < 0): dy = 0;

    ix = int(dx/sprite.scale)
    iy = int(dy/sprite.scale)
    return ix,iy

def main():
    image = "Pikachu.png"
    port = "/dev/ttyACM0"
    baudrate = 115200

    if (sys.argv.__len__() >= 2): image = sys.argv[1]
    if (sys.argv.__len__() >= 3): port = sys.argv[2]
    if (sys.argv.__len__() >= 4): baudrate = sys.argv[3]

    ser = serial.Serial(
        port=port,
        baudrate=baudrate
    )
    
    window = pyglet.window.Window()

    offsetX = 50
    offsetY = 50

    f = open(image, 'rb')
    img = pyglet.image.load(image, file=f)
    imgdata = img.get_data('RGB',img.width*3)
    scalex = float(window.width)/float(img.width)
    scaley = float(window.height)/float(img.height)
    scale = min(scalex,scaley);
    sprite = pyglet.sprite.Sprite(img)
    sprite.scale = scale;

    global leds
    leds = []

    def getPixel(image,data,x,y):
        flen = 3;
        index = y*image.width*flen + x*flen
        if (index >= data.__len__()): return 0,0,0
        return ord(data[index]), ord(data[index+1]), ord(data[index+2])

    global mousex,mousey
    mousex = 100;
    mousey = 100;

    def handleSerial():
        global leds
        try:
            while(True):
                writeLeds(ser,0,leds);
                ser.read(1)
        except:
            ser.close()
            raise

    t = threading.Thread(target=handleSerial)
    t.daemon = True
    t.start()


    @window.event
    def on_draw():
        global mousex,mousey
        window.clear()
        sprite.draw()
        pyglet.gl.glColor4f(0.0,0,1,1.0)
        pyglet.graphics.draw(2, pyglet.gl.GL_LINES,
            ('v2i', (0,mousey,sprite.width,mousey))
        )

    @window.event
    def on_mouse_motion(x,y,dx,dy):
        global mousex,mousey,leds
        mousex = x;
        mousey = y;

        newleds = []
        for i in range(125):
            w = int(i*(sprite.width/125.0))
            ix,iy = getImageCoordinates(sprite,w,y);
            r,g,b = getPixel(img,imgdata,ix,iy)
            newleds += [r/16,g/16,b/16]
        leds = newleds;
        

    pyglet.app.run()

    while(True):
        pass



main()
