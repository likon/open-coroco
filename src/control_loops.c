/*
 *This file is part of the open-coroco project.
 *
 *  Copyright (C) 2013  Sebastian Chinchilla Gutierrez <tumacher@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define NO_HALL_UPDATE	 	0
#define FIRST_HALL_UPDATE	1
#define SECOND_HALL_UPDATE	2
#define OPEN_LOOP		3
#define CLOSE_LOOP 		4


#define MAX_AMPLITUDE 1.0f
#define AMPLITUDE_REDUCTION 90.0f 
//#define MAX_PHASE_ADVANCE 90.0f*PI/180.0f
//#define KP (10.0f/(max_ticks*1.0f))
//#define KI 100.0f
//#define KC 100.0f

#define OPEN_LOOP_MIN_ATTENUATION 0.75f

void no_hall_update (int* rotor_speed_loop_state)
{
	if (hall1_data.hall_update)
	{
		attenuation = OPEN_LOOP_MIN_ATTENUATION;
		sine_freq=sine_freq_fixed;
		*rotor_speed_loop_state=FIRST_HALL_UPDATE;
	}

	else
	{
		attenuation = OPEN_LOOP_MIN_ATTENUATION;
		sine_freq=sine_freq_fixed;
		*rotor_speed_loop_state=NO_HALL_UPDATE;
	}

	max_ticks=pwmfreq_f/sine_freq;
}

void first_hall_update (int* rotor_speed_loop_state)
{
	if (hall1_data.hall_update)
	{
		attenuation = OPEN_LOOP_MIN_ATTENUATION;
		sine_freq=sine_freq_fixed;
		*rotor_speed_loop_state=OPEN_LOOP;
	}

	else
	{
		attenuation = OPEN_LOOP_MIN_ATTENUATION;
		sine_freq=sine_freq_fixed;
		*rotor_speed_loop_state=FIRST_HALL_UPDATE;
	}
	max_ticks=pwmfreq_f/sine_freq;
}
/*
void second_hall_update (int* rotor_speed_loop_state)
{
	
}
*/
void open_loop (int* rotor_speed_loop_state)
{	
	//gpio_set(GPIOD, GPIO13);
	//gpio_clear(GPIOD, GPIO15);
	//static int
		//frequency_change_counter=0;
	actual_sine_frequency=pwmfreq_f/(2.0f*previous_hall_ticks);

	if (sine_freq>125.0f)
	{
		//*rotor_speed_loop_state=CLOSE_LOOP;
	}
	else if (frequency_change_counter>max_sinusoidal_periods)
	{
		sine_freq=sine_freq+0.25f;
		
		if (sine_freq<50.0f)
			attenuation=0.75f;
		else 
			attenuation=1.0f;

		*rotor_speed_loop_state=OPEN_LOOP;
		frequency_change_counter=0;
	}
	else 
	{
		
		*rotor_speed_loop_state=OPEN_LOOP;
	}

/*

	if (hall1_data.hall_update)
	{
		attenuation = 0.5f;
		#define OPEN_LOOP_P 0.2f
		
		actual_sine_frequency=pwmfreq_f/(2.0f*previous_hall_ticks);


		if (actual_sine_frequency>40.0f)//open_loop_desired_sine_frequency*1.0f)
		{
			*rotor_speed_loop_state=CLOSE_LOOP;
			phase_sum=90.0f;
		}


		else
		{
			open_loop_error=open_loop_desired_sine_frequency-actual_sine_frequency;

			if (open_loop_error>8.0f)
				open_loop_error=8.0f;
			else if (open_loop_error<-8.0f)
				open_loop_error=-8.0f;

			sine_freq=actual_sine_frequency;//+OPEN_LOOP_P*open_loop_error;		
			*rotor_speed_loop_state=OPEN_LOOP;
		}
	}
	

	
	else 
	{			
		*rotor_speed_loop_state=OPEN_LOOP;		
	}
*/
	max_ticks=pwmfreq_f/sine_freq;

}

