
#!/usr/bin/env python
import socket
from threading import Thread
import json
import time
import cv2
import base64
import math

class UDPServer:
    __on_running = True
    __last_user = ['0.0.0.0', 0]
    __register = False
    __last_register_time = 0
    __max_len_package = 1024*32

    def __init__(self, port):
        self.__socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.__socket.bind(('0.0.0.0', port))
        self.__recv_thread = Thread(target=self.__recv_handle)
        self.__recv_thread.start()
        self.__send_thread = Thread(target=self.__send_handle)
        self.__send_thread.start()

    def __recv_handle(self):
        while(self.__on_running):
            try:
                data, addr = self.__socket.recvfrom(1024)
            except:
                continue
            recv_json = json.loads(data)
            if 'type' in recv_json:
                if(recv_json['type'] == 'pulse'):
                    send_json = {'type': 'pulse', 'data': 'pong'}
                    self.__socket.sendto(str(json.dumps(send_json)).encode(), addr)
                    if not self.__register or self.__last_user[0] != addr[0] or self.__last_user[1] != addr[1]:
                        self.__register = True
                        self.__last_register_time = time.time()
                        self.__last_user = addr
                    
                if(recv_json['type'] == 'control'):
                    print(recv_json['data'])
                    
                if(recv_json['type'] == 'speed'):
                    print('speed: ' + str(recv_json['data']))
            time.sleep(0.001)

    def __send_handle(self):
        video = cv2.VideoCapture("F:/home/yas/t-dt2023-vision/video/sentry.avi")
        while(self.__on_running):
            if not self.__register:
                continue
            if time.time() - self.__last_register_time > 5:
               self.__register = False
            try:
                ret, frame = video.read()
            except:
                continue
            if not ret:
                continue
            frame = cv2.resize(frame, (606, 450))
            image = cv2.imencode('.jpg', frame)[1]
            image_base64 = str(base64.b64encode(image))[2:-1]
            max_index = math.ceil(len(image_base64)/self.__max_len_package)
            for i in range(max_index):
                if(i == max_index - 1):
                    send_json = {'type': 'video', 'data': str(image_base64[i*self.__max_len_package:]), 'index': int(i + 1), 'max_index': int(max_index)}
                    self.__socket.sendto(str(json.dumps(send_json)).encode(), self.__last_user)
                else:
                    send_json = {'type': 'video', 'data': str(image_base64[i*self.__max_len_package:(i+1)*self.__max_len_package]), 'index': int(i + 1), 'max_index': int(max_index)}
                    self.__socket.sendto(str(json.dumps(send_json)).encode(), self.__last_user)
                
            cv2.imshow('frame', frame)
                
            input = cv2.waitKey(1)
            if input == ord('q'):
                on_running = False
                break

server = UDPServer(25560)