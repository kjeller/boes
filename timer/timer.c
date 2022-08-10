#include <stdio.h>
#include "pico/stdlib.h"
#include "timer.h"

void start(Timer *self)
{
    printf("Start triggered\n");

    if (self->state == Pause)
    {
        self->start_timestamp += time_us_64() - self->pause_timestamp;
    }

    self->state = Run;
}

void stop(Timer *self)
{
    printf("Reset triggered\n");

    self->start_timestamp = time_us_64();
    self->pause_timestamp = 0;
}

void reset(Timer *self)
{
    printf("Stop triggered\n");

    self->state = Pause;
    self->pause_timestamp = time_us_64();
}

void run(Timer *self)
{
    switch (self->state)
    {
        case Run:
            self->time_diff = time_us_64() - self->start_timestamp;
            break;

        case Pause:
            // Nothing to do in this state
            return;

        default:
            // Fixes issue when start time initially is not set
            self->start_timestamp = time_us_64();
            return;
    }

    // Update counter strings
    sprintf(self->sec_counter, "%3lld", self->time_diff / _1_S_IN_US);
    sprintf(self->ms_counter, "%3lld", (self->time_diff % _1_S_IN_US) / _1_MS_IN_US);
}
