import tkinter as tk
import serial
import re
#
ser = None
# Cấu hình cổng USB nối tiếp
USB_PORT = "/dev/ttyUSB0"  # Thay bằng cổng USB thực tế của bạn
BAUD_RATE = 115200

last_X = 0
last_Y = 0
last_Z = 0
last_HTemp = 0
last_BTemp = 0
class mySerial:
    def __init__(self):
        self.ser = None
        self.USB_PORT = USB_PORT
        self.BAUD_RATE = BAUD_RATE
        #define command
        self.cmd_connect_to_slave = "Connect"
        self.cmd_start = "Start"
        self.cmd_stop = "Stop"
        self.cmd_pause = "Pause"
        
        #Response from slave
        self.response_ready_to_receive_gcode = "G_CODE_NONE"
        self.response_busy = "BUSY"
        self.response_from_slave = None
        #define Connect State
        self.state_connected = True
        self.state_disconnected = False
        self.state = self.state_disconnected
        # Kết nối cổng serial
        try:
            self.ser = serial.Serial(self.USB_PORT, self.BAUD_RATE, timeout=1)
            print(f"Kết nối thành công với {self.USB_PORT}")
            self.state = self.state_connected
        except serial.SerialException as e:
            print(f"Lỗi kết nối: {e}")
            ser = None
        
    # Hàm gửi chuỗi xuống vi điều khiển
    def send_data(self,  data):
        if data:
            try:
                self.ser.write(f"{data}\n".encode('utf-8'))
                print(f"Gửi: {data}")
            except serial.SerialException as e:
                print(f"Lỗi gửi dữ liệu: {e}")

    def process_data(self, data):
        # Khởi tạo các giá trị mặc định
        global last_X, last_Y, last_Z, last_HTemp, last_BTemp
        X = last_X
        Y = last_Y
        Z = last_Z
        hotendTemp = last_HTemp
        hedbedTemp = last_BTemp
        # Xử lý các phản hồi sẵn sàng nhận G-code hoặc bận
        if data == self.response_ready_to_receive_gcode:
            self.response_from_slave = self.response_ready_to_receive_gcode
            print(f"Response From Slave: {self.response_ready_to_receive_gcode}")
        if data == self.response_busy:
            self.response_from_slave = self.response_busy
            print(f"Response From Slave: {self.response_busy}")
        
        # Tách chuỗi và xử lý các thành phần X, Y, Z
        if 'X' in data:
            X = self.extract_value(data, 'X')
        if 'Y' in data:
            Y = self.extract_value(data, 'Y')
        if 'Z' in data:
            Z = self.extract_value(data, 'Z')
        if 'H' in data:
            hotendTemp = self.extract_value(data, 'H')
        if 'B' in data:
            hefbedTemp = self.extract_value(data, 'B')

        print(f"Received data: X={X}, Y={Y}, Z={Z}, Hotend Temp={hotendTemp}, Bed Temp={hedbedTemp}")
            
        last_X = X
        last_Y = Y
        last_Z = Z
        last_HTemp = hotendTemp
        last_HTemp = hedbedTemp
        return X, Y, Z, hotendTemp, hedbedTemp

    def extract_value(self, data, param):
        """
        Hàm này dùng để tách giá trị sau ký tự param (X, Y, Z, H, B) từ chuỗi dữ liệu.
        """
        try:
            start = data.find(param) + 1  # Tìm vị trí của ký tự (X, Y, Z, H, B)
            end = start
            while end < len(data) and (data[end].isdigit() or data[end] == '.' or data[end] == '-' or data[end] == '+'):
                end += 1  # Tìm hết số thực sau ký tự (X, Y, Z, H, B)
            
            value = data[start:end]
            return float(value) if value else None
        except ValueError:
            return None



# Khởi tạo đối tượng mySerial
serial_obj = mySerial()  # Khởi tạo đối tượng mySerial để truy cập các phương thức của nó
