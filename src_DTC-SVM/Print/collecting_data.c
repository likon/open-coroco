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


bool collecting_sensorless_data = true;
bool transmitting_to_python = false;



#define COLLECTING_SPEED_DELAY 0

void collecting_floating_data(void)
{

  
  //static int countaa=0;
  //static int delay  =0;
  //static int absotule_time=0;
  
  if (collecting_sensorless_data==true)
  {
    collecting_sensorless_data=false;
    collecting_samples();
    transmitting_to_python=true;
  }
    timer=timer+1.0f;
    if (timer<0.0f)
        timer=0.0f;
    //if (timer>30000)
        //timer=0;

  //********************//
  //collecting_speed=true;  //para hacer que siempre capture dato (comentar si se quiere que solo capture al cambiar la referencia)
  //current_counter=0;
  //********************//


/*
  if (collecting_speed==true)
  {

    if (countaa<SAMPLES)
    {

      if (delay>COLLECTING_SPEED_DELAY || (absotule_time==0) ) 
      {
        collecting_samples(countaa);
        timer[countaa] = absotule_time;
        countaa++;
        delay=0;
      }
      delay++;
    }

    else 
    {
      collecting_speed=false;
      print_current=true;
    }
    absotule_time++;
  }
  
  else 
  {
    absotule_time=0;
    countaa=0;
    delay=0;    
  }
  
*/
}



/*
void collecting_speed_data(void)
{
  bool fin;
  fin=collecting_floating_data(true);

  if (collecting_speed==true)
  {
    if (fin)==false)
    {

    }
    else 
    {
      collecting_floating_data(false);
      collecting_speed = false;
      print_current      = true;
    }
  }

}
*/
void collecting_samples(void)
{
/*
        data_V_s          [sample_counter] = V_s;
        data_w_r          [sample_counter] = w_r;
        data_ref_freq_SVM [sample_counter] = ref_freq_SVM;

        data_state_SVM    [sample_counter] = state;
        data_CUR_FREQ     [sample_counter] = CUR_FREQ;

        data_psi_sD    [sample_counter]=psi_sD;
        data_psi_sQ    [sample_counter]=psi_sQ;
        data_psi_s     [sample_counter]=psi_s;
        data_psi_alpha [sample_counter]=psi_alpha;
        data_CUR_FREQ  [sample_counter]=CUR_FREQ;
        data_U_d       [sample_counter]=U_d;
        timer          [sample_counter]=flux_linkage_capture_counter;
        
	    data_CUR_FREQ[sample_counter]=CUR_FREQ;	

        data_i_sA [sample_counter]=i_sA;
        data_i_sB [sample_counter]=i_sB;
        data_U_d  [sample_counter]=U_d;

        data_i_sD[sample_counter]=i_sD;
        data_i_sQ[sample_counter]=i_sQ;
        //data_i_s[sample_counter]=i_s;
        //data_cita_i_s[sample_counter]=cita_i_s;


        data_V_sD[sample_counter]=V_sD;
        data_V_sQ[sample_counter]=V_sQ;
        data_V_s [sample_counter]=V_s;
        data_cita_V_s[sample_counter]=cita_V_s;
        data_cita_V_s_relative_angle[sample_counter]=cita_V_s;

        data_psi_sD[sample_counter]=psi_sD;
        data_psi_sQ[sample_counter]=psi_sQ;
        data_psi_s [sample_counter]=psi_s;
        data_psi_alpha[sample_counter]=psi_alpha;
        data_psi_s_alpha_SVM[sample_counter]=psi_s_alpha_SVM;

        data_t_e[sample_counter]=t_e;

        data_psi_s_ref[sample_counter]=psi_s_ref;
        data_t_e_ref[sample_counter]=t_e_ref;

        data_d_psi[sample_counter]=d_psi;
        data_d_te[sample_counter]=d_te;
        data_psi_delta_percentage[sample_counter]=psi_delta_percentage;
        data_t_e_delta_percentage[sample_counter]=t_e_delta_percentage;

        data_w_r[sample_counter]=w_r;   

        data_phase_advance_SVM[sample_counter]=phase_advance_SVM;
        data_V_s_ref_relative_angle[sample_counter]=V_s_ref_relative_angle;
        //data_T1[sample_counter]=T1;
        //data_T2[sample_counter]=T2;
        //data_T_min_on[sample_counter]=T_min_on;
        //data_T_med_on[sample_counter]=T_med_on;
        //data_T_max_on[sample_counter]=T_max_on;
        //data_attenuation[sample_counter]=attenuation;
            data_pi_control_SVM[sample_counter]	=SVM_pi_control;
        data_rotating_angle_SVM[sample_counter] =psi_rotating_angle_SVM;
*/
        data_V_s           = V_s;
        data_w_r           = w_r;
        data_ref_freq_SVM  = ref_freq_SVM;

        data_state_SVM     = state;
        data_CUR_FREQ      = CUR_FREQ;

        data_psi_sD    =psi_sD;
        data_psi_sQ    =psi_sQ;
        data_psi_s     =psi_s;
        data_psi_alpha =psi_alpha;
        data_CUR_FREQ  =CUR_FREQ;
        data_U_d       =U_d;
        //timer          =flux_linkage_capture_counter;
        
	    data_CUR_FREQ=CUR_FREQ;	

        data_i_sA =i_sA;
        data_i_sB =i_sB;
        data_U_d  =U_d;

        data_i_sD=i_sD;
        data_i_sQ=i_sQ;



        data_V_sD=V_sD;
        data_V_sQ=V_sQ;
        data_V_s =V_s;
        data_cita_V_s=cita_V_s;
        data_cita_V_s_relative_angle=cita_V_s;

        data_psi_sD=psi_sD;
        data_psi_sQ=psi_sQ;
        data_psi_s =psi_s;
        data_psi_alpha=psi_alpha;
        data_psi_s_alpha_SVM=psi_s_alpha_SVM;

        data_t_e=t_e;

        data_psi_s_ref=psi_s_ref;
        data_t_e_ref=t_e_ref;

        data_d_psi=d_psi;
        data_d_te=d_te;
        data_psi_delta_percentage=psi_delta_percentage;
        data_t_e_delta_percentage=t_e_delta_percentage;

        data_w_r=w_r;   

        data_phase_advance_SVM=phase_advance_SVM;
        data_V_s_ref_relative_angle=V_s_ref_relative_angle;

        data_pi_control_SVM	=SVM_pi_control;
        data_rotating_angle_SVM =psi_rotating_angle_SVM;
        data_timer=timer;
        
}

