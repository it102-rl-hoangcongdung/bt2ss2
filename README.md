# BÁO CÁO PHÂN TÍCH VÀ THIẾT KẾ GIẢI PHÁP: PHÂN HỆ TÍNH CƯỚC CHUYẾN XE ĐA YẾU TỐ THỜI GIAN THỰC

> 👤 **Học viên:** Hoàng Công Dũng | **Mã SV:** PTIT-HCM-009
> 🏫 **Môn học:** it102-rl-hoangcongdung

---

## 📊 Sơ đồ thiết kế hệ thống (Activity Diagram)

> 💡 *Sơ đồ dưới đây được render tự động trực tiếp trên GitHub bằng Mermaid. Bạn cũng có thể tải file **`bt2.drawio`** trong repository này để mở và chỉnh sửa trực tiếp trên [Draw.io (diagrams.net)](https://app.diagrams.net).* 

```mermaid
graph TD
  Start([Bắt đầu]) --> Input[Nhập vehicle_type, distance, is_peak_or_rain]
  Input --> CheckDist{distance <= 0.0 hoặc distance > 300.0?}
  CheckDist -- Đúng --> ErrDist[Báo lỗi cự ly bất thường] --> EndFail([Kết thúc: Hủy giao dịch])
  CheckDist -- Sai --> CheckType{vehicle_type hợp lệ (1 hoặc 2)?}
  CheckType -- Sai --> ErrType[Báo lỗi mã loại xe không hợp lệ] --> EndFail
  CheckType -- Đúng --> CheckPeak{is_peak_or_rain hợp lệ (0 hoặc 1)?}
  CheckPeak -- Sai --> ErrPeak[Báo lỗi trạng thái phụ phí không hợp lệ] --> EndFail
  CheckPeak -- Đúng --> CheckThreshold{distance <= 2.0 km?}
  CheckThreshold -- Đúng (Ngưỡng tối thiểu) --> CalcMin[Áp dụng cước cố định 2km đầu]
  CheckThreshold -- Sai (Vượt 2km) --> CalcExtra[Tính cước cơ sở theo số km vượt]
  CalcMin --> Fork[=== Phân nhánh tính toán tài chính ===]
  CalcExtra --> Fork
  Fork --> BaseFare[Xác định Cước cơ sở hoàn chỉnh]
  BaseFare --> CheckSurcharge{is_peak_or_rain == 1?}
  CheckSurcharge -- Có (1) --> ApplySurcharge[Nhân hệ số 1.2x phụ phí biến động]
  CheckSurcharge -- Không (0) --> NoSurcharge[Phụ phí bằng 0 VNĐ]
  ApplySurcharge --> Join[=== Tổng hợp kết quả ===]
  NoSurcharge --> Join
  Join --> TotalFare[Tính Tổng cước = Cước cơ sở + Phụ phí]
  TotalFare --> Display[Hiển thị hóa đơn chi tiết trên Console] --> EndSuccess([Kết thúc: Giao dịch thành công])
```

---

## Nhiệm vụ 1: Phân tích bài toán (I/O)

Để xây dựng mô-đun tính cước TripFare cho ứng dụng GrabRide, hệ thống cần tiếp nhận chính xác 3 tham số đầu vào từ bàn phím và trả về kết quả tài chính minh bạch cho hành khách và tài xế.

Các tham số đầu vào và đầu ra được định nghĩa chi tiết theo chuẩn kiểu dữ liệu ngôn ngữ C như sau:

- Tham số vehicle_type (Kiểu int): Mã phân loại dịch vụ phương tiện ('1' là GrabBike - Xe máy, '2' là GrabCar - Ô tô 4 chỗ).
- Tham số distance (Kiểu double): Khoảng cách thực tế của chuyến đi tính bằng đơn vị kilômét (km).
- Tham số is_peak_or_rain (Kiểu int): Trạng thái phụ phí thời tiết hoặc giờ cao điểm ('1' là Có phụ phí, '0' là Bình thường).
- Đầu ra console: In ra màn hình cước phí cơ sở, phụ phí biến đổi, tổng tiền cước thanh toán bằng VNĐ hoặc thông báo lỗi nghiệp vụ chi tiết nếu dữ liệu không hợp lệ.

## Nhiệm vụ 2: Đề xuất giải pháp và Thiết kế thuật toán rẽ nhánh

Thuật toán xử lý được xây dựng dựa trên cấu trúc điều kiện lồng nhau 'if-else' để đảm bảo không bỏ sót bất kỳ quy tắc nghiệp vụ nào. Các tầng kiểm tra lỗi biên (Boundary Check) được đặt ở vị trí cao nhất nhằm vô hiệu hóa lập tức các dữ liệu rác, cự ly âm hoặc mã xe không tồn tại.

Phương án xử lý ngưỡng cự ly tối thiểu '2.0 km' được kiểm soát chặt chẽ để tránh việc các chuyến đi ngắn bị chia nhỏ tiền cước, bảo vệ chính sách doanh thu tối thiểu của hãng.

- Bước 1: Kiểm tra tính hợp lệ của cự ly di chuyển ('distance > 0.0' và 'distance <= 300.0'). Nếu vi phạm, dừng chương trình và báo lỗi.
- Bước 2: Kiểm tra mã loại xe ('vehicle_type' phải bằng '1' hoặc '2') và trạng thái phụ phí ('is_peak_or_rain' phải bằng '0' hoặc '1').
- Bước 3: Phân loại cước cơ sở dựa trên hạng xe và áp dụng quy tắc chống chia nhỏ cước cho các chuyến đi ngắn dưới hoặc bằng '2.0 km'.
- Bước 4: Tính toán phụ phí biến đổi dựa trên hệ số nhân '1.2x' nếu có giờ cao điểm / trời mưa, sau đó cộng dồn ra tổng cước thanh toán.

## Nhiệm vụ 3: Bảng đặc tả kiểm soát lỗi dữ liệu biên (Edge Cases)

Bảng dưới đây mô tả các kịch bản kiểm tra lỗi dữ liệu đầu vào mà mô-đun 'TripFare' bắt buộc phải chặn đứng trước khi thực hiện bất kỳ phép tính tài chính nào:

| Mã lỗi | Loại dữ liệu dị biệt | Giá trị giả lập | Hành động xử lý của thuật toán | Thông báo hiển thị |
| --- | --- | --- | --- | --- |
| Lỗi 1 | Cự ly âm hoặc bằng 0 | distance = -1.5 hoặc 0.0 | Chặn ngay lập tức tại tầng if đầu tiên | Lỗi: Khoảng cách chuyến đi không hợp lệ! |
| Lỗi 2 | Cự ly vượt hạn mức đô thị | distance = 350.5 | Từ chối xử lý chuyến đi đường dài ngoài luồng | Lỗi: Cự ly vượt quá giới hạn cho phép (300km)! |
| Lỗi 3 | Mã loại xe không tồn tại | vehicle_type = 5 | Hủy hóa đơn, không tính tiền cước | Lỗi: Mã phân loại dịch vụ không tồn tại! |
| Lỗi 4 | Trạng thái phụ phí sai quy định | is_peak_or_rain = 9 | Hủy giao dịch để tránh sai lệch tài chính | Lỗi: Trạng thái phụ phí không hợp lệ (Chỉ nhận 0 hoặc 1)! |

---

## 📁 Danh sách tệp tin nộp bài trong Repository
- 📝 `bt2.docx`: Báo cáo tài liệu phân tích nghiệp vụ hoàn chỉnh.
- 🎨 `bt2.drawio`: File thiết kế sơ đồ chuẩn theo quy định đề bài (mở trực tiếp bằng [Draw.io](https://app.diagrams.net) hoặc Lucidchart).
- 💻 `main.c`: Mã nguồn chương trình.
