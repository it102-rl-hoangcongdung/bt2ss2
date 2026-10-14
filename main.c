#include <stdio.h>

int main() {
    int vehicle_type;
    double distance;
    int is_peak_or_rain;

    // Nhập dữ liệu đầu từ bàn phím
    printf("=== PHAN HE TINH CUOC GRABRIDE ===\n");
    printf("Nhap ma loai xe (1: GrabBike, 2: GrabCar): ");
    if (scanf("%d", &vehicle_type) != 1) {
        printf("Loi: Dinh dang dau vao khong hop le!\n");
        return 1;
    }

    printf("Nhap khoang cach di chuyen (km): ");
    if (scanf("%lf", &distance) != 1) {
        printf("Loi: Dinh dang dau vao khong hop le!\n");
        return 1;
    }

    printf("Nhap trang thai phu phi (0: Binh thuong, 1: Cao diem/Mua): ");
    if (scanf("%d", &is_peak_or_rain) != 1) {
        printf("Loi: Dinh dang dau vao khong hop le!\n");
        return 1;
    }

    // 1. Kiem tra loi bien ve cu ly (Loi 1)
    if (distance <= 0.0 || distance > 300.0) {
        printf("Loi nghiep vu: Khoảng cách di chuyển khong hop le (0.0 < distance <= 300.0).\n");
        return 0;
    }

    // 2. Kiem tra loi bien ve ma loai xe va trang thai phu phi (Loi 2)
    if ((vehicle_type != 1 && vehicle_type != 2) || (is_peak_or_rain != 0 && is_peak_or_rain != 1)) {
        printf("Loi nghiep vu: Ma loai xe hoac trang thai phu phi khong ton tai trong he thong.\n");
        return 0;
    }

    double base_fare = 0.0;
    double dynamic_surcharge = 0.0;
    double total_fare = 0.0;

    // 3. Tinh cuoc co so theo quy tac nghiep vu
    if (vehicle_type == 1) {
        // GrabBike
        if (distance <= 2.0) {
            base_fare = 12000.0; // Khoang cach <= 2km ap dung muc gia co dinh (Loi 3)
        } else {
            base_fare = 12000.0 + (distance - 2.0) * 4500.0;
        }
    } else if (vehicle_type == 2) {
        // GrabCar
        if (distance <= 2.0) {
            base_fare = 25000.0; // Khoang cach <= 2km ap dung muc gia co dinh (Loi 3)
        } else {
            base_fare = 25000.0 + (distance - 2.0) * 10000.0;
        }
    }

    // 4. Tinh phu phi bien doi theo thoi gian thuc
    if (is_peak_or_rain == 1) {
        dynamic_surcharge = base_fare * 0.2; // Phu phi them 20% (he so 1.2x tong)
    } else {
        dynamic_surcharge = 0.0;
    }

    // 5. Tong cuoc thanh toan cuoi cung
    total_fare = base_fare + dynamic_surcharge;

    // Hien thi hoa don chi tiet len console
    printf("\n--- HOA DON CHI TIET CHUYEN DI ---\n");
    printf("Loai dich vu: %s\n", (vehicle_type == 1) ? "GrabBike (Xe may)" : "GrabCar (O to 4 cho)");
    printf("Khoang cach: %.2f km\n", distance);
    printf("Trang thai: %s\n", (is_peak_or_rain == 1) ? "Co phu phi (Cao diem/Mua)" : "Binh thuong");
    printf("-----------------------------------\n");
    printf("Cuoc co so (Base Fare): %.0f VND\n", base_fare);
    printf("Phu phi bien doi (Surcharge): %.0f VND\n", dynamic_surcharge);
    printf("Tong tien thanh toan: %.0f VND\n", total_fare);
    printf("===================================\n");

    return 0;
}