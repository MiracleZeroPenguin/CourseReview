# Exercise 9

3180103570 卢佳盈

## 9.4

When searching for 1123456, we can find its lower-order 12bits "0100 0101 0110" and use it as the displacement into the page. And the remaining 20 bits "0001 0001 0001 0010 0011"  are used as the displacement in the page table.

## 9.5

a. no

b. no

c. no

d. yes

e. probably improve CPU utilization as more pages can remain resident and not require paging to or from the disks

f. yes, for as the disk bottleneck is removed by faster response and more throughput to the disks, the CPU will get more data more quickly

g. yes, the CPU will get more data faster, so it will be more in use. This is only the case if the paging action is amenable to prefetching.

h. perhaps, it will result in fewer page faults if data is being accessed sequentially. If data access is more or less random, more paging action could ensure because fewer pages can be kept in memory and more data is transferred per page fault. So it is likely to decrease utilization as it is increase it.

