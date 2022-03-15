import matplotlib.pyplot as plt
import numpy as np
import os


##### Test Values ######
values = []

cwd = os.path.dirname(os.path.realpath(__file__))
test_file_name = '3hits_ManyTimes_CymbalCrash_NoDelay.txt'

f = open(os.path.join(cwd, test_file_name),'r')
for row in f:
    try:
        rowAsNumber = float(row)
    except ValueError:
        print("Value error occured")
    values.append(rowAsNumber)

##### Filtering ######
param = 0.1
idle = 338.0
filteredValues = [0.0] * (len(values) + 1)
absoluteValues = [0.0] * (len(values) + 1)

for i, value in enumerate(values, start=1):
    try:
        # filter equation
        filteredValues[i] = ((1 - param) * filteredValues[i-1]) + param * value
        absoluteValues[i] = abs(filteredValues[i] - idle)
    except:
        print(i)
        filteredValues[i] = abs(value)

# Y axis parameter:
yaxis = np.array(values)
# plotting - unfiltered
plt.subplot(1, 3, 1)
plt.title("Unfiltered values")
plt.plot(yaxis)
plt.ylim([250, 450])
plt.xlim([4100, 4500])

# Y axis parameter:
yaxisFiltered = np.array(filteredValues)
# plotting - filtered
plt.subplot(1, 3, 2)
plt.title("Values filtered by IIR")
plt.plot(yaxisFiltered)
plt.ylim([250, 450])
plt.xlim([4100, 4500])

# Y axis parameter:
yaxisAbsolute = np.array(absoluteValues)
# plotting - absolute filtered
plt.subplot(1, 3, 3)
plt.title("Absolute filtered values")
plt.plot(yaxisAbsolute)
plt.ylim([0, 40])
plt.xlim([4100, 4500])

plt.show()
