import tkinter as tk
from tkinter import PhotoImage
from PIL import Image, ImageTk  # Import thư viện Pillow
import serial
import re
import threading
from mainMenu import MainMenu
from test_serial import serial_obj
import time
from gcode_read import gCodeRead_Obj

def Update_MainMenu(Hotend_Temperature, Hedbed_Temperature, Fan_speedPower, X_Val, Y_Val, Z_Val):
    menu_frame.hotendTempBox.delete(0, tk.END)
    menu_frame.hotendTempBox.insert(tk.END, f"{Hotend_Temperature}")
    menu_frame.hedbedTempBox.delete(0,tk.END)
    menu_frame.hedbedTempBox.insert(tk.END, f"{Hedbed_Temperature}")
    menu_frame.fanSpeedBox.delete(0, tk.END)
    menu_frame.fanSpeedBox.insert(tk.END, f"{Fan_speedPower}")
    menu_frame.X_mmBox.delete(0, tk.END)
    menu_frame.X_mmBox.insert(tk.END, f"{X_Val}")
    menu_frame.Y_mmBox.delete(0, tk.END)
    menu_frame.Y_mmBox.insert(tk.END, f"{Y_Val}")
    menu_frame.Z_mmBox.delete(0, tk.END)
    menu_frame.Z_mmBox.insert(tk.END, f"{Z_Val}")

# Hàm nhận dữ liệu trong một luồng riêng
def read_data_from_serial():
    while True:
        if serial_obj.ser.in_waiting > 0:
            received_data = serial_obj.ser.readline()
            print(f"Dữ liệu nhận (raw): {received_data}")
            try:
                received_text = received_data.decode('utf-8').strip()
                menu_frame.noteBox.delete(0,tk.END)
                menu_frame.noteBox.insert(tk.END, received_text)
                X, Y, Z, hotendTemp, hedbedTemp = serial_obj.process_data(received_text)
                 # Xóa sạch buffer nhập (input buffer) sau khi xử lý dữ liệu
                serial_obj.ser.reset_input_buffer()
                if(serial_obj.response_from_slave == serial_obj.response_ready_to_receive_gcode):
                    gCodeRead_Obj.sendNextGcodeComand()
                if(menu_frame.currentMenu == menu_frame.mainMenu & serial_obj.state == serial_obj.state_connected):
                    Update_MainMenu(hotendTemp,hedbedTemp,menu_frame.fanSpeedPower,X, Y, Z)

            except UnicodeError as e:
                print(f"Lỗi decode: {e}")
            
            
def connect_to_slave():
    
    # Kết nối cổng serial
    try:
        serial_obj.ser = serial.Serial(serial_obj.USB_PORT, serial_obj.BAUD_RATE, timeout=1)
        print(f"Kết nối thành công với {serial_obj.USB_PORT}")
        serial_obj.state = serial_obj.state_connected
    except serial.SerialException as e:
        print(f"Lỗi kết nối: {e}")
        ser = None
    if serial_obj.ser:
        serial_obj.send_data(serial_obj.cmd_connect_to_slave)

#**************************************MainForm********************************************#
# Tạo cửa sổ chính
root = tk.Tk()
root.title("Ứng Dụng với Form Cài Đặt")
root.geometry("840x480")
root.configure(bg="#0F0A3F")


menu_frame = MainMenu(root)
menu_frame.Start_button.configure(bg= "green")
menu_frame.Stop_button.configure(bg= "red")
menu_frame.Pause_button.configure(bg = "yellow")
menu_frame.X_mmBox.delete(0, tk.END)
menu_frame.X_mmBox.insert(tk.END, menu_frame.X)
menu_frame.button_connect_to_slave.configure(command= connect_to_slave)
menu_frame.noteBox.delete(0,tk.END)
menu_frame.noteBox.insert(tk.END, "Xin Chao")
# Khởi động luồng đọc dữ liệu
if serial_obj.ser:
    serial_thread = threading.Thread(target=read_data_from_serial, daemon=True)
    serial_thread.start()
# Chạy vòng lặp chính
root.mainloop()
