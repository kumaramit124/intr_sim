# intr_sim
20ms interrupt simulation. This driver has been created for the application which is expecting the 20ms interrupt status from the device. 
# Driver
In this driver I have used the used kernel timer api to generate the 20ms interrupt to simulate the HW.
# Test App
The test app is utilising this driver to check in the application.
Since the application was moving into the cloud so they need to simulate the same interrupt.
So the poupose of createing this module is to simulate the 20ms interrupt flow so that the application can varify their funcanality.  
