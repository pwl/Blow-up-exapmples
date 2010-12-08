#!/usr/bin/igawk -f


# the abs function definition
function abs(x)
{
    return (((x < 0.0) ? -x : x) + 0.0)
}
