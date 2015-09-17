#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
	key_t key;
	int shmid;
	key = ftok("/dev/null", '1');
	shmid = shmget(key, 1024, IPC_CREAT | 0666);
	printf("%d\n", shmid );
	void* segment=shmat(shmid, NULL, IPC_CREAT | 0666);
	char op = getchar();
	int * valores=(int*)segment;
	switch(op){
		case '+':
			valores[0]++;
			printf("%d\n",valores[0]);
			//sumar
			break;
		case '-':
			valores[0]--;
			printf("%d\n",valores[0]);
			//restar
			break;
		case 'd':
			//destroy segment
			shmdt(segment);
			shmctl(shmid,  IPC_RMID, NULL);
	}
	//desacoplamos el segmento
	shmdt(segment);
	return 0;
}