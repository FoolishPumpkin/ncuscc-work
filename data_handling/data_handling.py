from pathlib import Path
import csv
import matplotlib.pyplot as plt
path=Path("./data_0.csv") 
lines=path.read_text().splitlines()  
reader=csv.reader(lines)
head=next(reader)   
bubble_times=[]
heap_times=[]
fab_times=[]
sizes=[]
for row in reader:
    size=int(row[0])
    bubble_time=float(row[1])
    heap_time=float(row[2])
    fab_time=float(row[3])
    sizes.append(size)
    bubble_times.append(bubble_time)
    heap_times.append(heap_time)
    fab_times.append(fab_time)
plt.style.use('ggplot')
fig, ax = plt.subplots()
ax.plot(sizes,bubble_times, color='red')
ax.plot(sizes,heap_times, color='blue')
ax.plot(sizes,fab_times, color='green')
ax.set_title("sort", fontsize=24)
ax.set_xlabel("sizes", fontsize=16)
ax.set_ylabel("time", fontsize=16)
ax.tick_params(labelsize=16)
ax.tick_params(labelsize=16)
plt.savefig("./data")