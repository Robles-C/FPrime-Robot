`timescale 1ns / 1ps

module top_tb;

    reg clk = 0;
    reg rst = 1;
    reg scl = 1;
    reg sda_driver = 1;
    reg sda_drive_en = 1;
    wire sda;
    wire pwm_out;

    assign sda = sda_drive_en ? sda_driver : 1'bz;

    top uut (
        .clk(clk),
        .rst(rst),
        .scl(scl),
        .sda(sda),
        .pwm_out(pwm_out)
    );

    always #5 clk = ~clk;

    task i2c_start;
        begin
            sda_driver = 1; 
            sda_drive_en = 1;
            scl = 1;     
            #10;

            sda_driver = 0;  
            #10;

            scl = 0;       
            #10;
        end
    endtask

    task i2c_stop;
        begin
            sda_driver = 0; #10;
            scl = 1; #10;
            sda_driver = 1; #10;
            sda_drive_en = 0;
            #10; 
        end
    endtask


    task i2c_write_byte(input [7:0] byte);
        integer i;
        begin
            for (i = 7; i >= 0; i = i - 1) begin
                sda_driver = byte[i];
                sda_drive_en = 1;
                #5;     
                scl = 1; #10;
                scl = 0; #10;
            end

            sda_drive_en = 0;
            #2;
            scl = 1; #10;
            scl = 0; #5;
        end
    endtask




    initial $display("Simulation starting at time %t", $time);

    initial begin
        //$vcdpluson;

        rst = 1;
        #50 
        rst = 0;

        i2c_start();
        i2c_write_byte(8'h84); 
        i2c_write_byte(8'hAB);  // byte 1
        i2c_write_byte(8'h40);  // byte 2 pwm
        i2c_stop();

        #10000;
        $finish;
    end

endmodule
