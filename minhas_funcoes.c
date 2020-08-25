#include "minhas_funcoes.h"

/*
Lê todas as palavras 1 por linha, de 'fp', retorna ponteiro para ponteiro de todas as strings alocadas com sucesso, de outra forma retorna NULL, 'n' é atualizado com número de palavras lidas.
*/
char **pegaPalavras(FILE *fp, int *n)
{
    char **palavras = NULL;
    char buf[comprimentoDasPalavras + 1] = {0};
    int maxlen = comprimentoDaLista > 0 ? comprimentoDaLista : 1;
    if (!(palavras = calloc(maxlen, sizeof *palavras)))
    {
        fprintf(stderr, "pegaPalavras() erro: memória virtual exaurida.\n");
        return NULL;
    }
    while (fgets(buf, comprimentoDasPalavras + 1, fp))
    {
        size_t wordlen = strlen(buf);
        if (buf[wordlen - 1] == '\n')
            buf[--wordlen] = 0;
        palavras[(*n)++] = strdup(buf);
        if (*n == maxlen)
        {
            void *tmp = realloc(palavras, maxlen * 2 * sizeof *palavras);
            if (!tmp)
            {
                fprintf(stderr, "pegaPalavras() realloc: memória exaurida.\n");
                return palavras;
            }
            palavras = tmp;
            memset(palavras + maxlen, 0, maxlen * sizeof *palavras);
            maxlen *= 2;
        }
    }
    return palavras;
}
void liberaArray(char **words, int rows)
{
    int i;
    for (i = 0; i < rows; i++)
    {
        free(words[i]);
    }
    free(words);
}

//Verifico se a palavra existe na lista de palavras ou não
int Existe(char *palavra, char **palavras, int nPalavras)
{
    int i;
    for (i = 0; i < nPalavras; i++)
    {
        int comparacao = strcmp(palavra, palavras[i]);

        if (comparacao == 0)
        {
            return 1;
        }
    }
    return 0;
}

//Corrijo a palavra e retorno o que o usuário escolheu.
char *corrigePalavra(char *palavra, char **palavras, int nPalavras)
{
    int menor1 = levenshtein(palavra, palavras[0]);
    int menor2 = levenshtein(palavra, palavras[1]); //Preencho logo as 3 primeiras posições como as menores
    int menor3 = levenshtein(palavra, palavras[2]); //Se houverem distâncias menores, trocamos.
    int pos1 = 0;
    int pos2 = 0;
    int pos3 = 0;

    for (int i = 3; i < 1012040; i++)
    {
        int alterado = 0;
        int distancia = levenshtein(palavra, palavras[i]);
        if (distancia < menor1 && alterado == 0)
        {                       //Não é <= para não ficar pegando sempre as últimas palavras de mesma distância
            menor1 = distancia; //Para variar um pouco as sugestões.
            pos1 = i;
            alterado = 1;
        }
        else if (distancia <= menor2 && alterado == 0)
        {
            menor2 = distancia;
            pos2 = i;
            alterado = 1;
        }
        else if (distancia <= menor3 && alterado == 0)
        {
            menor3 = distancia;
            pos3 = i;
            alterado = 1;
        }
    }
    int opcao;
    do
    { //esse loop pelo menos uma vez antes de verificar a condição do while
        printf("\n \n A palavra %s pode estar escrita incorretamente, as opções de substituição são: \n 1.%s \n 2.%s \n 3.%s \n", palavra, palavras[pos1], palavras[pos2], palavras[pos3]);
        printf("\n Digite o número da palavra a ser usada ou 0 para permanecer com a palavra %s: ", palavra);
        scanf("%d", &opcao);

        if (opcao == 1)
        { //se escolher 1, escolhe a primeira palavra sugerida
            return palavras[pos1];
        }
        else if (opcao == 2)
        { //se escolher 2, escolhe a segunda palavra sugerida
            return palavras[pos2];
        }
        else if (opcao == 3)
        { //se ecolher 3, escolhe a terceira palavra sugerida
            return palavras[pos3];
        }
        else if (opcao == 0)
        {                   //se não, retorna ela original(Tem que ser dentro de um loop p forçar
            return palavra; //o usuário a digitar o número 0.)
        }
    } while (0 == 0);
}

//Função que retorna a distância de levenshtein.
int levenshtein(const char *s, const char *t)
{
    int ls = strlen(s), lt = strlen(t);
    int d[ls + 1][lt + 1];
    for (int i = 0; i <= ls; i++)
    {
        for (int j = 0; j <= lt; j++)
        {
            d[i][j] = -1;
        }
    }
    int dist(int i, int j)
    {
        if (d[i][j] >= 0)
            return d[i][j];
        int x;
        if (i == ls)
            x = lt - j;
        else if (j == lt)
            x = ls - i;
        else if (s[i] == t[j])
            x = dist(i + 1, j + 1);
        else
        {
            x = dist(i + 1, j + 1);
            int y;
            if ((y = dist(i, j + 1)) < x)
                x = y;
            if ((y = dist(i + 1, j)) < x)
                x = y;
            x++;
        }
        return d[i][j] = x;
    }
    return dist(0, 0);
}

