#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


//estruturas


typedef struct turma        		//inteiros representado o código da turma e o ano de oferta da disciplina.
{									//char idica a situação E - Encerrada A - Em andamento C - Cancelada P - Em Preparação
	int codigo,ano,semestre;		//contem ponteiro para alunos que se matricularam em uma disciplina
	char situacao;					//também contem ponteiro um único professor, responsável pela disciplina durante o semestre
	struct professor *prof;			//Turma encerrada é aquela que foi preparada ficou em andamento e encerrou ao fim do semestre normalmente
	struct disciplina *disc;		//Turma em andamento é a que ainda está em execução.
	struct aluno *alu;				//Turma cancelada é a que passou por preparação e momentos antes de encerrar foi finalizada.
	struct turma *proxturm;			//Turma em preparação é a que está aberta para matricula de alunos.
}turma;

typedef struct disciplina            //inteiro código único da disciplina.
{									 //contém ponteiro para uma struct tipo disciplina.
	int codigo,cargahoraria;
	char nome[20];
	struct disciplina *proxdisc;
}disciplina;


typedef struct aproveitamento         //possuei 3 notas referentes a 3 provas realizadas
{									 //em uma disciplina(informação acessada por meio da lista de turma)
	float notas[3];                  //contem ponteria para aproveitamento e para a turma.
	float frequencia;
	struct turma *turma;
	struct aproveitamento *proxaprov;
}aproveitamento;

typedef struct aluno                  //cada aluno possui um nome, 1 registro academico(ra único),
{									  //um ponteiro para o aproveitamento. 
	char nome[80];
	int ra;
	struct aluno *proxal;
	struct aproveitamento *aprov;
}aluno;

typedef struct professor             //ano de entrada = ano que o professor foi contratado
{				
	int codigo;					//ano de saída = ano que o professor encerrou suas atividades.
	char nome[80];					//há um ponteiro para a estrutura turma.
	int anoentrada,anosaida;
	struct turma *turmap;
	struct professor *proxprof;
}professor;

//OBS:
/*
	3 DIFERENTES VISUALIZAÇÕES : ALUNO,PROFESSOR E GERENCIADOR.
	ESSAS VISUALIZAÇÕES SERÃO CONTROLADAS POR UM LOGIN QUE O MESMO SERÁ CONTROLADO POR UM ARUQIVO DE MEMÓRIA EXTERNA.
	LOGIN E SENHA PESSOAL É AQUELE CADASTRADO PELO USUÁRIO EM SEU PRIMEIRO ACESSO
*/

