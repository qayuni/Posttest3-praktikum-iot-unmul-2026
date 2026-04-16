# 📘 README - Smart Dam IoT (Bendungan Pintar)

## 👥 Anggota Kelompok
- Qurrata A'yuni - 2309106001
- Nur Juzieatul Alifah - 2309106040
- Tiara Kasma Wati Putri - 2309106080

---

## 📌 Judul Studi Kasus
Sistem Monitoring dan Kontrol Bendungan Pintar Berbasis IoT Menggunakan MQTT dan Kodular

---

## 📖 Deskripsi
Proyek ini merupakan implementasi Internet of Things (IoT) untuk memonitor dan mengontrol miniatur bendungan secara real-time. Sistem menggunakan ESP32 sebagai mikrokontroler yang terhubung ke internet melalui WiFi dan menggunakan protokol MQTT sebagai media komunikasi data.

Sensor ketinggian air digunakan untuk membaca level air, kemudian data dikirim ke aplikasi Kodular untuk ditampilkan. Selain monitoring, sistem juga memungkinkan pengguna untuk mengontrol pintu air dan buzzer secara manual maupun otomatis.

Fitur utama:
- Monitoring ketinggian air secara real-time
- Mode otomatis berdasarkan kondisi air
- Mode manual untuk kontrol langsung
- Notifikasi bahaya menggunakan buzzer
- Kontrol pintu air menggunakan servo
- Kontrol buzzer secara manual
- Komunikasi menggunakan MQTT

---

## 🤖 Logika Sistem

Mode Otomatis:
- Level Aman (Sensor <= 800): Servo 0° (tertutup), buzzer mati
- Level Waspada (801 - 1500): Servo 90° (setengah), buzzer mati
- Level Bahaya (> 1500): Servo 180° (terbuka penuh), buzzer berkedip

Mode Manual:
- Servo dikontrol menggunakan button Tutup(0°), button Setengah(90°) dan button Terbuka(180°)
- Buzzer dikontrol menggunakan switch ON/OFF
- Mode otomatis tidak aktif saat manual digunakan

---

## 📱 Tampilan Aplikasi (Kodular)
Aplikasi dibuat menggunakan Kodular dengan fitur:
- Menampilkan nilai ketinggian air
- Menampilkan status level (Aman / Waspada / Bahaya)
- Menampilkan status buzzer (ON / OFF)
- Menampilkan posisi servo
- Tombol mode otomatis dan manual
- Tombol untuk kontrol servo
- Switch untuk kontrol buzzer

---

## 🧑‍💻 Pembagian Tugas
- Qurrata A'yuni (2309106001)  
  Mendesain tampilan aplikasi, membantu menyusun logika kontrol, serta melakukan pemrograman (coding) dan debugging.

- Nur Juzieatul Alifah (2309106040)  
  Mengatur logika aplikasi pada Kodular serta membantu dalam proses debugging kode.

- Tiara Kasma Wati Putri (2309106080)  
  Membuat board schematic menggunakan Wokwi, merancang rangkaian perangkat, serta mengedit video dokumentasi.

---

## 🔧 Komponen yang Digunakan
- ESP32
- Water Level Sensor
- Motor Servo
- Buzzer
- Kabel jumper
- Smartphone (Kodular)
- MQTT Broker (EMQX)

---

## 🧠 Teknologi yang Digunakan
- Arduino IDE
- MQTT Protocol
- Kodular
- WiFi

---

## 🔌 Alur Sistem
1. Sensor membaca ketinggian air
2. ESP32 memproses data
3. Data dikirim ke MQTT broker
4. Kodular menerima data dan menampilkan
5. User mengontrol melalui aplikasi (manual/otomatis)
6. ESP32 menerima perintah dan menggerakkan servo dan buzzer

---

## 🖼️ Gambar Board Schematic
ESP32 digunakan sebagai kontrol utama yang terhubung dengan relay, servo, dan buzzer.

<img width="906" height="584" alt="WhatsApp Image 2026-04-16 at 1 01 03 PM" src="https://github.com/user-attachments/assets/e1a4a495-b791-4c1f-b11c-8fa5f7df3061" />

### Relay
Relay digunakan sebagai simulasi sensor air.
- VCC → 3V
- GND → GND
- IN → GPIO 34

### Servo
Servo digunakan untuk membuka dan menutup pintu air.
- VCC → 5V
- GND → GND 
- Signal → GPIO 18  

### Buzzer
Buzzer digunakan sebagai alarm.
- (+) → GPIO 5
- (-) → GND

### Ringkasan GPIO
- Relay (Sensor Arir) → GPIO 34  
- Servo → GPIO 18  
- Buzzer → GPIO 5 

---

## 🚀 Catatan
- Pastikan koneksi WiFi stabil
- Gunakan topic MQTT yang sama antara ESP32 dan Kodular
- Mode otomatis dan manual tidak boleh aktif bersamaan
