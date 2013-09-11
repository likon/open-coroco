/*
 * Copyright (C) 2013 ARCOS-Lab Universidad de Costa Rica
 * Author: Federico Ruiz Ugalde <memeruiz@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

float i_sA = 0.0f;
float i_sB = 0.0f;
float U_d  = 0.0f;

float current_data_i_sA[1000];
float current_data_i_sB[1000];
float   switching_data_SA[1000];
float   switching_data_SB[1000];
float   switching_data_SC[1000];
bool  inductance_measure=true;

int current_counter=0;  
bool print_current=false;

void tim1_up_tim10_isr(void) 
{
   gpio_set(GPIOD, GPIO9);
   floating_switching_states (&switching_data_SA[current_counter],&switching_data_SB[current_counter],&switching_data_SC[current_counter]);
  // Clear the update interrupt flag
  timer_clear_flag(TIM1,  TIM_SR_UIF);

  calc_freq();
  start_up();
  gen_pwm();


  //pwm_parameter_identification();
   gpio_clear(GPIOD, GPIO9);
   gpio_set(GPIOB, GPIO15);
  voltage_measure (ADC1,ADC_CHANNEL1);
}




void adc_isr(void)
{ 
  static int adc_counter=0;
  float V_stm32_A  = 0.0f;
  float V_stm32_B  = 0.0f;
  float V_stm32_Ud = 0.0f;
  float V_shunt_A  = 0.0f;
  float V_shunt_B  = 0.0f;
  

  if (adc_counter==0)
  {
    V_stm32_A = adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    V_shunt_A = (V_stm32_A-V_DIFFERENTIAL_AMPLIFIER_REFFERENCE)/G_OP_AMP_A;
    i_sA      = V_shunt_A/R_SHUNT_A;
    //i_sA=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR)*SHUNT_A_CONVERTION_FACTOR;
    
    gpio_clear(GPIOB, GPIO15);

    voltage_measure (ADC1,ADC_CHANNEL2);
    adc_counter++;
    //printf ("\nFirst Convertion");

    //printf("\n %d ", gpio_get(GPIOD, GPIO11));

/*
    if ( gpio_get(GPIOD, GPIO11) && inductance_measure==true)
    {
      if (current_counter<1000)  
      { 
        inductance_measure=true;
        //print_current=false;
        collecting_current=true;
      }
      else 			
      { 
        inductance_measure=false;
        //print_current=true;
        //collecting_current=false;
      }
      
      current_counter=0;
      collecting_current=true;
      inductance_measure=false;
      printf("\ninductance");
    }
   */

   if ( (i_sA>=0.5f) &&  (inductance_measure==true) )
   {
     collecting_current=true;
     inductance_measure=false;
   }
 

    //------------------------------------------
    if (collecting_current==true)
    {
      //printf ("\nCollecting current!");
      if (current_counter<1000)
      {
        current_data_i_sA[current_counter]=i_sA;
        
	//current_counter++;
      }
      else
      {
        current_counter=0;
        collecting_current=false;
        print_current=true;
      }
      //------------------------------------------
    }

  }
  else if (adc_counter==1)
  {

    

    V_stm32_B = adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    V_shunt_B = (V_stm32_B-V_DIFFERENTIAL_AMPLIFIER_REFFERENCE)/G_OP_AMP_B;
    i_sB      = V_shunt_B/R_SHUNT_B;
    //i_sB=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR)*SHUNT_B_CONVERTION_FACTOR;
    
    gpio_set(GPIOB, GPIO15);

    voltage_measure (ADC1,ADC_CHANNEL3);
    adc_counter++; 
    //printf ("\nSecond Convertion");

    if (collecting_current==true)
    {
      //printf ("\nCollecting current!");
      if (current_counter<1000)
      {
        current_data_i_sB[current_counter]=i_sB;
	current_counter++;
      }
      else
      {
        current_counter=0;
        collecting_current=false;
        print_current=true;
      }
    }

  }
  else
  {
    gpio_clear(GPIOB, GPIO15);

    V_stm32_Ud = adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR);
    U_d        = V_stm32_Ud*BATTERY_VOLTAGE_CONVERTION_FACTOR;
    //U_d=adc_read_regular(ADC1)*(VREF/ADC_CONVERSION_FACTOR)*BATTERY_VOLTAGE_CONVERTION_FACTOR;
    adc_counter=0;
    //printf ("\nThird Convertion");

    
  }

  
}
