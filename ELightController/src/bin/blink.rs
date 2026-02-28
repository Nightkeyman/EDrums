//! GPIO test: blink LD2 (PA5) + toggle PA2 (DMX TX) + PA10 (DE).
//! Use multimeter to verify PA2 and PA10 change between 0V and 3.3V.

#![no_std]
#![no_main]

use panic_halt as _;

use cortex_m_rt::entry;
use stm32f4xx_hal::{
    gpio::GpioExt,
    pac,
    prelude::*,
    rcc::{Config, RccExt},
};

#[entry]
fn main() -> ! {
    let dp = pac::Peripherals::take().unwrap();
    let cp = cortex_m::peripheral::Peripherals::take().unwrap();

    let mut rcc = dp.RCC.freeze(Config::hsi().sysclk(84.MHz()));
    let mut delay = cp.SYST.delay(&rcc.clocks);

    let gpioa = dp.GPIOA.split(&mut rcc);
    let mut led = gpioa.pa5.into_push_pull_output();
    let mut pa2 = gpioa.pa2.into_push_pull_output();   // DMX TX pin
    let mut pa10 = gpioa.pa10.into_push_pull_output();  // DE/RE pin

    loop {
        led.set_high();
        pa2.set_high();
        pa10.set_high();
        delay.delay_ms(500u32);

        led.set_low();
        pa2.set_low();
        pa10.set_low();
        delay.delay_ms(500u32);
    }
}
