import sys
import matplotlib.pyplot as plt
import numpy as np

if(len(sys.argv)!=2):
    exit(1)

Ifile = open('mnistlossout.csv', 'rb')

X=np.linspace(1,3000,3000,dtype=np.float32)
Y=np.zeros([3000],np.float32)

YI=Ifile.readline().decode('utf-8').split(',')
for j in range(3000):
    Y[j]=float(YI[j])
plt.plot(X,Y)

plt.plot([1,3000],[float(YI[3000]),float(YI[3000])])

Ifile.close()

plt.savefig('./mnist_'+sys.argv[1]+'.png')