void close_loop(int* rotor_speed_loop_state)
{
	#define Pkp 1.0f/12.0f
	
	#define MAX_PHASE_ADVANCE 90.0f

	float Pki=1000000.0f;
	float Pkc=1.0f/1000.0f;

if (hall1_data.hall_update)
{

	gpio_clear(GPIOD, GPIO14);

	actual_sine_frequency=pwmfreq_f/(2.0f*previous_hall_ticks);
	close_loop_error=close_loop_desired_frequency-actual_sine_frequency;

	//desired_previous_hall_ticks=pwmfreq_f/(2.0f*close_loop_desired_frequency);
	//close_loop_tick_error= -0.25f*(desired_previous_hall_ticks-previous_hall_ticks);

	//phaseU=close_loop_tick_error*Pkp;//+phase_sum;
	phaseU=close_loop_error*2.25f; //2.0 mejor resultado hasta ahora


	if (true)//(attenuation<1)
	{
		//regular phase advance
		/*if (phaseU>MAX_PHASE_ADVANCE)
		{
			phase_advance=MAX_PHASE_ADVANCE;
			//attenuation=1.0f;//1.0f;
			attenuation=phase_advance*1.0f/150.0f;
			Pki=1.0f/1000.0f;//8.0f;
		}
		
		else if (phaseU<-MAX_PHASE_ADVANCE)
		{
			phase_advance=-MAX_PHASE_ADVANCE;
			//attenuation=1.0f;
			attenuation=-phase_advance*1.0f/150.0f;
			Pkc=8.0f;
		}*/

		if (true)//else
		{
			phase_advance=phaseU;
			
			if (phase_advance<0.0f)
			{
				attenuation=phase_advance*1.0f/150.0f;//-phase_advance/72.0f;
			}
			else
			{
				attenuation=phase_advance*1.0f/150.0f;//phase_advance/72.0f;
			}
		}

		

	}

	else 
	{
		//double phase advance
		attenuation=1.0f;

		if (phaseU>MAX_PHASE_ADVANCE*2.0f)
		{
			phase_advance=MAX_PHASE_ADVANCE*2.0f;
		} 
		else if (phaseU<-MAX_PHASE_ADVANCE*2.0f)
		{
			phase_advance=-MAX_PHASE_ADVANCE*2.0f;
		}
		else
		{
			phase_advance=phaseU;
		}
	}

	//phase_sum+phase_excess
	phase_excess=phaseU-phase_advance;

	//BEMF adjustment
	phase_sum+=close_loop_tick_error/Pki-Pkc*phase_excess;

	phase_stator=phase_rotor+phase_advance;


	*rotor_speed_loop_state=CLOSE_LOOP;


	if (attenuation>1.0f)
		attenuation=1.0f;

	//max_ticks=pwmfreq_f/sine_freq;
	max_ticks=2.0f*previous_hall_ticks;
	//ticks=ticks+phase_advance*max_ticks/360.0f;
	attenuation=0.75f;
	ticks=ticks-150.0f*max_ticks/360.0f;
		
	//ticks=ticks+45.0f*max_ticks/360.0f;
	//180° pierde sincronía completamente	
	//175° se corre y se invierte
	//150°  mantiene sincronia y frequencia de 50Hz -60Hz (pocas veces a 25Hz)
	//125°  mantiene sincronia y frequencia de 50Hz -60Hz (pocas veces a 25Hz)
	//+90°  mantiene velocidad constante (cerca de 50Hz, cambia exactamente entre 50 y 25)
	//45°   salta entre 29 y 18Hz
	//20°	entre 18 y 25 pero pierde sincronía y se detiene
	//open loop sin actualizar velocidad: 12-16Hz

	//-45° se invierte constantemente
	//-20 se invierte cte mente
	//-90 se invierte y se queda pegado
	//-150 se sale de sincronia completamente
	//-180
}


else
{

}



}

void PID_control_loop(void)
{

	static int 
		//frequency_change_counter=0,
		rotor_speed_loop_state=NO_HALL_UPDATE;

	V_hall_1_V1=gpio_get(GPIOA, GPIO1);

	hall_hysteresis_window(V_hall_1_V1,HALL_1_UPPER_BAND,HALL_1_LOWER_BAND,&hall1_data);
	
	
  	if (ticks<max_ticks)
	{
		ticks+=1.0f;
	}
	else
	{ 
		ticks=0.0f;
		frequency_change_counter++;
	}

	hall_ticks+=1.0f;
	//max_ticks=pwmfreq_f/sine_freq;



	if (hall1_data.hall_update )//|| hall2_data.hall_update || hall3_data.hall_update)
	{	
		//hall-sensor time	
		previous_hall_ticks=hall_ticks;
		hall_ticks=0.0f;
	}



	//waiting for having two hall sensor measures in order to calculate the rotor speed
	if (rotor_speed_loop_state==NO_HALL_UPDATE)
		no_hall_update(&rotor_speed_loop_state);

	else if (rotor_speed_loop_state==FIRST_HALL_UPDATE)
		first_hall_update(&rotor_speed_loop_state);
	
	//else if (rotor_speed_loop_state==SECOND_HALL_UPDATE)
		//second_hall_update(&rotor_speed_loop_state);

	else if (rotor_speed_loop_state==OPEN_LOOP)
		open_loop(&rotor_speed_loop_state);

	else if (rotor_speed_loop_state==CLOSE_LOOP)
		close_loop(&rotor_speed_loop_state);
	else
		rotor_speed_loop_state=NO_HALL_UPDATE;

}










