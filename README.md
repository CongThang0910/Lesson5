# UART STM32F103 – Giao tiếp với máy tính

## 📌 Mô tả
Chương trình sử dụng **UART1 trên STM32F103** để giao tiếp với máy tính:
- Khi khởi động, gửi chuỗi `"Hello from STM32!"` về terminal.
- Nếu người dùng gõ **"on"** → LED (PC13) bật, và STM32 gửi chuỗi `"LED on"`.
- Nếu người dùng gõ **"off"** → LED (PC13) tắt, và STM32 gửi chuỗi `"LED off"`.
- Nếu người dùng gõ chuỗi bất kỳ khác → STM32 echo lại chuỗi đó, ví dụ:

## 🛠️ Yêu cầu phần cứng
- **Board**: STM32F103C8T6 (Blue Pill)
- **LED**: gắn sẵn trên **PC13**
- **USB-TTL** để kết nối với máy tính
- **Kết nối chân**:
  - PA9 (TX)  → RX của USB-TTL
  - PA10 (RX) → TX của USB-TTL
  - GND      ↔ GND

## ⚙️ Cấu hình
- **GPIO (PC13 – LED)**: Output Push-Pull, speed 2 MHz, mặc định mức cao (LED tắt ban đầu).
- **UART1 (PA9 – TX, PA10 – RX)**: Baudrate 9600, 8 data bits, 1 stop bit, no parity, mode TX + RX, bật ngắt RXNE.
- **NVIC**: Bật ngắt `USART1_IRQn`, ưu tiên `PreemptionPriority = 0`, `SubPriority = 0`.

## 🖥️ Terminal
Mở phần mềm terminal (PuTTY, TeraTerm, RealTerm, …) với cấu hình:
- Baudrate: 9600
- Data bits: 8
- Stop bits: 1
- Parity: None

**Kết quả mong đợi**:
- Sau reset, terminal hiển thị:
