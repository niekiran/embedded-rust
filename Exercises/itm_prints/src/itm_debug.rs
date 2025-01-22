
use cortex_m::peripheral;

pub fn itm_init(peripherals: &mut peripheral::Peripherals) {
    unsafe {
    //Program the prescaler value for the baud rate of the Serial Wire Output 
    //(SWO). Asynchronous Clock Prescaler Register, TPIU_ACPR [C1.10.4]
    let prescaler = 7;
    peripherals.TPIU.acpr.write(prescaler);

    //Enable TRCENA bit in Debug Exception and Monitor Control Register, DEMCR [C1.6.5]
    peripherals.DCB.enable_trace();   

    //Enable required stimulus ports of the ITM in Trace Enable Registers, ITM_TER0-ITM_TER7 [C1.7.4]
    peripherals.ITM.ter[0].write(1); //this writes 1 to 0th bit field of ITM_TER0 register which
    //enables stimulus port 0

    //Select the protocol used for trace output in Selected Pin Protocol Register, TPIU_SPPR [C1.10.5] (Optional, Debugger configures it)
    //peripherals.TPIU.sppr.write(0b01);    

    //Write data to the ITM stimulus register, Stimulus Port registers, ITM_STIM0-ITM_STIM255[C1.7.3]
    }
}


pub fn itm_print(peripherals: &mut peripheral::Peripherals, msg: &str) {
    let stim0 = &mut peripherals.ITM.stim[0];
    cortex_m::iprintln!(stim0, "{}", msg);

}