# Computer Networks Project
## Question/Answer Chat Client for Academic staff and Students

### Mini Course Project (Group Number:12)
Submitted by: 
Gunjan Aggarwal (201A7PS050P),
Mohit Yadav (2014A7PS074P),
Vidhi Jain (2014A7PS113P),
Rishabh Mittar (2014A7PS141P)

Question/answer chat client for academic staff and students is an interactive interface wherein the students can get answer to frequently asked queries. It will store the questions related to academics and their answers provided by the staff. The staff and students will act as a client, and the server stores only the relevant information and provides a way of communication between them. This aims to improve the quality of doubt clearance, and also saving the time of  staff and students both: The chat client saves time of the staff as they do not have to answer the same queries repeatedly, and for the students, there will be immediate doubt clearance, thereby providing help and assisting the student to continue without any doubts.

### Requirements
* Linux (preferably, Ubuntu 16.04)
* C Compiler (preferably, gcc version 5.4.0)

### Directions to setup
#### Phase 1
Compile the server application
```
gcc tcpserver.c -o ser
```
Run the server
```
./ser
```
Now, open another terminal (on same or another machine that is connected locally)
```
gcc tcpclient.c -o client
```
Run the client application
```
./client <ipaddress of the server> <user_details>
```
The ipaddress of the server can be checked on the system on which the server is running by linux command 
```
ifconfig
```
The user details (for phase 1) is a numeric userid. This will be repalced by secure file argument which will be encrypted to ensure security.

Start of Chat Application: ![Screeshots](/image3.png)
Instance of a Chat: ![Screeshots](/image2.png)
