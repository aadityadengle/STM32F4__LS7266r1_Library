/*
 * LS7266 Chip Driver for STM32F405.
 *
 * @author	Aaditya Dengle
 * @email	aadityadengle@gmail.com
 * @version	v1.0 (Beta)
 * @Release 17_Jan_2015.
 * @ide		CooCox	CoIDE
 * @license	GNU GPL v3
 *
 * |----------------------------------------------------------------------
 * | Copyright (C) Aaditya Dengle, 2015
 * |
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * |
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 *
 * Define your LS7266r1 Data and control bus here.
 * |-------------------------------------------------------------------------------------------------------------
 * |Definition		|			Expected Value				|	Default		|
 * |--------------------------------------------------------------------------------------------------------------
 * |#define LS_DATA_PORT|	Your Port on which Data and Control bus is located. 	|	GPIOC		|
 * |			|	There is only single port to be used with both bus.	|			|
 * |#define LS_DATA_BUS	|	8 pins connected to D0.....D7 on LS7266R1.		|PC0....PC7 = D0....D7	|
 * |#define LS_Pin_CD	|	Pin connected to C/(D) pin on LS7266R1.			|	PC8		|
 * |#define LS_Pin_WR	|	Pin connected to (WR) pin on LS7266R1.			|	PC9		|
 * |#define LS_Pin_CS	|	Pin connected to (CS) pin on LS7266R1			|	PC12		|
 * |#define LS_Pin_RD	|	Pin connected to (RD) pin on LS7266R1			|	PC10		|
 * |#define LS_Pin_XY	|	Pin connected to (X)/Y pin on LS7266R1			|	PC11		|
 * --------------------------------------------------------------------------------------------------------------
 *
 * Pin_13 (PC13) usually found to be connected with LED. Avoid using pins Connected to LED. If you have not left
 * with any choice  then at least do not use LED pins for Data bus (i.e. D0...D7).
 * */
// Your Definitions Here.
