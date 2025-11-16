#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definições do cabeçalho
#define TAMBUFFER ( *( ( int* )p ) ) //Tamanho em bytes do pBuffer
#define PERTOREGISTER ( *( ( int* )( p + sizeof( int ) ) ) ) //um inteiro que, somado com pBuffer, resulta no endereço que antecede a ultima string da pessoa sendo inserida/pesquisada
#define NUMPER ( *( ( int* )( p + 2 * sizeof( int ) ) ) ) //numero de pessoas no pBuffer
#define FUNR ( *( ( int* )( p + 3 * sizeof( int ) ) ) ) //espaço de uso geral para as diversas funções

// Protótipos das funções
void * PedePessoa( void * p ); //regitra uma string no endereço seguinte a p + PERTOREGISTER. Utiliza de FUNR para ler char's
void * Adicionar( void * p ); //gerencia o processo de adicionar uma nova pessoa ao pBuffer
void Listar ( void * p ); //gerencia o processo de listagem de pessoas
void * Imprimir ( void * per); //apartir do endereço de uma pessoa (per), imprime nome, idade e email dessa pessoa
void * Buscar ( void * p ); //gerencia o processo de busca de uma pessoa
void * Pesquisar ( void * p, void * str); //verifica se uma pessoa com nome str encontra-se no pBuffer, retorna o endereço dessa pessoa no pBuffer, ou NULL caso ela não esteja no pBuffer
void * Remover ( void * p ); //gerencia o processo de remoção de uma pessoa
void Sobrescreve ( void * p, void * mop); //sobreescreve uma pessoa no endereço mop deslocando os dados seguintes a essa pessoa. Utiliza do dado em FUNR deixado por pesquisar.
void LimpaBuffer ( void * p ); //limpa residos na entrada de dados, como '\n's da leitura de inteiros no main. Utiliza de FUNR para controlar os residuos lidos

int main( ) {
    void *p; 

    if ( ( p = malloc( 4 * sizeof( int ) ) ) == NULL ) { //reserva espaço para o "Cabeçario" do pBuffer
        printf( "Não foi possivel alocar o pBuffer!" );
        exit( 1 );
    };

    // inicializações do cabeçario
    TAMBUFFER = 4 * sizeof( int ); 
    PERTOREGISTER = TAMBUFFER;     
    NUMPER = 0;                    

    for ( ; ; ) { //menu
        printf( "\n\n1- Adicionar Pessoa (Nome, Idade, email) \n2- Remover Pessoa \n3- Buscar Pessoa \n4- Listar todos \n5- Sair \n" );
        scanf( "%d", (int*)( p + 3 * sizeof( int ) ) ); //FUNR recebe a opção digitada
        
        switch ( FUNR ) {
            case 1:
                LimpaBuffer( p );
                p = Adicionar( p );
                break;
            case 2:
                LimpaBuffer( p );
                p = Remover( p );
                break;
            case 3:
                LimpaBuffer( p );
                p = Buscar( p );
                break;
            case 4:
                LimpaBuffer( p );
                Listar( p );
                break;
            case 5:
                LimpaBuffer( p );
                free( p );
                return 0;
            default:
                LimpaBuffer( p );
                printf( "Numero invalido!" );
        };
    };
}

// Funções
void * Adicionar( void * p ) { 

    //adição e verificação do nome
    
    if ( ( p = realloc( p, TAMBUFFER + sizeof( int ) ) ) == NULL ) { //Reserva espaço para o inteiro: tamanho do nome
        printf( "\nNão foi possivel expandir o pBuffer!\n" );
        exit( 2 );
    };
    *( int* )( p + PERTOREGISTER ) = 0; //define o tamanho do nome da pessoa para 0, pois o nome não foi lido ainda
    TAMBUFFER += sizeof( int );
    
    printf( "\nDigite um nome: " );
    p = PedePessoa( p ); 

    if ( Pesquisar( p, ( char* )( p + PERTOREGISTER + sizeof( int ) ) ) != NULL ) { //verifica se a pessoa já esta no pBuffer
        printf( "\nEssa pessoa já existe!!!\nSe você deseja atualiza-la, exclua-a e adicione-a novamente" );
        TAMBUFFER = PERTOREGISTER; //reseta o tamanho do pBuffer, eliminando o nome digitado e seu inteiro de tamanho do pBuffer
        if ( ( p = realloc( p, TAMBUFFER ) ) == NULL ) { //verifica se o pBuffer não conseguiu diminuir, o que seria bizaaaaaro, mes vai que...
            printf( "\nNão foi possivel realocar o pBuffer!\n" );
            exit( 3 );
        };
        return p;
    };
    
    //adição da idade
    if ( ( p = realloc( p, TAMBUFFER + sizeof( int ) ) ) == NULL ) {
        printf( "\nNão foi possivel expandir o pBuffer!\n" );
        exit( 2 );
    };

    printf( "\nDigite a idade desta pessoa: " );
    scanf( "%d", ( int* )( p + TAMBUFFER ) ); 
    
    TAMBUFFER += sizeof( int );

    LimpaBuffer( p ); //limpa o '\n' deixado pela leitura da idade
    
    //adição do email
    PERTOREGISTER = TAMBUFFER; //move PERTOREGISTER para a posição do tamanho do email, pois pedePessoa() precisa que PERTOREGISTER anteceda a string digitada

    if ( ( p = realloc( p, TAMBUFFER + sizeof( int ) ) ) == NULL ) {
        printf( "\nNão foi possivel expandir o pBuffer!\n" );
        exit( 2 );
    };
    
    *( int* )( p + PERTOREGISTER ) = 0; //define o tamanho do email para 0, pois o email não foi lido ainda
    TAMBUFFER += sizeof( int );

    printf( "\nDigite o e-mail: " );
    p = PedePessoa( p ); 

    NUMPER += 1; 
    
    PERTOREGISTER = TAMBUFFER;
    
    return p;
}

