#pragma once

#define PORT_NAM_RESET 0x0000

#define PORT_NAM_MASTER_VOLUME    0x0002
#define PORT_NAM_MONO_VOLUME 	  0x0006
#define PORT_NAM_PC_BEEP	  	  0x000a
#define PORT_NAM_PCM_VOLUME		  0x0018
#define PORT_NAM_EXT_AUDIO_ID	  0x0028
#define PORT_NAM_EXT_AUDIO_STC	  0x002a
#define PORT_NAM_FRONT_SPLRATE 	  0x002c
#define PORT_NAM_LR_SPLRATE 	  0x0032
#define PORT_NABM_POBDBAR 	      0x0010
#define PORT_NABM_POLVI 		  0x0015
#define PORT_NABM_POCONTROL 	  0x001b
#define PORT_NABM_GLB_CTRL_STAT   0x0060

void initAC97(float volume) // https://www-lowlevel-eu.translate.goog/wiki/AC97?_x_tr_sch=http&_x_tr_sl=fr&_x_tr_tl=en&_x_tr_hl=fr&_x_tr_pto=wapp
{
	int nambar, nabmbar;

	volume /= 100.;
	volume = 1 - volume;
	volume *= 63;

	outw(nambar + PORT_NAM_RESET, 42); 
	outb(nabmbar + PORT_NABM_GLB_CTRL_STAT, 0x02); 
	Sleep(100); 
	volume = 0; 
	outw(nambar + PORT_NAM_MASTER_VOLUME, (volume << 8) | volume); 
	outw(nambar + PORT_NAM_MONO_VOLUME, volume); 
	outw(nambar + PORT_NAM_PC_BEEP, volume);
	outw(nambar + PORT_NAM_PCM_VOLUME, (volume << 8) | volume);
	Sleep(10); 
	if(!(inw(nambar + PORT_NAM_EXT_AUDIO_ID) & 1)) 
	{
		;
	} 
	else 
	{
		outw(nambar + PORT_NAM_EXT_AUDIO_STC, inw(nambar + PORT_NAM_EXT_AUDIO_STC) | 1);
		Sleep(10); 
		outw(nambar + PORT_NAM_FRONT_SPLRATE, 44100); 
		outw(nambar + PORT_NAM_LR_SPLRATE, 44100);
		Sleep(10); 
	}
}