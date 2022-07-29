import board
from digitalio import DigitalInOut, Direction, Pull
from core import Timer

def btn_start(pin, timer):
  timer.start()

def btn_stop(pin, timer):
  timer.stop()

def btn_reset(pin, timer):
  timer.reset()

timer = Timer()
start_btn = DigitalInOut(board.GP3)
stop_btn = DigitalInOut(board.GP4)
rst_btn = DigitalInOut(board.GP5)

start_btn.direction = Direction.INPUT
start_btn.pull = Pull.UP
stop_btn.direction = Direction.INPUT
stop_btn.pull = Pull.UP
rst_btn.direction = Direction.INPUT
rst_btn.pull = Pull.UP

while True:
  timer.run()

  if not start_btn.value:
    timer.start()
  elif not stop_btn.value:
    timer.stop()
  elif not rst_btn.value:
    timer.reset()

  # TODO Use these strings to print to display
  #print(timer.get_seconds() + ":" + timer.get_milliseconds())