void * PedePessoa( void * p ) {//utilisa FUNR para ler o caracter e verifica-lo
    
    FUNR = getchar( ); 
    
    while ( ( char )FUNR != '\n' && FUNR != EOF ) { //verifica se o caracter lido é valido
        
        if ( ( p = realloc( p, TAMBUFFER + sizeof( char ) ) ) == NULL ) { 
            printf( "\nNão foi possivel expandir o pBuffer!\n" );
            exit( 2 );
        };

        ( *( ( char* )( p + TAMBUFFER ) ) ) = (char)FUNR; //grava o char lido na string

        ( *( ( int* )( p + PERTOREGISTER ) ) ) += 1; //incrementa o controle do tamanho da string

        TAMBUFFER += sizeof( char );
        
        
        FUNR = getchar( ); //lê o próximo char a ser avaliado
    };
    
    //coloca '\0' no final do string.
    if ( ( p = realloc( p, TAMBUFFER + sizeof( char ) ) ) == NULL ) { 
        printf( "\nNão foi possivel expandir o pBuffer!\n" );
        exit( 2 );
    };
    *( ( char* )( p + TAMBUFFER ) ) = '\0';
    
    
    ( *( ( int* )( p + PERTOREGISTER ) ) ) += 1; //incrementa o controlador do tamanho da string para incluir o '\0'
    
    TAMBUFFER += sizeof( char );
    
    return p;
}

void Listar( void * p ) {
    void * per = p + 4 * sizeof( int ); //cria um ponteiro para a primeira pessoa

    for ( FUNR = 0; FUNR < NUMPER; FUNR++ ) {//imprime as pessoas do pBuffer uma a uma
        printf( "\n\nRegistro %d: ", FUNR + 1 );
        per = Imprimir( per ); //Imprimir é responsavel por encontrar o endereço da proxima pessoa
    };
    
    PERTOREGISTER = TAMBUFFER;
}

void * Imprimir( void * per ) {//'per' começa apontando para tamanho do nome da pessoa

    printf( "\nNome: %s", ( char* )( per + sizeof( int ) ) ); //imprime o nome
    
    per += *( int* )per + sizeof( int ); //pula o nome + o inteiro de controle de tamanho do nome
    
    printf( "\nIdade: %d", *( int* )per ); //imprime a idade
    
    per += sizeof( int );//pula a idade
    
    printf( "\nE-mail: %s", ( char* )( per + sizeof( int ) ) );//imprime o email
    
    per += *( int* )per + sizeof( int );//pula o email e o inteiro de controle de tamanho do email, chegando ao endereço da proxima pessoa

    return per;
}

void * Pesquisar( void * p, void * str ) { //utiliza FUNR como controle de laço, mantem FUNR após a execução, o que permite devolver quantas pessoas foram lidas
    void * lop = p + 4 * sizeof( int );//cria um ponteiro para a primeira pessoa

    FUNR = 0;
    while ( ( FUNR < NUMPER ) && ( 0 != strcmp( ( char* )( lop + sizeof( int ) ), ( char* )str ) ) ) {//percorre as pessoas até que encontre o nome de str, ou até que se acabem as pessoas
        FUNR ++;

        //lop vai para a proxima pessoa
        lop += *( int* )lop + 2 * sizeof( int );
        lop += *( int* )lop + sizeof( int );
    };

    if ( FUNR == NUMPER ) {//verifica se a pessoa não foi encontrada
        lop = NULL;
    };

    return lop;
}

