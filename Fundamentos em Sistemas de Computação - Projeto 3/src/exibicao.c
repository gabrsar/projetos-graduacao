#include "exibicao.h"

/* printf fflush */
void pff(char *texto)
{

	fflush(stdout);
	printf(texto);
	fflush(stdout);

}

// Conta quantos caracteres que contam como 2 mas exibem como 1 uma string tem.
int exibicao_contarEspeciais(char *s)
{

	int especiais = 0;
	char *lista = "áéíóúàêâôÁÉÍÓÚÂÊÔÃÕ";

	int tl = strlen(lista);

	int t = strlen(s);
	for (int i = 0; i < t; i++)
	{

		for (int j = 0; j < tl; j++)
		{
			if (s[i] == lista[j])
			{
				especiais++;
				break;
			}
		}

	}

	return especiais / 2;
}

/* thread print formated flushed */
void TPFF(int t, char *formato, int argc, ...)
{

	char buffer[1024];

	va_list argptr;

	va_start(argptr, argc);

	int caracteresVisiveis = vsprintf(buffer, formato, argptr);

	va_end(argptr);

	if (t == 1)
	{

		// Exibe alinhado a esquerda
		printf("%s\n%sThread 1%s :: %s%s%s", _COR_TEXTO_PADRAO_, _COR_T1_, _COR_TEXTO_PADRAO_, _COR_T1_TEXTO_, buffer, _COR_TEXTO_PADRAO_);

	}
	else
	{
		if (_USAR_ALINHAMENTO_DE_TEXTO)
		{
			char buffer2[1024];
			int caracteresTotal = sprintf(buffer2, "%s%s%s%s :: %sThread 2%s", _COR_TEXTO_PADRAO_, _COR_T2_TEXTO_, buffer, _COR_TEXTO_PADRAO_, _COR_T2_, _COR_TEXTO_PADRAO_);

			int ajuste = caracteresTotal - caracteresVisiveis + exibicao_contarEspeciais(buffer2) - 12;

			printf("\n%*s", _TERMINAL_COLUNAS_ + ajuste, buffer2);
		}
		else
		{
			// Exibe alinhado a esquerda
			printf("%s\n%sThread 2%s :: %s%s%s", _COR_TEXTO_PADRAO_, _COR_T2_, _COR_TEXTO_PADRAO_, _COR_T2_TEXTO_, buffer, _COR_TEXTO_PADRAO_);

		}
	}
	fflush(stdout);
}

void exibicao_iniciar()
{
	struct winsize ws;
	ioctl(0, TIOCGWINSZ, &ws);

	_TERMINAL_LINHAS_ = ws.ws_row;
	_TERMINAL_COLUNAS_ = ws.ws_col;
	_USAR_ALINHAMENTO_DE_TEXTO = 1;

	//printf("Terminal com %dx%d caracteres.", _TERMINAL_LINHAS_, _TERMINAL_COLUNAS_);

}