//O ALUNO:
/*
	APÓS REALIZAR O LOGIN O ALUNO PODERÁ:
	-ALTERAR DADOS;
	-REALIZAR MATRICULA EM UMA DISCIPLINA;
	-CANCELAR MATRICULA EM UMA DISCIPLINA;
	VER SEU HISTÓRICO COMPLETO;
	VER HISTÓRICO POR SEMESTRE;
	
	PRIMEIRO ACESSO:
	{
		PARA REALIZAR O PRIMEIRO ACESSO O ALUNO DEVE ENTRAR NO SISTEMA UTILIZANDO O LOGIN E SENHA ENVIADO POR E-MAIL PELO GERENCIADOR(CADA ALUNO TEM POSSE DESSES DADOS).
		JÁ LOGADO COM LOGIN E SENHA O SISTEMA DEVE PEDIR PARA O ALUNO UMA NOVA SENHA.
		LOGIN:Student.
		SENHA:BCC%20#19&*
		
		APÓS REALIZAR O LOGIN O ALUNO DEVE-SE MATRICULAR NO CURSO DE BCC, PARA TAL É NECESSÁRIO SOLICITAR SEU RA E NOME COMPLETO.
		SOMENTE APÓS REALIZAR A MATRICULA O ALUNO TERÁ ACESSO AS OUTRAS INFORMAÇÕES.
		
		CASO O ALUNO ESCOLHA A OPÇÃO REALIZAR MATRICULA EM UMA DISCIPLINA: O SISTEMA DEVE SOLICIAR O NOME DA DISCIPLINA, SE A SITUAÇÃO DE TODAS AS TURMAS DA
		DISCIPLINA ESTIVEREM DIFERENTE DE "EM PREPARAÇÃO" OU A QUANTIDADE DE ALUNOS FOR 45, A MATRICULA DEVE SER REIJEITADA CASO CONTRARIO CADASTRA-SE O ALUNO 
		NA PRIMEIRA TURMA DISPONIVEL.
		
		CASO O ALUNO ESCOLHA CANCELAR MATRICULA EM UMA DISCIPLINA: O SISTEMA DEVE SOLICITAR O NOME DA DISCIPLINA, SE O ALUNO JA POSSUIR ALGUMA NOTA NA TURMA QUE OFERTA
		A DISCIPLINA ENTÃO O CANCELAMENTO DEVE SER REJEITADO, CASO CONTRÁRIO A MATRICULA NA DISCIPLINA SERÁ CANCELADA.
		
		CASO O ALUNO ESCOLHA A OPÇÃO VER HISTORICO COMPLETO: O SISTEMA DEVE MOSTRAR O NOME DE CADA DISCIPLINA ASSOCIADA A UMA TURMA, O ANO EM QUE O ALUNO CURSOU A DISCIPLINA
		AO LADO DE CADA UMA DAS MÉDIAS OBTIDAS.
		
		CASO O ALUNO ESCOLHA A OPÇÃO VER HISTÓRICO POR SEMESTRE: O SISTEMA DEVE SOLICITAR AO ALUNO O ANO E O SEMESTRE E DEVE MOSTRAR O NOME DE CADA DISCIPLINA ASSOCIADA A UMA
		TURMA O ANO DE CADA DE CADA EM QUE O ALUNO CURSOU A DISCIPLINA, AO LADO DE CADA UMA DAS MÉDIAS NO SEMESTRE ESPECIFICADO.
	}

//O PROFESSOR:
/*
	O PROFESSOR APÓS REALIZAR SEU LOGIN DEVE TER ACESSO AS SEGUINTES OPÇÕES:
	-INSERIR NOTAS;
	-ALTERAR NOTAS;
	-INSERIR FREQUENCIA;
	-ALTERAR FREQUENCIA;
	-DESEMPENHO DE UM ALUNO POR SEMESTRE;
	-DESEMPENHO DE UMA TURMA POR SEMESTRE;
	-VER ALUNOS APROVADOS EM TODAS AS DISCIPLINAS QUE LECIONOU;
	-VER ALUNOS REPROVADOS EM TODAS AS DISCIPLINAS QUE LECIONOU;
	-GERAR RELATÓRIO COMPLETO.
	
	PARA REALIZAR O PRIMEIRO ACESSO, O PROFESSOR DEVE ENTRAR NO SISTEMA UTILIZANDO O LOGIN E SENHA PADRÃO.
	LOGADO O SISTEMA DEVE PEDIR UM NOVO USUÁRIO E SENHA PARA CADRASTAR.(O PROFESSOR NÃO DEVE TER ACESSO AS OUTRAS OPÇÕES SEM TER FEITO LOGIN).
	
	LOGIN:Professor
	Senha:BCC%20#19&*
	
	ASSIM QUE O PROFESSOR LOGAR O SISTEMA DEVE PEDIR SEU NOME COMPLETO E OBTER O ANO ATUAL DE SISTEMA PARA INSERIR NO CAMPO ANOENTRADA.
	O CAMPO DE SAIDA DEVE SER REPRESENTADO PELO CARACTER '-' ENQUANTO O PROFESSOR ESTIVER NA INSTITUIÇÃO.
	
	CASO O PROFESSOR ESCOLHA INSERIR NOTAS E FREQUENCIA: O SISTEMA DEVE SOLICITAR O NOME DA DISCIPLINA
	COM O NOME DA DISCIPLINA O SISTEMA DEVE VERIFICAR SE EXISTE ALGUMA TURMA EM ANDAMENTO QUE ELE SEJA RESPONSAVEL, CASO EXISTA
	O SISTEMA DEVE MOSTRAR O NOME DE CADA UM DOS ALUNOS E SOLICITAR CADA UMA DAS 3 NOTAS PREVISTAS E A FREQUENCIA FINAL.
	
	CASO O PROFESSOR ESCOLHA ALTERAR NOTAS E FREQUENCIA: CASO EXISTA ALGUMA TURMA EM SEU NOME O PROFESSOR DEVE DIGITAR O NOME DO ALUNO NO QUAL DESEJA
	ALTERAR AS NOTAS E FREQUENCIA E O SISTEMA DEVE SOLICITAR CADA UMA DAS 3 NOTAS PREVISTAS SEGUIDO DA FREQUENCIA TOTAL
	
	CASO O PROFESSOR ESCOLHAR VER DESEMPENHO DE UM ALUNO POR SEMESTRE: O SISTEMA DEVE SOLICITAR O NOME DO ALUNO, COM O NOME DO ALUNO DEVE VERIFICAR SE O ALUNO 
	EXISTE, CASO EXISTA O SISTEMA DEVE SOLICIAR O ANO E O SEMESTRE PARA BUSCAR, ELE DEVE MOSTRAR O NOME DO ALUNO E ABAIXO UMA LISTA COM O NOME DE CADA UMA DAS DISCIPLINAS
	AO LADO DO ANO EM QUE O ALUNO AS CURSOU, ABAIXO DE CADA DISCIPLINA E ANO O SISTEMA DEVE MOSTRAR A MÉDIA ARITMÉTICA OBTIDA NA DISCIPLINA E SE O ALUNO ESTÁ APROVADO 
	OU REPROVADO
	
	CASO O PROFESSOR ESCOLHA VER DESEMPENHO DE UMA TURMA POR SEMESTRE: O SISTEMA DEVE SOLICITAR O NOME DA DISCIPLINA O ANO E O SEMESTRE, COM OS DADOS DA TURMA
	DEVE VERIFICAR SE ELA EXISTE, CASO SIM O SISTEMA DEVE MOSTRAR O NOME DO ALUNO AO LADO DA MÉDIA ARITMÉTICA OBTIDA NO SEMESTRE ESPECIFICADO SEGUIDO DA CONDIÇÃO DO ALUNO
	"APROVADO OU REPROVADO". (DEVE-SE SER FEITO EM TODOS OS ALUNOS DA TURMA ESCOLHIDA).
	
	CASO O PROFESSOR ESCOLHA VER ALUNOS APROVADOS EM TODAS AS MATÉRIAS QUE OFERTOU: O SISTEMA DEVE REALIZAR UMA BUSCA EM TODAS AS MATÉRIAS QUE OFERTOU, EM TODAS AS TURMAS
	QUE O PROFESSOR LECIONOU E MOSTRAR O NOME E A MÉDIA DE CADA ALUNO QUE FOI APROVADO.(SEPARAR OS DADOS DOS ALUNOS PELO NOME DA DISCIPLINA).
	
	CASO O PROFESSOR ESCOLHA VER ALUNOS REPROVADOS EM TODAS AS MATÉRIAS QUE OFERTOU: REPETIR PROCEDIMENTO DE ALUNOS APROVADOS.
	
	CASO O PROFESSOR ESCOLHA ENCERRAR TURMAS: O SISTEMA DEVE BUSCAR TODAS AS TURMAS QUE O PROFESSOR LECIONA E VER SE A MESMA ESTÁ EM ANDAMENTO E ALTERA-LA PARA ENCERRADA
	
	CASO O PROFESSOR ESCOLHA GERAR RELATORIO COMPLETO: O SISTEMA DEVE REALIZAR UMA BUSCA EM TODAS AS TURMAS QUE O PROFESSOR LECIONOU E MOSTRAR TODOS OS NOMES DE TODAS AS
	DISCIPLINAS QUE LECIONOU ESPECIFICANDO O ANO E O SEMESTRE.
*/


//O GERENCIADOR
/*
	O GERENCIADOR APÓES REALIZAR LOGIN DEVE TER ACESSO AS SEGUINTES OPÇÕES:
	-CADASTRAR ALUNO;
	-CADASTRAR DISCIPLINA;
	-CADASTRAR TURMA;
	-CADASTRAR PROFESSOR;
	-EXCLUIR DISCIPLINA;
	-EXCLUIR TURMA;
	-EXCLUIR PROFESSOR;
	-EXCLUIR ALUNO;
	
	PARA REALIZAR O PRIMEIRO ACESSO O GERENCIADOR DEVE ENTRAR COM LOGIN E SENHA PADRÃO
	
	LOGADO O SISTEMA DEVE PEDIR UM NOVO LOGIN E SENHA PARA CADRASTRAR.
	
	Login:Gerenciador
	Senha:BCC%20#19&*
	
	CASO O GERENCIADOR ESCOLHA A OPÇÃO CADASTRAR ALUNO: O SISTEMA DEVE SOLICITAR O NOME DO ALUNO E GERAR UM NÚMERO DE RA.
	
	CASO O GERENCIADOR ESCOLHA CADASTRAR DISCIPLINA: O SISTEMA DEVE SOLICITAR O NOME E O CÓDIGO DA DISCIPLINA.
	
	CASO O GERENCIADOR ESCOLHA CADASTRAR TURMA: O SISTEMA DEVE SOLICITAR CÓDIGO DA TURMA E NOME DA DISCIPLINA, COM O NOME DA DISCIPLINA DEVE VERIFICAR O ANO ATUAL
	DE SISTEMA PARA INSERIR NO CAMPO ANO. O CAMPO SEMESTRE E O NOME DO PROFESSOR RESPONSAVEL DEVEM SER SOLICITADOS, CASO A DISCIPLINA NÃO EXISTA OU NÃO FORA
	POSSIVEL ENCONTRAR O PROFESSOR RESPONSAVEL  O SISTEMA DEVERÁ IGNORAR O CADASTRO E EMITIR UMA MENSAGEM APROPRIADA.
	TODA TURMA DEVERÁ SER INICIADA COM SITUAÇÃO EM PREPARAÇÃO.
	
	CASO O GERENCIADOR ESCOLHA EXCLUIR DISCIPLINA: O SISTEMA DEVE SOLICITAR O NOME DA DISCIPLINA, COM O NOME DA DISCIPLINA DEVE-SE VERIFICAR NA LISTA TURMA SE
	EXISTE ALGUMA RELAÇÃO COM A DISCIPLINA EM QUESTÃO , CASO HAJA LIGAÇÃO A DISCIPLINA NÃO PODERÁ SER EXCLUIDA,
	
	CASO O GERENCIADOR ESCOLHA EXCLUIR TURMA: O SISTEMA DEVE SOLICITAR O NOME DA DISCIPLINA O ANO E O SEMESTRE. COM OS DADOS DA TURMA ELE DEVE VERIFICAR EM TODAS AS
	LISTAS SE EXISTE ALGUMA RELAÇÃO COM A TURMA EM QUESTÃO, CASO HAJA LIGAÇÃO A TURMA NÃO PODERÁ SER EXCLUIDA.
	
	CASO O GERENCIADOR ESCOLHA EXCLUIR PROFESSOR: O SISTEMA DEVE SOLICITAR O NOME DO PROFESSOR, O PROFESSOR SÓ DEVERÁ SER EXCLUIDO CASO NÃO POSSUA LIGAÇÃO COM OUTRAS LISTAS.
*/


//OBS:

/*
	ALUNO APROVADO = P1+P2+P3 >= 6.
	ALUNO REPROVADO = P1+P2++3 < 6.
*/

/* essa função pega os dados do aluno na hora de logar e joga para o arquivo de login, ex : ra e nome completo
   e então coloca o ra no arquivo de login para saber qual aluno está sendo logado.
*/
void dadosaluno(aluno **primaluno,FILE *arqlogin);

/*essa função pega os dados do professor na hora de logar e joga para o arquivo de login, ex: nome completo e atribui
  um id para o professor colocando esse id no campo código na struct professor.
*/
int dadosprofessor(professor **primprofessor,FILE *arqlogin);

//essa função executa as funções do gerenciador que estão no menu atribuido a ele
void funcoesgerenciador(professor **primprof, aluno **primaluno, turma **primturma,disciplina **primdisc,int *idprof,int *op);

//essa função executa as funções do professor para aquele professor em especifico
void funcoesprofessor(professor *no,int op);


/*OBS : TODAS AS FUNÇÕES DE ALOCAR ALGUM TIPO DE STRUCT OS PONTEIROS DESSA STRUCT APONTAM PARA NULL
  MENOS O PONTEIRO DA STRUCT EM SI OU SEJA CRIA A LISTA DO TIPO SOLICITADO MAS O PONTEIRO PARA AS OUTRAS
  PERMANESCE NULO
  EX: NA FUNÇÃO INSERETURMA
		   noaux->prof = NULL;
			noaux->disc = NULL;
			noaux->alu = NULL;
			noaux->proxturm = *primturma;
			*primturma = noaux;
  
*/

//essa função aloca um nó do tipo aluno
void inserealuno(aluno **primaluno,char nome[],int ra);

//essa função aloca um nó do tipo disciplina
void alocadisciplina(disciplina **primdisc,int codigo,int cargahoraria,char nome[]);

//essa função deleta um nó do tipo disciplina
void deletadisciplina(disciplina **primdisc,disciplina *noaux);

//essa função aloca um nó do tipo turma
void insereturma(turma **primturma,int codigo,int ano,int semestre,char situacao);

//essa função deleta um nó do tipo turma
void deletaturma(turma **primturma,turma *noaux);

//essa função aloca um nó do tipo professor
void alocaprofessor(professor **primprofessor, int anoentrada, int anosaida, char nome[],int codigo);

//esta funcção deleta um nó do tipo professor
void deletaprofessor(professor **primprof,professor *noaux);

//essa função só vai ser executa no primeiro acesso do aluno pedindo um novo login e senha
void primacessoaluno(aluno **primaluno,FILE *arqlogin,int *flag) ;

//essa função só vai ser executa no primeiro acesso do professor pedindo um novo login e senha
void primacessoprofessor(professor **primprofessor,FILE *arqlogin,int *flag);

/*essa função só vai ser executa no primeiro acesso
 do gerenciador pedindo um novo login e senha
  e ja mostra e executa as opções do seu menu.
*/
void primacessogerenciador(FILE *arqlogin,int *flag,professor **primprofessor,aluno **primaluno,
							turma **primturma,disciplina **primdisc,int *idprof);

//executa o login do sistema e ja verifica qual o login que está sendo identficado ex: prof,aluno,gerenciador,primacesso,etc.
void loginn(turma **primturma,aluno **primaluno,disciplina **primdisc,professor **primprofessor,int *idprof);

//no caso se não for o primeiro acesso da pessoa eu tenho que procurar no arquivo o login dela.
void procuralogin(char login[],char senha[],FILE *arqlogin,int *flag,int *codigo);

//essas funções abaixo só leem de um arquivo especifico.
void learquivodisciplina(disciplina **primdisc);
void learquivoturma1(turma **primturma);
void learquivoaluno(aluno **primaluno);
void learquivoprofessor(professor **primprofessor,int *idprof);


//essa função retorna o ano atual do sistema
int pegaanosistema();





void menugerenciador()
{
	printf("1 - CADASTRAR ALUNO\n2 - CADASTRAR DISCIPLINA\n3 - CADASTRAR TURMA\n4 - CADASTRAR PROFESSOR\n5 - EXCLUIR DISCIPLINA\n6 - EXCLUIR TURMA\n7 - EXCLUIR PROFESSOR\n8 - EXCLUIR ALUNO\n");
}

void menuprofessor()
{
	printf("1 - INSERIR NOTAS\n2 - ALTERAR NOTAS\n3 - INSERIR FREQUENCIA\n4 - ALTERAR FREQUENCIA\n5 - DESEMPENHO DE UM ALUNO POR SEMESTRE\n6 - DESEMPENHO DE UMA TURMA POR SEMESTRE\n7 - VER ALUNOS APROVADOS EM TODAS AS DISCIPLINAS\n8 - VER ALUNOS REPROVADOS EM TODAS AS DISCIPLINAS\n9 - GERAR RELATÓRIO COMPLETO\n");
}

void menualuno()
{
	printf("1 - ALTERAR DADOS\n2 - REALIZAR MATRICULA EM UMA DISCIPLINA\n3 - CANCELAR MATRICULA EM UMA DISCIPLINA\n4 - VER SEU HISTÓRICO COMPLETO\n5 - VER HISTÓRICO POR SEMESTRE\n");
}

void primacessoaluno(aluno **primaluno,FILE *arqlogin,int *flag) 
{
	char clogin[15];
	FILE *arq2 = NULL;
	if(arqlogin != NULL)
	{
		fclose(arqlogin);
		arqlogin = NULL;
	}
	arqlogin = fopen("logins.txt","a");
	if(arqlogin == NULL)
	{
		printf("falha ao abrir arquivo de login\n");
		return;
	}
	else
	{
		char login[15],senha[15];
		do
		{
			arq2 = fopen("logins.txt","r");
			if(arq2 == NULL)
			{
				printf("falha ao abrir o arquivo\n");
			}
			printf("digite um novo usuário\n");
			scanf("%s*\n",login);
			setbuf(stdin,NULL);
			printf("digite uma nova senha\n");
			scanf("%s*\n",senha);
			setbuf(stdin,NULL);
			while(!feof(arq2))
			{
				fscanf(arq2,"%s",clogin);
				if(feof(arq2))
				{
					fprintf(arqlogin,"%s %s %d ",login,senha,0);
					*flag = 1;
					dadosaluno(primaluno,arqlogin);
					break;
				}
				if(strcmp(login,clogin) == 0)
				{
					printf("login ja cadastrado\n");
					fclose(arq2);
					arq2 = NULL;
					*flag = 0;
					break;
				}
			}
		}while(*flag == 0);
	}
}

void primacessoprofessor(professor **primprofessor,FILE *arqlogin,int *flag) 
{
	int op = 0,id;
	char clogin[15];
	FILE *arq2 = NULL;
	professor *noaux = *primprofessor;
	if(arqlogin != NULL)
	{
		fclose(arqlogin);
		arqlogin = NULL;
	}
	arqlogin = fopen("logins.txt","a");
	if(arqlogin == NULL)
	{
		printf("falha ao abrir arquivo de login\n");
		return;
	}
	else
	{
		char login[15],senha[15];
		do
		{
			arq2 = fopen("logins.txt","r");
			if(arq2 == NULL)
			{
				printf("falha ao abrir o arquivo\n");
			}
			printf("digite um novo usuário\n");
			scanf("%s*\n",login);
			setbuf(stdin,NULL);
			printf("digite uma nova senha\n");
			scanf("%s*\n",senha);
			setbuf(stdin,NULL);
			while(!feof(arq2))
			{
				fscanf(arq2,"%s",clogin);
				if(feof(arq2))
				{
					fprintf(arqlogin,"%s %s %d ",login,senha,1);
					*flag = 2;
					id = dadosprofessor(primprofessor,arqlogin);
					break;
				}
				if(strcmp(login,clogin) == 0)
				{
					printf("login ja cadastrado\n");
					fclose(arq2);
					arq2 = NULL;
					*flag = 0;
					break;
				}
			}
		}while(*flag == 0);
	}
	while(noaux != NULL)
	{
		if(noaux->codigo == id)
			break;
		noaux = noaux->proxprof;
	}
	printf("bem vindo %s\n",noaux->nome);
	do
	{
		menuprofessor();
		scanf("%d",&op);
		funcoesprofessor(noaux,op);
	}while(op != 0);
}

