#!/usr/bin/env awk -f

{
    if ($5 == "C3")
        startTime[$6] = $1
    if ($5 == "R3") {
        latency = ($1 - startTime[$6]) * 1000
        if (latency <= 1)
            print latency
    }
}