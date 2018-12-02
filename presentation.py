import tkinter as tk
import socket

UDP_IP = "192.168.43.125"
UDP_PORT = 4210
MESSAGE = "Connected"

print ("UDP target IP:", UDP_IP)
print ("UDP target port:", UDP_PORT)
print ("message:", MESSAGE)

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.sendto(MESSAGE.encode('utf-8'), (UDP_IP, UDP_PORT))

root = tk.Tk()


def key(event):
    frame.focus_set()
    key_press = event.keysym.lower()
    print(key_press)
    global MESSAGE
    if key_press == 'w':
        MESSAGE = "forward"
    elif key_press == 's':
        MESSAGE = "back"
    elif key_press == 'a':
        MESSAGE = "left"
    elif key_press == 'd':
        MESSAGE = "right"
    elif key_press == 'z':
        MESSAGE = "lt"
    elif key_press == 'c':
        MESSAGE = "rt"
    elif key_press == 'x':
        MESSAGE = "stop"
    elif key_press == 'q':
        MESSAGE = "up"
    elif key_press == 'e':
        MESSAGE = "down"
    else:
        print("w,a,s,d to move, space to stop, q to raise lift, e to lower")
    sock.sendto(MESSAGE.encode('utf-8'), (UDP_IP, UDP_PORT))
    
frame = tk.Frame(root, width=10, height=10)
frame.bind_all("<Key>", key)
frame.pack()

root.mainloop()
