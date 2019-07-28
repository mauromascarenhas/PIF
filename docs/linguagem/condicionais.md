# Estruturas condicionais

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

Perceba que para blocos `senão se` "encadeados", não se deve terminar o bloco condicional principal, ou seja, usa-se apenas um `fim-se` ao término da estrutura.

Note que a `<condição>` nada mais é do que uma expressão booleana sem assinalamento (não pode haver o operador `=`) e sem operações aritméticas adjacentes, ou seja, operações de soma, subtração, multiplicação e divisão seguidas de comparação. A seguir um exemplo do que pode ser feito e o que não deve ser feito:

### Incorreto

```
	se (x * x) > k então
		# código a ser executado se a condição for satisfeita
		# incorreto
		[...]
	fim-se
```

### Correto

```
	quadrado : numérico
	
	quadrado = x * x
	
	se quadrado > k então
		# código a ser executado se a condição for satisfeita
		[...]
		
		quadrado = x * x
	fim-se
```

## Links úteis (aprenda mais)

1. [Índice geral.](../README.md)

# Licença

Todo o conteúdo de documentação encontra-se sob a Licença Creative Commons Attribution-Share Alike 4.0 International (CC-BY-SA).

<p align="center">
	<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Licença Creative Commons" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />Este trabalho está licenciado com uma Licença <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons - Atribuição-CompartilhaIgual 4.0 Internacional</a>.
</p>
