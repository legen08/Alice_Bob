#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<gmp.h>
//#include"mini-gmp.h"
//#include"mini-gmp.c"

mpz_t g, p, A;
//mpz_init(g);
//mpz_init(p);
//mpz_init(A);

struct threads {
	pthread_t thread;
	int step;
	int id;
};

void* worker_behavior(struct threads* t) {
	mpz_t a, A_temp;
	mpz_init_set_ui(a, (unsigned long int)t->id);
	mpz_init_set_ui(A_temp, (unsigned long int)t->id);
	while (mpz_cmp(A, A_temp)) {
		mpz_powm(A_temp, g, a, p);
		mpz_add_ui(a, a, t->step);
	}
	mpz_sub_ui(a, a, t->step);
	printf("result: ");
	mpz_out_str(stdout, 10, a);
	printf("\n");
	exit(0);
}

int main(int argc, char* argv[]) {
	if (argc > 5) {
		fprintf(stderr, "Неверное количество аргументов\n");
		return 1;
	}
	int worker_count = atoi(argv[1]);
	struct threads worker[worker_count];
	mpz_set_str(g, argv[2], 10);
	mpz_set_str(p, argv[3], 10);
	mpz_set_str(A, argv[4], 10);
	int i;
	for (i = 0; i < worker_count; i++) {
		worker[i].id = i + 1;
		worker[i].step = worker_count;
		pthread_create(&worker[i].thread, NULL, (void* (*) (void *)) worker_behavior, &worker[i]);
	}
	void* result;
	for (i = 0; i < worker_count; i++) {
		pthread_join(worker[i].thread, &result);
	}
	mpz_clear(g);
	mpz_clear(p);
	mpz_clear(A);
	return 0;
}
