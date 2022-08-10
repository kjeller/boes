#include <stdio.h>
#include "pico/stdlib.h"
#include "timer.h"

void Timer::start()
{
    printf("Start triggered\n");

    if (this->state == Pause)
    {
        this->start_timestamp += time_us_64() - this->pause_timestamp;
    }

    this->state = Run;
}

void Timer::stop()
{
    printf("Reset triggered\n");

    this->start_timestamp = time_us_64();
    this->pause_timestamp = 0;
}

void Timer::reset()
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
            return;

        default:
            // Fixes issue when start time initially is not set
            this->start_timestamp = time_us_64();
            return;
    }

    // Update counter strings
    sprintf(this->sec_counter, "%003lld\0", this->time_diff / _1_S_IN_US);
    sprintf(this->ms_counter, "%003lld\0", (this->time_diff % _1_S_IN_US) / _1_MS_IN_US);
}
