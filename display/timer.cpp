#include <stdio.h>
#include "pico/stdlib.h"
#include "timer.h"

void Timer::start()
{
    printf("Start triggered\n");

    // Handles case when going from stop to start when continuing timer.
    // Time diff is checked to not allow transition reset to start.
    if (this->state == Pause && this->time_diff != 0)
    {
        this->start_timestamp += time_us_64() - this->pause_timestamp;
    }

    this->state = Run;
}

void Timer::reset()
{
    printf("Reset triggered\n");

    this->start_timestamp = time_us_64();
    this->pause_timestamp = 0;
    this->time_diff = 0;
}

void Timer::stop()
{
    printf("Stop triggered\n");

    this->state = Pause;
    this->pause_timestamp = time_us_64();
}

void Timer::run()
{
    switch (this->state)
    {
        case Run:
            this->time_diff = time_us_64() - this->start_timestamp;
            break;

        case Pause:
            // Nothing to do in this state
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