void * Buscar( void * p ) {

    //cria a string de busca
    if ( ( p = realloc( p, TAMBUFFER + sizeof( int ) ) ) == NULL ) {
        printf( "Não foi possivel expandir o pBuffer!" );
        exit( 2 );
    };
    *( int* )( p + PERTOREGISTER ) = 0; 
    TAMBUFFER += sizeof( int );
    
    printf( "\nDigite o nome da pessoa que deseja buscar: " );
    p = PedePessoa( p ); 

    //procura a pessoa
    void * per = Pesquisar( p, p + PERTOREGISTER + sizeof( int ) );
    
    if ( per == NULL ) {//sinaliza caso a pessoa não foi encontrada
        printf( "\nPessoa não encontrada!" );
    } else {//imprime caso a pessoa foi encontrada
        Imprimir( per );
    };

    //desaloca o espaço da string de busca
    TAMBUFFER = PERTOREGISTER;

    if ( ( p = realloc( p, TAMBUFFER ) ) == NULL ) {
        printf( "\nNão foi possivel realocar o pBuffer!\n" );
        exit( 3 );
    };

    return p;
}

void * Remover( void * p ) {

    //cria a string de busca
    if ( ( p = realloc( p, TAMBUFFER + sizeof( int ) ) ) == NULL ) {
        printf( "Não foi possivel expandir o pBuffer!" );
        exit( 2 );
    };
    *( int* )( p + PERTOREGISTER ) = 0; 
    TAMBUFFER += sizeof( int );
    
    printf( "\nDigite o nome da pessoa que deseja remover: " );
    p = PedePessoa( p ); 

    //procura a pessoa
    void * per = Pesquisar( p, p + PERTOREGISTER + sizeof( int ) ); 
    
    if ( per == NULL ) {//sinaliza caso a pessoa não foi encontrada
        printf( "\nPessoa não encontrada!" );

        //desaloca a string de busca
        TAMBUFFER = PERTOREGISTER;
        if ( ( p = realloc( p, TAMBUFFER ) ) == NULL ) {
            printf( "\nNão foi possivel realocar o pBuffer!\n" );
            exit( 3 );
        };

        return p;
    } else {//remove a pessoa caso ela seja encontrada

        //mostra os dados da pessoa a ser removida
        printf( "\nPessoa removida: ");
        Imprimir( per );

        //guarda o tamanho da pessoa a ser removida
        *( int* )( p + PERTOREGISTER ) = ( 3 * sizeof( int ) /* inteiros do tamanho do nome, idade e tamanho do email */ + *( int* )per /* tamanho do nome */ + *( int* )( per + *( int* )per + 2 * sizeof( int ) ) /* tamanho do email */); 

        Sobrescreve( p, per );//efetivamente apaga a pessoa encontrada

        //desaloca o espaço da pessoa removida e o espaço da string de busca
        PERTOREGISTER -= *( int* )( p + PERTOREGISTER );//PERTOREGISTER já é o espaço do pBuffer sem a string de busca, isto tira o espaço espaço da pessoa removida do PERTOREGISTER;

        TAMBUFFER = PERTOREGISTER;

        if ( ( p = realloc( p, TAMBUFFER ) ) == NULL ) {
            printf( "\nNão foi possivel realocar o pBuffer!\n" );
            exit( 3 );
        };
        
        NUMPER -= 1;

        return p;
    };
}

void Sobrescreve( void * p, void * mop ) {//a partir do FUNR de pesquisar, desloca todas as pessoas para sobrescrever mop
    void * ptm = mop;
    
    //torna ptm o endereço da proxima pessoa
    ptm += *( int* )ptm + 2 * sizeof( int ); 
    ptm += *( int* )ptm + sizeof( int ); 

    //mop -= sizeof( char );
    //ptm -= sizeof( char );

    while ( FUNR < NUMPER - 1 ) {//transcreve ptm para mop. Precisa do FUNR de pesquisar para saber quantas pessoas restam no pBuffer
        //mop += sizeof( char );
        //ptm += sizeof( char );
        FUNR ++;

        *( int* )mop = *( int* )ptm;//copia o tamanho do nome

        mop += sizeof( int );
        ptm += sizeof( int );

        while ( *( char* )ptm != '\0' ) {//copia ptm.nome para mop.nome 
            *( char* )mop = *( char* )ptm;
            mop += sizeof( char );
            ptm += sizeof( char );
        };

        *( char* )mop = *( char* )ptm;//copia o '\0' que quebrou o laço
        mop += sizeof( char );
        ptm += sizeof( char );

        *( int* )mop = *( int* )ptm;//copia a idade

        mop += sizeof( int );
        ptm += sizeof( int );

        *( int* )mop = *( int* )ptm;//copia o tamanho do email

        mop += sizeof( int );
        ptm += sizeof( int );
        
        while ( *( char* )ptm != '\0' ) {//copia ptm.email para mop.email 
            *( char* )mop = *( char* )ptm;
            mop += sizeof( char );
            ptm += sizeof( char );
        };

        *( char* )mop = *( char* )ptm;//copia o '\0' que quebrou o laço
        mop += sizeof( char );
        ptm += sizeof( char );
    };
}

void LimpaBuffer( void * p ) {
    FUNR = getchar( );
    
    // Continua lendo enquanto não for '\n' e não for EOF
    while ( FUNR != '\n' && FUNR != EOF ) {
        FUNR = getchar( );
    };
}