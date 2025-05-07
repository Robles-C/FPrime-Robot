`timescale 1ns / 1ps

module i2cSlave (
    input wire clk,
    input wire rst,
    input wire scl,
    inout wire sda,
    output reg [7:0] pwm_value
);
    integer log_fd;
    initial log_fd = $fopen("i2c_debug.log", "w");

    reg prev_sda = 1'b1;
    reg prev_scl = 1'b1;

    reg sda_out;
    reg sda_oe;
    wire sda_in;
    
    reg [1:0] scl_sync;
    reg [1:0] sda_sync;


    assign sda = sda_oe ? sda_out : 1'bz;
    assign sda_in = sda;

    parameter [2:0] IDLE = 3'd0,
                    ADDRESS = 3'd1,
                    ACK_ADDR = 3'd2,
                    DATA1 = 3'd3,
                    ACK1 = 3'd4,
                    DATA2 = 3'd5,
                    ACK2 = 3'd6,
                    STOP = 3'd7;

    reg [2:0] state;
    reg [7:0] shift_reg;
    reg [2:0] bit_count;
    reg [7:0] byte1, byte2;

    parameter SLAVE_ADDR = 7'h42;

    wire scl_rising      = (scl_sync == 2'b01);
    wire scl_falling     = (scl_sync == 2'b10);
    wire start_condition = (sda_sync == 2'b10 && scl_sync[1]);
    wire stop_condition  = (sda_sync == 2'b01 && scl_sync[1]); 



    always @(posedge clk) begin
        scl_sync <= {scl_sync[0], scl};
        sda_sync <= {sda_sync[0], sda};
    end


    always @(posedge clk or posedge rst) begin
        if (rst) begin
            state <= IDLE;
            sda_oe <= 0;
            sda_out <= 1;
            pwm_value <= 0;
            bit_count <= 0;
        end else begin
            case (state)
                IDLE: begin
                    sda_oe <= 0;
                    $display("FSM is in IDLE at %t", $time);
                    if (start_condition) begin
                        $display("START DETECTED at %t", $time);
                        state <= ADDRESS;
                        bit_count <= 0;
                    end
                end

                ADDRESS: begin
                    if (scl_rising) begin
                        shift_reg <= {shift_reg[6:0], sda_in};
                        bit_count <= bit_count + 1;
                        if (bit_count == 3'd7) begin
                            $display(log_fd, "Address received: %b", {shift_reg[6:0], sda_in});
                            state <= ACK_ADDR;
                        end
                    end
                end

                ACK_ADDR: begin
                    if (scl_falling) begin
                        $display(log_fd, "Address received (ACK always): %h", shift_reg[7:1]);
                        sda_out <= 0;      // drive SDA low- ACK
                        sda_oe <= 1;
                        state <= DATA1;
                        bit_count <= 0;
                    end
                    if (scl_rising)
                        sda_oe <= 0;
                end
                

                DATA1: begin
                    sda_oe <= 0;
                    if (scl_rising) begin
                        shift_reg <= {shift_reg[6:0], sda_in};
                        bit_count <= bit_count + 1;
                        if (bit_count == 3'd7) begin
                            byte1 <= {shift_reg[6:0], sda_in};
                            state <= ACK1;
                        end
                    end
                end

                ACK1: begin
                    if (scl_falling) begin
                        sda_out <= 0;
                        sda_oe <= 1;
                        bit_count <= 0;
                        state <= DATA2;
                    end
                    if (scl_rising)
                        sda_oe <= 0;
                end

                DATA2: begin
                    sda_oe <= 0;
                    if (scl_rising) begin
                        shift_reg <= {shift_reg[6:0], sda_in};
                        bit_count <= bit_count + 1;
                        if (bit_count == 3'd7) begin
                            byte2 <= {shift_reg[6:0], sda_in};
                            state <= ACK2;
                        end
                    end
                end

                ACK2: begin
                    if (scl_falling) begin
                        sda_out <= 0;
                        sda_oe <= 1;
                        pwm_value <= byte2;
                        $display(log_fd, "PWM VALUE SET TO %h from byte2 at %t", byte2, $time);
                        state <= STOP;
                    end
                    if (scl_rising)
                        sda_oe <= 0;
                end

                STOP: begin
                    if (stop_condition) begin
                        sda_oe <= 0;
                        state <= IDLE;
                    end
                end

                default: state <= IDLE;
            endcase
        end
    end
endmodule
