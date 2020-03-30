# Copy the repo folder to the pi using rsync (very fast compared to normal copy)
# for testing purposes. 
# 
# 1- First: add the pi address to your hosts file (/etc/hosts) such as:
## 10.241.0.10  pi
# 2 - Run the following script or line

rsync -r ../bzzzbz pi@pi:~/test 