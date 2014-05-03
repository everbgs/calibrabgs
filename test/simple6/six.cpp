#include <stdio.h>
#include <stdlib.h>


void cls(void)
{
	#ifdef 	__linux__
	system("clear");
	#endif
}

int main()
{
	int op;

	do{			
		printf("*** Menu Principal ***\n"
				"1 - Gravar Video\n"
				"2 - Exibir Gravacao\n"
				"3 - Gravar Video (Movimento)\n"
				"4 - Calibrar Ruido\n"
				"0 - Sair\n-> ");
		scanf("%d", &op);
		cls();
	}while (op != 0);	

	return 0;
}
