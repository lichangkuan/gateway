#include "app_device.h"
#include <unistd.h>
#include "app_bt.h"

int main(int argc, char const *argv[])
{
    Device *device = app_device_init("/home/proof/桌面/gateway/serial_test");

    app_bt_init(device);

    app_device_start();

    sleep(100);

    app_device_close();

    return 0;
}
