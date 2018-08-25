import numpy as np
import cv2
import math
import copy
import time
mode = 0
cap = 0
pici = 0
def getpic():
	global pici
	frame = cv2.imread("originpic"+str(2)+".jpg")
	cv2.imshow("origin",frame)
	cv2.waitKey(0)
	cv2.destroyAllWindows()
	return frame
	
def detectline(img):
	cv2.startWindowThread()
#	cv2.namedWindow('input image',cv2.WINDOW_NORMAL)
	img = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    #resize
	resizeImg = cv2.resize(img,(320,240),interpolation = cv2.INTER_AREA)
#	cv2.imshow('input image', resizeImg)
#	cv2.waitKey(0)
#BINARY has three methods ,below is Otsu
#Gaussian
	blur = cv2.GaussianBlur(resizeImg,(11,11),0)
	ret, binary = cv2.threshold(blur,40,255,cv2.THRESH_BINARY)
#    binary = cv2.adaptiveThreshold(blur,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,11,2)
#	binary = cv2.GaussianBlur(binary,(11,11),0)
#	cv2.imshow('binary', binary)
#	cv2.waitKey(0)
	xgrad = cv2.Sobel(binary,cv2.CV_16SC1,1,0)
	ygrad = cv2.Sobel(binary,cv2.CV_16SC1,0,1)
	edge_output = cv2.Canny(xgrad,ygrad,50,150)
	cv2.imshow('canny',edge_output)
	cv2.waitKey(0)
	lines = cv2.HoughLinesP(edge_output,1, np.pi / 90, 40 ,minLineLength = 30, maxLineGap = 60)
	i = 0
	edge_output[:,:] = 0
	L = []
	for line in lines:
		i = i + 1
		x1,y1,x2,y2 = line[0]
		cv2.line(edge_output,(x1,y1),(x2,y2),(255,255,255),1,lineType = cv2.LINE_AA)
		cv2.imshow(str(i)+".jpg",edge_output)
		cv2.waitKey(0)
		print("line：",line)
		if ( x2 - x1 ) == 0 :
			k = 0
			b = x1
			theta = 90
		else :
			k = float(y2-y1)/float(x2-x1)
			b = y1-k*x1
			theta = math.atan(k)*180.0/(math.pi)	    
		L.append([x1,y1,x2,y2,k,b,theta])
#	print("originL", L)   
	return L,edge_output
	
def linesort(L,edge_output):
	Lsorted = sorted(L, key = lambda x:(x[6],x[4]))
	print("aftersorted", Lsorted)
	Lsorted_length = len(Lsorted)
	for line in Lsorted:
		edge_output[:,:] = 0
		cv2.line(edge_output,(line[0],line[1]),(line[2],line[3]),(255,255,255),1,lineType = cv2.LINE_AA)
		cv2.imshow("aftersorted"+".jpg",edge_output)
		cv2.waitKey(0)
		line.append(np.sqrt(np.square(line[0]-line[2]) + np.square(line[1]-line[3])))
	#(x1,y1,x2,y2,k,b,theta,length)
