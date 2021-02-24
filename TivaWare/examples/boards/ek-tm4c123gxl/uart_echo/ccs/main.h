/*
 * main.h
 *
 *  Created on: Feb 24, 2021
 *      Author: paduc
 */

#ifndef MAIN_H_
#define MAIN_H_


#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif
enum {
    CAN_TX,
    CAN_RX,
};



#endif /* MAIN_H_ */
