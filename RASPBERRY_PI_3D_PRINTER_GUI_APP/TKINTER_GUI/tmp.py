import tkinter as tk

def on_scale_change(value):
    print("Giá trị thước cuộn:", value)

# Tạo cửa sổ chính
root = tk.Tk()
root.title("Thước cuộn tăng/giảm giá trị")

# Tạo widget Scale (thước cuộn)
scale = tk.Scale(root, from_=0, to=100, orient="horizontal", command=on_scale_change)
scale.pack()

# Chạy ứng dụng
root.mainloop()     
        
       