import pygame
from random import random
import numpy as np
from math import pi, sin, cos
import struct
import threading


red = pygame.Color(255, 0, 0, 0)
y = 500

def rand_real(a, b):
    if a == b:
        return a
    return random() * (b - a) + a


class ReadingThread(threading.Thread):
    def __init__(self, name, file, format_buf):
        threading.Thread.__init__(self)
        self.name = name
        self.file = file
        self.format_buf = format_buf
        self.buf_length = struct.calcsize(format_buf)
    
    def run(self, buffer):
        buffer[:] = struct.unpack(self.format_buf, self.file.read(self.buf_length))


class Circle(pygame.sprite.Sprite):
    def __init__(self, max_y, radius):
        pygame.sprite.Sprite.__init__(self)
        width = int(radius * y / max_y)
        foo = width * 2
        self.image = pygame.Surface((foo, foo))
        self.image.set_colorkey(pygame.Color(0,0,0))
        self.image.fill(pygame.Color(0,0,0))
        pygame.draw.circle(self.image, red, (width, width), width)
        self.rect = pygame.Rect(0, 0, foo, foo)


class Table:
    def __init__(self, filename):
        self.file = open(filename, 'rb')
        self.dims = struct.unpack("H", self.file.read(struct.calcsize("H")))[0]
        self.N = struct.unpack("L", self.file.read(struct.calcsize("L")))[0]
        self.shape = struct.unpack("d" * self.dims, self.file.read(struct.calcsize("d" * self.dims)))
        self.radius = struct.unpack("d", self.file.read(struct.calcsize("d")))[0]
        self.circles = pygame.sprite.Group(Circle(self.shape[1], self.radius) for i in range(self.N))
        self.buf_format = f"{self.N * self.dims}d"
        self.buf_size = struct.calcsize(self.buf_format)
        self.read_thread = ReadingThread('rdthrd', self.file, self.buf_format)
        self.x = int(y * self.shape[0] / self.shape[1])
        self.scale = y / self.shape[1]
        self.pause = True
        self.eof = False

    def restart(self):
        self.pause = True
        self.eof = False
        self.buffer = [0.0] * self.dims * self.N
        self.file.seek(struct.calcsize("d") + struct.calcsize("d" * self.dims) + struct.calcsize("L") + struct.calcsize("H"), 0)
        self.read_thread.run(self.buffer)
        self.update(force=True)
    
    def update(self, force=False):
        if self.pause and not force:
            return
        if self.read_thread.is_alive():
            self.read_thread.join()
        for c, i in enumerate(self.circles.sprites()):
            i.rect.center = (int(self.buffer[c] * self.scale), int(self.buffer[c + self.N] * self.scale))
        try:
            self.read_thread.run(self.buffer)
        except:
            self.eof = True
            self.play_pause()
    
    def play_pause(self):
        if self.pause:
            self.pause = False
        elif not self.eof:
            self.pause = True


table = Table(input("path to file: "))
clock = pygame.time.Clock()
pygame.display.init()
screen = pygame.display.set_mode(size=(table.x, y))
screen.set_colorkey(pygame.Color(0,0,0))


run = True
table.restart()
while run:
    for vnt in pygame.event.get():
        if vnt.type == pygame.QUIT:
            run = False
        if vnt.type == pygame.KEYDOWN:
            if vnt.key == pygame.K_r:
                table.restart()
            if vnt.key == pygame.K_SPACE:
                table.play_pause()
    table.update()
    screen.fill((0,0,0))
    table.circles.draw(screen)
    pygame.display.flip()
    clock.tick(60)