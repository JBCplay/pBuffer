# pBuffer

## Jean Barros Correa

### Agenda
Faça uma agenda com o seguinte menu:<br/>
1- Adicionar Pessoa (Nome, Idade, email)<br/>
2- Remover Pessoa<br/>
3- Buscar Pessoa<br/>
4- Listar todos<br/>
5- Sair
 
O desafio é não poder criar variáveis e sim um buffer de memória (void *pBuffer).
 
Regras:<br/>
1. Nenhuma variável pode ser declarada em todo o programa, somente ponteiros. Todos os dados do programa devem ser guardados dentro do pBuffer.<br/>
   - Nem mesmo como parâmetro de função. Só ponteiros que apontam para dentro do pBuffer.<br/>
   - Exemplo do que não pode: int c; char a; int v[10];  void Funcao(int parametro)<br/>
2. Não pode usar struct em todo o programa.
