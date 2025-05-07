`timescale 1ns / 1ps

module pwmGen (
    input wire clk,
    input wire rst,
    input wire [7:0] duty,
    output reg pwm_out
);

    reg [7:0] counter;

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            counter <= 8'd0;
            pwm_out <= 1'b0;
        end else begin
            counter <= counter + 1;
            pwm_out <= (counter < duty);
        end
    end

endmodule
