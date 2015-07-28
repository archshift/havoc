# havoc

A cross-platform configuration tool for the [CM Storm Havoc](http://gaming.coolermaster.com/en/products/mice/havoc/) mouse.

## Compiling

In order to compile havoc, simply run:

```
mkdir build && cd build
cmake ..
make
```

### Dependencies

You must have installed on your computer:

 - `cmake`
 - `pkgconfig`
 - `libusb-1.0`
 - `tclap`

## Quirks

On certain distributions of Linux, you will either need to run this program as root, or add a udev rule for the CM Storm Havoc. For example, you might add this rule in `/etc/udev/rules.d/cmstorm-havoc.rules`:

```
SUBSYSTEM=="usb", ATTRS{idVendor}=="2516", ATTRS{idProduct}=="001d", MODE="0666"
SUBSYSTEM=="usb_device", ATTRS{idVendor}=="2516", ATTRS{idProduct}=="001d", MODE="0666"
```

## License

havoc is licensed under the GNU Public License version 2 or later. 
