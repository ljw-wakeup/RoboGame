lastinput = 0
lastlastinput = 0
lastoutput = 0

kp1 = 1
ki1 = 0
kd1 = 0

def pidFromGray(input):
    global lastoutput
    global lastlastinput
    global lastinput

    incre = kp1 * (input - lastinput) + ki1 * input + kd1 * (input - 2 * lastinput + lastlastinput )
    output = lastoutput + incre
    lastoutput = output
    lastlastinput = lastinput
    lastinput = input
    return -output
