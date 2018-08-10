
def caculGrayForPid(inputlist):
	blackcount = 0
	curblacklen = 0
	curblackstart = 0
	blackdict = {}
	curnumber = 0
	parament = 1

	for i in inputlist: 
		#print('i=',i)
		curnumber += 1
		#print('curnumber=', curnumber)
		if i == 0: 
			curblackstart = 0
			curblacklen = 0
		else :
			curblacklen += 1
			if curblacklen == 1 :
				blackcount += 1
				curblackstart = curnumber
			blackdict[str(curblackstart)] = curblacklen
			#print(blackdict)
	print(blackdict)
	dictlen = len(blackdict)
	blacklist = list(blackdict.items())
	lenth = len(blacklist)
	if lenth == 0:
		status = False
		output = 0
		return status, output
	elif lenth == 1:
		list1 = (list)(blacklist[0])
		
		output = ((2*int(list1[0])+list1[1]-1)/2-4.5)* parament
		status = True
		return status, output
	else :
		temp = 0
		maxcount = 0
		maxpos = 0
		j = -1
		for blacklistItem in blacklist:
			j += 1
			a = list(blacklistItem)[1]
			if a > temp :
				maxcount = 1
				temp = a
				maxpos = j
			elif a == temp :
				maxcount += 1
		if maxcount != 1 :
			status = False
			output = 0
			return status, output
		else :
			b = list(blacklist[maxpos])
			output = (( 2 * int(b[0]) + b[1] - 1 )/2 - 4.5) * parament
			status = True
			return status, output
			
		
inputlist = [1,1,1,0,1,1,1,1]
print(caculGrayForPid(inputlist))

				
				
			
			
			
			
	