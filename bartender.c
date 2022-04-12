/*
 * bartender.c
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include "globals.h"
#include "bartender.h"

/**
 * Code for bartender thread.
 * Do not touch.
 */
void *bartender(void *args)
{
	int i;
	for (i = 0; i < num_threads; i++)
	{
		waitForCustomer();
		makeDrink();
		receivePayment();
	}
	return NULL;
}

/**
 * Waits in a closet until a customer enters.
 */
void waitForCustomer()
{
	// TODO - synchronize
	printf("\t\t\t\t\t\t\t\t\t\t\t| Bartender\n");
	sem_wait(customerArrives);
}

/**
 * When a customer places an order it takes the Bartender
 * a random amount of time between 5 ms and 1000 ms to make the drink.
 */
void makeDrink()
{
	// TODO - synchronize
	sem_wait(customerOrders);

	// makes drink
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\tBartender\n");
	unsigned int mixTime = (rand()%995 + 5) * 1000;
	usleep(mixTime);
	sem_post(drinkReady);
}

/**
 * Gets payment from the correct customer
 */
void receivePayment()
{
	// TODO - synchronize
	// at the register waiting for customer to pay
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\tBartender\n");
	sem_wait(customerPays);
	sem_post(paymentConfirmed);

	// got paid by the customer!
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\t\t\tBartender\n");
}