void primacessogerenciador(FILE *arqlogin,int *flag,professor **primprofessor,aluno **primaluno,turma **primturma,disciplina **primdisc,int *idprof)
{
	int op=0;
	char clogin[15];
	FILE *arq2 = NULL;
	if(arqlogin != NULL)
	{
		fclose(arqlogin);
		arqlogin = NULL;
	}
	arqlogin = fopen("logins.txt","a");
	if(arqlogin == NULL)
	{
		printf("falha ao abrir arquivo de login\n");
		return;
	}
	else
	{
		char login[15],senha[15];
		do
		{
			arq2 = fopen("logins.txt","r");
			if(arq2 == NULL)
			{
				printf("falha ao abrir o arquivo\n");
			}
			printf("digite um novo usuário\n");
			scanf("%s*\n",login);
			setbuf(stdin,NULL);
			printf("digite uma nova senha\n");
			scanf("%s*\n",senha);
			setbuf(stdin,NULL);
			while(!feof(arq2))
			{
				fscanf(arq2,"%s",clogin);
				if(feof(arq2))
				{
					fprintf(arqlogin,"%s %s %d %d\n",login,senha,2,0);
					*flag = 3;
					fclose(arqlogin);
					arqlogin = NULL;
					break;
				}
				if(strcmp(login,clogin) == 0)
				{
					printf("login ja cadastrado\n");
					fclose(arq2);
					arq2 = NULL;
					*flag = 0;
					break;
				}
			}
		}while(*flag == 0);
		do
		{
			menugerenciador();
			scanf("%d",&op);
			funcoesgerenciador(primprofessor,primaluno,primturma,primdisc,idprof,&op);
		}while(op != 0);
		loginn(primturma,primaluno,primdisc,primprofessor,idprof);
	}
}

void funcoesgerenciador(professor **primprof, aluno **primaluno, turma **primturma,disciplina **primdisc,int *idprof,int *op)
{
	switch(*op)
	{
		case 0:
		{
			break;
		}
		case 1:
		{
			aluno *noaux = *primaluno;
			char nome[80];
			int ra,flag = 0;
			srand(time(NULL));
			do
			{
				flag=0;
				ra = (rand()%9+1)*1000000 + rand()%10000;
				setbuf(stdin,NULL);
				printf("ra = %d\n",ra);
				while(noaux != NULL)
				{
					if(noaux->ra == ra)
					{
						flag=1;
						noaux = *primaluno;
						printf("ra ja cadastrado\n");
						break;
					}
					noaux = noaux->proxal;
				}
				if(flag == 0)
				{
					noaux = *primaluno;
					do
					{
						flag = 0;
						printf("digite o nome do aluno\n");
						scanf("%[^\n]",nome);
						setbuf(stdin,NULL);
						while(noaux != NULL)
						{
							if(strcmp(noaux->nome,nome) == 0)
							{
								flag=1;
								noaux = *primaluno;
								printf("nome ja cadastrado\n");
								break;
							}
							noaux = noaux->proxal;
						}
					}while(flag == 1);
					inserealuno(primaluno,nome,ra);
				}
			}while(flag == 1);
			break;
		}
		case 2:
		{
			char nome[20];
			int codigo,flag = 0,cargahoraria;
			disciplina *noaux = *primdisc;
			printf("digite o nome da disciplina\n");
			scanf("%s*\n",nome);
			setbuf(stdin,NULL);
			while(noaux != NULL)
			{
				if(strcmp(nome,noaux->nome) == 0)
				{
					printf("disciplina ja cadastrada\n");
					flag = 1;
					break;
				}
				noaux = noaux->proxdisc;
			}
			if(flag == 0)
			{
				do
				{
					flag = 0 ;
					printf("digite o código da disciplina\n");
					scanf("%d",&codigo);
					noaux = *primdisc;
					while(noaux != NULL)
					{
						if(codigo == noaux->codigo)
						{
							printf("código ja cadastrado\n");
							flag = 1;
						}
						noaux = noaux->proxdisc;
					}
				}while(flag == 1);
				printf("digite a carga horaria da disciplina\n");
				scanf("%d",&cargahoraria);
				alocadisciplina(primdisc,codigo,cargahoraria,nome);
			}
			break;
		}
		case 3:
		{
			break;
		}
		case 4:
		{
			professor *noaux = *primprof;
			char nome[80],flag=0;
			do
			{
				noaux = *primprof;
				flag = 0;
				printf("digite o nome do professor\n");
				scanf("%s*\n",nome);
				setbuf(stdin,NULL);
				while(noaux != NULL)
				{
					if(strcmp(noaux->nome,nome) == 0)
					{
						printf("nome ja existente\n");
						flag = 1;
						break;
					}
					noaux = noaux->proxprof;
				}
			}while(flag == 1);
			(*idprof)++;
			alocaprofessor(primprof,0,0,nome,*idprof);
			break;
		}
		case 5:
		{
			turma *noaux = *primturma;
			disciplina *noaux2 = *primdisc;
			char nome[20];
			printf("digite o nome da disciplina\n");
			scanf("%s*\n",nome);
			setbuf(stdin,NULL);
			while(noaux2 != NULL)
			{
				if(strcmp(noaux2->nome,nome) == 0)
					break;
				noaux2 = noaux2->proxdisc;
				if(noaux2 == NULL)
					printf("disciplina inexistente\n");
			}
			while(noaux != NULL)
			{
				if(noaux->disc == noaux2)
				{
					printf("não é possivel excluir esta disciplina\n");
					break;
				}
				noaux = noaux->proxturm;
			}
			if(noaux == NULL)
			{
				deletadisciplina(primdisc,noaux2);
			}
			break;
		}
		case 6:
		{
			int codigo,ano,semestre;
			turma *noaux = *primturma;
			printf("digite o código da turma\n");
			scanf("%d",&codigo);
			printf("digite o ano\n");
			scanf("%d",&ano);
			printf("digite o semestre\n");
			scanf("%d",&semestre);
			while(noaux != NULL)
			{
				if(noaux->codigo == codigo && noaux->ano == ano && noaux->semestre == semestre)
				{
					break;
				}
				noaux = noaux->proxturm;
			}
			if(noaux == NULL)
			{
				printf("turma inexistente\n");
			}
			else
			{
				if(noaux->disc != NULL || noaux->alu != NULL || noaux->prof != NULL)	
					printf("esta turma possui relação com outras listas e não pode ser excluida\n");
				else
					deletaturma(primturma,noaux);
			}
			break;
		}
		case 7:
		{
			professor *noaux = *primprof;
			char nome[80];
			printf("digite o nome do professor\n");
			scanf("%s",nome);
			setbuf(stdin,NULL);
			while(noaux != NULL)
			{
				if(strcmp(noaux->nome,nome) == 0)
					break;
				noaux = noaux->proxprof;
			}
			if(noaux == NULL)
				printf("este professor não está cadastrado\n");
			else
			{
				if(noaux->turmap != NULL)
					printf("este professor está associado a uma turma e não pode ser deletado\n");
				else
				{
					deletaprofessor(primprof,noaux);
				}
			}
			break;
		}
		case 8:
		{
			aluno *noaux = *primaluno;
			char nome[80];
			printf("digite o nome do aluno\n");
			scanf("%s*\n",nome);
			while(noaux != NULL)
			{
				if(strcmp(noaux->nome,nome) == 0)
					break;
				noaux = noaux->proxal;
			}
			if(noaux == NULL)
				printf("este aluno não está cadastrado\n");
			else
			{
				//faz alguma coisa
			}
			break;
		}
		default:
		{
			printf("opção inválida\n");
			break;
		}
	}
}

