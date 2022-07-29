from threading import Thread
from code import Timer
import sys

def handle_input():
  while True:
    i = sys.stdin.readline()

    if i == 'r':
      timer.restart()
    elif i == 's':
      timer.start()
    elif i == 'x':
      timer.stop()

timer = Timer()
t1 = Thread(target=handle_input()) 
t2 = Thread(target=timer.run())

t1.start()
t2.start()

t1.join()
t2.join()
