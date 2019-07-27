# Índice geral de introdução à linguagem PIF

## Comentários

Para inserir comentários, basta utilizar o caractere `#`, que todo o conteúdo digitado após ele será desconsiderado no momento da compilação.

Ex.:

```
	# Estas duas linhas
	# estão comentadas
	
	leia nome # Esta linha lê o nome do usuário (e isto é um comentário)
```

## Entrada e saída de dados

### Entrada

Para efetuar a leitura dos dados, você deve utilizar a função `leia` com as respectivas variáveis como argumento (separadas por vírgula). Note que as variáveis devem ser previamente declaradas.

Ex.:

```
	leia idade
	
	[...]
	
	leia nome, sobrenome, ano_nasc, mes_nasc
```

### Saída

Para escrever/imprimir os dados na saída padrão da aplicação (normalmente o console), você deverá fazer o uso da função `escreva` com seus respectivos argumentos (separados por '+', sem aspas).

Ex.:

```
	escreva "Ola mundo"
	
	[...]
	
	escreva "Seu nome eh " + nome + "."
```

### Observações e informações adicionais

Para mais informações a respeito de entrada e saída de dados, por favor, dirija-se à página específica de [Entrada e saída de dados.](./operacoes_es.md).

## Operações com variáveis de tipo literal

Apenas as operações de concatenação e assinalamento são permitidas para variáveis de tipos literais (por hora, as operações booleanas não são).
Para realizar as operações de concatenação basta realizar o uso do operador '+', sem aspas, entre as variáveis. Note que é possível assinalar/concatenar variáveis de qualquer tipo a um literal.

Ex.:

```
	nome, sobrenome, nome_completo : literal
	
	[...]
	
	nome_completo = nome + " " + sobrenome
```

Ex.:

```
	x : numerico
	res : literal
	
	[...]
	
	res = "O resultado de x eh : " + x
```

### Observações e informações adicionais

Para mais informações a respeito de operações com variáveis de tipo literal, por favor, dirija-se à página específica de [Operações com o tipo literal.](./operacoes_literal.md).

## Estruturas condicionais

Estruturas condicionais são usadas para realizar "desvios" no programa, caso determidada relação seja satisfeita (Por exemplo: Podemos realizar uma divisão somente se x for diferente de 0, ou então só podemos tirar a raíz quadrada de x se x for positivo, etc.).

A estrutura condicional é escrita da seguinte forma:

```
	se <condição> então
		# código a ser executado se a condição for satisfeita
		[...]
	fim-se
```

Pode-se também executar um código diferente caso a condição não seja satisfeita:

```
	se <condição> então
		# código a ser executado se a condição for satisfeita
		[...]
	senão
		# código a ser executado se a condição NÃO for satisfeita
		[...]
	fim-se
```

Também é possível encadear condicionais:

```
	se <condição 1> então
		# código a ser executado se a condição 1 for satisfeita
		[...]
	senão se <condição 2> então
		# código a ser executado se a condição 1 NÃO for satisfeita, mas a 2 sim
		[...]
	senão
		# código a ser executado se nenhuma das condições anteriores for satisfeita
		[...]
	fim-se
```

Note que a `<condição>` nada mais é do que uma expressão booleana sem assinalamento.

### Observações e informações adicionais

Para mais informações a respeito de estruturas condicionais, por favor, dirija-se à página específica de [Estruturas condicionais.](./condicionais.md).

## Estruturas de repetição

Esta linguagem suporta duas estruturas de repetição diferentes, a `faça ... enquanto` e a `enquanto`. Em ambos as estruturas a ideia é que o bloco de código que estiver dentro delas seja executado enquanto determinada condição for satisfeita (verdadeira).

A etrutura de repetição `enquanto` é descrita da seguinte forma :

```
	enquanto <condição> faça
		# código a ser executado enquanto a condição for satisfeita
		[...]
	fim-enquanto
```

Já a estrutura `faça ... enquanto` é descrita da seguinte forma :

```
	faça
		# código a ser executado enquanto a condição for satisfeita
		# ou na primeira chamada ao trecho do código
		[...]
	enquanto <condição>
```

Note que o código contido na segunda estrutura será executado ao menos uma vez (é muito útil para realizar a validação de dados de entrada).

Também é importante perceber que a `<condição>` nada mais é do que uma expressão booleana sem assinalamento.

### Observações e informações adicionais

Para mais informações a respeito de estruturas de repetição, por favor, dirija-se à página específica de [Estruturas de repetição.](./repeticao.md).

# Licença

Todo o conteúdo de documentação encontra-se sob a Licença Creative Commons Attribution-Share Alike 4.0 International (CC-BY-SA).

<p align="center">
	<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Licença Creative Commons" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />Este trabalho está licenciado com uma Licença <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons - Atribuição-CompartilhaIgual 4.0 Internacional</a>.
</p>
