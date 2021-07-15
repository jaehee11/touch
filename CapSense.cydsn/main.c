/******************************************************************************
* Project Name		: CapSense
* File Name			: main.c
* Version 			: 1.0
* Device Used		: CY8C4245AXI-483
* Software Used		: PSoC Creator 4.2
* Compiler    		: ARM GCC 5.4.1
* Related Hardware	: CY8CKIT-042 PSoC 4 Pioneer Kit
*
*******************************************************************************
* Copyright (2019), Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* (“Software”), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries (“Cypress”) and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software (“EULA”).
*
* If no EULA applies, Cypress hereby grants you a personal, nonexclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress’s integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death (“High Risk Product”). By
* including Cypress’s product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

/******************************************************************************
*                           THEORY OF OPERATION
* This project demonstrates the CapSense application using on board 5 segment
* CapSense slider. The multicolor LED on the CY8CKIT-042 changes color from red
* to green as you move the finger along the slider.
*
* Hardware connection on the Kit
* Slider	 - P1[1:5]
* LED_Green	 - P0[2]
* LED_Red 	 - P1[6]
******************************************************************************/
#include <project.h>


/* Define constant for CapSense slider */
#define NO_FINGER 0xFFFFu

/* Uncomment the following line to enable the tuner. Disabling the tuner allows
 * the project to operate in a stand-alone fashion without connecting
 * the I2C tuning interface */
#define ENABLE_TUNER

uint16 sliderPosition = NO_FINGER;
uint16 lastPosition = NO_FINGER;

void UpdateRGB(void);

int main()
{
	/* Enable Global Interrupts */
	CyGlobalIntEnable;

	/* Start all the components */
	Clock_Start();
	PWM_Start();
   
	#ifdef ENABLE_TUNER
		CapSense_TunerStart();
	#else
		CapSense_Start();

	/* Initialize CapSense baselines by scanning enabled sensors */
	    CapSense_InitializeAllBaselines();
	#endif
    
	for(;;)
	{
		#ifdef ENABLE_TUNER
			CapSense_TunerComm();
	            	UpdateRGB();
		#else
			/* Update all baselines */
			CapSense_UpdateEnabledBaselines();

			/* Start scanning all enabled sensors */
	    	CapSense_ScanEnabledWidgets();

			/* Wait for scanning to complete */
			while(CapSense_IsBusy()!= 0)
        	{
				UpdateRGB();
			}

		#endif
	}
}
void UpdateRGB(void)
{
	/* Find Slider Position */
	sliderPosition = CapSense_GetCentroidPos(CapSense_LINEARSLIDER0__LS);

	/*If finger is detected on the slider*/
	if(sliderPosition != NO_FINGER)
	{
		/* If finger position on the slider is changed then update the LED color */
    	if(sliderPosition != lastPosition)
    	{
			/* Update the PWM pulse width with the new slider position */
        	PWM_WriteCompare(sliderPosition);
			lastPosition = sliderPosition;
		}
	}
}

/* [] END OF FILE */
