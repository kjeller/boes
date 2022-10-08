#include <stdio.h>
#include <string>
#include "pico/stdlib.h"
#include "timer.h"

Timer::Timer() {
  state = Init;
  start_timestamp = 0;
  pause_timestamp = 0;
  time_diff = 0;
}

void Timer::start() {
  printf("Start triggered\n");

  // Handles case when going from stop to start when continuing timer.
  // Time diff is checked to not allow transition reset to start.
  if (this->state == Pause && this->time_diff != 0) {
    this->start_timestamp += time_us_64() - this->pause_timestamp;
  }

  this->state = Run;
}

void Timer::reset() {
  printf("Reset triggered\n");

  this->state = Reset;
}

void Timer::stop() {
  printf("Stop triggered\n");

  // Ignore setting pause timestamp when timer goes transitioning reset -> pause.
  // Otherwise the timer thinks that the timestamp needs to be adjusted when 
  // transitioning pause -> start.
  if (this->state == Reset || this->state == Init) {
    return;
  }
  else {
    this->pause_timestamp = time_us_64();
  }
  
  this->state = Pause;
}

void Timer::run() {
  switch (this->state) {
    case Run:
      this->time_diff = time_us_64() - this->start_timestamp;
      break;

    case Pause:
      // TODO Create constant for timeout1 state
      if ((time_us_64() - this->pause_timestamp) / _1_S_IN_US >= 30) {
        this->state = Timeout_1;
        this->timeout_timestamp = time_us_64();
      }
      break;

    case Timeout_1:
      // TODO Create constant for timeout2 state
      if ((time_us_64() - this->pause_timestamp) / _1_S_IN_US >= 60) {
        this->state = Timeout_2;
        this->timeout_timestamp = time_us_64();
      }
      break;

    case Timeout_2:
      // Nothing to do in this state
      break;

    case Reset:
      this->start_timestamp = time_us_64();
      this->pause_timestamp = 0;
      this->time_diff = 0;
      break;

    default:
      // Fixes issue when start time initially is not set
      this->start_timestamp = time_us_64();
      break;
  }

  // Update counter strings
  sprintf(this->sec_counter, "%003lld\0", this->time_diff / _1_S_IN_US);
  sprintf(this->ms_counter, "%003lld\0", (this->time_diff % _1_S_IN_US) / _1_MS_IN_US);
}
