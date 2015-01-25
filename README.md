# STM32F4__LS7266r1_Library
Initialize and read LS7266r1 with STM32f4

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
 *	This library drives LS7266r1 from LSI/CSI to read Quadrature Encoder.
 *	Although STM32F4xx and other ARM Cortex MCUs has good capability to decode
 *	the Quadrature Encoder; It is always better to use dedicated hardware to do the
 *	type of work.
 *************** IMPORTANT *******************************************************************
 *	This library uses Tilen Majerle's tm_stm32f4_delay library which can be fetched from
 *	http://stm32f4-discovery.com/2014/04/library-03-stm32f429-discovery-system-clock-and-pretty-precise-delay-library/
 *********************************************************************************************
 */
