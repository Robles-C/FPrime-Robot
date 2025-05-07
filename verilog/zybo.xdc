# System Clock
set_property -dict { PACKAGE_PIN K17 IOSTANDARD LVCMOS33 } [get_ports clk]
create_clock -add -name sys_clk_pin -period 10.00 -waveform {0 5} [get_ports clk]

# Reset
set_property -dict { PACKAGE_PIN Y16 IOSTANDARD LVCMOS33 } [get_ports rst]


# I2C lines (on Pmod JC)
set_property -dict { PACKAGE_PIN V15 IOSTANDARD LVCMOS33 } [get_ports scl]
set_property -dict { PACKAGE_PIN W15 IOSTANDARD LVCMOS33 } [get_ports sda]

# PWM output (on Pmod JC)
set_property -dict { PACKAGE_PIN T11 IOSTANDARD LVCMOS33 } [get_ports pwm_out]
