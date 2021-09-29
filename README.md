#  FlightAnomalyDetectorServer
I am thrilled to present the flight anomaly detection server.
This server finds anomalies using flight's recorded data (TimeSeries csv file).
This server supports handling of multiple clients simultaneously.

### The working method:
  The server working options are listed in a specific CLI (Command Line Interface) which will be sent to the client first when it connects and after processing each request.
  The CLI:
  ![image](https://user-images.githubusercontent.com/71394850/132982278-bae3db93-b5d7-45d1-9029-1668c29a275d.png)
  Each time the client receives a menu, he should replied the number that symbolizes the desired choice and additional information according to his choice.

Notes:
- The server should be run on linux.
- The server should be run with specific port in which it listens for accepting new clients. (you can see a run example in the main file).
- An example of a properly TimeSeries csv file: ![image](https://user-images.githubusercontent.com/71394850/132982828-e1dc6b02-70d2-4f37-ac93-194a396ce79b.png)
- For the CLI commands implenetation the following design patterns were used: Command and Bridge.
  Also it is described in the following uml:
  ![image](https://user-images.githubusercontent.com/71394850/132984618-be0d9967-0ff9-471e-b460-c93544e897af.png)
  .

There is an example of the server running including handling of two different clients in the main file.



  
