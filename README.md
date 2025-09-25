# 🎮 Catch the Fruit

**Catch the Fruit** là một trò chơi mini đơn giản, nơi bạn điều khiển một chiếc giỏ để hứng các loại trái cây rơi xuống.  
Nhiệm vụ của bạn là hứng được càng nhiều trái cây càng tốt, tránh để chúng rơi ra ngoài.  

---

## 📌 Yêu cầu hệ thống

- **Hệ điều hành:** Windows 7 trở lên  
- **Trình biên dịch:** MinGW  
- **IDE đề xuất:** 
  - Code::Blocks  
  - Visual Studio Code (có hướng dẫn chi tiết trong phần dưới)  
  - Visual Studio  

---

## ⚙️ Hướng dẫn cài đặt

### 1. Tải thư viện SFML
- Truy cập trang chính thức [SFML](https://www.sfml-dev.org/download.php) để tải phiên bản phù hợp.  
- Giải nén và lưu thư mục SFML tại ổ đĩa (ví dụ: `D:\SFML`).  

### 2. Clone repository
```bash
git clone https://github.com/tanghongai1704/Game_CatchTheFruit
cd Catch_the_fruit
```

### 3. Biên dịch source code

Mở terminal trong thư mục chứa source code và chạy lệnh:

```bash
g++ game.cpp -o game -ID:\SFML\include -LD:\SFML\lib -lsfml-graphics -lsfml-window -lsfml-system -O2 -s
```

### 4. Thêm file DLL

Copy các file sau từ thư mục **SFML/bin** vào cùng thư mục chứa `game.exe`:

- `sfml-graphics-...dll`  
- `sfml-window-...dll`  
- `sfml-system-...dll`  

👉 Điều này đảm bảo game có thể chạy được khi mở file `game.exe`.

---

### 5. Chạy game

- Nếu compile thành công, mở **`game.exe`** để bắt đầu chơi.  
- Nếu không cài đặt được, có thể sử dụng bản build sẵn trong thư mục **`demo_game/demo_game.exe`**.  

---

## ⚠️ Lưu ý

- Hướng dẫn trên chỉ áp dụng cho **Windows**.  
- Trên **Linux** và **macOS**, bạn có thể cài SFML qua package manager:

  **Ubuntu/Debian**:
  ```bash
  sudo apt install libsfml-dev
  ```
  **macOS (Homebrew):**
  ```bash
  brew install sfml
  ```
- Cần đảm bảo **phiên bản MinGW và SFML tương thích**.  
- Luôn copy đầy đủ file **DLL** vào thư mục chứa file `.exe` thì trò chơi mới chạy được.  

---

## 📧 Liên hệ  

Nếu bạn gặp bất kỳ vấn đề nào trong quá trình cài đặt hoặc sử dụng, hãy tạo **issue** trên repository.

---

✨ Chúc bạn chơi game vui vẻ!