void funcoesprofessor(professor *noaux,int op)
{
}

void procuralogin(char login[],char senha[],FILE *arqlogin,int *flag,int *codigo) //se n é login padrão então procuramos se o login é de aluno
{
	char clogin[15],csenha[15];
	int id = 0;
	if(arqlogin != NULL)
	{
		fclose(arqlogin);
		arqlogin = NULL;
	}
	arqlogin = fopen("logins.txt","r");
	if(arqlogin == NULL)
	{
		printf("falha ao abrir arquivo de login");
		return;
	}
	while(!feof(arqlogin))
	{
		fscanf(arqlogin,"%s %s %d %d",clogin,csenha,&id,codigo);
		if(feof(arqlogin))
		{
			fclose(arqlogin);
			arqlogin = NULL;
			*flag = 0;
			break;
		}
		if(strcmp(clogin,login) == 0 && strcmp(csenha,senha) == 0 && id == 0)
		{
			printf("login de aluno\n");
			*flag = 4;
			break;
		}
		if(strcmp(clogin,login) == 0 && strcmp(csenha,senha) == 0 && id == 1)
		{
			printf("login de professor\n");
			*flag = 5;
			break;
		}
		if(strcmp(clogin,login) == 0 && strcmp(csenha,senha) == 0 && id == 2)
		{
			printf("login de gerenciador\n");
			*flag = 6;
			break;
		}
	}
}

void dadosaluno(aluno **primaluno,FILE *arqlogin)
{
	aluno *noaux = *primaluno;
	int op=0;
	char nome[80];
	int ra,flag=0;
	do
	{
		printf("digite seu nome completo\n");
		scanf("%[^\n]s",nome);
		setbuf(stdin,NULL);
		while(noaux != NULL)
		{
			if(strcmp(nome,noaux->nome) == 0)
			{
				flag = 1;
				do
				{
					printf("digite seu ra\n");
					scanf("%d",&ra);
					if(ra != noaux->ra)
						printf("ra inválido\n");
				}while(ra != noaux->ra);
			}
			noaux = noaux->proxal;
		}
		if(flag == 0 )
		{
			printf("nome inválido\n");
			noaux = *primaluno;
		}
	}while(flag != 1);
	printf("%d\n",ra);
	printf("digite o curso em que deseja matricular-se:\n");
	do
	{
		printf("1 - CIÊNCIA DA COMPUTAÇÃO\n");
		scanf("%d%*c",&op);
		setbuf(stdin,NULL);
		if(op != 1)
		{
			printf("opção inválida tente novamente\n");
		}
	}while(op != 1);
	fprintf(arqlogin,"%d\n",ra);
	fclose(arqlogin);
	arqlogin = NULL;
	menualuno();
}

int pegaanosistema()
{
	char dateTime[26], date[5];
	int year;
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	strcpy(dateTime,asctime(tm));
	date[0] = dateTime[strlen(dateTime)-5];
	date[1] = dateTime[strlen(dateTime)-4];
	date[2] = dateTime[strlen(dateTime)-3];
	date[3] = dateTime[strlen(dateTime)-2];
	date[4] = '\0';
	year = atoi(date);
	printf("%d\n",year);
	return year;
}

int dadosprofessor(professor **primprofessor,FILE *arqlogin)
{
	professor *noaux = *primprofessor;
	int flag = 0,anoentrada;
	char nome[80];
	do
	{
		printf("digite seu nome completo\n");
		scanf("%[^\n]",nome);
		setbuf(stdin,NULL);
		while(noaux != NULL)
		{
			if(strcmp(noaux->nome,nome) == 0)
			{
				flag = 1;
				fprintf(arqlogin,"%d\n",noaux->codigo);
				printf("%d\n",noaux->codigo);
				fclose(arqlogin);
				arqlogin = NULL;
				break;
			}
			noaux = noaux->proxprof;
		}
		if(flag == 0)
		{
			printf("nome inválido\n");
			noaux = *primprofessor;
		}
	}while(flag != 1);
	anoentrada = pegaanosistema();
	if(noaux->anoentrada == 0)
	{
		noaux->anoentrada = anoentrada;
		noaux->anosaida = 0;
	}
	return noaux->codigo;
}

