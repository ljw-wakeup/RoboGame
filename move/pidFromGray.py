#增量式PID
lastinput = 0
lastlastinput = 0
lastoutput = 0
output = 0
kp1 = 0
ki1 = 0
kd1 = 0
kp2 = 0
ki2 = 0
kd2 = 0	
def pidFromGray(input): 
	parament = 20
	incre = kp1 * (input - lastinput) + ki1 * input + kd1 * (input - 2 * lastinput + lastlastinput )
	output = lastoutput + incre
	lastoutput = output
	lastlastinput = lastinput
	lastinput = input
	return -output
