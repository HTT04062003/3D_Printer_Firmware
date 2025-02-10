import tkinter as tk
from tkinter import PhotoImage
from PIL import Image, ImageTk  # Import thư viện Pillow
from numeric_keypad import show_keyboard
from tkinter  import filedialog
import json
import os
from test_serial import serial_obj
from gcode_read import gCodeRead_Obj
#
# Đường dẫn đến file lưu trữ
CONFIG_FILE = "/home/pi/TKINTER_GUI/config.json"
# Biến global cho keyboard_frame
keyboard_frame = None
#
print_icon_image_path = "print_icon.png"
setting_icon_image_path = "setting_icon.png"
hotend_temp_icon_image_path = "hotend_temp_icon.png"
hedbed_temp_icon_image_path = "hedbed_temp_icon.png"
fanSpeed_icon_iamge_path = "fanSpeed_icon.png"
doC_icon_iamge_path = "doC.png"
time_icon_image_path = "tim_icon.png"
X_icon_iamge_path = "X_icon.png"
Y_icon_image_path = "Y_icon.png"
Z_icon_iamge_path = "Z_icon.png"
#
class MainMenu:
    def __init__(self, root):
        #define
        self.mainMenu = 1
        self.settingMenu = 2
        self.axisResolutionMenu = 3
        self.PIDParameterMenu = 4
        self.manControlMenu = 5
        #Tao mot bien chua gia tri menu hien tai
        self.currentMenu = self.mainMenu
        # Tạo một frame chứa main Menu
        self.frame = tk.Frame(root, bg="#0F0A3F")
        
        self.frame.pack(fill=tk.BOTH, expand=True)  # Đặt `pack` để frame chiếm toàn bộ diện tích
        #******************************************Padding Frame***************************************************************************************#
        self.padding_frame = tk.Frame(self.frame, bg= "#0F0A3F", width=840, height=480)
        self.child_frame = tk.Frame(self.frame, bg= "#2e2e2e", width= 430, height=300)
        self.child_frame.place(x= 400, y = 10)
        #Global Variable
        # Biến toàn cục để theo dõi thời gian
        self.seconds = 0
        self.running = False  # Biến trạng thái để kiểm tra đồng hồ đang chạy hay không
        self.hotendTemp = 0
        self.hedbedTemp = 0
        self.fanSpeedPower = 0
        self.X = 0
        self.Y = 0
        self.Z = 0
        self.Kp_THotend = 0
        self.Ki_THotend = 0
        self.Kd_THotend = 0
        self.Kb_THotend = 0
        self.Kp_THedbed = 0
        self.Ki_Thedbed = 0
        self.Kd_THedbed = 0
        self.Kb_tHedbed = 0
        self.BLUX = 0
        self.BLUY = 0
        self.BLUZ = 0
        self.BLUE = 0
        config = self.load_config()
        self.Kp_THedbed = config["Kp_Hedbed"]
        self.Ki_THedbed = config["Ki_Hedbed"]
        self.Kd_THedbed = config["Kd_Hedbed"]
        self.Kp_THotend = config["Kp_Hotend"]
        self.Ki_THotend = config["Ki_Hotend"]
        self.Kd_THotend = config["Kd_Hotend"]
        self.X = config["X_val"]
        self.Y = config["Y_Val"]
        self.Z = config["Z_Val"]
        self.BLUX = config["BLUX"]
        self.BLUY = config["BLUY"]
        self.BLUZ = config["BLUZ"]
        self.BLUE = config["BLUE"]
        # Gọi hàm tạo các phần tử bên trong menu
        self.create_main_mernu_widgets()
        self.create_setting_menu_widget()

    def create_main_mernu_widgets(self):
        # Tạo các nút và Entry trong menu
        
        # Nút Settings
        # Nút bóng (shadow) lệch xuống dưới và sang phải
        self.shadow_setting_button = tk.Button(self.frame, text="⚙️ Setting", font=("Arial", 12), 
                                    bg="#556F78", fg="white", width=10, height=3, bd=0, relief="flat")
        self.shadow_setting_button.place(x = 14, y = 14)  # Lệch vị trí để tạo hiệu ứng bóng
        self.setting_button = tk.Button(self.frame, text="⚙️ Setting", font=("Arial", 12), command=self.toggle_settings, bg="#0F0A3F", fg="white", width=10, height=3)
        self.setting_button.place(x=10, y = 10)
        # Mở hình ảnh bằng Pillow
        image = Image.open(print_icon_image_path)  # Đảm bảo rằng tệp hình ảnh đúng
        image = image.resize((24, 24))  # Thay đổi kích thước hình ảnh (tuỳ chọn)

        # Chuyển hình ảnh thành PhotoImage để Tkinter có thể sử dụng
        self.print_icon = ImageTk.PhotoImage(image)

        self.shadow_Print_button = tk.Button(self.frame, text="Print",font=("Arial", 12), 
                                    bg="#556F78", fg="white", width=10, height=3, bd=0, relief="flat")
        self.shadow_Print_button.place(x=144, y=14)  # Lệch vị trí để tạo hiệu ứng bóng
        self.Print_button = tk.Button(self.frame, text="Print", font=("Arial", 12),  bg="#0F0A3F", fg="white", width=10, height=3,
                                      command= gCodeRead_Obj.readGcodeFile)
        self.Print_button.place(x=140, y=10)
        # Tạo Label để hiển thị hình ảnh
        self.print_icon_label = tk.Label(self.frame, image=self.print_icon)
        self.print_icon_label.place(x = 142, y = 12)
        #******************************************************************Start button**********************************************************************#
        self.shadow_Start_button = tk.Button(self.frame, text="Start",font=("Arial", 12), 
                                    bg="#556F78", fg="white", width=10, height=3, bd=0, relief="flat")
        self.shadow_Start_button.place(x=274, y=14)  # Lệch vị trí để tạo hiệu ứng bóng
        self.Start_button = tk.Button(self.frame, text="Start", font=("Arial", 12), command=self.start_button_callback, bg="#0F0A3F", fg="white", width=10, height=3)
        self.Start_button.place(x=270, y=10)
        #*******************************************************************Stop Button**********************************************************************#
        self.shadow_Stop_button = tk.Button(self.frame, text="Stop",font=("Arial", 12), 
                                    bg="#556F78", fg="white", width=10, height=3, bd=0, relief="flat")
        self.shadow_Stop_button.place(x=274, y=99)  # Lệch vị trí để tạo hiệu ứng bóng
        self.Stop_button = tk.Button(self.frame, text="Stop", font=("Arial", 12), command=self.toggle_settings, bg="#0F0A3F", fg="white", width=10, height=3)
        self.Stop_button.place(x=270, y=95)
        #******************************************************************Pause Button***********************************************************************#
        self.shadow_Pause_button = tk.Button(self.frame, text="Pause",font=("Arial", 12), 
                                    bg="#556F78", fg="white", width=10, height=3, bd=0, relief="flat")
        self.shadow_Pause_button.place(x=274, y=189)  # Lệch vị trí để tạo hiệu ứng bóng
        self.Pause_button = tk.Button(self.frame, text="Pause", font=("Arial", 12), command=self.pause_button_callback, bg="#0F0A3F", fg="white", width=10, height=3)
        self.Pause_button.place(x=270, y=185)
        #**************************************************Button connect****************************************************************************#
        self.button_connect_to_slave = tk.Button(self.frame, text= "Connect", font= ("Arial", 12),bg="#0F0A3F", fg="white", width=10, height=3 )
        self.button_connect_to_slave.place(x = 270, y = 265)
        #********************************************************Tao icon hien thi bieu tuong hotend*************************************************#
        image = Image.open("hotend_temp_icon.png")  # Đảm bảo rằng tệp hình ảnh đúng
        image = image.resize((40, 40))  # Thay đổi kích thước hình ảnh (tuỳ chọn)
        self.hotend_temp_icon = ImageTk.PhotoImage(image)

        self.hotend_temp_icon_label = tk.Label(self.frame, image=self.hotend_temp_icon, bd=0, relief="flat", highlightthickness=0)
        self.hotend_temp_icon_label.place(x= 10, y = 85)
        # Tạo Entry để hiển thị số nhập
        self.hotendTempBox = tk.Entry(self.frame, font=("Arial", 14, ), width=12, background="#2e2e2e", fg="white")
        self.hotendTempBox.place(x=55, y=95)
        # Gán giá trị cho luxBox
        self.hotendTempBox.delete(0, tk.END)  # Xóa tất cả nội dung hiện tại trong luxBox
        self.hotendTempBox.insert(tk.END, "0.0125")  # Thêm giá trị mới vào cuối
        #***************************************************************8**Tao icon do C**************************************************************#
        image = Image.open(doC_icon_iamge_path)  # Đảm bảo rằng tệp hình ảnh đúng
        image = image.resize((40, 40))  # Thay đổi kích thước hình ảnh (tuỳ chọn)
        self.doC_icon = ImageTk.PhotoImage(image)

        self.doC_icon_label1 = tk.Label(self.frame, image=self.doC_icon, bd=0, relief="flat", highlightthickness=0)
        self.doC_icon_label1.place(x= 180, y = 90)
        #***************************************************************Tao icon cho Hedbed*****************************************************8*****#
        #
        image = Image.open(hedbed_temp_icon_image_path)  # Đảm bảo rằng tệp hình ảnh đúng
        image = image.resize((40, 40))  # Thay đổi kích thước hình ảnh (tuỳ chọn)
        self.hedbed_temp_icon = ImageTk.PhotoImage(image)
        self.hedbed_temp_icon_label = tk.Label(self.frame, image=self.hedbed_temp_icon, bd=0, relief="flat", highlightthickness=0)
        self.hedbed_temp_icon_label.place(x= 10, y = 125)
        # Tạo Entry để hiển thị số nhập
        self.hedbedTempBox = tk.Entry(self.frame, font=("Arial", 14, ), width=12, background="#2e2e2e", fg="white")
        self.hedbedTempBox.place(x=55, y=135)
        # Gán giá trị cho luxBox
        self.hedbedTempBox.delete(0, tk.END)  # Xóa tất cả nội dung hiện tại trong luxBox
        self.hedbedTempBox.insert(tk.END, "0.0125")  # Thêm giá trị mới vào cuối
        #
        #***************************************************Tao icon do C cho Hebded Temperature*****************************************************#
        self.doC_icon_label2 = tk.Label(self.frame, image=self.doC_icon, bd=0, relief="flat", highlightthickness=0)
        self.doC_icon_label2.place(x= 180, y = 135)
        #*************************************************************Tao icon hien thi Fan Speed****************************************************#
        image = Image.open(fanSpeed_icon_iamge_path)  # Đảm bảo rằng tệp hình ảnh đúng
        image = image.resize((40, 40))  # Thay đổi kích thước hình ảnh (tuỳ chọn)
        self.fanSpeed_icon = ImageTk.PhotoImage(image)

        self.fanSpeed_icon_label = tk.Label(self.frame, image=self.fanSpeed_icon, bd=0, relief="flat", highlightthickness=0)
        self.fanSpeed_icon_label.place(x= 10, y = 165)
        # Tạo Entry để hiển thị số nhập
        self.fanSpeedDVLabel = tk.Label(self.frame, text= "%",font=("Arial", 12, ), width=9, background="#0F0A3F", fg="white")
        self.fanSpeedDVLabel.place(x=150, y=175)
        self.fanSpeedBox = tk.Entry(self.frame, font=("Arial", 14, ), width=12, background="#2e2e2e", fg="white")
        self.fanSpeedBox.place(x=55, y=175)

        # Gán giá trị cho luxBox
        self.fanSpeedBox.delete(0, tk.END)  # Xóa tất cả nội dung hiện tại trong luxBox
        self.fanSpeedBox.insert(tk.END, "0.0125")  # Thêm giá trị mới vào cuối
        #****************************************************Tao icon Hien thi Bieu Tuong Thoi Gian****************************************************#
        image = Image.open(time_icon_image_path)  # Đảm bảo rằng tệp hình ảnh đúng
        image = image.resize((40, 40))  # Thay đổi kích thước hình ảnh (tuỳ chọn)
        self.tim_icon = ImageTk.PhotoImage(image)

        self.tim_icon_label = tk.Label(self.frame, image=self.tim_icon, bd=0, relief="flat", highlightthickness=0)
        self.tim_icon_label.place(x= 10, y = 205)
        # Tạo Entry để hiển thị số nhập
        self.timeDVLabel = tk.Label(self.frame, text= "mm:hh",font=("Arial", 14, ), width=10, background="#0F0A3F", fg="white")
        self.timeDVLabel.place(x=160, y=215)
        self.timeBox = tk.Entry(self.frame, font=("Arial", 14, ), width=12, background="#2e2e2e", fg="white")
        self.timeBox.place(x=55, y=215)
        # Gán giá trị cho luxBox
        self.timeBox.delete(0, tk.END)  # Xóa tất cả nội dung hiện tại trong luxBox
        self.timeBox.insert(tk.END, "00:00")  # Thêm giá trị mới vào cuối
        #********************************************************************Tao Icon hien thi hop thoai X***************************************************#
        image = Image.open(X_icon_iamge_path)  # Đảm bảo rằng tệp hình ảnh đúng
        image = image.resize((40, 40))  # Thay đổi kích thước hình ảnh (tuỳ chọn)
        self.X_icon = ImageTk.PhotoImage(image)

        self.X_icon_label = tk.Label(self.frame, image=self.X_icon, bd=0, relief="flat", highlightthickness=0)
        self.X_icon_label.place(x= 10, y = 250)
        self.mm_label1 = tk.Label(self.frame, text= "mm",font=("Arial", 14, ), width=12, background="#0F0A3F", fg="white")
        self.mm_label1.place(x=140, y=260)
        self.X_mmBox = tk.Entry(self.frame, font=("Arial", 14, ), width=12, background="#2e2e2e", fg="white")
        self.X_mmBox.place(x=55, y=260)
        # Gán giá trị cho luxBox
        self.X_mmBox.delete(0, tk.END)  # Xóa tất cả nội dung hiện tại trong luxBox
        self.X_mmBox.insert(tk.END, f"{self.X}")  # Thêm giá trị mới vào cuối
        #*****************************************************Tao Icon hein thi hop thoai Y******************************************************************#
        image = Image.open(Y_icon_image_path)  # Đảm bảo rằng tệp hình ảnh đúng
        image = image.resize((40, 40))  # Thay đổi kích thước hình ảnh (tuỳ chọn)
        self.Y_icon = ImageTk.PhotoImage(image)

        self.Y_icon_label = tk.Label(self.frame, image=self.Y_icon, bd=0, relief="flat", highlightthickness=0)
        self.Y_icon_label.place(x= 10, y = 290)
        self.mm_label2 = tk.Label(self.frame, text= "mm",font=("Arial", 14, ), width=12, background="#0F0A3F", fg="white")
        self.mm_label2.place(x=140, y=300)
        self.Y_mmBox = tk.Entry(self.frame, font=("Arial", 14, ), width=12, background="#2e2e2e", fg="white")
        self.Y_mmBox.place(x=55, y=300)
        # Gán giá trị cho luxBox
        self.Y_mmBox.delete(0, tk.END)  # Xóa tất cả nội dung hiện tại trong luxBox
        self.Y_mmBox.insert(tk.END, f"{self.Y}")  # Thêm giá trị mới vào cuối
        #***************************************************Tao icon hien thi hop thoai Z********************************************************************#
        image = Image.open(Z_icon_iamge_path)  # Đảm bảo rằng tệp hình ảnh đúng
        image = image.resize((40, 40))  # Thay đổi kích thước hình ảnh (tuỳ chọn)
        self.Z_icon = ImageTk.PhotoImage(image)

        self.Z_icon_label = tk.Label(self.frame, image=self.Z_icon, bd=0, relief="flat", highlightthickness=0)
        self.Z_icon_label.place(x= 10, y = 330)
        self.mm_label3 = tk.Label(self.frame, text= "mm",font=("Arial", 14, ), width=12, background="#0F0A3F", fg="white")
        self.mm_label3.place(x=140, y=340)
        self.Z_mmBox = tk.Entry(self.frame, font=("Arial", 14, ), width=12, background="#2e2e2e", fg="white")
        self.Z_mmBox.place(x=55, y=340)
        #********************8***************************                                 *******************************************************************#
        # Gán giá trị cho luxBox
        self.Z_mmBox.delete(0, tk.END)  # Xóa tất cả nội dung hiện tại trong luxBox
        self.Z_mmBox.insert(tk.END, f"{self.Z}")  # Thêm giá trị mới vào cuối

        #*****************************************Text box *****************************************************#
        self.noteBox = tk.Entry(self.child_frame, width= 200, bg= "white")
        self.noteBox.place(x = 5, y = 5)
    
    def create_menu_pause_widgets(self):
        self.infoBox = tk.Label(self.padding_frame, text="Paused !!!", font=("Arial", 12), bd=0, relief="flat", highlightthickness=0,
                                bg= "white", fg="black")
        self.infoBox.place(x = 50, y = 50)
        self.continue_button = tk.Button(self.padding_frame, text="Continue", font=("Airal", 12), fg= "black", bg= "green",
                                         command= self.continue_button_callback)
        self.continue_button.place(x = 50, y=80)
   

    def create_setting_menu_widget(self):
        # Các nút trong setting_frame
        self.menu_motor_button = tk.Button(self.padding_frame, text="Axis Resolution", font=("Arial", 12), command=self.show_axis_resolution_menu, bg="black", fg="white", width=10, height=1)
        self.menu_motor_button.place(x=5, y=0)

        self.menu_hotend_button = tk.Button(self.padding_frame, text="PID_Setting", font=("Arial", 12), command=self.show_PID_Paremeter_menu, bg="black", fg="white", width=10, height=1)
        self.menu_hotend_button.place(x=5, y=30)

        self.close_setting_button = tk.Button(self.padding_frame, text="Close", font=("Arial", 12), command=self.off_setting_menu, bg="red", fg="white", width=10, height=1)
        self.close_setting_button.place(x=5, y=90)

        self.menu_manControl_button = tk.Button(self.padding_frame, text="MAN Control", font=("Arial", 12), command=self.show_manControl_menu, bg="black", fg="white", width=10, height=1)
        self.menu_manControl_button.place(x=5, y=60)
    
    def create_man_control_menu_widgets(self):
        self.hotend_temp_icon_label = tk.Label(self.padding_frame, image=self.hotend_temp_icon, bd=0, relief="flat", highlightthickness=0)
        self.hotend_temp_icon_label.place(x= 10, y = 10)
        # Tạo widget Scale (thước cuộn)
        self.hotend_temp_value_scale = tk.Scale(self.padding_frame, from_=0, to=500, orient="horizontal", command=self.on_hotend_temp_scale_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 630)
        self.hotend_temp_value_scale.set(self.hotendTemp)
        self.hotend_temp_value_scale.place(x = 60, y = 10)
        #
        self.hedbed_temp_icon_label = tk.Label(self.padding_frame, image=self.hedbed_temp_icon, bd=0, relief="flat", highlightthickness=0)
        self.hedbed_temp_icon_label.place(x= 10, y = 80)
        self.hedbed_temp_value_scale = tk.Scale(self.padding_frame, from_=0, to=120, orient="horizontal", command=self.on_hedbed_temp_scale_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 630)
        self.hedbed_temp_value_scale.set(self.hedbedTemp)
        self.hedbed_temp_value_scale.place(x= 60, y = 80)
        #
        self.fanSpeed_icon_label = tk.Label(self.frame, image=self.fanSpeed_icon, bd=0, relief="flat", highlightthickness=0)
        self.fanSpeed_icon_label.place(x= 10, y = 150)
        self.fan_speed_value_scale = tk.Scale(self.padding_frame, from_=0, to=100, orient="horizontal", command=self.on_fan_speed_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 630)
        self.fan_speed_value_scale.set(self.fanSpeedPower)
        self.fan_speed_value_scale.place(x= 60, y = 150)
        #
        self.X_icon_label = tk.Label(self.padding_frame, image=self.X_icon, bd=0, relief="flat", highlightthickness=0)
        self.X_icon_label.place(x= 10, y = 220)
        self.X_value_scale = tk.Scale(self.padding_frame, from_=-250, to=250, resolution=0.5, orient="horizontal", command=self.on_X_value_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 630)
        self.X_value_scale.set(self.X)
        self.X_value_scale.place(x= 60, y= 220)
        #
        self.Y_icon_label = tk.Label(self.padding_frame, image=self.Y_icon, bd=0, relief="flat", highlightthickness=0)
        self.Y_icon_label.place(x= 10, y = 290)
        self.Y_value_scale = tk.Scale(self.padding_frame, from_=-250, to=250, resolution=0.5, orient="horizontal", command=self.on_Y_value_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 630)
        self.Y_value_scale.set(self.Y)
        self.Y_value_scale.place(x= 60, y = 290)
        #
        self.Z_icon_label = tk.Label(self.padding_frame, image=self.Z_icon, bd=0, relief="flat", highlightthickness=0)
        self.Z_icon_label.place(x= 10, y = 360)
        self.Z_value_scale = tk.Scale(self.padding_frame, from_=-250, to=250, resolution=0.5, orient="horizontal", command=self.on_Z_value_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 630)
        self.Z_value_scale.set(self.Z)
        self.Z_value_scale.place(x = 60, y = 360)
        #
        self.close_menu_man_control_button = tk.Button(self.padding_frame, text= "Close", font= ("Arial", 12), fg= "white", 
                                                                                             command=self.off_manControl_menu, bg = "red", bd = 0, width=5, height=1)
        self.close_menu_man_control_button.place(x=700, y = 10)
        #
        self.set_home_button = tk.Button(self.padding_frame, text="Home", font=("Arial", 12), bg = "green",
                                         command= self.set_home, width=5, height=1, fg= "black")
        self.set_home_button.place(x = 700, y = 50)
    
    def create_PID_Parameter_menu_widgets(self):
        self.label1 = tk.Label(self.padding_frame, bd=0, relief="flat", highlightthickness=0, bg="#2e2e2e", width=50, height=17)
        self.label1.place(x=10, y=60)
        self.label2 = tk.Label(self.padding_frame, bd=0, relief="flat", highlightthickness=0, bg="#2e2e2e", width=50, height=17)
        self.label2.place(x=425, y=60)
        self.label3 = tk.Label(self.padding_frame, bd=1, relief="flat", highlightthickness=1, bg="white", width=18, height=1,
                               text="Hotend PID Parameter", font=("Arial", 12), fg = "black")
        self.label3.place(x =115, y = 30)
        self.label4 = tk.Label(self.padding_frame, bd=1, relief="flat", highlightthickness=1, bg="white", width=18, height=1,
                               text="Hedbed PID Parameter", font=("Arial", 12), fg = "black")
        self.label4.place(x =545, y = 30)
        #
        self.label5 = tk.Label(self.padding_frame, bd=1, relief="flat", highlightthickness=1, bg="black", width=4, height=1,
                               text="Kp_T", font=("Arial", 12), fg = "white")
        self.label5.place(x =20, y = 70)

        self.Kp_T_Hotend_value_scale = tk.Scale(self.padding_frame, from_=-250, to=250, resolution=0.5, orient="horizontal", command=self.on_Kp_THotend_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 340)
        self.Kp_T_Hotend_value_scale.set(f"{self.Kp_THotend}")
        self.Kp_T_Hotend_value_scale.place(x = 65, y = 70)

        self.label6 = tk.Label(self.padding_frame, bd=1, relief="flat", highlightthickness=1, bg="black", width=4, height=1,
                               text="Ki_T", font=("Arial", 12), fg = "white")
        self.label6.place(x =20, y = 140)

        self.Ki_T_Hotend_value_scale = tk.Scale(self.padding_frame, from_=-250, to=250, resolution=0.5, orient="horizontal", command=self.on_Ki_THoteng_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 340)
        self.Ki_T_Hotend_value_scale.set(f"{self.Ki_THotend}")
        self.Ki_T_Hotend_value_scale.place(x = 65, y = 140)
        

        self.label7 = tk.Label(self.padding_frame, bd=1, relief="flat", highlightthickness=1, bg="black", width=4, height=1,
                               text="Kd_T", font=("Arial", 12), fg = "white")
        self.label7.place(x =20, y = 210)

        self.Kd_T_Hotend_value_scale = tk.Scale(self.padding_frame, from_=-250, to=250, resolution=0.5, orient="horizontal", command=self.on_Kd_THotend_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 340)
        self.Kd_T_Hotend_value_scale.set(f"{self.Kd_THotend}")
        self.Kd_T_Hotend_value_scale.place(x = 65, y = 210)

        self.label8 = tk.Label(self.padding_frame, bd=1, relief="flat", highlightthickness=1, bg="black", width=4, height=1,
                               text="Kb_T", font=("Arial", 12), fg = "white")
        self.label8.place(x =20, y = 280)

        self.Kb_T_Hotend_value_scale = tk.Scale(self.padding_frame, from_=-250, to=250, resolution=0.5, orient="horizontal", command=self.on_Z_value_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 340)
        self.Kb_T_Hotend_value_scale.place(x = 65, y = 280)
        #
        self.label9 = tk.Label(self.padding_frame, bd=1, relief="flat", highlightthickness=1, bg="black", width=4, height=1,
                               text="Kp_T", font=("Arial", 12), fg = "white")
        self.label9.place(x =435, y = 70)

        self.Kp_THedbed_value_scale = tk.Scale(self.padding_frame, from_=-250, to=250, resolution=0.5, orient="horizontal", command=self.on_Kp_THedbed_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 340)
        self.Kp_THedbed_value_scale.set(f"{self.Kp_THedbed}")
        self.Kp_THedbed_value_scale.place(x = 480, y = 70)

        self.label10 = tk.Label(self.padding_frame, bd=1, relief="flat", highlightthickness=1, bg="black", width=4, height=1,
                               text="Ki_T", font=("Arial", 12), fg = "white")
        self.label10.place(x =435, y = 140)

        self.Ki_THedbed_value_scale = tk.Scale(self.padding_frame, from_=-250, to=250, resolution=0.5, orient="horizontal", command=self.on_Ki_THedbed_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 340)
        self.Ki_THedbed_value_scale.set(f"{self.Ki_THedbed}")
        self.Ki_THedbed_value_scale.place(x = 480, y = 140)

        self.label11 = tk.Label(self.padding_frame, bd=1, relief="flat", highlightthickness=1, bg="black", width=4, height=1,
                               text="Kd_T", font=("Arial", 12), fg = "white")
        self.label11.place(x =435, y = 210)

        self.Kd_THedbed_value_scale = tk.Scale(self.padding_frame, from_=-250, to=250, resolution=0.5, orient="horizontal", command=self.on_Kd_THedbed_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 340)
        self.Kd_THedbed_value_scale.set(f"{self.Kd_THedbed}")
        self.Kd_THedbed_value_scale.place(x = 480, y = 210)

        self.label12 = tk.Label(self.padding_frame, bd=1, relief="flat", highlightthickness=1, bg="black", width=4, height=1,
                               text="Kb_T", font=("Arial", 12), fg = "white")
        self.label12.place(x =435, y = 280)

        self.Kb_THedbed_value_scale = tk.Scale(self.padding_frame, from_=-250, to=250, resolution=0.5, orient="horizontal", command=self.on_Z_value_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 340)
        self.Kb_THedbed_value_scale.place(x = 480, y = 280)


        self.close_PID_Parameter_Menu_button = tk.Button(self.padding_frame, text= "Close", font= ("Arial", 12), fg= "white", 
                                                                                             command=self.off_PID_Parameter_menu, bg = "red", bd = 0, width=5, height=1)
        self.close_PID_Parameter_Menu_button.place(x=10, y = 360)
        #
    def create_axis_resolution_menu_widgets(self):
        self.blux_label = tk.Label(self.padding_frame, bd=0, relief="flat", highlightthickness=0, bg="#2e2e2e", width=8, height=1,
                                   text="BLU X", font=("Arial", 14), fg= "yellow")
        self.blux_label.place(x = 10, y = 10)
        self.blux_value_scale = tk.Scale(self.padding_frame, from_=-0.1, to=0.1, resolution=0.00001, orient="horizonta", command=self.on_BLUX_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 600)
        self.blux_value_scale.set(f"{self.BLUX}")
        self.blux_value_scale.place(x = 100, y = 10)

        self.bluy_label = tk.Label(self.padding_frame, bd=0, relief="flat", highlightthickness=0, bg="#2e2e2e", width=8, height=1,
                                   text="BLU Y", font=("Arial", 14), fg= "yellow")
        self.bluy_label.place(x = 10, y = 80)
        self.bluy_value_scale = tk.Scale(self.padding_frame, from_=-0.1, to=0.1, resolution=0.00001, orient="horizonta", command=self.on_BLUY_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 600)
        self.bluy_value_scale.set(f"{self.BLUY}")
        self.bluy_value_scale.place(x = 100, y = 80)

        self.bluz_label = tk.Label(self.padding_frame, bd=0, relief="flat", highlightthickness=0, bg="#2e2e2e", width=8, height=1,
                                   text="BLU Z", font=("Arial", 14), fg= "yellow")
        self.bluz_label.place(x = 10, y = 150)
        self.bluz_value_scale = tk.Scale(self.padding_frame, from_=-0.001, to=0.001, resolution=0.0000001, orient="horizonta", command=self.on_BLUZ_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 600)
        self.bluz_value_scale.set(f"{self.BLUZ}")
        self.bluz_value_scale.place(x = 100, y = 150)

        self.blue_label = tk.Label(self.padding_frame, bd=0, relief="flat", highlightthickness=0, bg="#2e2e2e", width=8, height=1,
                                   text="BLU E", font=("Arial", 14), fg= "yellow")
        self.blue_label.place(x = 10, y = 220)
        self.blue_value_scale = tk.Scale(self.padding_frame, from_=-0.1, to=0.1, resolution=0.001, orient="horizonta", command=self.on_BLUE_change, bg= "black", 
                              font=("Helvetica", 14), fg="white", width= 32, length= 600)
        self.blue_value_scale.set(f"{self.BLUE}")
        self.blue_value_scale.place(x = 100, y = 220)
        
        self.close_menu_axis_resolution_button = tk.Button(self.padding_frame, text="Close", font= ("Arial", 12), fg= "white",
                                                           bg="red", command=self.off_axis_resolution_menu)
        self.close_menu_axis_resolution_button.place(x=710, y =10)
        
     
    def on_hotend_temp_scale_change(self, hotend_value):
        self.hotendTemp = hotend_value
        serial_obj.send_data(f"M104 S{self.hotendTemp}")
        print(f"Giá trị nhiet do hotend thay đổi: {hotend_value}")
    def on_hedbed_temp_scale_change(self, hedbed_value):
        self.hedbedTemp = hedbed_value
        serial_obj.send_data(f"M140 S{self.hedbedTemp}")
        print(f"Gia tri nhiet do hedbed:{hedbed_value}")
    def on_fan_speed_change(self, fanspeed_value):
        self.fanSpeedPower = fanspeed_value
        serial_obj.send_data(f"M106 S{self.fanSpeedPower}")
        print(f"Gia tri fan speed:{fanspeed_value}")
    def on_X_value_change(self, X_value):
        self.X = X_value
        serial_obj.send_data(f"X{self.X}")
        self.save_config(X_val= X_value)
        print(f"Gia tri X: {X_value}")
    def on_Y_value_change(self, Y_value):
        self.Y = Y_value
        serial_obj.send_data(f"Y{self.Y}")
        self.save_config(Y_Val= Y_value)
        print(f"Gia tri Y: {Y_value}")
    def on_Z_value_change(self, Z_value):
        self.Z = Z_value
        serial_obj.send_data(f"Z{self.Z}")
        self.save_config(Z_Val= Z_value)
        print(f"Gia tri Z: {Z_value}")

    def on_Kp_THedbed_change(self, Kp_Hedbed_Val):
        self.Kp_THedbed = Kp_Hedbed_Val
        serial_obj.send_data(f"SETKpHBed{self.Kp_THedbed}")
        self.save_config(Kp_Hedbed = self.Kp_THedbed)
    def on_Ki_THedbed_change(self, Ki_Hedbed_val):
        self.Ki_THedbed = Ki_Hedbed_val
        serial_obj.send_data(f"SETKiHBed{self.Ki_THedbed}")
        self.save_config(Ki_Hedbed = self.Ki_THedbed)
    def on_Kd_THedbed_change(self, Kd_Hedbed_Val):
        self.Kd_THedbed = Kd_Hedbed_Val
        serial_obj.send_data(f"SETKdHBed{self.Kd_THedbed}")
        self.save_config(Kd_Hedbed = self.Kd_THedbed)
    def on_Kp_THotend_change(self, Kp_Hotend_val):
        self.Kp_THotend = Kp_Hotend_val
        serial_obj.send_data(f"SETKpHTend{self.Kp_THotend}")
        self.save_config(Kp_Hotend = self.Kp_THotend)
    def on_Ki_THoteng_change(self, Ki_Hotend_Val):
        self.Ki_THotend = Ki_Hotend_Val
        serial_obj.send_data(f"SETKiHTend{self.Ki_THotend}")
        self.save_config(Ki_Hotend = self.Ki_THotend)
    def on_Kd_THotend_change(self, Kd_hotend_Val):
        self.Kd_THotend = Kd_hotend_Val
        serial_obj.send_data(f"SETKdHTend{self.Kd_THotend}")
        self.save_config(Kd_Hotend = self.Kd_THotend)
    def on_BLUX_change(self, BLUX_Val):
        self.BLUX = BLUX_Val
        serial_obj.send_data(f"SETBLUx{self.BLUX}")
        self.save_config(BLUX = self.BLUX)
    def on_BLUY_change(self, BLUY_Val):
        self.BLUY = BLUY_Val
        serial_obj.send_data(f"SETBLUy{self.BLUY}")
        self.save_config(BLUY = self.BLUY)
    def on_BLUZ_change(self, BLUZ_Val):
        self.BLUZ = BLUZ_Val
        serial_obj.send_data(f"SETBLUz{self.BLUZ}")
        self.save_config(BLUZ = self.BLUZ)
    def on_BLUE_change(self, BLUE_Val):
        self.BLUE = BLUE_Val
        serial_obj.send_data(f"SETBLUz{self.BLUE}")
        self.save_config(BLUE = self.BLUE)

    def toggle_settings(self):
        self.currentMenu = self.settingMenu
        if self.padding_frame:
            self.padding_frame.destroy()  # Xóa frame cũ
        # Tạo frame mới
        self.padding_frame = tk.Frame(self.frame)
        self.padding_frame.configure(width=130, height= 150, bg= "#2e2e2e")
        self.create_setting_menu_widget()
        self.padding_frame.place(x=5, y=80)
    # Hàm hiển thị màn hình điều khiển
    def off_setting_menu(self):
        self.currentMenu = self.mainMenu
        self.padding_frame.place_forget()
        if self.padding_frame:
            self.padding_frame.destroy()  # Xóa frame cũ
        # Tạo frame mới
    def show_axis_resolution_menu(self):
        self.currentMenu = self.axisResolutionMenu
        if self.padding_frame:
            self.padding_frame.destroy()  # Xóa frame cũ
        # Tạo frame mới
        self.padding_frame = tk.Frame(self.frame)
        self.padding_frame.configure(width= 840, height= 480, bg= "#0F0A3F")
        self.create_axis_resolution_menu_widgets()
        self.padding_frame.place(x= 0, y = 0)
    def off_axis_resolution_menu(self):
        self.currentMenu = self.mainMenu
        self.padding_frame.place_forget()
        if self.padding_frame:
            self.padding_frame.destroy()  # Xóa frame cũ
        # Tạo frame mới
   
    def show_manControl_menu(self):
        self.currentMenu =self.manControlMenu
        if self.padding_frame:
            self.padding_frame.destroy()  # Xóa frame cũ
        # Tạo frame mới
        self.padding_frame = tk.Frame(self.frame)
        self.padding_frame.configure(width= 840, height= 480, bg= "#0F0A3F")
        self.create_man_control_menu_widgets()
        self.padding_frame.place(x= 0, y = 0)
    
    def off_manControl_menu(self):
        self.currentMenu = self.mainMenu
        self.padding_frame.place_forget()
        if self.padding_frame:
            self.padding_frame.destroy()  # Xóa frame cũ
        
    
    def show_PID_Paremeter_menu(self):
        self.currentMenu == self.PIDParameterMenu
        if self.padding_frame:
            self.padding_frame.destroy()  # Xóa frame cũ
        # Tạo frame mới
        self.padding_frame = tk.Frame(self.frame)
        self.padding_frame.configure(width= 840, height= 480, bg= "#0F0A3F")
        self.create_PID_Parameter_menu_widgets()
        self.padding_frame.place(x= 0, y = 0)
    
    def off_PID_Parameter_menu(self):
        self.currentMenu = self.mainMenu
        self.padding_frame.place_forget()

    def blux_toggle_keyboard(self):
        global keyboard_frame
        if keyboard_frame is not None:
            if keyboard_frame.winfo_ismapped():
                keyboard_frame.place_forget()
            else:
                keyboard_frame.place(x=300, y=300)  # Vị trí đặt bàn phím
        else:
            # Nếu keyboard_frame là None, tạo mới
            keyboard_frame = show_keyboard(self.frame, self.bluxBox)  # Hiển thị bàn phím nếu chưa có

    def bluy_toggle_keyboard(self):
        global keyboard_frame
        if keyboard_frame is not None and keyboard_frame.winfo_ismapped():
            keyboard_frame.place_forget()  # Ẩn bàn phím nếu nó đang hiển thị
        else:
            keyboard_frame = show_keyboard(self.frame, self.bluyBox)  # Hiển thị bàn phím nếu chưa có

    def bluz_toggle_keyboard(self):
        global keyboard_frame
        if keyboard_frame is not None and keyboard_frame.winfo_ismapped():
            keyboard_frame.place_forget()  # Ẩn bàn phím nếu nó đang hiển thị
        else:
            keyboard_frame = show_keyboard(self.frame, self.bluzBox)  # Hiển thị bàn phím nếu chưa có      

    def blue_toggle_keyboard(self):
        global keyboard_frame
        if keyboard_frame is not None and keyboard_frame.winfo_ismapped():
            keyboard_frame.place_forget()  # Ẩn bàn phím nếu nó đang hiển thị
        else:
            keyboard_frame = show_keyboard(self.frame, self.blueBox)  # Hiển thị bàn phím nếu chưa có
    def set_home(self):
        self.X = 0
        serial_obj.send_data(f"X{self.X}")
        self.Y = 0
        serial_obj.send_data(f"Y{self.Y}")
        self.Z = 0
        serial_obj.send_data(f"Z{self.Z}")
    
    # Hàm cập nhật đồng hồ
    def update_time(self):
        if self.running:  # Chỉ cập nhật khi đồng hồ đang chạy
            hours = self.seconds // 3600  # Tính số giờ
            minutes = (self.seconds % 3600) // 60  # Tính số phút
            secs = self.seconds % 60  # Tính số giây

            # Hiển thị thời gian dưới dạng HH:MM:SS
            time_str = f"{hours:02}:{minutes:02}:{secs:02}"
            self.timeBox.delete(0, tk.END)  # Xóa nội dung cũ
            self.timeBox.insert(tk.END, time_str)  # Chèn thời gian mới

            # Tăng số giây và gọi lại update_time sau 1 giây
            self.seconds += 1
            self.timeBox.after(1000, self.update_time)  # Cập nhật sau 1 giây

    # Hàm xử lý khi nhấn nút Start
    def start_button_callback(self):
        
        self.seconds = 0  # Reset lại thời gian về 0
        self.running = True  # Đánh dấu là đồng hồ đang chạy
        self.update_time()  # Bắt đầu hiển thị thời gian
        self.noteBox.delete(0, tk.END)
        self.noteBox.insert(tk.END, gCodeRead_Obj.gcode_commands[gCodeRead_Obj.current_command_index])
        gCodeRead_Obj.sendGcodeComand()

    # Hàm xử lý khi nhấn nút Pause
    def pause_button_callback(self):
        
        self.running = False  # Dừng đồng hồ khi nhấn Pause
        self.currentMenu = self.axisResolutionMenu
        if self.padding_frame:
            self.padding_frame.destroy()  # Xóa frame cũ
            # Tạo frame mới
            self.padding_frame = tk.Frame(self.frame)
            self.padding_frame.configure(width= 200, height= 200, bg= "white")
            self.create_menu_pause_widgets()
            self.padding_frame.place(x= 600, y = 60)
    def continue_button_callback(self):
        self.running = True
        self.update_time()
        self.currentMenu = self.mainMenu
        self.padding_frame.place_forget()
# Hàm xử lý khi nhấn nút Resume
    # Hàm để lưu các giá trị vào file JSON
    def save_config(self, **kwargs):
        # Load current config
        config = self.load_config()  # Đọc cấu hình hiện tại
        
        # Cập nhật các tham số thay đổi (dựa trên kwargs)
        config.update(kwargs)
        
        # Lưu lại vào file config.json
        with open('config.json', 'w') as f:
            json.dump(config, f, indent=4)

    # Hàm để tải giá trị từ file JSON
    def load_config(self):
        if os.path.exists(CONFIG_FILE):
            with open(CONFIG_FILE, 'r') as f:
                return json.load(f)
        else:
            return {"hotend_temp_value": 0}  # Nếu file không tồn tại, trả về giá trị mặc định

    
        

 
