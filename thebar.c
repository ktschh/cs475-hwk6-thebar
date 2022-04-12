/*
 * TheBar.c
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include "globals.h"
#include "customer.h"
#include "bartender.h"

void printBanner();
void init();
void cleanup();

/**
 * Main function
 */
int main(int argc, char **argv)
{
	num_threads = atoi(argv[1]);

	//printf("%d\n", num_threads);

	printBanner();
	init(); // initialize semaphores

	srand(time(NULL));

	// TODO - fire off customer thread
	pthread_t *bartender_thread = (pthread_t *)malloc(sizeof(pthread_t));
	pthread_t *customers = (pthread_t *)malloc(num_threads * sizeof(pthread_t));

	//printf("allocated space for threads\n");

	int *ids = (int *)malloc(sizeof(int) * num_threads);
	for (int i = 0; i < num_threads; i++)
	{
		ids[i] = i;
		pthread_create(&customers[i], NULL, customer, &ids[i]);
	}

	// TODO - fire off bartender thread
	pthread_create(bartender_thread, NULL, bartender, NULL);

	//printf("created bartender thread\n");

	// TODO - wait for all threads to finish
	for (int i = 0; i < num_threads; i++) 
	{
		pthread_join(customers[i], NULL);
	}

	//printf("exited loop");

	pthread_join(*bartender_thread, NULL);

	//printf("bartender thread rejoined");

	free(bartender_thread);
	free(customers);
	free(ids);

	cleanup(); // cleanup and destroy semaphores
	return 0;
}

/**
 * Prints the activity banner.
 * Do not touch.
 */
void printBanner()
{
	printf("Customer:\t\t\t\t\t\t\t\t\t\t| Employee:\n");
	printf("Traveling\tArrived\t\tOrdering\tBrowsing\tAt Register\tLeaving");
	printf("\t| Waiting\tMixing Drinks\tAt Register\tPayment Recv\n");
	printf("----------------------------------------------------------------------------------------+");
	printf("-----------------------------------------------------------\n");
}

/**
 * Create and initialize semaphores
 */
void init()
{
	// TODO - unlink semaphores
	sem_unlink("/barEmpty");
	sem_unlink("/drinkReady");
	sem_unlink("/paymentConfirmed");
	sem_unlink("/customerArrives");
	sem_unlink("/customerOrders");
	sem_unlink("/customerPays");

	// TODO - create semaphores

	// for customers
	barEmpty = sem_open("/barEmpty", O_CREAT, 0600, 1);
	drinkReady = sem_open("/drinkReady", O_CREAT, 0600, 0);
	paymentConfirmed = sem_open("/paymentConfirmed", O_CREAT, 0600, 0);

	// for bartender
	customerArrives = sem_open("/customerArrives", O_CREAT, 0600, 0);
	customerOrders = sem_open("/customerOrders", O_CREAT, 0600, 0);
	customerPays = sem_open("/customerPays", O_CREAT, 0600, 0);

	printf("finished initializing\n");
}

/**
 * Cleanup and destroy semaphores
 */
void cleanup()
{
	// TODO - close semaphores
	sem_close(barEmpty);
	sem_close(drinkReady);
	sem_close(paymentConfirmed);
	sem_close(customerArrives);
	sem_close(customerOrders);
	sem_close(customerPays);

	sem_unlink("/barEmpty");
	sem_unlink("/drinkReady");
	sem_unlink("/paymentConfirmed");
	sem_unlink("/customerArrives");
	sem_unlink("/customerOrders");
	sem_unlink("/customerPays");
}
