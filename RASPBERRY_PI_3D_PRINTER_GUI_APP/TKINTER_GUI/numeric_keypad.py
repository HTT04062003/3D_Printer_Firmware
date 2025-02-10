import tkinter as tk

# Hàm thêm số hoặc dấu chấm vào entry
def add_digit(digit, entry):
    current_value = entry.get()  # Lấy giá trị hiện tại trong entry
    if digit == '.' and '.' in current_value:
        return  # Không cho phép nhập dấu chấm khi đã có dấu chấm
    entry.delete(0, tk.END)  # Xóa giá trị cũ trong entry
    entry.insert(tk.END, current_value + digit)  # Thêm số hoặc dấu chấm vào cuối giá trị hiện tại

# Hàm xử lý nút backspace
def backspace(entry):
    current_value = entry.get()  # Lấy giá trị hiện tại trong entry
    if current_value:  # Kiểm tra nếu có giá trị
        entry.delete(len(current_value)-1, tk.END)  # Xóa ký tự cuối

# Hàm xử lý khi nhấn Enter
def enter_key(entry, keyboard_frame):
    print("Nút Enter được nhấn. Giá trị hiện tại trong Entry:", entry.get())
    keyboard_frame.place_forget()  # Ẩn bàn phím khi nhấn Enter

# Hàm hiển thị bàn phím
def show_keyboard(root, entry):
    # Tạo một frame cho bàn phím
    keyboard_frame = tk.Frame(root, bg="gray", width=300, height=200)
    keyboard_frame.place(x=260, y=130)  # Hiển thị bàn phím dưới cùng của cửa sổ chính
    
    # Tạo các nút số từ 0 đến 9 và gán sự kiện cho chúng
    buttons = [
        ('1', 0, 0), ('2', 1, 0), ('3', 2, 0),
        ('4', 0, 1), ('5', 1, 1), ('6', 2, 1),
        ('7', 0, 2), ('8', 1, 2), ('9', 2, 2),
        ('0', 1, 3),
        ('.', 2, 3),  # Nút dấu chấm
        ('←', 0, 3),  # Nút backspace
        ('OK', 0, 4),  # Nút OK
        ('Enter', 1, 4)  # Nút Enter
    ]
    
    for (text, x, y) in buttons:
        if text == '←':  # Nút backspace
            button = tk.Button(keyboard_frame, text=text, font=("Arial", 10), width=3, height=1, command=lambda: backspace(entry))
        elif text == 'OK':  # Nút OK
            button = tk.Button(keyboard_frame, text=text, font=("Arial", 10), width=3, height=1, command=keyboard_frame.place_forget)
        elif text == 'Enter':  # Nút Enter
            button = tk.Button(keyboard_frame, text=text, font=("Arial", 10), width=3, height=1, command=lambda: enter_key(entry, keyboard_frame))
        else:  # Các nút số và dấu chấm
            button = tk.Button(keyboard_frame, text=text, font=("Arial", 10), width=3, height=1, command=lambda digit=text: add_digit(digit, entry))
        button.grid(row=y, column=x, padx=2, pady=2)
    
    return keyboard_frame