#![no_std]
#![no_main]
/*
fan_control 
-----------
   board we act studion stm32 f411ceu6
   pin used 
      c13 led
      b0 input pull up
      a0 pwm output 22 kHz
*/

use defmt::*;
use embassy_executor::Spawner;
use embassy_stm32::gpio::{Input, Level, Output, Pull, Speed};
use embassy_time::Timer;
use embassy_stm32::time::Hertz;
use embassy_stm32::{bind_interrupts, peripherals, usb, Config};
use embassy_stm32::gpio::OutputType;
use embassy_stm32::time::khz;
use embassy_stm32::timer::simple_pwm::{PwmPin, SimplePwm};
use embassy_stm32::timer::Channel;

use {defmt_rtt as _, panic_probe as _};

#[embassy_executor::main]
async fn main(_spawner: Spawner) {

    let mut config = Config::default();
    {
        use embassy_stm32::rcc::*;
        config.rcc.hse = Some(Hse {
            freq: Hertz(25_000_000),
            mode: HseMode::Oscillator,
        });
        config.rcc.pll_src = PllSource::HSE;
        config.rcc.pll = Some(Pll {
            prediv: PllPreDiv::DIV25,
            mul: PllMul::MUL192,
            divp: Some(PllPDiv::DIV2), // 25mhz / 25 * 192 / 2 = 96Mhz.
            divq: Some(PllQDiv::DIV4), // 25mhz / 25 * 192 / 4 = 48Mhz.
            divr: None,
        });
        config.rcc.ahb_pre = AHBPrescaler::DIV1;
        config.rcc.apb1_pre = APBPrescaler::DIV2;
        config.rcc.apb2_pre = APBPrescaler::DIV2;
        config.rcc.sys = Sysclk::PLL1_P;
        config.rcc.mux.clk48sel = mux::Clk48sel::PLL1_Q;
    }
    let p = embassy_stm32::init(config);
   //let p = embassy_stm32::init(Default::default());
    info!("rcc initialized");
    let mut led = Output::new(p.PC13, Level::High, Speed::Low);
    let ch1 = PwmPin::new_ch1(p.PA0, OutputType::PushPull);
    let mut pwm = SimplePwm::new(p.TIM2, Some(ch1), None, None, None, khz(22), Default::default());
    let max = pwm.get_max_duty();
    pwm.enable(Channel::Ch1);
    info!("PWM initialized");
    info!("PWM max duty {}", max);
    let button = Input::new(p.PB0, Pull::Up);
    let mut koefficient:f32 = 0.02;
    let mut fan_pwm_speed;
    let mut wait_time_ms;
    loop {
        if button.is_low(){
            wait_time_ms = 100;
            koefficient = koefficient + 0.01;
            if koefficient >= 0.7{
                koefficient = 0.02;
            }
        }else{
            wait_time_ms = 500;
        }
        fan_pwm_speed = ((max as f32)*koefficient) as u32 ;
        if led.is_set_high(){
            led.set_low();    
        }else {
            led.set_high();
        }
        pwm.set_duty(Channel::Ch1, fan_pwm_speed);
        Timer::after_millis(wait_time_ms).await;
    }
}