#	print("(x1,y1,x2,y2,k,b,theta,length)" , Lsorted)
	Lmerge_first = []
	i = 0
	while i < Lsorted_length:
		print("i",i)
		j = i + 1
		maxlong = 0
		while j < Lsorted_length:
			print("j",j) 
			print("abs(Lsorted[i][6] - Lsorted[j][6])", abs(Lsorted[i][6] - Lsorted[j][6]))
			print("distance", abs(Lsorted[i][5] - Lsorted[j][5])/math.sqrt(1+Lsorted[i][4] * Lsorted[j][4]))
			print("")
			standardy = (Lsorted[i][1]+Lsorted[i][3]+Lsorted[j][1]+Lsorted[j][3]
			if abs(Lsorted[i][6] - Lsorted[j][6]) < 5 :
				if abs(Lsorted[i][6]) >45 :
					refer_y = (Lsorted[i][1]+Lsorted[i][3]+Lsorted[j][1]+Lsorted[j][3]
					line_x_i = (refer_y - Lsorted[i][4])/Lsorted[i][5]
					line_x_j = (refer_y - Lsorted[j][4])/Lsorted[j][5]
					if abs(line_x_i - line_x_j) < 20 :
						if Lsorted[i][7] > maxlong :
							maxlong = Lsorted[i][7]
						j = j + 1
					else :
						break
						
				else :
					refer_x = (Lsorted[i][0]+Lsorted[i][2]+Lsorted[j][0]+Lsorted[j][2]
					line_y_i = refer_x * Lsorted[i][5] + Lsorted[i][4]
					line_y_i = refer_x * Lsorted[j][5] + Lsorted[j][4]
					if abs(line_y_i - line_y_i) < 20 :
						if Lsorted[i][7] > maxlong :
							maxlong = Lsorted[i][7]
						j = j + 1
					else :
						break :
			else :
				break
		print("maxlong",maxlong)
		x = i
		maxlinelist = []
		while x < j :
			if Lsorted[x][7] > 0.6 * maxlong :
				maxlinelist.append(x)
			x += 1
		maxlong = 0
		print("maxlinelist",maxlinelist)
		maxlinenum = len(maxlinelist)
		newline = [0,0,0,0,0,0,0,0]
		for item in maxlinelist :
			y = 0
			while y < 4 :
				newline[y] += Lsorted[item][y]
				y += 1
			y = 0
			while y < 4 :
				newline[y] = float(newline[y])/float(maxlinenum)
				y += 1
		edge_output[:,:] = 0
		cv2.line(edge_output,(int(newline[0]),int(newline[1])),(int(newline[2]),int(newline[3])),(255,255,255),1,lineType = cv2.LINE_AA)
		cv2.imshow("newline"+str(i)+".jpg",edge_output)
		cv2.waitKey(0)
		if newline[2]-newline[0] == 0:
			newline[4] = 0
			newline[5] = newline[0]
			newline[6] = 90
			newline[7] = np.sqrt(np.square(newline[3]-newline[1]))
		else :
			newline[4] = (newline[3]-newline[1])/(newline[2]-newline[0])
			newline[5] = newline[1] - newline[4] * newline[0]
			newline[6] = math.atan(newline[4])*180.0/(math.pi)
			newline[7] = np.sqrt(np.square(newline[0]-newline[2]) + np.square(newline[1] - newline[3]))    
			Lmerge_first.append(newline)
		i = j
	print("Lmerge_first",Lmerge_first)
	return Lmerge_first

	
def orbitmerge(Lmerge):
	normal = []
	considerated = []
	length = len(Lmerge)
	i = 0
	while i < length :
		j = i + 1
		while j < length :
			print("abs(Lmerge[i][6] - Lmerge[j][6]) < 5",abs(Lmerge[i][6] - Lmerge[j][6]) < 5)
			standardy = (Lmerge[i][1]+Lmerge[i][3]+Lmerge[j][1]+Lmerge[j][3]
			if abs(Lmerge[i][6] - Lmerge[j][6]) < 5 :
				if abs(Lmerge[i][6]) >45 :
					refer_y = (Lmerge[i][1]+Lmerge[i][3]+Lmerge[j][1]+Lmerge[j][3]
					line_x_i = (refer_y - Lmerge[i][4])/Lmerge[i][5]
					line_x_j = (refer_y - Lmerge[j][4])/Lmerge[j][5]
					print("distance:",abs(line_x_i - line_x_j))
					if abs(line_x_i - line_x_j) < 70 :
						if Lmerge[i][7] > maxlong :
							maxlong = Lmerge[i][7]
						j = j + 1
					else :
						break
						
				else :
					refer_x = (Lmerge[i][0]+Lmerge[i][2]+Lmerge[j][0]+Lmerge[j][2]
					line_y_i = refer_x * Lmerge[i][5] + Lmerge[i][4]
					line_y_i = refer_x * Lmerge[j][5] + Lmerge[j][4]
					if abs(line_y_i - line_y_i) < 20 :
						if Lmerge[i][7] > maxlong :
							maxlong = Lmerge[i][7]
						j = j + 1
					else :
						break :
			else :
				break
		if j - i == 1 :
			line = Lmerge[i][:]
			considerated.append(line)
		else :
			line = [0,0,0,0,0,0,0,0]
			x = i
			while x < j :
				line[0] += Lmerge[x][0]
				line[1] += Lmerge[x][1]
				line[2] += Lmerge[x][2]
				line[3] += Lmerge[x][3]
				x += 1
			line[0] /= j-i
			line[1] /= j-i
			line[2] /= j-i
			line[3] /= j-i
			if line[2]-line[0] == 0 :
				line[4] = 0
				line[5] = line[0]
				line[6] = 90
				line[7] = np.square(line[3]-line[1])
			else :
				line[4] = (line[3]-line[1])/(line[2]-line[0])
				line[5] = line[1] - line[4] * line[0]
				line[6] = math.atan(line[4])*180.0/(math.pi)
				line[7] = np.square(line[0] - line[2]) + np.square(line[1] - line[3]) 
			normal.append(line)
		i = j
	print("normal", normal)
	print("considerated", considerated)
	return normal, considerated
	
def calculateForMode(normal, considerated) :
	global mode
	verticle = 0
	verticlelist = []
	horizon = 0
	horizonlist = []
	for line in normal :
		if line[6] > 45 or line[6] < -45 :
			verticle += 1
			verticlelist.append(line)
		else :
			horizon += 1
			horizonlist.append(line)
	if mode == 0 :
		for line in verticlelist:
			if line[6] == 90 :
				line.append(line[0])
			else :
				line.append((240 - line[5])/line[4] - 80)
		verticlelist = sorted(verticlelist, key = lambda x:abs(x[8]))
#        print(verticlelist)
	return verticlelist[0][8]
	


img = getpic()
L,edge_output = detectline(img)
Lmerge = linesort(L,edge_output)
normal,considerated = orbitmerge(Lmerge)
print(calculateForMode(normal, considerated))
cv2.waitKey(0)
cv2.destroyAllWindows()


