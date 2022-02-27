import matplotlib.pyplot as plt
import numpy as np

values = []

f = open('3hits_ManyTimes_CymbalCrash_NoDelay.txt','r')
for row in f:
    try:
        rowAsNumber = float(row)
    except ValueError:
        print("Value error occured")
    values.append(rowAsNumber)

# Y axis parameter:
yaxis = np.array(values)

##### Filtering ######
param = 0.1
filteredValues = [None] * (len(values) + 1)

for i, value in enumerate(values, start=1):
    try:
        # filter equation
        filteredValues[i] = ((1 - param) * filteredValues[i-1]) + param * value
    except:
        print(i)
        filteredValues[i] = value

# Y axis parameter:
yaxisFiltered = np.array(filteredValues)


# plotting - unfiltered
plt.subplot(1, 2, 1)
plt.title("Unfiltered values")
plt.plot(yaxis)
plt.ylim([0, 800])

# plotting - filtered
plt.subplot(1, 2, 2)
plt.title("Values filtered by IIR")
plt.plot(yaxisFiltered)
plt.ylim([0, 800])
plt.show()
