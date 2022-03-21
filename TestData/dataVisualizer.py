import matplotlib.pyplot as plt
import numpy as np
import os


# CONFIGURABLE VARIABLES
test_file_name = '2HitsTests_FourTimes_CymbalCrash_NoDelay.txt'
x1_marker = 0
x2_marker = 30000
y_lower = 0
y_higher = 200
param = 0.2
idle = 338.0

##### Test Values ######
values = []
cwd = os.path.dirname(os.path.realpath(__file__))

f = open(os.path.join(cwd, test_file_name),'r')
for row in f:
    try:
        rowAsNumber = float(row)
    except ValueError:
        print("Value error occured")
    values.append(rowAsNumber)

##### Filtering ######
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
plt.xlim([x1_marker, x2_marker])

# Y axis parameter:
yaxisFiltered = np.array(filteredValues)
# plotting - filtered
plt.subplot(1, 3, 2)
plt.title("Values filtered by IIR")
plt.plot(yaxisFiltered)
plt.ylim([250, 450])
plt.xlim([x1_marker, x2_marker])

# Y axis parameter:
yaxisAbsolute = np.array(absoluteValues)
# plotting - absolute filtered
plt.subplot(1, 3, 3)
plt.title("Absolute filtered values")
plt.plot(yaxisAbsolute)
plt.ylim([y_lower, y_higher])
plt.xlim([x1_marker, x2_marker])

plt.show()
