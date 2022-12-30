#include <libusb-1.0/libusb.h>
#include <cstdio>
#include <iostream>

void printdev(libusb_device* dev);


int main(){
    libusb_device** devs; // указатель на указатель на устройство, используется для получения списка устройств
    libusb_context* ctx = NULL; // контекст сессии libusb


    int r; // для возвращаемых значений
    ssize_t cnt; // число найденных USB-устройств
    r = libusb_init(&ctx); // инициализировать библиотеку libusb, открыть сессию работы с libusb


    if (r < 0){
        fprintf(stderr, "Ошибка: инициализация не выполнена, код: %d.\n", r);
        return 1;
    }


    // получить список всех найденных USB- устройств
    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0){
        fprintf(stderr, "Ошибка: список USB устройств не получен. Код: %d\n", r);
        return 1;
    }


    printf("devices found: %ld\n", cnt);


    for (ssize_t i = 0; i < cnt; i++) { // цикл перебора всех устройств
        printdev(devs[i]); // печать параметров устройства
    }


    libusb_free_device_list(devs, 1); // освободить память, выделенную функцией получения списка устройств
    libusb_exit(ctx); // завершить работу с библиотекой libusb, закрыть сессию работы с libusb


    return 0;
}


void printdev(libusb_device* dev){
    libusb_device_descriptor desc{}; // дескриптор устройства
    libusb_device_handle* handle = NULL;


    unsigned char str[256];


    int r = libusb_get_device_descriptor(dev, &desc); // получить дескриптор
    if (r < 0){
        fprintf(stderr, "Ошибка: дескриптор устройства не получен, код: %d.\n", r);
        return;
    }


    printf("class: %.2x vendor: %.4x product: %.4x ",
        (int)desc.bDeviceClass,

        desc.idVendor,
        desc.idProduct
    );


    libusb_open(dev, &handle);
    printf("serial number: ");
    if (handle && desc.iSerialNumber) {
        r = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, str, sizeof(str));
        if (r != 0) {
            printf("%s\n", str);
        }
        else{
            printf("null\n");
        }
    }
    else {
        printf("empty\n");
    }
}
