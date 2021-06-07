# DMX

- Diagrams: Describes how the project is working
- Documentation: Documentation on the DMX protocol

- Source: Source code for all actors
    - RemoteControl: Send commands to the DMX chain via wifi
    - Raspberry Pi: Access point which forwards RemoteControll and Android App commands to the Sender and host an web server with apache2
    - Generator: Send Commands to the DMX chain properly formated acquired with the Raspberry Pi's gpio port