void print_captured_data(void)
{
/*
  printf("t:%10d :freq_ref:%6.2f :freq: %6.2f :hall_freq:%6.2f :Vs: %6.2f ",
          timer             [current_counter],
          data_ref_freq_SVM [current_counter],
          data_w_r          [current_counter],
          data_CUR_FREQ[current_counter],
          data_V_s          [current_counter]
          );
          //printf (":phase_adv: %12.8f :isA: %6.2f :isB: %6.2f :isC: %6.2f :isD: %6.2f :isQ: %6.2f :is: %6.2f :iscita: %6.2f :psisD: %7.4f :psisQ: %7.4f :psis: %6.4f :psisa: %6.2f :psiref: %6.4f :te: %6.2f :VsD: %8.2f :VsQ: %8.2f :Vs: %6.2f :Vscita: %6.2f :Vscitar: %6.2f :T1: %4.2f :T2: %4.2f :Tmin: %5.2f :Tmed: %4.2f :Tmax: %4.2f :Ud: %6.2f :pi: %12.8f :maxpi: %12.8f\n",
          printf ("pA %12.8f iA %6.2f iB %6.2f iC %6.2f iD %6.2f iQ %6.2f pD %7.4f pQ %7.4f ps %6.4f pa %6.2f pf %6.4f te %6.2f VD %8.2f VQ %8.2f Vs %6.2f Vc %6.2f Vr %6.2f U %6.2f pi %12.8f mx: %12.8f\n",

          data_phase_advance_SVM[current_counter],
          data_i_sA [current_counter],  
          data_i_sB [current_counter],
          -data_i_sA[current_counter]-data_i_sB[current_counter],
          data_i_sD[current_counter],
          data_i_sQ[current_counter],
          //data_i_s[current_counter], 
          //data_cita_i_s[current_counter],

          data_psi_sD[current_counter],
          data_psi_sQ[current_counter],
          data_psi_s[current_counter],
          data_psi_s_alpha_SVM[current_counter],
          data_psi_s_ref[current_counter],

          data_t_e[current_counter],

          data_V_sD                    [current_counter],
          data_V_sQ                    [current_counter],
          data_V_s                     [current_counter],
          data_cita_V_s                [current_counter],
          data_cita_V_s_relative_angle [current_counter],

          //data_T1[current_counter],
          //data_T2[current_counter],
          //data_T_min_on[current_counter],
          //data_T_med_on[current_counter],
          //data_T_max_on[current_counter],

          data_U_d[current_counter],
          data_pi_control_SVM[current_counter],
          pi_max
          );


*/
          
}





