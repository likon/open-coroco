#!/usr/bin/python

#/*
# *This file is part of the open-coroco project.
# *
# *  Copyright (C) 2013  Sebastian Chinchilla Gutierrez <tumacher@gmail.com>
# *
# *  This program is free software: you can redistribute it and/or modify
# *  it under the terms of the GNU General Public License as published by
# *  the Free Software Foundation, either version 3 of the License, or
# *  (at your option) any later version.
# *
# *  This program is distributed in the hope that it will be useful,
# *  but WITHOUT ANY WARRANTY; without even the implied warranty of
# *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# *  GNU General Public License for more details.
# *
# *  You should have received a copy of the GNU General Public License
# *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
# */
import datetime
import csv
import serial
from serial import SerialException
import sys
import select
import matplotlib.pyplot as plt

from byte_to_float import *

class Serial_Stm32f4(object):


    def initializing_values(self):

        #pyserial configuration
        self.path           = "/home/tumacher/local/src/repositories/arcoslab/open-coroco/src_DTC-SVM/Python/measures/"
        self.dev_type       ="/dev/ttyACM"
        self.serial_speed   =115200
        self.serial_timeout =1

        #control data for pyserial
        self.connecting         = True
        self.transmition_error  = False
        self.counter            = 0
        self.capture_data       = False
        self.capture_counter    = 0
        self.print_selection    = 0
        self.new_data_line      = ''
        

        #data from stm32F4 (impedance control+DTC-SVM+PID+HALL)
        self.time                   = 0.0
       
        self.reference_frequency    = 0.0
        self.electric_frequency     = 0.0
        self.hall_frequency         = 0.0
  
        self.isA                    = 0.0
        self.isB                    = 0.0
        self.isC                    = 0.0
        self.isD                    = 0.0
        self.isQ                    = 0.0
   
        self.VsD                    = 0.0
        self.VsQ                    = 0.0
        self.Vs                     = 0.0
        self.Vs_cita                = 0.0
        self.Vs_relative_cita       = 0.0

        self.psi_sD                 = 0.0
        self.psi_sQ                 = 0.0
        self.psi_s                  = 0.0
        self.psi_s_alpha            = 0.0
        self.psi_s_reference        = 0.0
                 
        self.te                     = 0.0
        self.te_ref                 = 0.0
        self.Ud                     = 0.0
        self.pi_control             = 0.0
        self.pi_max                 = 0.0   

        self.checksum_stm32         = 0.0
        self.checksum_python        = 0.0


    def creating_data_vectors(self):
        self.time_vector = []
        self.reference_frequency_vector = []
        self.electric_frequency_vector = []
        self.hall_frequency_vector = []
  
        self.isA_vector = []
        self.isB_vector = []
        self.isC_vector = []
        self.isD_vector = []
        self.isQ_vector = []
   
        self.VsD_vector              = []
        self.VsQ_vector              = []
        self.Vs_vector               = []
        self.Vs_cita_vector          = []
        self.Vs_relative_cita_vector = []

        self.psi_sD_vector          = []
        self.psi_sQ_vector          = []
        self.psi_s_vector           = []
        self.psi_s_alpha_vector     = []
        self.psi_s_reference_vector = []
                 
        self.te_vector          = []
        self.te_ref_vector      = []
        self.Ud_vector          = []
        self.pi_control_vector  = []
        self.pi_max_vector      = []


    def connecting_to_stm32F4(self):
        while self.connecting==True:
            try:
                self.ser = serial.Serial(self.dev_type+str(self.counter),self.serial_speed , timeout=self.serial_timeout)
                self.connecting=False
                self.ser.flushInput()
                print "Connected to the STM32F4"
                
                self.ser.write('p')
                self.ser.write(' ')
                self.ser.write('0')
                self.ser.write('\n')
                self.ser.write('\r')
                
            except SerialException:
                print"Disconnected from the STM32, cua cua"
                self.counter=self.counter+1
                if (self.counter>100):
                    self.counter=0
                self.connecting=True                

  
    def create_log_file(self):
        self.log_file = open( self.path +"data" +"." + datetime.datetime.now().ctime() +".csv", "wb")
        self.writer   = csv.writer(self.log_file, delimiter=' ',quotechar=' ', quoting=csv.QUOTE_MINIMAL)
        header_csv    = "t t ref_freq ref_freq electric_frequency electric_frequency hall_freq hall_freq " + \
                        "isA isA isB isB isC isC isD isD isQ isQ "                                         + \
                        "VsD VsD VsQ VsQ Vs Vs Vs_cita Vs_cita Vs_cita_relative Vs_cita_relative "         + \
                        "psisD psisD psisQ psisQ psis psis psis_alpha psis_alpha psis_ref psis_ref "       + \
                        "te te Ud Ud pi_control pi_control pi_max pi_max"  
        split_header = header_csv.split()                  
        self.writer.writerow(split_header)        


    def get_value(self,string,output_variable,split_info,i,error):
        if split_info[i] == string: 
            convertion = bytes_to_float(split_info[i+1])
            if (convertion[0]==True):                       
                output_variable = 27#convertion[1]
                return True    
            else:
                return False 


    def get_data_and_checksum(self):
                    bytes=1
                    info=''
                    i=0
                    while i<4:
                        single_character = self.ser.read(bytes)
                        #print "single_character_byte_to_float: "+single_character + " ord: " +str(ord(single_character))
                        info +=single_character
                        i=i+1;
                    convertion = bytes_to_float(info)
                    if (convertion[0]==True):                      
                        data = convertion[1]
                        self.transmition_error=False
                        #print "found, whaat!"+str(data)
                    else:   
                        self.transmition_error=True
                        data=0
                        #print " four bytes not found" 
                    
                    info_counter=0
                    for ch in info:
                        if info_counter<len(info)-1:
                            self.checksum_python=self.checksum_python+ord(ch)
                            if self.checksum_python>=256:
                                self.checksum_python=self.checksum_python-256
                        #print ord(ch)
                    return data


    def read_data_from_stm32(self):
        bytes = 1
        #info = ''   #info needs to be set before splitting it, otherwise pythons says it is uninitialized
        single_character   = self.ser.read(bytes)
        self.checksum_python=0
        self.checksum_stm32=0
        if(single_character == "X"):

            while (single_character != "m"):
                single_character = self.ser.read(bytes)

                if   (single_character=='t'):   self.time                =self.get_data_and_checksum()
                elif (single_character=='r'):   self.reference_frequency =self.get_data_and_checksum()
                elif (single_character=='h'):   self.hall_frequency      =self.get_data_and_checksum()
                elif (single_character=='e'):   self.electric_frequency  =self.get_data_and_checksum()

 
                elif (single_character=='A'):   self.isA =self.get_data_and_checksum()
                elif (single_character=='B'):   self.isB =self.get_data_and_checksum()
                elif (single_character=='C'):   self.isC =self.get_data_and_checksum()
                elif (single_character=='D'):   self.isD =self.get_data_and_checksum()
                elif (single_character=='Q'):   self.isQ =self.get_data_and_checksum()

                elif (single_character=='d'):   self.VsD                =self.get_data_and_checksum()
                elif (single_character=='q'):   self.VsQ                =self.get_data_and_checksum()
                elif (single_character=='s'):   self.Vs                 =self.get_data_and_checksum()
                elif (single_character=='c'):   self.Vs_cita            =self.get_data_and_checksum()
                elif (single_character=='R'):   self.Vs_relative_cita   =self.get_data_and_checksum()

                elif (single_character=='p'):   self.psi_sD                =self.get_data_and_checksum()
                elif (single_character=='P'):   self.psi_sQ                =self.get_data_and_checksum()
                elif (single_character=='L'):   self.psi_s                 =self.get_data_and_checksum()
                elif (single_character=='O'):   self.psi_s_alpha           =self.get_data_and_checksum()
                elif (single_character=='v'):   self.psi_s_reference       =self.get_data_and_checksum()  
                                                #print "entering v ord: " + str(ord('v')) 
                elif (single_character=='u'):   self.te                =self.get_data_and_checksum()
                elif (single_character=='y'):   self.te_ref            =self.get_data_and_checksum()

                elif (single_character=='U'):   self.Ud                =self.get_data_and_checksum()
                elif (single_character=='l'):   self.pi_control        =self.get_data_and_checksum()
                elif (single_character=='x'):   self.pi_max            =self.get_data_and_checksum()
                                
                elif (single_character=='k'):
                    self.checksum_stm32 = ord(self.ser.read(bytes))
                #print "single_character: " + single_character
        
        if (self.checksum_python!=self.checksum_stm32):
            self.transmition_error=True            
 

 
    def append_new_data_to_vectors(self):

        if self.transmition_error==False: 

                self.time_vector.append(self.time)
                self.reference_frequency_vector.append(self.reference_frequency)
                self.electric_frequency_vector.append(self.electric_frequency)
                self.hall_frequency_vector.append(self.hall_frequency)
  
                self.isA_vector.append(self.isA)
                self.isB_vector.append(self.isB)
                self.isC_vector.append(self.isC)
                self.isD_vector.append(self.isD)
                self.isQ_vector.append(self.isQ)
   
                self.VsD_vector.append(self.VsD)
                self.VsQ_vector.append(self.VsQ)
                self.Vs_vector.append(self.Vs)
                self.Vs_cita_vector.append(self.Vs_cita_vector)
                self.Vs_relative_cita_vector.append(self.Vs_relative_cita_vector)

                self.psi_sD_vector.append(self.psi_sD)
                self.psi_sQ_vector.append(self.psi_sQ)
                self.psi_s_vector.append(self.psi_s_vector)
                self.psi_s_alpha_vector.append(self.psi_s_alpha_vector)
                self.psi_s_reference_vector.append(self.psi_s_reference_vector)
                         
                self.te_vector.append(self.te)
                self.te_ref_vector.append(self.te_ref)
                self.Ud_vector.append(self.Ud)
                self.pi_control_vector.append(self.pi_control)
                self.pi_max_vector.append(self.pi_max)        


    def full_print_string (self):
           #print "inside full_print_string"
           self.new_data_line=  "t %6.2f "                  %self.time                  + \
                                    " ref_freq %6.2f"           %self.reference_frequency   + \
                                    " electric_frequency %6.2f" %self.electric_frequency    + \
                                    " hall_freq %6.2f"          %self.hall_frequency        + \
                                    " isA %6.2f"                %self.isA                   + \
                                    " isB %6.2f"                %self.isB                   + \
                                    " isC %6.2f"                %self.isC                   + \
                                    " isD %6.2f"                %self.isD                   + \
                                    " isQ %6.2f"                %self.isQ                   + \
                                    " VsD %6.2f"                %self.VsD                   + \
                                    " VsQ %6.2f"                %self.VsQ                   + \
                                    " Vs %6.2f"                 %self.Vs                    + \
                                    " Vs_cita %6.2f"            %self.Vs_cita               + \
                                    " Vs_cita_relative %6.2f"   %self.Vs_relative_cita      + \
                                    " psisD %10.8f"              %self.psi_sD                + \
                                    " psisQ %10.8f"              %self.psi_sQ                + \
                                    " psis %6.2f"               %self.psi_s                 + \
                                    " psis_alpha %6.2f"         %self.psi_s_alpha           + \
                                    " psis_ref %10.8f"           %self.psi_s_reference       + \
                                    " te %6.2f"                 %self.te                    + \
                                    " Ud %6.2f"                 %self.Ud                    + \
                                    " pi_control %12.9f"         %self.pi_control            + \
                                    " pi_max %10.6f"             %self.pi_max  

    def print_selection_print_string(self):
       
        if   self.print_selection==0:
            self.new_data_line= "t: %6.2f "                   %self.time                + \
                                " ref_freq: %6.2f"            %self.reference_frequency + \
                                " electric_frequency: %10.2f" %self.electric_frequency  + \
                                " hall_freq: %6.2f"           %self.hall_frequency        

        elif self.print_selection==1:
            self.new_data_line= "t: %6.2f "                  %self.time                 + \
                                " isA: %6.2f"                %self.isA                  + \
                                " isB: %6.2f"                %self.isB                  + \
                                " isC: %6.2f"                %self.isC                   

        elif self.print_selection==2:
            self.new_data_line= "t: %6.2f "                   %self.time                + \
                                " isD: %6.2f"                %self.isD                  + \
                                " isQ: %6.2f"                %self.isQ                   

        elif self.print_selection==3:
            self.new_data_line= "t: %6.2f "                  %self.time                + \
                                " VsD: %6.2f"                %self.VsD                 + \
                                " VsQ: %6.2f"                %self.VsQ                 

        elif self.print_selection==4:
            self.new_data_line= "t: %6.2f "                  %self.time                + \
                                " Vs: %6.2f"                 %self.Vs                  + \
                                " Ud: %6.2f"                 %self.Ud                  
                                #" Vs_cita: %6.2f"            %self.Vs_cita             + \
                                #" Vs_cita_relative: %6.2f"   %self.Vs_relative_cita      

        elif self.print_selection==5:
            self.new_data_line= "t: %6.2f "                  %self.time                + \
                                " psisD: %10.6f"             %self.psi_sD              + \
                                " psisQ: %10.6f"             %self.psi_sQ              

        elif self.print_selection==6:
            self.new_data_line= "t: %6.2f "                  %self.time                + \
                                " te: %10.6f"                %self.te                  + \
                                " te_ref: %10.6f"            %self.te_ref                  

                                
        elif self.print_selection==7:
            self.new_data_line= "t: %6.2f "                  %self.time                + \
                                " pi_control: %12.8f"        %self.pi_control          + \
                                " pi_max %12.8f:"            %self.pi_max

        '''
        elif self.print_selection==6:
            self.new_data_line= "t: %6.2f "                  %self.time                + \
                                " psis: %10.6f"              %self.psi_s               + \
                                " psis_alpha: %6.2f"         %self.psi_s_alpha         + \
                                " psis_ref: %10.8f"          %self.psi_s_reference       
        '''

    def save_data_to_csv_file(self):
                new_data_line_csv=  "t %6.2f "                  %self.time                  + \
                                    " ref_freq %6.2f"           %self.reference_frequency   + \
                                    " electric_frequency %6.2f" %self.electric_frequency    + \
                                    " hall_freq %6.2f"          %self.hall_frequency        + \
                                    " isA %6.2f"                %self.isA                   + \
                                    " isB %6.2f"                %self.isB                   + \
                                    " isC %6.2f"                %self.isC                   + \
                                    " isD %6.2f"                %self.isD                   + \
                                    " isQ %6.2f"                %self.isQ                   + \
                                    " VsD %6.2f"                %self.VsD                   + \
                                    " VsQ %6.2f"                %self.VsQ                   + \
                                    " Vs %6.2f"                 %self.Vs                    + \
                                    " Vs_cita %6.2f"            %self.Vs_cita               + \
                                    " Vs_cita_relative %6.2f"   %self.Vs_relative_cita      + \
                                    " psisD %10.6f"              %self.psi_sD                + \
                                    " psisQ %10.6f"              %self.psi_sQ                + \
                                    " psis %6.2f"               %self.psi_s                 + \
                                    " psis_alpha %6.2f"         %self.psi_s_alpha           + \
                                    " psis_ref %10.6f"           %self.psi_s_reference       + \
                                    " te %10.6f"                 %self.te                    + \
                                    " Ud %6.2f"                 %self.Ud                    + \
                                    " pi_control %6.2f"         %self.pi_control            + \
                                    " pi_max %6.2f"             %self.pi_max  

                if self.transmition_error==False: 
                    split_new_data_line = new_data_line_csv.split()
                    self.writer.writerow(split_new_data_line)     


    


    def print_to_console(self):
        #print "print_selection: " + str(self.print_selection)
        if self.print_selection!=9  :  self.print_selection_print_string()
        else                        :  self.full_print_string ()
        #print "check_sum python: "+str(self.checksum_python)+" stm32: "+str(self.checksum_stm32)

        if self.transmition_error==False:      
            print   self.new_data_line
        '''
        else :
            #print   self.new_data_line
            print "warning: transmition_error"
        '''


    def plot_frequencies(self,rows,columns,subplot_index):
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector,self.electric_frequency_vector ,label='electric' )
                        plt.plot(self.time_vector,self.hall_frequency_vector     ,label='hall'     )
                        plt.plot(self.time_vector,self.reference_frequency_vector,label='reference')                     
                        plt.title('frequency vs time')
                        plt.xlabel('time (ticks)')
                        plt.ylabel('frequency (Hz)')
                        plt.legend()

    def plot_three_phase_currents(self,rows,columns,subplot_index):
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.isA_vector,label='isA')
                        plt.plot(self.time_vector, self.isB_vector,label='isB')
                        plt.plot(self.time_vector, self.isC_vector,label='isC')
                        plt.title('current vs time')
                        plt.xlabel('time (ticks)')
                        plt.ylabel('three-phase currents (A)')
                        plt.legend()

    def plot_quadrature_vs_direct_currents(self,rows,columns,subplot_index):
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.isD_vector, self.isQ_vector)
                        plt.title ('isQ vs isD')
                        plt.xlabel('isD (A)')
                        plt.ylabel('isQ (A)')
                        plt.legend() 

    def plot_quadrature_vs_direct_voltages(self,rows,columns,subplot_index):
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.VsD_vector, self.VsQ_vector)
                        plt.title('VsQ vs VsD')
                        plt.xlabel('VsD (A)')
                        plt.ylabel('VsQ (A)')
                        plt.legend() 

    def plot_voltage_magnitude(self,rows,columns,subplot_index):    
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.Vs_vector,label='Vs')
                        plt.plot(self.time_vector, self.Ud_vector,label='Ud')
                        plt.title('voltage vs time')
                        plt.xlabel('t (ticks)')
                        plt.ylabel('Voltage (V)')
                        plt.legend()     

    def plot_flux_linkage(self,rows,columns,subplot_index):    
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.psi_sD_vector, self.psi_sQ_vector)
                        plt.title('psi_sQ vs psi_sD')
                        plt.xlabel('psi_sD (Wb)')
                        plt.ylabel('psi_sQ (Wb)')
                        plt.legend() 

    def plot_electromagnetic_torque(self,rows,columns,subplot_index):    
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.te_vector,label='te')
                        plt.plot(self.time_vector, self.te_ref_vector,label='te_ref')
                        plt.title('torque vs time')
                        plt.xlabel('time (ticks)')
                        plt.ylabel('torque (Nm)')
                        plt.legend() 

    def plot_phase_advance(self,rows,columns,subplot_index):    
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.pi_control_vector,label='pi_control')
                        plt.plot(self.time_vector, self.pi_max_vector,label='pi_max')
                        plt.title('pi increment vs time')
                        plt.xlabel('time (ticks)')
                        plt.ylabel('pi (degrees)')
                        plt.legend()

    def plot_all_in_one(self):
                        rows = 4
                        columns = 2 
                        subplot_index = 1
                        
                        plt.figure(num=1, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')  
                        
                        self.plot_frequencies                   (rows,columns,subplot_index)
                        subplot_index=subplot_index+1

                        self.plot_three_phase_currents          (rows,columns,subplot_index)
                        subplot_index=subplot_index+1

                        self.plot_quadrature_vs_direct_currents (rows,columns,subplot_index)
                        subplot_index=subplot_index+1    

                        self.plot_quadrature_vs_direct_voltages (rows,columns,subplot_index)
                        subplot_index=subplot_index+1

                        self.plot_voltage_magnitude             (rows,columns,subplot_index)
                        subplot_index=subplot_index+1
               
                        self.plot_flux_linkage                       (rows,columns,subplot_index)
                        subplot_index=subplot_index+1    

                        self.plot_electromagnetic_torque             (rows,columns,subplot_index)
                        subplot_index=subplot_index+1    

                        self.plot_phase_advance                      (rows,columns,subplot_index)
                        subplot_index=subplot_index+1  
                        
                        '''
                        left  = 0.125  # the left side of the subplots of the figure
                        right = 0.9    # the right side of the subplots of the figure
                        bottom = 0.1   # the bottom of the subplots of the figure
                        top = 0.9      # the top of the subplots of the figure
                        wspace = 0.2   # the amount of width reserved for blank space between subplots
                        hspace = 0.2   # the amount of height reserved for white space between subplots
                        '''
                        plt.subplots_adjust(hspace=0.4)
                        plt.subplots_adjust(wspace=0.2)
                        #plt.show()                        
                        plt.savefig(self.path+ "all_graphs" +"." + datetime.datetime.now().ctime() +".jpg")


    def plot_one_by_one(self):
                        rows = 1
                        columns = 1 
                        subplot_index = 1
                                              
                        plt.figure(num=2, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k') 
                        self.plot_frequencies                   (rows,columns,subplot_index)
                        plt.savefig(self.path+ "frequencies" +"." + datetime.datetime.now().ctime() +".jpg")

                        plt.figure(num=3, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k') 
                        self.plot_three_phase_currents          (rows,columns,subplot_index)
                        plt.savefig(self.path+ "three-phase_currents" +"." + datetime.datetime.now().ctime() +".jpg")

                        plt.figure(num=4, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k') 
                        self.plot_quadrature_vs_direct_currents (rows,columns,subplot_index)
                        plt.savefig(self.path+ "isQ_vs_isD" +"." + datetime.datetime.now().ctime() +".jpg")

                        plt.figure(num=5, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_quadrature_vs_direct_voltages (rows,columns,subplot_index)
                        plt.savefig(self.path+ "VsQ_vs_VsD" +"." + datetime.datetime.now().ctime() +".jpg")

                        plt.figure(num=6, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_voltage_magnitude             (rows,columns,subplot_index)
                        plt.savefig(self.path+ "voltage_magnitude" +"." + datetime.datetime.now().ctime() +".jpg")
               
                        plt.figure(num=7, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_flux_linkage                       (rows,columns,subplot_index)
                        plt.savefig(self.path+ "flux-linkage" +"." + datetime.datetime.now().ctime() +".jpg")

                        plt.figure(num=8, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_electromagnetic_torque             (rows,columns,subplot_index)
                        plt.savefig(self.path+ "electromagnetic_torque" +"." + datetime.datetime.now().ctime() +".jpg")

                        plt.figure(num=9, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_phase_advance                      (rows,columns,subplot_index)
                        plt.savefig(self.path+ "phase_advance_pi" +"." + datetime.datetime.now().ctime() +".jpg")
    
    def plot_selection(self):
        rows = 1
        columns = 1 
        subplot_index = 1
                        
        if   self.print_selection==0:
            plt.figure(num=2, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k') 
            self.plot_frequencies                   (rows,columns,subplot_index)
            plt.savefig(self.path+ "frequencies" +"." + datetime.datetime.now().ctime() +".jpg")

        elif self.print_selection==1:
            plt.figure(num=3, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k') 
            self.plot_three_phase_currents          (rows,columns,subplot_index)
            plt.savefig(self.path+ "three-phase_currents" +"." + datetime.datetime.now().ctime() +".jpg")

        elif self.print_selection==2:
            plt.figure(num=4, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k') 
            self.plot_quadrature_vs_direct_currents (rows,columns,subplot_index)
            plt.savefig(self.path+ "isQ_vs_isD" +"." + datetime.datetime.now().ctime() +".jpg")

        elif self.print_selection==3:
            plt.figure(num=5, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
            self.plot_quadrature_vs_direct_voltages (rows,columns,subplot_index)
            plt.savefig(self.path+ "VsQ_vs_VsD" +"." + datetime.datetime.now().ctime() +".jpg")

        elif self.print_selection==4:
            plt.figure(num=6, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
            self.plot_voltage_magnitude             (rows,columns,subplot_index)
            plt.savefig(self.path+ "voltage_magnitude" +"." + datetime.datetime.now().ctime() +".jpg")

        elif self.print_selection==5:
            plt.figure(num=7, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
            self.plot_flux_linkage                       (rows,columns,subplot_index)
            plt.savefig(self.path+ "flux-linkage" +"." + datetime.datetime.now().ctime() +".jpg")

        elif self.print_selection==6:
            plt.figure(num=8, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
            self.plot_electromagnetic_torque             (rows,columns,subplot_index)
            plt.savefig(self.path+ "electromagnetic_torque" +"." + datetime.datetime.now().ctime() +".jpg")

        elif self.print_selection==7:
            plt.figure(num=9, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
            self.plot_phase_advance                      (rows,columns,subplot_index)
            plt.savefig(self.path+ "phase_advance_pi" +"." + datetime.datetime.now().ctime() +".jpg")
      


    def __init__(self):
        self.initializing_values()        
        self.creating_data_vectors()
        self.create_log_file()
        self.connecting_to_stm32F4()
               


            

    def __del__(self):
        self.log_file.close()
        self.ser.close()
        print "---Serial port closed: disconnected from the STM32F4---"
	

        	    	    		
		
    def read(self): 
            self.read_data_from_stm32()
                                        
            if self.capture_data==True:
                self.append_new_data_to_vectors()
                self.save_data_to_csv_file()
                print "apending data to vectors"

            self.print_to_console() 
            '''
            if self.transmition_error==True:
                print "transmition error"
            else:
                print "transmition ok"
            '''

    def write(self):	
         while sys.stdin in select.select([sys.stdin], [], [], 0)[0]:	#read from standart input if there is something, otherwise not 
            line = sys.stdin.readline()     #you must press enter before typing the new command
            if line:
                line=raw_input("Enter new command: ")   #the printing of the stm32f4 data is stopped until you type a new reference
                if line:


                    split_command = line.split()

                    #updating reference frequency
                    if     split_command[0]=='d':
                        self.ser.write(line)
                        self.ser.write('\n')
                        self.ser.write('\r')
                    
                    #capturing data into csv
                    elif   split_command[0]=='c':
                        self.capture_data=True
                        self.capture_counter=0
                    elif split_command[0]=='f':
                        self.capture_data=False
                        if   self.print_selection==9: 
                            self.plot_all_in_one()
                            self.plot_one_by_one()
                        else                        :
                            self.plot_selection() 

                    #selecting what to print
                    elif split_command[0]=='p':
                        self.capture_data=False
                        self.print_selection=int(split_command[1])
                        print "line: " + line + " 0: "+split_command[0] +" 1: "+split_command[1]+" int: "+ str(self.print_selection)     
                        self.ser.write(line)
                        self.ser.write('\n')
                        self.ser.write('\r')                       
                                           



