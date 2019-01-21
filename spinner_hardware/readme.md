Hardware
====

Tools required:
-------
 - FDM 3D printer
 - Metal lathe
 - CNC Mill
 - Laser cutter (optional)

Bill of materials:
-----
 - Arduino-compatible microcontroller (Arduino Mega 2560 recommended)
 - BLDC outrunner motor: Low KV, high torque. (AL4263-900KV recommended)
 - ESC: Maytech 45A-OPTO
 - PSU: 12V, 50A or higher. 
 - Tachometer: SparkFun Line Sensor Breakout - QRE1113
 - Flexible vacuum hose (OD = 7mm, ID = 4mm)
 - Couplers: MJC25-10-A, MJC25-6-A, JD16-25-92Y
 - Airtight sealed bearings, 2x: OD = 28mm, ID = 12mm, h = 8mm
 - Circlips, 2x: Ø11.50mm
 - Round aluminium bar (for shaft and chuck)
 - Filament (for 3D printed parts)
 - 5mm PMMA (acrylic) sheet (for lasercut parts)
 - Vibration-dampening rubber feet, 4x: SFF-018
 - Large O-rings, 2x: 26.5x3mm
 - Small O-rings, 3x: 9.25x178mm
 - White and black tape
 - Vacuum ejector: ZH10BS-06-06 

Instructions:
-----
 - Obtain all the materials in 'bill of materials'. 
 - 3D print the following files (found in 'stl' folder).
	* 1x: [SOLW_20170704_vacuum_chamber_for_print.STL](https://github.com/cphnano/spincoater/raw/master/spinner_hardware/STL%20files/SOLW_20170704_vacuum_chamber_for_print.STL)
	* 1x: [SOLW_20170718_Cover.STL](https://github.com/cphnano/spincoater/raw/master/spinner_hardware/STL%20files/SOLW_20170718_Cover.STL)
	* 1x: [SOLW_20170719_Chamber.STL](https://github.com/cphnano/spincoater/raw/master/spinner_hardware/STL%20files/SOLW_20170719_Chamber.STL)
	* 1x: [SOLW_20170724_tuber_holder.STL](https://github.com/cphnano/spincoater/raw/master/spinner_hardware/STL%20files/SOLW_20170724_tube_holder.STL)
	* 2x: [SOLW_20170724_leg_of_Frequency_sensor.STL](https://github.com/cphnano/spincoater/raw/master/spinner_hardware/STL%20files/SOLW_20170724_leg_of_Frequency_sensor.STL)
	* 1x: [Splash-guard SOLW_20170517_Chamber_inclined.STL](https://github.com/cphnano/spincoater/raw/master/spinner_hardware/STL%20files/SOLW_20170517_Chamber_inclined.STL)

 - Laser cut or 3D print the following files: 
	* 1x: [SOLW_20170719_Bottom.STL](https://github.com/cphnano/spincoater/raw/master/spinner_hardware/STL%20files/SOLW_20170719_Bottom.STL)
	* 1x: [SOLW_20170706_Base_plate.STL](https://github.com/cphnano/spincoater/raw/master/spinner_hardware/STL%20files/SOLW_20170706_Base_plate.STL)

 - Use a metal lathe to create the shaft from aluminium bar stock (see [SOLW_20170704_double_bearing_Shaft_with_Oring_seat.pdf](https://github.com/cphnano/spincoater/raw/master/spinner_hardware/Drawings/SOLW_20170704_double_bearing_Shaft_with_Oring_seat.pdf) for working drawing). The shaft should be tapped with an M5 internal thread. 
 - Use CNC mill for making the chuck (see [SOLW_20170727_chuck.PDF](https://github.com/cphnano/spincoater/raw/master/spinner_hardware/Drawings/SOLW_20170727_chuck.PDF) for working drawing). There is also DXF and DWG files available for CNC in the 'Drawings' folder. 
 - Cover half of the outrunner motor in black tape, and the other half in white tape, such that it alternates between white and black as it spins. This is used by the reflectance sensor to determine rotation speed. 
 - Use a steel M5 bolt and drill a Ø2mm hole through the center. This is for airflow for the vacuum chuck.
 - Assemble the spinner. Refer to [/assembly/SOLW_20180625_spinner_assembly.SLDASM](https://github.com/cphnano/spincoater/raw/master/spinner_hardware/Assembly/SOLW_20180625_spinner_assembly.SLDASM) or the cross-section view found in [/renders/SOLW_20180625_spinner_assembly_render2.png](https://github.com/cphnano/spincoater/raw/master/spinner_hardware/renders/SOLW_20180625_spinner_assembly_render2.png).
 - Mount the chuck to the shaft using the hollow M5 bolt. 
 - Plug one end of the vacuum tube into the vacuum chamber. Plug the other end into the output side of the vacuum ejector. 
 - Connect compressed air to the vacuum ejector. 
 - Connect motor to ESC.
 - Connect ESC to PSU.
 - Connect ESC and tachometer to arduino. 
 - Flash firmware onto arduino (download the firmware from the 'spinner_firmware' folder and upload it to the board like any other arduino program).
 - You are now ready to start spin coating. See 'spinner_software' folder for further instructions.

