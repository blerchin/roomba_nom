import printer

p=printer.ThermalPrinter(serialport="/dev/ttyAMA0")
p.print_text("\nHello Ben\n")
p.linefeed()
p.linefeed()
p.linefeed()
p.linefeed()
p.linefeed()