void loginn(turma **primturma,aluno **primaluno,disciplina **primdisc,professor **primprofessor,int *idprof)
{
	FILE *arqlogin=NULL;
	int flag=0,codigo=0,op = 0;
	char login[15],senha[15];
	do
	{
		printf("digite o login\n");
		scanf("%s*\n",login);
		if(strcmp(login,"00") == 0)
		{
			return;
		}
		setbuf(stdin,NULL);
		printf("digite a senha\n");
		scanf("%s*\n",senha);
		setbuf(stdin,NULL);
		if(strcmp(login,"Student") == 0 && strcmp(senha,"BCC%20#19&*") == 0)
		{
			primacessoaluno(primaluno,arqlogin,&flag);
		}
		else
		{
			if(strcmp(login,"Professor") == 0 && strcmp(senha,"BCC%20#19&*") == 0)
			{
				if(arqlogin != NULL)
				{
					fclose(arqlogin);
					arqlogin = NULL;
				}
				primacessoprofessor(primprofessor,arqlogin,&flag);
			}
			else
			{
				if(strcmp(login,"Gerenciador") == 0 && strcmp(senha,"BCC%20#19&*") == 0)
				{
					if(arqlogin != NULL)
					{
						fclose(arqlogin),&flag;
						arqlogin = NULL;
					}
					primacessogerenciador(arqlogin,&flag,primprofessor,primaluno,primturma,primdisc,idprof);
				}
				else
				{
					procuralogin(login,senha,arqlogin,&flag,&codigo); // id = ra aluno, codigo professor, 0 gerenciador
					if(flag == 4)
					{
						printf("\nra do aluno = %d\n",codigo);
						menualuno();
					}
					else
					{
						if(flag == 5)
						{
							printf("id  do professor = %d\n",codigo);
							do
							{
								professor *noaux = *primprofessor;
								while(noaux != NULL)
								{
									if(noaux->codigo == codigo)
										break;
									noaux = noaux->proxprof;
								}
								printf("bem vindo %s\n",noaux->nome);
								menuprofessor();
								scanf("%d",&op);
								funcoesgerenciador(primprofessor,primaluno,primturma,primdisc,idprof,&op);
							}while(op != 0);
							loginn(primturma,primaluno,primdisc,primprofessor,idprof);
						}
						else
						{
							if(flag == 6)
							{
								do
								{
									menugerenciador();
									scanf("%d",&op);
									funcoesgerenciador(primprofessor,primaluno,primturma,primdisc,idprof,&op);
								}while(op != 0);
								loginn(primturma,primaluno,primdisc,primprofessor,idprof);
							}
							else
							{
								printf("login inexistente\n");
							}
						}
					}
				}
			}
		}
	}while(flag == 0);
}


void inserealuno(aluno **primaluno,char nome[],int ra)
{
	aluno *noaux = (aluno*)malloc(sizeof(aluno));  
	if(noaux == NULL)
		return;
	noaux->ra = ra;
	strcpy(noaux->nome,nome);
	noaux->aprov = NULL;
	noaux->proxal = *primaluno;
	*primaluno = noaux;
}

void learquivoaluno(aluno **primaluno)
{
	char nome[80];
	int ra;
	FILE *arq = NULL;
	arq = fopen("aluno.txt","r");
	if(arq == NULL)
	{
		printf("falha ao abrir arquivo\n");
	}
	else
	{
		while(!feof(arq))
		{
			fscanf(arq,"%s %d",nome,&ra);
			if(feof(arq))
				break;
			if(strcmp(nome,"NULL") != 0)
				inserealuno(primaluno,nome,ra);
		}
		fclose(arq);
		arq = NULL;
	}
}


void alocadisciplina(disciplina **primdisc,int codigo,int cargahoraria,char nome[])
{
	disciplina *noaux = (disciplina*)malloc(sizeof(disciplina));
	if(noaux == NULL)
		return;
	noaux->codigo = codigo;
	noaux->cargahoraria = cargahoraria;
	strcpy(noaux->nome,nome);
	noaux->proxdisc = *primdisc;
	*primdisc = noaux;
}

void deletadisciplina(disciplina **primdisc,disciplina *noaux)
{
	disciplina *noaux2 = *primdisc;
	if(noaux->proxdisc == NULL)
	{
		while(noaux2->proxdisc != noaux)
		{
			noaux2 = noaux2->proxdisc;
		}
		free(noaux);
		noaux = NULL;
		noaux2->proxdisc = NULL;
	}
	else
	{
		if(noaux == *primdisc)
		{
			noaux2 = noaux2->proxdisc;
			free(noaux);
			noaux = NULL;
			*primdisc = noaux2;
		}
		else
		{
			while(noaux2->proxdisc != noaux)
			{
				noaux2 = noaux2->proxdisc;
			}
			noaux2->proxdisc = noaux->proxdisc;
			free(noaux);
			noaux = NULL;
		}
	}
}

void learquivodisciplina(disciplina **primdisc)
{
	FILE *arq = fopen("disciplina.txt","r");
	int codigo,cargahoraria;
	char nome[20],endereco[20];
	if(arq == NULL)
	{
		printf("falha ao abrir arquivo\n");
	}
	else
	{
		while(!feof(arq))
		{
			fscanf(arq,"%d %d %s %s",&codigo,&cargahoraria,nome,endereco);
			if(strcmp(nome,"NULL") != 0)
				alocadisciplina(primdisc,codigo,cargahoraria,nome);
			if(feof(arq))
				break;
		}
	}
}

void alocaprofessor(professor **primprofessor, int anoentrada, int anosaida, char nome[],int codigo)
{
	professor *noaux = (professor*)malloc(sizeof(professor));
	if(noaux == NULL)
		return;
	strcpy(noaux->nome,nome);
	noaux->anoentrada = anoentrada;
	noaux->anosaida = anosaida;
	noaux->turmap = NULL;
	noaux->codigo = codigo;
	noaux->proxprof = *primprofessor;
	*primprofessor = noaux;	
}

