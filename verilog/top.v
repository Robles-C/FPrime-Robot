module top (
    input wire clk,
    input wire rst,
    input wire scl,
    inout wire sda,
    output wire pwm_out
);

    wire [7:0] pwm_val;

    i2cSlave i2c_inst (
        .clk(clk),
        .rst(rst),
        .scl(scl),
        .sda(sda),
        .pwm_value(pwm_val)
    );

    pwmGen pwm_inst (
        .clk(clk),
        .rst(rst),
        .duty(pwm_val),
        .pwm_out(pwm_out)
    );

endmodule
