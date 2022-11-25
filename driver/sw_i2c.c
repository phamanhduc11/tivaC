#include <stdio.h>
// I2C implement
//
// SCL
// SDA
//

bool gpio_state = false;
#define SCL_PIN 
#define SDA_PIN



void set_SDA(void);
void clr_SDA(void);
void set_SCL(void);
void clr_SCL(void);

void i2c_start(void);
void i2c_stop(void);
void i2c_delay(void);


void i2c_write_byte(void);
void i2c_read_byte(void);
void i2c_write_mul_bytes(void);
void i2c_read_mul_bytes(void);

void i2c_delay(void) {
    __nop__();
    __nop__();
    __nop__();
    __nop__();
    __nop__();
    __nop__();
    __nop__();
    __nop__();
}

void i2c_start(void) {
    set_SDA();
    set_SCL();
    i2c_delay();
    clear_SDA();
    i2c_delay();
    clear_SCL();
    i2c_delay();
}

void i2c_stop(void) {
    clear_SDA();
    i2c_delay();
    set_SCL();
    i2c_delay();
    set_SDA();
    i2c_delay();
}