void deletaprofessor(professor **primprof,professor *noaux)
{
	professor *noaux2 = *primprof;
	if(noaux->proxprof == NULL)
	{
		while(noaux2->proxprof != noaux)
		{
			noaux2 = noaux2->proxprof;
		}
		free(noaux);
		noaux = NULL;
		noaux2->proxprof = NULL;
	}
	else
	{
		if(noaux == *primprof)
		{
			noaux2 = noaux2->proxprof;
			free(noaux);
			noaux = NULL;
			*primprof = noaux2;
		}
		else
		{
			while(noaux2->proxprof!= noaux)
			{
				noaux2 = noaux2->proxprof;
			}
			noaux2->proxprof = noaux->proxprof;
			free(noaux);
			noaux = NULL;
		}
	}
}

void learquivoprofessor(professor **primprofessor,int *idprof)
{
	FILE *arqprofessor = fopen("professor.txt","r");
	if(arqprofessor == NULL)
		printf("falha ao abrir arquivo dos professores\n");
	char nome[80];
	int anoentrada,anosaida;
	while(!feof(arqprofessor))
	{
		fscanf(arqprofessor,"%s %d %d",nome,&anoentrada,&anosaida);
		if(feof(arqprofessor))
		{
			break;
		}
		if(strcmp(nome,"NULL") != 0)
		{
			(*idprof)++;
			alocaprofessor(primprofessor,anoentrada,anosaida,nome,*idprof);
		}
	}
}

void insereturma(turma **primturma,int codigo,int ano,int semestre,char situacao)
{
	turma *noaux = (turma*)malloc(sizeof(turma));
	if(noaux == NULL)
		return;
	noaux->codigo = codigo;
	noaux->ano = ano;
	noaux->semestre = semestre;
	noaux->situacao = situacao;
	noaux->prof = NULL;
	noaux->disc = NULL;
	noaux->alu = NULL;
	noaux->proxturm = *primturma;
	*primturma = noaux;
}

void deletaturma(turma **primturma,turma *noaux)
{
	turma *noaux2 = *primturma;
	if(noaux->proxturm == NULL)
	{
		while(noaux2->proxturm != noaux)
		{
			noaux2 = noaux2->proxturm;
		}
		free(noaux);
		noaux = NULL;
		noaux2->proxturm = NULL;
	}
	else
	{
		if(noaux == *primturma)
		{
			noaux2 = noaux2->proxturm;
			free(noaux);
			noaux = NULL;
			*primturma = noaux2;
		}
		else
		{
			while(noaux2->proxturm != noaux)
			{
				noaux2 = noaux2->proxturm;
			}
			noaux2->proxturm = noaux->proxturm;
			free(noaux);
			noaux = NULL;
		}
	}
}
void learquivoturma1(turma **primturma)
{
	// int codigo,ano;					
	// char semestre,situacao;			
	// struct professor *prof;		
	// struct disciplina *disc;		
	// struct aluno *alu;				
	// struct turma *proxturm;	
	int codigo,semestre,ano;
	char situacao,endereco[50];
	FILE *arq = fopen("turma.txt","r");
	if(arq == NULL)
		printf("falha ao abrir arquivo\n");
	while(!feof(arq))
	{
		fscanf(arq,"%d %d %d %c %[^\n]",&codigo,&ano,&semestre,&situacao,endereco);
		insereturma(primturma,codigo,ano,semestre,situacao);
		if(feof(arq))
			break;
	}
}	


void main()
{
	int idprof=0,a;
	a = pegaanosistema();
	turma *primturma = NULL,*noaux4 = NULL;
	aluno *primaluno = NULL,*noaux = NULL;
	disciplina *primdisc = NULL,*noaux3 = NULL;
	professor *primprofessor = NULL,*noaux2 = NULL;
	learquivoaluno(&primaluno);
	learquivoprofessor(&primprofessor,&idprof);
	learquivodisciplina(&primdisc);
	learquivoturma1(&primturma);
	noaux = primaluno;
	noaux2 = primprofessor;
	noaux3 = primdisc;
	noaux4 = primturma;
	printf("ALUNOS:\n");
	while(noaux != NULL)
	{
		printf("%s %d\n",noaux->nome,noaux->ra);
		noaux = noaux->proxal;
	}
	printf("\nPROFESSORES:\n");
	while(noaux2 != NULL)
	{
		printf("%s %d %d %d\n",noaux2->nome,noaux2->anoentrada,noaux2->anosaida,noaux2->codigo);
		noaux2 = noaux2->proxprof;
	}
	printf("\nDISCIPLINAS:\n");
	while(noaux3 != NULL)
	{
		printf("%d %d %s\n",noaux3->codigo,noaux3->cargahoraria,noaux3->nome);
		noaux3 = noaux3->proxdisc;
	}
	printf("\nTURMAS:\n");
	while(noaux4 !=  NULL)
	{
		printf("%d %d %d %c\n",noaux4->codigo,noaux4->ano,noaux4->semestre,noaux4->situacao);
		noaux4 = noaux4->proxturm;
	}
	loginn(&primturma,&primaluno,&primdisc,&primprofessor,&idprof); //login
	noaux = primaluno;
	noaux2 = primprofessor;
	noaux3 = primdisc;
	noaux4 = primturma;
	printf("ALUNOS:\n");
	while(noaux != NULL)
	{
		printf("%s %d\n",noaux->nome,noaux->ra);
		noaux = noaux->proxal;
	}
	printf("\nPROFESSORES:\n");
	while(noaux2 != NULL)
	{
		printf("%s %d %d %d\n",noaux2->nome,noaux2->anoentrada,noaux2->anosaida,noaux2->codigo);
		noaux2 = noaux2->proxprof;
	}
	printf("\nDISCIPLINAS:\n");
	while(noaux3 != NULL)
	{
		printf("%d %d %s\n",noaux3->codigo,noaux3->cargahoraria,noaux3->nome);
		noaux3 = noaux3->proxdisc;
	}
	printf("\nTURMAS:\n");
	while(noaux4 !=  NULL)
	{
		printf("%d %d %d %c\n",noaux4->codigo,noaux4->ano,noaux4->semestre,noaux4->situacao);
		noaux4 = noaux4->proxturm;
	}
}