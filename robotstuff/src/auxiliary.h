//Joe Lombardi what a guy
//Repository for simple functions that take up space in main.c, but could really
//just be placed anywhere else

//Following example placed by Will in the other RBELIB files,
//We define conditions such that the compiler will be happy :^)

#ifndef AUXILIARY_H_
#define AUXILIARY_H_

volatile uint8_t tot_overflow;

void armInitialization(void);
void initializeButtons(void);
int readThatAmperage(int link);

#endif /* AUXILIARY_H_ */
