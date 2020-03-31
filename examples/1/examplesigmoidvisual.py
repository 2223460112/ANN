import sys
import matplotlib.pyplot as plt
import numpy as np

def beast_func(x):
    return -0.9 + 0.122592525544741 * np.sin(x) + 0.430783383039806 * np.sinh(x) - 0.56304347826087 * x * x + 1.530434782608697 * pow(2.0, -x) + 4.682744405392363 * np.tan(x / 8.0)

if(len(sys.argv)!=2):
    exit(1)

Ifile = open('1_sigmoidtest.csv', 'rb')

X=np.linspace(-4.0,3.2,10000,dtype=np.float32)
Y=np.zeros([10000],np.float32)
for i in range(20):
    YI=Ifile.readline().decode('utf-8').split(',')
    for j in range(10000):
        Y[j]=float(YI[j])
    plt.plot(X,Y)

Ifile.close()

for j in range(10000):
    Y[j]=beast_func(X[j])
plt.plot(X,Y)

plt.savefig('./1_sigmoid_'+sys.argv[1]+'.png')
