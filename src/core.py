import time

_1_S_IN_NS = 1000000000
_1_MS_IN_NS = _1_S_IN_NS / 1000

class State():
  Pause = 1
  Run = 2

class Timer:
  state = None
  start_timestamp = 0
  pause_timestamp = 0
  time_diff = 0

  # Start/Resume clock
  def start(self):
    print("Start triggered")
    if self.state == State.Pause:
      self.start_timestamp += time.monotonic_ns() - self.pause_timestamp

    self.state = State.Run

  def reset(self):
    print("Reset triggered")
    self.start_timestamp = time.monotonic_ns() 
    self.pause_timestamp = 0

  def stop(self):
    print("Stop triggered")
    self.state = State.Pause
    self.pause_timestamp = time.monotonic_ns()

  def get_seconds(self):
    sec = self.time_diff / _1_S_IN_NS
    return "{:03d}".format(int(sec))
  
  def get_milliseconds(self):
    milli_left = (self.time_diff % _1_S_IN_NS) / _1_MS_IN_NS
    return "{:03d}".format(int(milli_left))

  def run(self):
    if self.state == State.Run:
      self.time_diff = time.monotonic_ns() - self.start_timestamp

    elif self.state == State.Pause:
      # Nothing to do in this state
      return

    else:
      # Fixes issue when start time initially is not set
      self.start_timestamp = time.monotonic_ns()
      return
