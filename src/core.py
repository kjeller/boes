import time

_1_MS_IN_NS = 1000000
_1_S_IN_MS = 1000

class State():
  Pause = 1
  Run = 2

class Timer:
  state = State.Pause 
  millisecond_counter = 0
  prev_time = 0

  # Start/Resume clock
  def start(self):
    #print("Start triggered")
    self.state = State.Run

  def reset(self):
    #print("Reset triggered")
    self.millisecond_counter = 0

  def stop(self):
    #print("Stop triggered")
    self.state = State.Pause

  def get_seconds(self):
    sec = self.millisecond_counter / _1_S_IN_MS
    milli_left = self.millisecond_counter % _1_S_IN_MS
    return "{:03d}".format(int(sec))
  
  def get_milliseconds(self):
    milli_left = self.millisecond_counter % _1_S_IN_MS
    return "{:03d}".format(int(milli_left))

  def run(self):
    
    if self.state == State.Run:
      curr_time = time.monotonic_ns()

      if (curr_time - self.prev_time) > _1_MS_IN_NS:
        print(curr_time)
        self.millisecond_counter += 1
        prev_time = curr_time

    elif self.state == State.Pause:
      return

    else:
      print("UNEXPTECTED")