union float_union_t
{
    float f;
    char byte[sizeof(float)];
};



void print_regular_data(void)
{
    char checksum=0;
    printf ("X")   ;

    if      (print_selection==0)
    {
        printf ("t")  ;  checksum=           print_float_as_bytes(data_timer                   );
        printf ("r")  ;  checksum=checksum  +print_float_as_bytes(data_ref_freq_SVM            );
        printf ("e")  ;  checksum=checksum  +print_float_as_bytes(data_w_r                     );
        printf ("h")  ;  checksum=checksum  +print_float_as_bytes(0.01f);//data_CUR_FREQ                );
    }
    else if (print_selection==1)
    {
        printf ("t")  ;  checksum=           print_float_as_bytes(data_timer                   );
        printf ("A")  ;  checksum=checksum  +print_float_as_bytes(data_i_sA                    );
        printf ("B")  ;  checksum=checksum  +print_float_as_bytes(data_i_sB                    );
        printf ("C")  ;  checksum=checksum  +print_float_as_bytes(-data_i_sA-data_i_sB         );
        printf ("D")  ;  checksum=checksum  +print_float_as_bytes(data_i_sD                    );
        printf ("Q")  ;  checksum=checksum  +print_float_as_bytes(data_i_sQ                    );
    }
    else if (print_selection==3)
    {
        printf ("t")  ;  checksum=           print_float_as_bytes(data_timer                   );
        printf ("d")  ;  checksum=checksum  +print_float_as_bytes(data_V_sD                    );
        printf ("q")  ;  checksum=checksum  +print_float_as_bytes(data_V_sQ                    );
        printf ("s")  ;  checksum=checksum  +print_float_as_bytes(data_V_s                     );
        printf ("c")  ;  checksum=checksum  +print_float_as_bytes(data_cita_V_s                );
        printf ("R")  ;  checksum=checksum  +print_float_as_bytes(data_cita_V_s_relative_angle );
    }
    else if (print_selection==5)
    {
        printf ("t")  ;  checksum=           print_float_as_bytes(data_timer                   );
        printf ("p")  ;  checksum=checksum  +print_float_as_bytes(data_psi_sD                  );
        printf ("P")  ;  checksum=checksum  +print_float_as_bytes(data_psi_sQ                  );
    }
    else if (print_selection==6)
    {
        //printf ("t")  ;  checksum=           print_float_as_bytes(data_timer                   );
        //printf ("L")  ;  checksum=checksum  +print_float_as_bytes(data_psi_s                   );
        //printf ("O")  ;  checksum=checksum  +print_float_as_bytes(data_psi_s_alpha_SVM         );
        printf ("v")  ;  checksum=print_float_as_bytes(0.010f);//data_psi_s_ref               );
    }
    else if (print_selection==7)
    {
        printf ("t")  ;  checksum=           print_float_as_bytes(data_timer                   );
        printf ("u")  ;  checksum=checksum  +print_float_as_bytes(data_t_e                     );
        printf ("U")  ;  checksum=checksum  +print_float_as_bytes(data_U_d                     );
        printf ("l")  ;  checksum=checksum  +print_float_as_bytes(data_pi_control_SVM          );
        printf ("x")  ;  checksum=checksum  +print_float_as_bytes(pi_max                       );
    }

    printf ("k");
    printf ("%c", checksum);
    printf ("m");

}


/*
void print_float_as_bytes(float floating_value)
{
    unsigned int i=0;
    union float_union_t float_byte;

    float_byte.f=floating_value;

    while (i<sizeof(float))
    {
      printf("%c",float_byte.byte[i]);
      i++;
    }
}
*/

char print_float_as_bytes(float floating_value)
{
    unsigned int i=0;
    char checksum=0;
    union float_union_t float_byte;

    float_byte.f=floating_value;

    while (i<sizeof(float))
    {
      printf("%c",float_byte.byte[i]);
      checksum=checksum+float_byte.byte[i];
      i++;
    }
    return checksum;
}
